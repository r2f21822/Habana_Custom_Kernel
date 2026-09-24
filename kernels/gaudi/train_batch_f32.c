/**********************************************************************
 * Kernel: Treinamento em LOTE (batch gradient descent) com MSE -- TPC-C
 *
 * Traducao direta da classe Python "redeNeural" que voce mandou:
 *
 *   forwardPass:      y_pred = X @ W + b                    (sem ativacao)
 *   backPropagation:  dMse = 2*(y_pred - y_true) / n
 *                     dW   = X.T @ dMse   (soma de x_i * dMse_i por feature)
 *                     db   = soma(dMse)
 *   atualizaPesos:    W -= taxa * dW
 *                     b -= taxa * db
 *
 * DIFERENCA-CHAVE em relacao ao kernel de treino anterior (treino_kernel_tpc.c):
 * aquele atualizava os pesos a CADA amostra (stochastic gradient descent).
 * Este acumula o gradiente de TODAS as amostras primeiro, e so atualiza
 * os pesos UMA VEZ por epoca -- exatamente como o "train()" em Python faz
 * (forwardPass processa X inteiro de uma vez, backPropagation calcula um
 * dW/db so, atualizaPesos aplica uma vez).
 *
 * MESMA ESTRATEGIA DE SEGURANCA DE MEMORIA do kernel anterior: pesos e
 * bias sao lidos do tensor UMA VEZ no inicio, tudo o resto roda em
 * variaveis escalares locais, e so no final (apos todas as epocas) os
 * valores aprendidos sao escritos de volta no tensor. Isso evita o
 * problema de coerencia (store em tensor seguido de load escalar nao
 * coerente sem "aso") que discutimos antes -- aqui nunca relemos o
 * tensor de pesos depois de comecar a escrever nele.
 *
 * STATUS DE VERIFICACAO: mesmos intrinsecos ja confirmados nas etapas
 * anteriores (gen_addr, s_f32_ld_g, s_f32_mac, broadcast escalar->vetor,
 * v_f32_st_tnsr). Nenhum intrinseco novo introduzido aqui.
 
 TREINA UMA REDE DO TIPO 
 WX + B =Y
 equivalenge a regressão linear
 **********************************************************************/

#pragma tpc_printf (enable)

#define NUM_FEATURES  2
#define NUM_SAMPLES   4
#define EPOCHS        1000
#define LEARNING_RATE 0.01f

void main(tensor input,       // [NUM_FEATURES, NUM_SAMPLES] -- equivalente ao "X" do Python
          tensor targets,     // [NUM_SAMPLES] -- equivalente ao "y" do Python
          
          tensor weights_in,  // [NUM_FEATURES] -- pesos INICIAIS (entrada)
          tensor bias_in,     // [1] -- bias INICIAL (entrada)
          
          tensor weights_out, // [NUM_FEATURES] -- pesos APRENDIDOS (saida)
          tensor bias_out)    // [1] -- bias APRENDIDO (saida)
{


    int5 coord = {0};

    float w[NUM_FEATURES];
    // coord[0] a coord[4] representa uma coordenada em uma dimensão do tensor.

//gen_addr só calcula um endereço na memória global, a partir das coordenadas e do descritor do tensor
    for (int i = 0; i < NUM_FEATURES; i++)
    {
        coord[0] = i;
        //O s_f32_ld_g lê da memória global (o _g é de global) e coloca o valor em um registrador escalar (por isso s)
        w[i] = s_f32_ld_g(gen_addr(coord, weights_in));
    }

    coord[0] = 0;
    float b = s_f32_ld_g(gen_addr(coord, bias_in));
    

    float x[NUM_SAMPLES][NUM_FEATURES];
    float y[NUM_SAMPLES];

    for (int s = 0; s < NUM_SAMPLES; s++)
    {
        for (int i = 0; i < NUM_FEATURES; i++)
        {
            coord[0] = i;
            coord[1] = s;
            x[s][i] = s_f32_ld_g(gen_addr(coord, input));
        }
        coord[0] = s;
        coord[1] = 0;   
        y[s] = s_f32_ld_g(gen_addr(coord, targets));
    }

     
    printf("x[3][1]=%f\n", x[3][1]);
    printf("y[3]=%f\n", y[3]);
    printf("w0=%f\n", w[0]);
    printf("b=%f\n", b);


    for (int epoch = 0; epoch < EPOCHS; epoch++)
    {
      
//zerar gradiente
        float dW[NUM_FEATURES];
        for (int i = 0; i < NUM_FEATURES; i++)
        {
            dW[i] = 0.0f;
        }
        float db = 0.0f;
   

        for (int s = 0; s < NUM_SAMPLES; s++)
        {
            // y_pred = X @ W + b, para a amostra s
            
            /*instrução de nível de montagem (assembly) ou uma função intrínseca de hardware que realiza a operação de Multiplicação e Acúmulo em precisão simples (ponto flutuante de 32 bits), comumente abreviada como MAC (Multiply-Accumulate).*/
            float acc = b;
            for (int i = 0; i < NUM_FEATURES; i++)
            {
                acc = s_f32_mac(w[i], x[s][i], acc);
            }
            float y_pred = acc;
        

            // gradiente do erro
            float dMse = 2.0f * (y_pred - y[s]) / (float) NUM_SAMPLES;

            for (int i = 0; i < NUM_FEATURES; i++)
            {
                dW[i] = s_f32_mac(x[s][i], dMse, dW[i]);
                // dW[i] = dW[i] + (x[s][i] * dMse) -- confirmado: s_f32_mac.
            }

            db = db + dMse;
        }

        //atualizaPesos
        for (int i = 0; i < NUM_FEATURES; i++)
        {
            w[i] = w[i] - LEARNING_RATE * dW[i];
        }
        b = b - LEARNING_RATE * db;
    }

    //  Escreve os pesos e bias aprendidos de volta no tensor -
    for (int i = 0; i < NUM_FEATURES; i++)
    {
        coord[0] = i;
        float64 vecW = w[i];

        //float64 não é um número de ponto flutuante de 64 bits. No TPC, float64 é um tipo vetorial que contém 64 valores f32
      //pega o valor escalar w[0] e o replica em todas as 64 posições do vetor 
        // vetorial pra tensor (nao existe store escalar pra memoria global/tensor).
        v_f32_st_tnsr(coord, weights_out, vecW);
        //store em um tensor, memoria global
        //escalar - vetor - memoria global
    }

    coord[0] = 0;
    float64 vecB = b;
    v_f32_st_tnsr(coord, bias_out, vecB);
  
    printf("final w0=%f\n", w[0]);
    printf("final w1=%f\n", w[1]);
    printf("final b=%f\n", b);
}
//roda em apenas uma thread tpc, ent os espaços de indice nao etsao bem definidos


