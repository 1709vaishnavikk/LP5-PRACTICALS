%%writefile matmul.cu

#include <cuda_runtime.h>
#include <iostream>
#include <time.h>

using namespace std;

// GPU Kernel
__global__ void matmul(int* A, int* B, int* C, int N)
{
    int Row = blockIdx.y * blockDim.y + threadIdx.y;

    int Col = blockIdx.x * blockDim.x + threadIdx.x;

    if (Row < N && Col < N)
    {
        int Pvalue = 0;

        for (int k = 0; k < N; k++)
        {
            Pvalue +=
                A[Row * N + k] *
                B[k * N + Col];
        }

        C[Row * N + Col] = Pvalue;
    }
}

int main()
{
    int N = 512;

    int size = N * N * sizeof(int);

    // Host matrices
    int *A, *B, *C_gpu, *C_cpu;

    cudaMallocHost(&A, size);
    cudaMallocHost(&B, size);
    cudaMallocHost(&C_gpu, size);

    C_cpu = new int[N * N];

    // Initialize matrices
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            A[i * N + j] = i + j;

            B[i * N + j] = i - j;
        }
    }

    // ---------------- CPU MATRIX MULTIPLICATION ----------------

    clock_t cpu_start, cpu_end;

    cpu_start = clock();

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int sum = 0;

            for (int k = 0; k < N; k++)
            {
                sum +=
                    A[i * N + k] *
                    B[k * N + j];
            }

            C_cpu[i * N + j] = sum;
        }
    }

    cpu_end = clock();

    double cpu_time =
        ((double)(cpu_end - cpu_start))
        / CLOCKS_PER_SEC;

    // ---------------- GPU MATRIX MULTIPLICATION ----------------

    int *dev_A, *dev_B, *dev_C;

    cudaMalloc(&dev_A, size);
    cudaMalloc(&dev_B, size);
    cudaMalloc(&dev_C, size);

    clock_t gpu_start, gpu_end;

    gpu_start = clock();

    // Copy CPU -> GPU
    cudaMemcpy(dev_A, A, size,
               cudaMemcpyHostToDevice);

    cudaMemcpy(dev_B, B, size,
               cudaMemcpyHostToDevice);

    // Block and Grid size
    dim3 dimBlock(16, 16);

    dim3 dimGrid(
        (N + dimBlock.x - 1) / dimBlock.x,
        (N + dimBlock.y - 1) / dimBlock.y
    );

    // Launch kernel
    matmul<<<dimGrid, dimBlock>>>
    (dev_A, dev_B, dev_C, N);

    cudaDeviceSynchronize();

    // Copy GPU -> CPU
    cudaMemcpy(C_gpu, dev_C, size,
               cudaMemcpyDeviceToHost);

    gpu_end = clock();

    double gpu_time =
        ((double)(gpu_end - gpu_start))
        / CLOCKS_PER_SEC;

    // ---------------- OUTPUT ----------------

    cout << "First 10x10 Result Matrix:\n\n";

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            cout << C_gpu[i * N + j]
                 << " ";
        }

        cout << endl;
    }

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
