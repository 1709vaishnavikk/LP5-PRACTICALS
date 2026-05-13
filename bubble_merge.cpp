#include <iostream>
#include <vector>
#include <omp.h>
#include <time.h>

using namespace std;


// ===== BUBBLE SORT =====

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

        // Even Phase
        #pragma omp parallel for shared(arr, sorted)
        for (int i = 0; i < n - 1; i += 2) {

            if (arr[i] > arr[i + 1]) {

                swap(arr[i], arr[i + 1]);

                sorted = false;
            }
        }

        // Odd Phase
        #pragma omp parallel for shared(arr, sorted)
        for (int i = 1; i < n - 1; i += 2) {

            if (arr[i] > arr[i + 1]) {

                swap(arr[i], arr[i + 1]);

                sorted = false;
            }
        }
    }
}



// ===== MERGE SORT =====

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

    // ===== RANDOM INPUT =====

    int n = 100;

    vector<int> arr(n);

    for(int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;
    }



    /*
    ===== MANUAL INPUT =====

    int n;

    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);

    cout << "Enter elements:\n";

    for(int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    */


    vector<int> arr_copy = arr;

    clock_t start, end;



    // ===== ORIGINAL ARRAY =====

    cout << "Original Array:\n";

    for(int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }

    cout << "\n\n";



    // ===== BUBBLE SORT =====

    // Sequential Bubble Sort
    start = clock();

    bubble_sort(arr, n);

    end = clock();

    double bubble_seq =
    ((double)(end - start) * 1000)
    / CLOCKS_PER_SEC;

    cout << "Sequential Bubble Sort Output:\n";

    for(int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }

    cout << "\nTime Taken : "
         << bubble_seq << " ms\n\n";


    // Parallel Bubble Sort
    arr = arr_copy;

    start = clock();

    parallel_bubble_sort(arr, n);

    end = clock();

    double bubble_par =
    ((double)(end - start) * 1000)
    / CLOCKS_PER_SEC;

    cout << "Parallel Bubble Sort Output:\n";

    for(int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }

    cout << "\nTime Taken : "
         << bubble_par << " ms\n";

    cout << "Bubble Speedup : "
         << bubble_seq / bubble_par << "\n\n";



    // ===== MERGE SORT =====

    // Sequential Merge Sort
    arr = arr_copy;

    start = clock();

    merge_sort(arr, 0, n - 1);

    end = clock();

    double merge_seq =
    ((double)(end - start) * 1000)
    / CLOCKS_PER_SEC;

    cout << "Sequential Merge Sort Output:\n";

    for(int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }

    cout << "\nTime Taken : "
         << merge_seq << " ms\n\n";


    // Parallel Merge Sort
    arr = arr_copy;

    start = clock();

    parallel_merge_sort(arr, 0, n - 1);

    end = clock();

    double merge_par =
    ((double)(end - start) * 1000)
    / CLOCKS_PER_SEC;

    cout << "Parallel Merge Sort Output:\n";

    for(int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }

    cout << "\nTime Taken : "
         << merge_par << " ms\n";

    cout << "Merge Speedup : "
         << merge_seq / merge_par << endl;

    return 0;
}