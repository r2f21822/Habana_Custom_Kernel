#ifndef _XOR_HPP
#define _XOR_HPP

#include "gc_interface.h"
#include "tpc_kernel_lib_interface.h"

class Xor
{
public:
    Xor() {}
    virtual ~Xor() {}

    virtual tpc_lib_api::GlueCodeReturn
    GetGcDefinitions(tpc_lib_api::HabanaKernelParams*        in_defs,
                     tpc_lib_api::HabanaKernelInstantiation* out_defs);

    virtual tpc_lib_api::GlueCodeReturn
    GetKernelName(char kernelName[tpc_lib_api::MAX_NODE_NAME]);

private:
    Xor(const Xor& other) = delete;
    Xor& operator=(const Xor& other) = delete;
};

#endif
