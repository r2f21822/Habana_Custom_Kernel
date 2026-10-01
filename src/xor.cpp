#include <cstring>
#include "xor.hpp"

// Simbolos gerados a partir do objeto xor.o (mesmo padrao do kernel anterior)
extern unsigned char _binary___xor_o_start;
extern unsigned char _binary___xor_o_end;

// XOR: 2 entradas, 4 amostras, 2 neuronios escondidos
static const uint64_t c_numFeatures = 2;
static const uint64_t c_numSamples  = 4;
static const uint64_t c_numHidden   = 2;

// Cada work-item enxerga o tensor inteiro
template <typename AccessPattern>
static void mapWholeTensor(AccessPattern& ap, const uint64_t* sizes)
{
    for (unsigned d = 0; d < gcapi::MAX_TENSOR_DIM; d++)
    {
        ap.mapping[d].indexSpaceDim = 0;
        ap.mapping[d].a             = 0;
        ap.mapping[d].start_b       = 0;
        ap.mapping[d].end_b         = (sizes[d] > 0) ? (int)sizes[d] - 1 : 0;
    }
}

// GUID do kernel: deve ser o mesmo do entry_points.cpp e do teste
tpc_lib_api::GlueCodeReturn Xor::GetKernelName(
        char kernelName[tpc_lib_api::MAX_NODE_NAME])
{
    strcpy(kernelName, "custom_xor");
    return tpc_lib_api::GLUE_SUCCESS;
}

tpc_lib_api::GlueCodeReturn Xor::GetGcDefinitions(
        tpc_lib_api::HabanaKernelParams*        in_defs,
        tpc_lib_api::HabanaKernelInstantiation* out_defs)
{
    // 1. Quantidade de tensores: 5 entradas (X, W1, B1, W2, B2) e 1 saida
    if (in_defs->inputTensorNr != 5)
    {
        in_defs->inputTensorNr = 5;
        return tpc_lib_api::GLUE_INCOMPATIBLE_INPUT_COUNT;
    }
    if (in_defs->outputTensorNr != 1)
    {
        in_defs->outputTensorNr = 1;
        return tpc_lib_api::GLUE_INCOMPATIBLE_OUTPUT_COUNT;
    }

    // 2. Tamanhos
    const uint64_t* inSz  = in_defs->inputTensors[0].geometry.maxSizes;  // [2, 4]
    const uint64_t* w1Sz  = in_defs->inputTensors[1].geometry.maxSizes;  // [2, 2]
    const uint64_t* b1Sz  = in_defs->inputTensors[2].geometry.maxSizes;  // [2]
    const uint64_t* w2Sz  = in_defs->inputTensors[3].geometry.maxSizes;  // [2, 1]
    const uint64_t* b2Sz  = in_defs->inputTensors[4].geometry.maxSizes;  // [1]
    const uint64_t* outSz = in_defs->outputTensors[0].geometry.maxSizes; // [1, 4]

    if (inSz[0]  != c_numFeatures || inSz[1] != c_numSamples ||
        w1Sz[0]  != c_numFeatures || w1Sz[1] != c_numHidden  ||
        b1Sz[0]  != c_numHidden   ||
        w2Sz[0]  != c_numHidden   || w2Sz[1] != 1            ||
        b2Sz[0]  != 1             ||
        outSz[0] != 1             || outSz[1]  != c_numSamples)
    {
        return tpc_lib_api::GLUE_INCOMPATIBLE_INPUT_SIZE;
    }

    // 3. Tipos: tudo float32
    bool typesOk = true;
    for (unsigned i = 0; i < 5; i++)
        if (in_defs->inputTensors[i].geometry.dataType != tpc_lib_api::DATA_F32)
            typesOk = false;
    if (in_defs->outputTensors[0].geometry.dataType != tpc_lib_api::DATA_F32)
        typesOk = false;

    if (!typesOk)
    {
        for (unsigned i = 0; i < 5; i++)
            in_defs->inputTensors[i].geometry.dataType = tpc_lib_api::DATA_F32;
        in_defs->outputTensors[0].geometry.dataType = tpc_lib_api::DATA_F32;
        return tpc_lib_api::GLUE_INCOMPATIBLE_DATA_TYPE;
    }

    // 4. Index space: 1 work-item que acessa os tensores inteiros
    out_defs->indexSpaceRank        = 1;
    out_defs->indexSpaceGeometry[0] = 1;

    for (unsigned i = 0; i < 5; i++)
        mapWholeTensor(out_defs->inputTensorAccessPattern[i],
                       in_defs->inputTensors[i].geometry.maxSizes);

    mapWholeTensor(out_defs->outputTensorAccessPattern[0],
                   in_defs->outputTensors[0].geometry.maxSizes);

    // 5. Copia o ELF do kernel
    unsigned isaSize         = (unsigned)(&_binary___xor_o_end -
                                          &_binary___xor_o_start);
    unsigned givenBinarySize = out_defs->kernel.elfSize;
    out_defs->kernel.elfSize = isaSize;

    if (givenBinarySize < isaSize)
        return tpc_lib_api::GLUE_INSUFFICIENT_ELF_BUFFER;

    memcpy(out_defs->kernel.kernelElf, &_binary___xor_o_start, isaSize);
    return tpc_lib_api::GLUE_SUCCESS;
}
