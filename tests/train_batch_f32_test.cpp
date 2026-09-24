#include <cmath>
#include <cstring>
#include <iostream>
#include "train_batch_f32_test.hpp"
#include "entry_points.hpp"
#include <chrono>
#include <x86intrin.h> 

static const int   NUM_FEATURES = 2;
static const int   NUM_SAMPLES  = 4;
static const int   EPOCHS       = 1000;
static const float LR           = 0.01f;

//para rodar no hos
void TrainBatchF32Test::trainbatch_reference_implementation(
        const float X[4][2], const float y[4], float w[2], float& b)
{
    for (int e = 0; e < EPOCHS; e++)
    {
        float dW[NUM_FEATURES] = {0.f, 0.f};
        float db = 0.f;
        for (int s = 0; s < NUM_SAMPLES; s++)
        {
            float p = b;
            for (int i = 0; i < NUM_FEATURES; i++) p += w[i] * X[s][i];
            float d = 2.0f * (p - y[s]) / (float)NUM_SAMPLES;
            for (int i = 0; i < NUM_FEATURES; i++) dW[i] += X[s][i] * d;
            db += d;
        }
        for (int i = 0; i < NUM_FEATURES; i++) w[i] -= LR * dW[i];
        b -= LR * db;
    }
}

int TrainBatchF32Test::runTest()
{
    // Dados: y = 2*x0 + 3*x1 + 1
    const float X[NUM_SAMPLES][NUM_FEATURES] = {{1,2},{2,1},{3,4},{4,3}};
    float y[NUM_SAMPLES];
    for (int s = 0; s < NUM_SAMPLES; s++) y[s] = 2*X[s][0] + 3*X[s][1] + 1;

 
    uint64_t dimsInput[]   = {NUM_FEATURES, NUM_SAMPLES, 1, 1, 1};
    uint64_t dimsTargets[] = {NUM_SAMPLES, 1, 1, 1, 1};
    uint64_t dimsW[]       = {NUM_FEATURES, 1, 1, 1, 1};
    uint64_t dimsB[]       = {1, 1, 1, 1, 1};
    uint64_t dimsWout[]    = {128, 1, 1, 1, 1};
    uint64_t dimsBout[]    = {128, 1, 1, 1, 1};

    float_5DTensor input(dimsInput);
    float_5DTensor targets(dimsTargets);
    float_5DTensor w_in(dimsW);
    float_5DTensor b_in(dimsB);
    float_5DTensor w_out(dimsWout);
    float_5DTensor b_out(dimsBout);

    for (int s = 0; s < NUM_SAMPLES; s++)
    {
        for (int i = 0; i < NUM_FEATURES; i++)
        {
            int c[5] = {i, s, 0, 0, 0};
            input.SetElement(c, X[s][i]);
        }
        int cy[5] = {s, 0, 0, 0, 0};
        targets.SetElement(cy, y[s]);
    }
    int c0[5] = {0, 0, 0, 0, 0};
    int c1[5] = {1, 0, 0, 0, 0};
    w_in.SetElement(c0, 0.0f);
    w_in.SetElement(c1, 0.0f);
    b_in.SetElement(c0, 0.0f);

    // Referencia na CPU (mesmas contas do kernel)
    float w_ref[NUM_FEATURES] = {0.f, 0.f};
    float b_ref = 0.f;
    //trainbatch_reference_implementation(X, y, w_ref, b_ref);
    const int REPS = 10000;
    volatile float sink = 0.f;   // impede o compilador de apagar o laço
    auto t0 = std::chrono::steady_clock::now();
    unsigned long long tsc0 = __rdtsc();
    for (int r = 0; r < REPS; r++)
    {
        w_ref[0] = 0.f; w_ref[1] = 0.f; b_ref = 0.f;
        trainbatch_reference_implementation(X, y, w_ref, b_ref);
        sink = sink + b_ref;
    }
    unsigned long long tsc1 = __rdtsc();
    auto t1 = std::chrono::steady_clock::now();
    double ns = std::chrono::duration<double, std::nano>(t1 - t0).count() / REPS;
    std::cout << "CPU: " << ns << " ns por treino, "
              << (tsc1 - tsc0) / REPS << " ciclos" << std::endl;
    // Entrada do glue code
    m_in_defs.deviceId = tpc_lib_api::DEVICE_ID_GAUDI;

    m_in_defs.inputTensorNr = 4;
    LoadTensorToGcDescriptor(&(m_in_defs.inputTensors[0]), input);
    LoadTensorToGcDescriptor(&(m_in_defs.inputTensors[1]), targets);
    LoadTensorToGcDescriptor(&(m_in_defs.inputTensors[2]), w_in);
    LoadTensorToGcDescriptor(&(m_in_defs.inputTensors[3]), b_in);

    m_in_defs.outputTensorNr = 2;
    LoadTensorToGcDescriptor(&(m_in_defs.outputTensors[0]), w_out);
    LoadTensorToGcDescriptor(&(m_in_defs.outputTensors[1]), b_out);

    tpc_lib_api::GuidInfo *guids = nullptr;
    unsigned kernelCount = 0;
    tpc_lib_api::GlueCodeReturn result = GetKernelGuids(tpc_lib_api::DEVICE_ID_GAUDI, &kernelCount, guids);
    guids = new tpc_lib_api::GuidInfo[kernelCount];
    result = GetKernelGuids(tpc_lib_api::DEVICE_ID_GAUDI, &kernelCount, guids);
    if (result != tpc_lib_api::GLUE_SUCCESS)
    {
        std::cout << "Can't get kernel name!! " << result << std::endl;
        ReleaseKernelNames(guids, kernelCount);
        return -1;
    }

    strcpy(m_in_defs.guid.name, guids[GAUDI_KERNEL_TRAIN_BATCH_F32].name);
    //InstantiateTpcKernel, que roda o glue code e devolve as instruções do kernel
    result = InstantiateTpcKernel(&m_in_defs, &m_out_defs);
    if (result != tpc_lib_api::GLUE_SUCCESS)
    {
        std::cout << "Glue test failed, can't load kernel " << result << std::endl;
        ReleaseKernelNames(guids, kernelCount);
        return -1;
    }

    std::vector<TensorDesc2> vec;
    vec.push_back(input.GetTensorDescriptor());
    vec.push_back(targets.GetTensorDescriptor());
    vec.push_back(w_in.GetTensorDescriptor());
    vec.push_back(b_in.GetTensorDescriptor());
    vec.push_back(w_out.GetTensorDescriptor());
    vec.push_back(b_out.GetTensorDescriptor());
    TestBase::RunSimulation(vec, m_in_defs, m_out_defs);
    ReleaseKernelNames(guids, kernelCount);

    
    const float tol = 1e-4f;
    bool ok = true;
    for (int i = 0; i < NUM_FEATURES; i++)
    {
        int c[5] = {i, 0, 0, 0, 0};
        float got = w_out.ElementAt(c);
        std::cout << "w[" << i << "] kernel=" << got << " ref=" << w_ref[i] << std::endl;
        if (std::fabs(got - w_ref[i]) > tol) ok = false;
    }
    float b_got = b_out.ElementAt(c0);
    std::cout << "b    kernel=" << b_got << " ref=" << b_ref << std::endl;
    if (std::fabs(b_got - b_ref) > tol) ok = false;

    if (!ok)
    {
        std::cout << "Train batch F32 test failed!!" << std::endl;
        return -1;
    }
    std::cout << "Train batch F32 test pass!!" << std::endl;
    return 0;
}
