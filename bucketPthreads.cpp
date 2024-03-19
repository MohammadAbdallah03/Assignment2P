#include <iostream>
#include <vector>
#include <pthread.h>
#include <ctime>
#include <thread>

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

struct ThreadArgs {
    vector<int>* bucket;
};

void* threadFunc(void* arg) {
    cout << "Helloo from thread " << std::this_thread::get_id() << "\n";
    ThreadArgs* threadArgs = reinterpret_cast<ThreadArgs*>(arg);
    vector<int>& bucket = *(threadArgs->bucket);
    insertionSort(bucket);
    return nullptr;
}

void bucketSort(vector<int>& arr, int bucks) {
    int n = arr.size();
    vector<int> buckets[bucks];

    int mn = arr[0], mx = arr[0];
    for (int i = 0; i < n; ++i) {
        mn = min(mn, arr[i]);
        mx = max(mx, arr[i]);
    }
    int buckSize = ((mx - mn) / bucks) + 1;

    for (int i = 0; i < n; i++) {
        int pos = (arr[i] - mn) / buckSize;
        buckets[pos].push_back(arr[i]);
    }

    pthread_t threads[bucks];
    ThreadArgs threadArgs[bucks];
    for (int i = 0; i < bucks; ++i) {
        threadArgs[i].bucket = &buckets[i];
        pthread_create(&threads[i], nullptr, threadFunc, &threadArgs[i]);
    }

    for (int i = 0; i < bucks; ++i) {
        pthread_join(threads[i], nullptr);
    }

    int index = 0;
    for (int i = 0; i < bucks; i++) {
        for (int j = 0; j < buckets[i].size(); j++) {
            arr[index++] = buckets[i][j];
        }
    }
}

int main() {
    int n, bucks;
    cout << "Please enter the nb of buckets\n";
    cin >> bucks;
    cout << "Please enter the size of the array\n";
    cin >> n;

    vector<int> arr(n);

    // Generate random numbers for the array
    for (int i = 0; i < n; ++i) {
        arr[i] = rand();
    }

    clock_t start = clock();
    bucketSort(arr, bucks);
    clock_t end = clock();
    double total_time = ((double)(end - start)) / CLOCKS_PER_SEC;


    cout << "Execution time is " << total_time << " ms \n";

    return 0;
}