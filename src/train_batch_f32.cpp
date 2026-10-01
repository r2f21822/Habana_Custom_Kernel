
#include <vector>
#include <cstring>
#include <iostream>
#include "train_batch_f32.hpp"

extern unsigned char _binary___train_batch_f32_o_start;
extern unsigned char _binary___train_batch_f32_o_end;

// Deve bater com o kernel: NUM_FEATURES=2, NUM_SAMPLES=4

static const uint64_t c_numFeatures = 2;
static const uint64_t c_numSamples  = 264;
static const uint64_t c_scratchSize = 64;

// Kernel de 1 thread, o unico work item acessa o tensor INTEIRO

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

//Só devolve o nome do kernel "custom_train_batch_f32". É o nome que o teste (e o entry_points.cpp) encontra o kernel
tpc_lib_api::GlueCodeReturn TrainBatchF32::GetKernelName(
        char kernelName [tpc_lib_api::MAX_NODE_NAME])
{
    strcpy(kernelName, "custom_train_batch_f32");
    return tpc_lib_api::GLUE_SUCCESS;
}


/*Confere se a chamada faz sentido:

4 tensores de entrada e 2 de saída. 
Se não, devolve erro.
Os tamanhos são os esperados ([2,4], [4], [2], [1]).
Todos são float32.*/
tpc_lib_api::GlueCodeReturn TrainBatchF32::GetGcDefinitions(
        tpc_lib_api::HabanaKernelParams* in_defs,
        tpc_lib_api::HabanaKernelInstantiation* out_defs)
{
    tpc_lib_api::GlueCodeReturn retVal;

    // se tem 4 entradas: input, targets, weights_in, bias_in
    if (in_defs->inputTensorNr != 4)
    {
        in_defs->inputTensorNr = 4;
        return tpc_lib_api::GLUE_INCOMPATIBLE_INPUT_COUNT;
    }
    
    // se tem 3 saidas: weights_out, bias_out, scratch
    if (in_defs->outputTensorNr != 3)
    {
        in_defs->outputTensorNr = 3;
        return tpc_lib_api::GLUE_INCOMPATIBLE_OUTPUT_COUNT;
    }


    const uint64_t* inSz  = in_defs->inputTensors[0].geometry.maxSizes;  // [2,4]
    const uint64_t* tgSz  = in_defs->inputTensors[1].geometry.maxSizes;  // [4]
    const uint64_t* wSz   = in_defs->inputTensors[2].geometry.maxSizes;  // [2]
    const uint64_t* bSz   = in_defs->inputTensors[3].geometry.maxSizes;  // [1]
    if (inSz[0] != c_numFeatures || inSz[1] != c_numSamples ||
        tgSz[0] != c_numSamples  ||
        wSz[0]  != c_numFeatures ||
        bSz[0]  != 1)
    {
        return tpc_lib_api::GLUE_INCOMPATIBLE_INPUT_SIZE;
    }
    
        const uint64_t* scratchSz = in_defs->outputTensors[2].geometry.maxSizes;
    if (scratchSz[0] != c_scratchSize)
    {
        return tpc_lib_api::GLUE_INCOMPATIBLE_OUTPUT_SIZE;
    }

    // tipos: tudo F32
    bool typesOk = true;
    for (unsigned i = 0; i < 4; i++)
        if (in_defs->inputTensors[i].geometry.dataType != tpc_lib_api::DATA_F32) typesOk = false;
    for (unsigned i = 0; i < 3; i++)
        if (in_defs->outputTensors[i].geometry.dataType != tpc_lib_api::DATA_F32) typesOk = false;
    if (!typesOk)
    {
        for (unsigned i = 0; i < 4; i++)
            in_defs->inputTensors[i].geometry.dataType = tpc_lib_api::DATA_F32;
        for (unsigned i = 0; i < 3; i++)
            in_defs->outputTensors[i].geometry.dataType = tpc_lib_api::DATA_F32;
        return tpc_lib_api::GLUE_INCOMPATIBLE_DATA_TYPE;
    }
    
    
    
    //*******************************
//divide o trabalho em pedaços, e cada pedaço roda o kernel uma vez. Aqui só existe 1 pedaço. É por isso que o kernel roda numa única thread TPC
    out_defs->indexSpaceRank = 1;
    out_defs->indexSpaceGeometry[0] = 1;

//Diz qual parte de cada tensor cada pedaço enxerga. Como só há 1 pedaço e ele precisa de tudo, cada tensor é mapeado inteiro: do índice 0 até tamanho - 1 em cada dimensão. 
//end_b é o último índice do tensor que o pedaço de trabalho vai acessar. Como a contagem de índices começa em 0, o último índice é sempre tamanho - 1.
//O targets tem 4 elementos, e os índices deles são:
    for (unsigned i = 0; i < in_defs->inputTensorNr; i++)
        mapWholeTensor(out_defs->inputTensorAccessPattern[i],
                       in_defs->inputTensors[i].geometry.maxSizes);

    for (unsigned i = 0; i < in_defs->outputTensorNr; i++)
        mapWholeTensor(out_defs->outputTensorAccessPattern[i],
                       in_defs->outputTensors[i].geometry.maxSizes);


    unsigned IsaSize = (&_binary___train_batch_f32_o_end - &_binary___train_batch_f32_o_start);
    unsigned givenBinarySize = out_defs->kernel.elfSize;
    out_defs->kernel.elfSize = IsaSize;

    if (givenBinarySize >= IsaSize)
    {
        memcpy(out_defs->kernel.kernelElf,
               &_binary___train_batch_f32_o_start,
               IsaSize);
    }
    else
    {
        retVal = tpc_lib_api::GLUE_INSUFFICIENT_ELF_BUFFER;
        return retVal;
    }

    return tpc_lib_api::GLUE_SUCCESS;
}


