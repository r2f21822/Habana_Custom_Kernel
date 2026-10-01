#ifndef XOR_TEST_HPP
#define XOR_TEST_HPP

#include "test_base.hpp"

class XorTest : public TestBase
{
public:
    XorTest() = default;
    ~XorTest() = default;

    int runTest();

    // Referencia na CPU: 2 entradas, 2 escondidos, 4 amostras
    static void xor_reference_implementation(
        const float X[2][4],
        const float W1[2][2], const float B1[2],
        const float W2[2][1], const float B2[1],
        float out[4]);

private:
    tpc_lib_api::HabanaKernelParams        m_in_defs{};
    tpc_lib_api::HabanaKernelInstantiation m_out_defs{};

    XorTest(const XorTest& other) = delete;
    XorTest& operator=(const XorTest& other) = delete;
};

#endif // XOR_TEST_HPP
