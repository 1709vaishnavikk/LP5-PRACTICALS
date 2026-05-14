%%writefile vector_add.cu

#include <iostream>
#include <cuda_runtime.h>
#include <time.h>

using namespace std;

// GPU Kernel
__global__ void addVectors(int* A, int* B, int* C, int n)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < n)
    {
        C[i] = A[i] + B[i];
    }
}

int main()
{
    int n = 100000;

    int size = n * sizeof(int);

    // Host arrays
    int *A, *B, *C_gpu, *C_cpu;

    cudaMallocHost(&A, size);
    cudaMallocHost(&B, size);
    cudaMallocHost(&C_gpu, size);

    C_cpu = new int[n];

    // Initialize vectors
    for (int i = 0; i < n; i++)
    {
        A[i] = i;
        B[i] = i * 2;
    }

    // ---------------- CPU ADDITION ----------------

    clock_t cpu_start, cpu_end;

    cpu_start = clock();

    for (int i = 0; i < n; i++)
    {
        C_cpu[i] = A[i] + B[i];
    }

    cpu_end = clock();

    double cpu_time =
        ((double)(cpu_end - cpu_start)) / CLOCKS_PER_SEC;

    // ---------------- GPU ADDITION ----------------

    int *dev_A, *dev_B, *dev_C;

    cudaMalloc(&dev_A, size);
    cudaMalloc(&dev_B, size);
    cudaMalloc(&dev_C, size);

    clock_t gpu_start, gpu_end;

    gpu_start = clock();

    // Copy CPU -> GPU
    cudaMemcpy(dev_A, A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(dev_B, B, size, cudaMemcpyHostToDevice);

    // Launch kernel
    int blockSize = 256;

    int numBlocks =
        (n + blockSize - 1) / blockSize;

    addVectors<<<numBlocks, blockSize>>>
    (dev_A, dev_B, dev_C, n);

    cudaDeviceSynchronize();

    // Copy GPU -> CPU
    cudaMemcpy(C_gpu, dev_C, size,
               cudaMemcpyDeviceToHost);

    gpu_end = clock();

    double gpu_time =
        ((double)(gpu_end - gpu_start)) / CLOCKS_PER_SEC;

    // ---------------- OUTPUT ----------------

    cout << "First 10 Result Elements:\n";

    for (int i = 0; i < 10; i++)
    {
        cout << C_gpu[i] << " ";
    }

    cout << endl;

    cout << "\nCPU Execution Time: "
         << cpu_time << " seconds";

    cout << "\nGPU Execution Time: "
         << gpu_time << " seconds\n";

    // Free memory
    cudaFree(dev_A);
    cudaFree(dev_B);
    cudaFree(dev_C);

    cudaFreeHost(A);
    cudaFreeHost(B);
    cudaFreeHost(C_gpu);

    delete[] C_cpu;

    return 0;
}
