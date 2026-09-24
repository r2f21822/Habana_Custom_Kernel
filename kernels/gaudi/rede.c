

#pragma tpc_printf (enable)
// Habilita a extensao de printf do compilador TPC, util pra debugar valores
// intermediarios durante os testes.

void main(tensor input,   // Tensor de entrada: [N features, M amostras]. Cada coluna e uma amostra de teste.
          tensor weights, // Tensor de pesos: [N] -> um peso por feature.
          tensor bias,    // Tensor de bias: [1] -> b (nao escala com N).
          tensor output)  // Tensor de saida: [1, M amostras].
{
    const int5 indexSpaceStart = get_index_space_offset();
    // Le o offset inicial do espaco de indices deste work-item. Definido pelo host (glue code),
    // fora deste arquivo — precisa bater com o numero de amostras (M) que voce vai testar.

    const int5 indexSpaceEnd = get_index_space_size() + indexSpaceStart;
    // Calcula onde o loop de amostras deve parar, somando o tamanho ao offset inicial.

    const int numFeatures = get_dim_size(input, 0);
    // Le o tamanho real da dimensao 0 do tensor de entrada (quantas features existem, N).
    // Confirmado: e a mesma funcao usada no seu kernel original de matmul
    // (const int commonSize = get_dim_size(aMatrix, 0);). Calculado uma vez so, fora do
    // loop de amostras, pois e o mesmo pra todas.

    int5 xCoord = {0};
    // Coordenada usada pra indexar o tensor de entrada: dim0 = feature, dim1 = amostra.

    int5 wCoord = {0};
    // Coordenada usada pra indexar o tensor de pesos: dim0 = indice do peso.

    int5 bCoord = {0};
    // Coordenada usada pra indexar o tensor de bias (so tem 1 posicao, dim0 = 0 sempre).

    int5 oCoord = {0};
    // Coordenada usada pra escrever o resultado no tensor de saida: dim0 = amostra.

    bCoord[0] = 0;
    // Fixa a coordenada do bias em 0 (so existe 1 bias neste kernel).

    __global__ char* p_b = gen_addr(bCoord, bias);
    // Gera o endereco de memoria global do bias, usando GEN_ADDR (confirmado). Fica fora do
    // loop de amostras porque o bias e o mesmo pra toda amostra (nao muda por iteracao).

    for (int sample = indexSpaceStart[0]; sample < indexSpaceEnd[0]; sample++)
    // Percorre cada amostra de teste do batch (amostra 0, 1, 2... ate M-1).
    {
        float acc = s_f32_ld_g(p_b);
        // Carrega o bias como um float ESCALAR de verdade (sem vetor, sem broadcast, sem
        // suposicao). Confirmado: s_f32_ld_g e usado exatamente assim no exemplo oficial
        // de printf da documentacao. "acc" comeca valendo o bias.

        for (int i = 0; i < numFeatures; i++)
        // Percorre todas as features desta amostra (0 a numFeatures-1).
        {
            xCoord[0] = i;
            // Define qual feature esta sendo lida.

            xCoord[1] = sample;
            // Define qual amostra esta sendo lida.

            __global__ char* p_x = gen_addr(xCoord, input);
            // Gera o endereco global do valor de entrada input[i, sample].

            float xVal = s_f32_ld_g(p_x);
            // Carrega o valor de entrada como float escalar puro (confirmado).

            wCoord[0] = i;
            // Define qual peso esta sendo lido, correspondente a feature i.

            __global__ char* p_w = gen_addr(wCoord, weights);
            // Gera o endereco global do peso weights[i].

            float wVal = s_f32_ld_g(p_w);
            // Carrega o peso como float escalar puro (confirmado).

            acc = s_f32_mac(wVal, xVal, acc);
            // Multiply-accumulate ESCALAR: acc = acc + (wVal * xVal). Confirmado na pagina
            // "Arithmetic" da doc oficial: "float s_f32_mac(float a, float b, float
            // accumulator, ...)" com retorno "acc = acc + (a*b)". Depois de todas as
            // iteracoes, acc = (x0*w0) + (x1*w1) + ... + (xN*wN) + bias.
        }

        float64 vecAcc = acc;
        // Converte o resultado escalar final pra vetor, via atribuicao simples. A doc
        // confirma explicitamente que isso faz broadcast do escalar pra todas as 64 lanes
        // do vetor (exemplo oficial: "float a = 65; float64 b = a;"). Isso NAO e mais uma
        // suposicao — e comportamento documentado da linguagem.

        vecAcc = v_sigmoid_f32(vecAcc);
        // Aplica sigmoid ao vetor (confirmado na tabela de Built-in Special Functions —
        // so existe essa forma vetorial, por isso precisamos do vecAcc acima).
        // >>> REMOVER ESTA LINHA PARA REGRESSAO LINEAR (sem ativacao) <<<

        oCoord[0] = sample;
        // Define em qual posicao do tensor de saida o resultado desta amostra sera gravado.

        v_f32_st_tnsr(oCoord, output, vecAcc);
        // Escreve o vetor no tensor de saida (confirmado — ja usado no seu kernel original
        // de matmul). Como vecAcc e um broadcast de verdade (todas as 64 lanes tem o mesmo
        // valor real, nao lixo), a gravacao esta correta independente de como o ST_TNSR
        // faz o "partial cull" internamente — qualquer lane que ele escolha gravar tem o
        // valor certo.
    }
}

