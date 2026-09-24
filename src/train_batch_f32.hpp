#ifndef _TRAIN_BATCH_F32_HPP
#define _TRAIN_BATCH_F32_HPP

#include "gc_interface.h"
#include "tpc_kernel_lib_interface.h"

class TrainBatchF32
{
    public:
        TrainBatchF32() {}
        virtual ~TrainBatchF32() {}

        virtual tpc_lib_api::GlueCodeReturn
        GetGcDefinitions(tpc_lib_api::HabanaKernelParams*      in_defs,
                         tpc_lib_api::HabanaKernelInstantiation* out_defs);

        virtual tpc_lib_api::GlueCodeReturn GetKernelName(
                char kernelName [tpc_lib_api::MAX_NODE_NAME]);

    private:
        TrainBatchF32(const TrainBatchF32& other) = delete;
        TrainBatchF32& operator=(const TrainBatchF32& other) = delete;
};

#endif
