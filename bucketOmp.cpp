#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

void insertionSort(vector<int>& bucket) {
    for (int i = 1; i < bucket.size(); ++i) {
        int key = bucket[i];
        int j = i - 1;
        while (j >= 0 && bucket[j] > key) {
            bucket[j + 1] = bucket[j];
            j--;
        }
        bucket[j + 1] = key;
    }
}

void bucketSort(vector<int> arr, int n, int bucks) {


    vector<int> buckets[bucks];

    int mn = arr[0] , mx = arr[0];
    for (int i = 0; i < n; ++i) {
        mn = min(mn, arr[i]);
        mx = max(mx,arr[i]);
    }
    int buckSize = ((mx -mn ) / bucks)  +1;


    for (int i = 0; i < n; i++) {
        int pos = (arr[i] - mn)/ buckSize;
        buckets[pos].push_back(arr[i]);
    }


    // 3) Sort individual buckets using insertion sort
    omp_set_num_threads(bucks);
    #pragma omp parallel for
    for (int i = 0; i < bucks; i++) {
        printf("This is thread %d\n" ,omp_get_thread_num() );
        insertionSort(buckets[i]);
    }


    // 4) Concatenate all buckets into arr[]
    int index = 0;
    for (int i = 0; i < bucks; i++) {
        for (int j = 0; j < buckets[i].size(); j++) {
            arr[index++] = buckets[i][j];
        }
    }
}

// Driver program to test above function
int main() {
    int n , bucks;
    cout << "Please enter the nb of buckets\n";
    cin >> bucks;
    cout << "Please enter the size of the array\n";
    cin >> n;
    vector<int> arr(n) ;


    for (int i = 0; i < n; ++i) {
        arr[i] =rand();
    }
    clock_t start = clock();
    bucketSort(arr, n, bucks);
    clock_t end = clock();
    double total_time = ((double)(end-start)) / CLOCKS_PER_SEC;


    cout << "Execution time is " << total_time << " s \n";
    return 0;
}