#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <chrono>
#include <iostream>
#include <type_traits>
#include <vector>
#include <x86intrin.h>
#include "xor_test.hpp"
#include "entry_points.hpp"

static const int NUM_FEATURES = 2;
static const int NUM_HIDDEN   = 2;
static const int NUM_SAMPLES  = 4;

namespace {

// Tipo do elemento de um campo que pode ser ponteiro (T*) ou array (T[N])
template <typename F>
struct Elem
{
    using type = typename std::remove_cv<
        typename std::remove_pointer<
            typename std::remove_extent<F>::type>::type>::type;
};

// Se o campo e ponteiro, aponta para a memoria do teste.
template <typename F, typename S>
void bindStorage(F*& field, S* storage) { field = storage; }

// Se o campo ja e um array dentro da struct, nao ha nada a fazer.
template <typename F, std::size_t N, typename S>
void bindStorage(F (&)[N], S*) {}

float sigmoid_cpu(float x) { return 1.0f / (1.0f + std::exp(-x)); }

} // namespace

void XorTest::xor_reference_implementation(
        const float X[NUM_FEATURES][NUM_SAMPLES],
        const float W1[NUM_FEATURES][NUM_HIDDEN], const float B1[NUM_HIDDEN],
        const float W2[NUM_HIDDEN][1], const float B2[1],
        float out[NUM_SAMPLES])
{
    for (int s = 0; s < NUM_SAMPLES; s++)
    {
        float h[NUM_HIDDEN];
        for (int j = 0; j < NUM_HIDDEN; j++)
        {
            float acc = B1[j];
            for (int i = 0; i < NUM_FEATURES; i++)
                acc += X[i][s] * W1[i][j];
            h[j] = sigmoid_cpu(acc);
        }

        float accOut = B2[0];
        for (int j = 0; j < NUM_HIDDEN; j++)
            accOut += h[j] * W2[j][0];
        out[s] = sigmoid_cpu(accOut);
    }
}

int XorTest::runTest()
{
    std::cout << "=== Teste do Kernel TPC XOR (2 inputs, 4 samples) ===" << std::endl;

    // Tabela verdade: X[feature][amostra]
    const float X_data[NUM_FEATURES][NUM_SAMPLES] = {
        {0.0f, 0.0f, 1.0f, 1.0f},
        {0.0f, 1.0f, 0.0f, 1.0f}
    };

    // Pesos pre-treinados: h1 = OR, h2 = NAND, saida = AND(h1, h2) = XOR
    const float W1_data[NUM_FEATURES][NUM_HIDDEN] = {
        { 20.0f, -20.0f},
        { 20.0f, -20.0f}
    };
    const float B1_data[NUM_HIDDEN]    = {-10.0f, 30.0f};
    const float W2_data[NUM_HIDDEN][1] = {{20.0f}, {20.0f}};
    const float B2_data[1]             = {-30.0f};

    // Tensores
    uint64_t dimsInput[]  = {NUM_FEATURES, NUM_SAMPLES, 1, 1, 1};
    uint64_t dimsW1[]     = {NUM_FEATURES, NUM_HIDDEN, 1, 1, 1};
    uint64_t dimsB1[]     = {NUM_HIDDEN, 1, 1, 1, 1};
    uint64_t dimsW2[]     = {NUM_HIDDEN, 1, 1, 1, 1};
    uint64_t dimsB2[]     = {1, 1, 1, 1, 1};
    uint64_t dimsOutput[] = {1, NUM_SAMPLES, 1, 1, 1};

    float_5DTensor input(dimsInput);
    float_5DTensor w1(dimsW1);
    float_5DTensor b1(dimsB1);
    float_5DTensor w2(dimsW2);
    float_5DTensor b2(dimsB2);
    float_5DTensor output(dimsOutput);

    int c[5] = {0, 0, 0, 0, 0};

    for (int i = 0; i < NUM_FEATURES; i++)
        for (int s = 0; s < NUM_SAMPLES; s++)
        {
            c[0] = i; c[1] = s;
            input.SetElement(c, X_data[i][s]);
        }

    for (int i = 0; i < NUM_FEATURES; i++)
        for (int j = 0; j < NUM_HIDDEN; j++)
        {
            c[0] = i; c[1] = j;
            w1.SetElement(c, W1_data[i][j]);
        }

    c[1] = 0;
    for (int j = 0; j < NUM_HIDDEN; j++)
    {
        c[0] = j;
        b1.SetElement(c, B1_data[j]);
        w2.SetElement(c, W2_data[j][0]);
    }

    c[0] = 0;
    b2.SetElement(c, B2_data[0]);

    // Referencia na CPU
    float y_ref[NUM_SAMPLES] = {0.0f};

    auto t0 = std::chrono::steady_clock::now();
    unsigned long long tsc0 = __rdtsc();
    xor_reference_implementation(X_data, W1_data, B1_data, W2_data, B2_data, y_ref);
    unsigned long long tsc1 = __rdtsc();
    auto t1 = std::chrono::steady_clock::now();

    std::cout << "Execucao CPU: "
              << std::chrono::duration<double, std::nano>(t1 - t0).count()
              << " ns, " << (tsc1 - tsc0) << " ciclos." << std::endl;

    // ---- Memoria para onde as defs apontam ----
    using InTensorT  = Elem<decltype(m_in_defs.inputTensors)>::type;
    using OutTensorT = Elem<decltype(m_in_defs.outputTensors)>::type;
    using InAPT      = Elem<decltype(m_out_defs.inputTensorAccessPattern)>::type;
    using OutAPT     = Elem<decltype(m_out_defs.outputTensorAccessPattern)>::type;

    InTensorT  inTensors[5]  = {};
    OutTensorT outTensors[1] = {};
    InAPT      inAccess[5]   = {};
    OutAPT     outAccess[1]  = {};
    std::vector<char> elfBuf(256 * 1024);

    bindStorage(m_in_defs.inputTensors,  inTensors);
    bindStorage(m_in_defs.outputTensors, outTensors);
    bindStorage(m_out_defs.inputTensorAccessPattern,  inAccess);
    bindStorage(m_out_defs.outputTensorAccessPattern, outAccess);

    m_out_defs.kernel.kernelElf = elfBuf.data();
    m_out_defs.kernel.elfSize   = (unsigned)elfBuf.size();

    // ---- Chamada do glue code ----
    m_in_defs.deviceId = tpc_lib_api::DEVICE_ID_GAUDI;

    m_in_defs.inputTensorNr = 5;
    LoadTensorToGcDescriptor(&(m_in_defs.inputTensors[0]), input);
    LoadTensorToGcDescriptor(&(m_in_defs.inputTensors[1]), w1);
    LoadTensorToGcDescriptor(&(m_in_defs.inputTensors[2]), b1);
    LoadTensorToGcDescriptor(&(m_in_defs.inputTensors[3]), w2);
    LoadTensorToGcDescriptor(&(m_in_defs.inputTensors[4]), b2);

    m_in_defs.outputTensorNr = 1;
    LoadTensorToGcDescriptor(&(m_in_defs.outputTensors[0]), output);

    snprintf(m_in_defs.guid.name, tpc_lib_api::MAX_NODE_NAME, "custom_xor");

    tpc_lib_api::GlueCodeReturn result = InstantiateTpcKernel(&m_in_defs, &m_out_defs);
    if (result != tpc_lib_api::GLUE_SUCCESS)
    {
        std::cout << "Falha no Glue Code! Codigo de erro: " << result << std::endl;
        return -1;
    }

    // ---- Simulacao ----
    std::vector<TensorDesc2> vec;
    vec.push_back(input.GetTensorDescriptor());
    vec.push_back(w1.GetTensorDescriptor());
    vec.push_back(b1.GetTensorDescriptor());
    vec.push_back(w2.GetTensorDescriptor());
    vec.push_back(b2.GetTensorDescriptor());
    vec.push_back(output.GetTensorDescriptor());

    TestBase::RunSimulation(vec, m_in_defs, m_out_defs);

    // ---- Comparacao ----
    const float tol = 1e-3f;
    bool ok = true;

    std::cout << "\nIn 1 | In 2 | Kernel TPC | CPU Ref" << std::endl;
    std::cout << "-----------------------------------" << std::endl;

    for (int s = 0; s < NUM_SAMPLES; s++)
    {
        c[0] = 0; c[1] = s;
        float got = output.ElementAt(c);

        std::cout << "  " << X_data[0][s] << "  |  " << X_data[1][s]
                  << "  |  " << got << "  |  " << y_ref[s] << std::endl;

        if (std::fabs(got - y_ref[s]) > tol)
            ok = false;
    }

    if (!ok)
    {
        std::cout << "\n[ERRO] O Teste do XOR no TPC Falhou!" << std::endl;
        return -1;
    }

    std::cout << "\n[SUCESSO] O Teste do XOR no TPC Passou!" << std::endl;
    return 0;
}
