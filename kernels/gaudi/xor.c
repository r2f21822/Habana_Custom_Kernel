/**********************************************************************
 * Kernel TPC-C: forward pass de uma rede XOR
 *
 *   Entrada [2, S] -> Escondida [H] (sigmoid) -> Saida [1, S] (sigmoid)
 *
 * Index space = 1 work-item: ele percorre todas as S amostras.
 * As ativacoes escondidas ficam em memoria local VETORIAL.
 **********************************************************************/

#define MAX_WIDTH 16

__local__ float64 hidV[MAX_WIDTH];

void main(tensor input,    // [2, S]
          tensor weights1, // [2, H]
          tensor bias1,    // [H]
          tensor weights2, // [H, 1]
          tensor bias2,    // [1]
          tensor output)   // [1, S]
{
    const int N = get_dim_size(input, 0);
    const int S = get_dim_size(input, 1);
    const int H = get_dim_size(weights1, 1);

    for (int s = 0; s < S; s++)
    {
        // ---- camada escondida ----
        for (int j = 0; j < H; j++)
        {
            int5 cB = {0};
            cB[0] = j;
            float acc = s_f32_ld_g(gen_addr(cB, bias1));

            for (int i = 0; i < N; i++)
            {
                int5 cX = {0};
                cX[0] = i;
                cX[1] = s;
                int5 cW = {0};
                cW[0] = i;
                cW[1] = j;

                float x = s_f32_ld_g(gen_addr(cX, input));
                float w = s_f32_ld_g(gen_addr(cW, weights1));
                acc = s_f32_mac(w, x, acc);
            }

            float64 v = acc;
            hidV[j] = v_sigmoid_f32(v);
        }

        // ---- camada de saida ----
        int5 cB2 = {0};
        float b2s = s_f32_ld_g(gen_addr(cB2, bias2));
        float64 accOut = b2s;

        for (int j = 0; j < H; j++)
        {
            int5 cW2 = {0};
            cW2[0] = j;
            float ws = s_f32_ld_g(gen_addr(cW2, weights2));
            float64 wv = ws;
            accOut = accOut + hidV[j] * wv;
        }

        float64 vOut = v_sigmoid_f32(accOut);

        int5 cO = {0};
        cO[1] = s;
        v_f32_st_tnsr(cO, output, vOut);
    }
}
