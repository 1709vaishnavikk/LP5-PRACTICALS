#include <iostream>
#include <vector>
#include <omp.h>
#include <time.h>

using namespace std;


// ================= SEQUENTIAL FUNCTIONS =================

// Sequential Sum
int seqSum(vector<int>& arr) {

    int sum = 0;

    for(int i = 0; i < arr.size(); i++) {
        sum += arr[i];
    }

    return sum;
}


// Sequential Average
double seqAvg(vector<int>& arr) {

    int sum = seqSum(arr);

    return (double)sum / arr.size();
}


// Sequential Minimum
int seqMin(vector<int>& arr) {

    int min_val = arr[0];

    for(int i = 0; i < arr.size(); i++) {

        if(arr[i] < min_val)
            min_val = arr[i];
    }

    return min_val;
}


// Sequential Maximum
int seqMax(vector<int>& arr) {

    int max_val = arr[0];

    for(int i = 0; i < arr.size(); i++) {

        if(arr[i] > max_val)
            max_val = arr[i];
    }

    return max_val;
}



// ================= PARALLEL FUNCTIONS =================

// Parallel Sum
int parSum(vector<int>& arr) {

    int sum = 0;

    #pragma omp parallel for reduction(+:sum)
    for(int i = 0; i < arr.size(); i++) {

        sum += arr[i];
    }

    return sum;
}


// Parallel Average
double parAvg(vector<int>& arr) {

    int sum = parSum(arr);

    return (double)sum / arr.size();
}


// Parallel Minimum
int parMin(vector<int>& arr) {

    int min_val = arr[0];

    #pragma omp parallel for reduction(min:min_val)
    for(int i = 0; i < arr.size(); i++) {

        if(arr[i] < min_val)
            min_val = arr[i];
    }

    return min_val;
}


// Parallel Maximum
int parMax(vector<int>& arr) {

    int max_val = arr[0];

    #pragma omp parallel for reduction(max:max_val)
    for(int i = 0; i < arr.size(); i++) {

        if(arr[i] > max_val)
            max_val = arr[i];
    }

    return max_val;
}




// ================= MAIN FUNCTION =================

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


    // ===== DISPLAY ARRAY =====

    cout << "Array Elements:\n";

    for(int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }

    cout << "\n";


    clock_t start, end;



    // ================= SUM =================

    start = clock();

    int s1 = seqSum(arr);

    end = clock();

    double seq_sum_time =
    ((double)(end - start) * 1000)
    / CLOCKS_PER_SEC;

    cout << "\nSequential Sum = " << s1;

    cout << "\nTime = "
         << seq_sum_time
         << " ms\n";


    start = clock();

    int s2 = parSum(arr);

    end = clock();

    double par_sum_time =
    ((double)(end - start) * 1000)
    / CLOCKS_PER_SEC;

    cout << "\nParallel Sum = " << s2;

    cout << "\nTime = "
         << par_sum_time
         << " ms\n";

    cout << "Speedup = "
         << seq_sum_time / par_sum_time
         << "\n";



    // ================= AVERAGE =================

    start = clock();

    double a1 = seqAvg(arr);

    end = clock();

    double seq_avg_time =
    ((double)(end - start) * 1000)
    / CLOCKS_PER_SEC;

    cout << "\nSequential Average = " << a1;

    cout << "\nTime = "
         << seq_avg_time
         << " ms\n";


    start = clock();

    double a2 = parAvg(arr);

    end = clock();

    double par_avg_time =
    ((double)(end - start) * 1000)
    / CLOCKS_PER_SEC;

    cout << "\nParallel Average = " << a2;

    cout << "\nTime = "
         << par_avg_time
         << " ms\n";

    cout << "Speedup = "
         << seq_avg_time / par_avg_time
         << "\n";



    // ================= MINIMUM =================

    start = clock();

    int mn1 = seqMin(arr);

    end = clock();

    double seq_min_time =
    ((double)(end - start) * 1000)
    / CLOCKS_PER_SEC;

    cout << "\nSequential Minimum = " << mn1;

    cout << "\nTime = "
         << seq_min_time
         << " ms\n";


    start = clock();

    int mn2 = parMin(arr);

    end = clock();

    double par_min_time =
    ((double)(end - start) * 1000)
    / CLOCKS_PER_SEC;

    cout << "\nParallel Minimum = " << mn2;

    cout << "\nTime = "
         << par_min_time
         << " ms\n";

    cout << "Speedup = "
         << seq_min_time / par_min_time
         << "\n";



    // ================= MAXIMUM =================

    start = clock();

    int mx1 = seqMax(arr);

    end = clock();

    double seq_max_time =
    ((double)(end - start) * 1000)
    / CLOCKS_PER_SEC;

    cout << "\nSequential Maximum = " << mx1;

    cout << "\nTime = "
         << seq_max_time
         << " ms\n";


    start = clock();

    int mx2 = parMax(arr);

    end = clock();

    double par_max_time =
    ((double)(end - start) * 1000)
    / CLOCKS_PER_SEC;

    cout << "\nParallel Maximum = " << mx2;

    cout << "\nTime = "
         << par_max_time
         << " ms\n";

    cout << "Speedup = "
         << seq_max_time / par_max_time
         << "\n";


    return 0;
}