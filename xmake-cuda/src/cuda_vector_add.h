#ifndef CUDA_VECTOR_ADD_H
#define CUDA_VECTOR_ADD_H

#include <cuda_runtime_api.h>

cudaError_t addWithCuda(int* c, const int* a, const int* b, unsigned int size);

#endif // CUDA_VECTOR_ADD_H
