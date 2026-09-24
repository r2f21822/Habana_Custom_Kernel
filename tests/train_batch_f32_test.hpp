#ifndef TRAIN_BATCH_F32_TEST_HPP
#define TRAIN_BATCH_F32_TEST_HPP

#include "test_base.hpp"
#include "tensor.h"
#include "train_batch_f32.hpp"

class TrainBatchF32Test : public TestBase
{
public:
    TrainBatchF32Test() {}
    ~TrainBatchF32Test() {}
    int runTest();

    inline static void trainbatch_reference_implementation(
            const float X[4][2], const float y[4], float w[2], float& b);
private:
    TrainBatchF32Test(const TrainBatchF32Test& other) = delete;
    TrainBatchF32Test& operator=(const TrainBatchF32Test& other) = delete;
};

#endif /* TRAIN_BATCH_F32_TEST_HPP */
