#include <iostream>
#include <vector>
#include <omp.h>
#include <time.h>

using namespace std;


// Sequential Bubble Sort
void bubble_sort(vector<int>& arr, int n) {

    for (int i = 0; i < n - 1; i++) {

        for (int j = 0; j < n - i - 1; j++) {

            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}


// Parallel Bubble Sort
void parallel_bubble_sort(vector<int>& arr, int n) {

    bool sorted = false;

    while (!sorted) {

        sorted = true;

        // Even phase
        #pragma omp parallel for shared(arr, sorted)
        for (int i = 0; i < n - 1; i += 2) {

            if (arr[i] > arr[i + 1]) {

                swap(arr[i], arr[i + 1]);

                sorted = false;
            }
        }

        // Odd phase
        #pragma omp parallel for shared(arr, sorted)
        for (int i = 1; i < n - 1; i += 2) {

            if (arr[i] > arr[i + 1]) {

                swap(arr[i], arr[i + 1]);

                sorted = false;
            }
        }
    }
}


int main() {

    // ===== RANDOM LARGE INPUT =====

    int n = 5000;

    vector<int> arr(n);

    for(int i = 0; i < n; i++) {
        arr[i] = rand() % 10000;
    }



    /*
    ===== MANUAL INPUT =====

    int n;

    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);

    cout << "Enter elements:\n";

    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    */


    vector<int> arr_copy = arr;

    clock_t start, end;


    // Sequential Bubble Sort
    start = clock();

    bubble_sort(arr, n);

    end = clock();

    double seq_time =
    ((double)(end - start) * 1000)
    / CLOCKS_PER_SEC;

    cout << "\nSequential Bubble Sort Output:\n";

    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }

    cout << "\nTime Taken : "
         << seq_time << " ms\n";


    // Parallel Bubble Sort
    arr = arr_copy;

    start = clock();

    parallel_bubble_sort(arr, n);

    end = clock();

    double par_time =
    ((double)(end - start) * 1000)
    / CLOCKS_PER_SEC;

    cout << "\nParallel Bubble Sort Output:\n";

    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }

    cout << "\nTime Taken : "
         << par_time << " ms\n";

    cout << "Speedup : "
         << seq_time / par_time << endl;

    return 0;
}