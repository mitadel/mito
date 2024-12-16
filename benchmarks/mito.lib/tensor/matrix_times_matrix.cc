// -*- coding: utf-8 -*-
//
// bianca giovanardi
// (c) 1998-2024 all rights reserved
//

// get the benchmark library
#include <benchmark/benchmark.h>

// get array
#include <array>
// get mito tensor library
#include <mito/tensor.h>


// strip the namespace
using namespace pyre::tensor;

auto
multiply_tensor(const auto & matrix1, const auto & matrix2)
{
    return matrix1 * matrix2;
}

auto
multiply_array(const std::array<double, 9> & matrix1, const std::array<double, 9> & matrix2)
{
    const auto size_rows = matrix1.size() / 3;
    auto result = std::array<double, 9>{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    // matrix * matrix (array)
    for (size_t i = 0; i < size_rows; ++i) {
        for (size_t j = 0; j < size_rows; ++j) {
            for (size_t k = 0; k < size_rows; ++k) {
                result[j + i * size_rows] = matrix1[k + i * size_rows] * matrix2[j + k * size_rows];
            }
        }
    }
    return result;
}

static void
MatrixTimesMatrix_Tensor(benchmark::State & state)
{
    auto matrix1 = matrix_t<3>{ 1.0, -1.0, 2.0, 1.0, 0.0, 1.0, 2.0, -1.0, 0.0 };
    auto matrix2 = matrix_t<3>{ 1.0, -1.0, 2.0, 1.0, 0.0, 1.0, 2.0, -1.0, 0.0 };

    // repeat the operation sufficient number of times
    for (auto _ : state) {
        benchmark::DoNotOptimize(multiply_tensor(matrix1, matrix2));
    }
}

static void
MatrixTimesMatrix_Array(benchmark::State & state)
{
    auto matrix1 = std::array<double, 9>{ 1.0, -1.0, 2.0, 1.0, 0.0, 1.0, 2.0, -1.0, 0.0 };
    auto matrix2 = std::array<double, 9>{ 1.0, -1.0, 2.0, 1.0, 0.0, 1.0, 2.0, -1.0, 0.0 };

    // repeat the operation sufficient number of times
    for (auto _ : state) {
        benchmark::DoNotOptimize(multiply_array(matrix1, matrix2));
    }
}

static void
MatrixTimesSymmetricMatrix_Tensor(benchmark::State & state)
{
    auto matrix1 = matrix_t<3>{ 1.0, -1.0, 2.0, 1.0, 0.0, 1.0, 2.0, -1.0, 0.0 };
    auto matrix2 = symmetric_matrix_t<3>{ 1.0,
                                          -1.0,
                                          2.0,
                                          /*-1.0,*/ 1.0,
                                          0.0,
                                          /*2.0, 0.0,*/ 1.0 };

    // repeat the operation sufficient number of times
    for (auto _ : state) {
        benchmark::DoNotOptimize(multiply_tensor(matrix1, matrix2));
    }
}

static void
SymmetricMatrixTimesSymmetricMatrix_Tensor(benchmark::State & state)
{
    auto matrix1 = symmetric_matrix_t<3>{ 1.0,
                                          -1.0,
                                          2.0,
                                          /*-1.0,*/ 1.0,
                                          0.0,
                                          /*2.0, 0.0,*/ 1.0 };
    auto matrix2 = symmetric_matrix_t<3>{ 1.0,
                                          -1.0,
                                          2.0,
                                          /*-1.0,*/ 1.0,
                                          0.0,
                                          /*2.0, 0.0,*/ 1.0 };

    // repeat the operation sufficient number of times
    for (auto _ : state) {
        benchmark::DoNotOptimize(multiply_tensor(matrix1, matrix2));
    }
}

BENCHMARK(MatrixTimesMatrix_Array);
BENCHMARK(MatrixTimesMatrix_Tensor);
BENCHMARK(MatrixTimesSymmetricMatrix_Tensor);
BENCHMARK(SymmetricMatrixTimesSymmetricMatrix_Tensor);

BENCHMARK_MAIN();


// end of file
