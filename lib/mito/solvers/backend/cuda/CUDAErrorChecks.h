// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


// code guard
#pragma once


// CUDA error checking macro
#define CHECK_CUDA_ERROR(call)                                                   \
    do {                                                                         \
        cudaError_t err = call;                                                  \
        if (err != cudaSuccess) {                                                \
            fprintf(stderr, "CUDA error at %s:%d\n", __FILE__, __LINE__);        \
            fprintf(stderr, "  Error: %s (%d)\n", cudaGetErrorString(err), err); \
            exit(EXIT_FAILURE);                                                  \
        }                                                                        \
    } while (0)

// function to convert cuSOLVER error codes to strings
inline const char *
cusolverGetErrorString(cusolverStatus_t status)
{
    switch (status) {
        case CUSOLVER_STATUS_SUCCESS:
            return "CUSOLVER_STATUS_SUCCESS";
        case CUSOLVER_STATUS_NOT_INITIALIZED:
            return "CUSOLVER_STATUS_NOT_INITIALIZED";
        case CUSOLVER_STATUS_ALLOC_FAILED:
            return "CUSOLVER_STATUS_ALLOC_FAILED";
        case CUSOLVER_STATUS_INVALID_VALUE:
            return "CUSOLVER_STATUS_INVALID_VALUE";
        case CUSOLVER_STATUS_ARCH_MISMATCH:
            return "CUSOLVER_STATUS_ARCH_MISMATCH";
        case CUSOLVER_STATUS_MAPPING_ERROR:
            return "CUSOLVER_STATUS_MAPPING_ERROR";
        case CUSOLVER_STATUS_EXECUTION_FAILED:
            return "CUSOLVER_STATUS_EXECUTION_FAILED";
        case CUSOLVER_STATUS_INTERNAL_ERROR:
            return "CUSOLVER_STATUS_INTERNAL_ERROR";
        case CUSOLVER_STATUS_MATRIX_TYPE_NOT_SUPPORTED:
            return "CUSOLVER_STATUS_MATRIX_TYPE_NOT_SUPPORTED";
        default:
            return "Unknown cuSOLVER error";
    }
}

// cuSOLVER error checking macro
#define CHECK_CUSOLVER_ERROR(call)                                                                 \
    do {                                                                                           \
        cusolverStatus_t status = (call);                                                          \
        if (status != CUSOLVER_STATUS_SUCCESS) {                                                   \
            fprintf(                                                                               \
                stderr, "cuSOLVER error: %s at %s:%d\n", cusolverGetErrorString(status), __FILE__, \
                __LINE__);                                                                         \
            exit(EXIT_FAILURE);                                                                    \
        }                                                                                          \
    } while (0)
