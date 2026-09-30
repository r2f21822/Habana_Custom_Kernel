#pragma tpc_printf (enable)

#define NUM_FEATURES  2
#define NUM_SAMPLES   264
#define EPOCHS        1000
#define LEARNING_RATE 0.01f


void main(tensor input, tensor targets, tensor weights_in, tensor bias_in,
          tensor weights_out, tensor bias_out,
          tensor scratch)   // <-- tensor extra só de rascunho, tamanho [64]
{


// ---- index space: cada work item pega uma fatia de amostras ----
    const int5 index_space_start = get_index_space_offset();
    const int5 index_space_end   = get_index_space_size() + index_space_start;

    // Cada work item processa amostras [sampleStart, sampleEnd)
    const int sampleStart = index_space_start[0] * 64;
    const int sampleEnd   = index_space_end[0]   * 64;
    // (limitado ao total de amostras)
    const int sEnd = (sampleEnd < NUM_SAMPLES) ? sampleEnd : NUM_SAMPLES;
    const int sStart = (sampleStart < NUM_SAMPLES) ? sampleStart : NUM_SAMPLES;

    int5 coord = {0};
    float w[NUM_FEATURES];
    for (int i = 0; i < NUM_FEATURES; i++) { coord[0]=i; w[i]=s_f32_ld_g(gen_addr(coord, weights_in)); }
    coord[0]=0; float b = s_f32_ld_g(gen_addr(coord, bias_in));

    for (int epoch = 0; epoch < EPOCHS; epoch++)
    {
        float dW[NUM_FEATURES] = {0};
        float db = 0.0f;

         for (int s = sStart; s < sEnd; s += 64)
        {
            float64 y_pred = b;
            float64 x_vec[NUM_FEATURES];
            for (int i = 0; i < NUM_FEATURES; i++)
            {
                coord[0]=i; coord[1]=s;
                x_vec[i] = v_f32_ld_g(gen_addr(coord, input));
                y_pred = v_f32_mac_b(w[i], x_vec[i], y_pred);
            }
            coord[0]=s; coord[1]=0;
            float64 y_true = v_f32_ld_g(gen_addr(coord, targets));
            float64 dMse = (y_pred - y_true) * (2.0f / (float)NUM_SAMPLES);

            for (int i = 0; i < NUM_FEATURES; i++)
            {
                float64 partial = x_vec[i] * dMse;

                // ---- ponte vetor -> escalar via tensor de rascunho ----
                coord[0] = 0;
                
                v_f32_st_tnsr(coord, scratch, partial);      // grava os 64 valores
                for (int lane = 0; lane < 64; lane++)
                {
                    coord[0] = lane;
                    dW[i] += s_f32_ld_g(gen_addr(coord, scratch));  // lê 1 a 1, soma escalar
                }
               // dW[i] += v_f32_reduce_add(partial);
            }

            coord[0] = 0;
          
            v_f32_st_tnsr(coord, scratch, dMse);
            for (int lane = 0; lane < 64; lane++)
            {
                coord[0] = lane;
                db += s_f32_ld_g(gen_addr(coord, scratch));
            
           // db += v_f32_reduce_add(dMse);
           }
        }
     // ---- ATENÇÃO: cada work item tem seu próprio dW/db ----
        // Se houver mais de 1 work item, isso precisa ser reduzido globalmente.
        // Por enquanto, cada work item atualiza os pesos por conta própria.
        for (int i = 0; i < NUM_FEATURES; i++) w[i] -= LEARNING_RATE * dW[i];
        b -= LEARNING_RATE * db;
    }

    for (int i = 0; i < NUM_FEATURES; i++) { coord[0]=i; float64 v=w[i]; v_f32_st_tnsr(coord, weights_out, v); }
    coord[0]=0; float64 vb=b; v_f32_st_tnsr(coord, bias_out, vb);
}

