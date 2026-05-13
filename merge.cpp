#include <iostream>
#include <vector>
#include <omp.h>
#include <time.h>

using namespace std;


// Merge Function
void merge(vector<int>& arr, int start, int mid, int end) {

    vector<int> left(arr.begin() + start,
                     arr.begin() + mid + 1);

    vector<int> right(arr.begin() + mid + 1,
                      arr.begin() + end + 1);

    int i = 0, j = 0, k = start;

    while (i < left.size() && j < right.size()) {

        if (left[i] <= right[j]) {
            arr[k++] = left[i++];
        }

        else {
            arr[k++] = right[j++];
        }
    }

    while (i < left.size()) {
        arr[k++] = left[i++];
    }

    while (j < right.size()) {
        arr[k++] = right[j++];
    }
}


// Sequential Merge Sort
void merge_sort(vector<int>& arr, int start, int end) {

    if (start < end) {

        int mid = (start + end) / 2;

        merge_sort(arr, start, mid);

        merge_sort(arr, mid + 1, end);

        merge(arr, start, mid, end);
    }
}


// Parallel Merge Sort
void parallel_merge_sort(vector<int>& arr,
                         int start,
                         int end) {

    if (start < end) {

        int mid = (start + end) / 2;

        #pragma omp parallel sections
        {

            #pragma omp section
            parallel_merge_sort(arr, start, mid);

            #pragma omp section
            parallel_merge_sort(arr, mid + 1, end);
        }

        merge(arr, start, mid, end);
    }
}


int main() {

    // ===== RANDOM LARGE INPUT =====

    int n = 100;

    vector<int> arr(n);

    for(int i = 0; i < n; i++) {
        arr[i] = rand() % 100000;
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


    // Sequential Merge Sort
    start = clock();

    merge_sort(arr, 0, n - 1);

    end = clock();

    double seq_time =
    ((double)(end - start) * 1000)
    / CLOCKS_PER_SEC;

    cout << "\nSequential Merge Sort Output:\n";

    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }

    cout << "\nTime Taken : "
         << seq_time << " ms\n";


    // Parallel Merge Sort
    arr = arr_copy;

    start = clock();

    parallel_merge_sort(arr, 0, n - 1);

    end = clock();

    double par_time =
    ((double)(end - start) * 1000)
    / CLOCKS_PER_SEC;

    cout << "\nParallel Merge Sort Output:\n";

    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }

    cout << "\nTime Taken : "
         << par_time << " ms\n";

    cout << "Speedup : "
         << seq_time / par_time << endl;

    return 0;
}