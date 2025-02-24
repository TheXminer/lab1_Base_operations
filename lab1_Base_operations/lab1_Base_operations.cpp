#include <thread>
#include <iostream>
#include <chrono>
void threadFunc(const int* numbers, const long startIndex, const long length, long long& sum) {
    long long cursum = 0;
    for (int i = startIndex; i < startIndex + length; i++) {
        cursum += numbers[i] * numbers[i];
    }
    sum += cursum;//this one must be synchronized
    
}

void get_vector_norm(const long len, const long threadNumber) {

    if (len <= 0) {
        std::cout << "Error: Vector length must be greater than 0!" << std::endl;
        return;
    }
    if (threadNumber <= 0 || threadNumber > len) {
        std::cout << "Error: Number of threads must be between 1 and " << len << "!" << std::endl; \
            return;
    }

    //std::cout << "Len: " << len << " Thread number: " << threadNumber << std::endl;

    int* vector = new int[len];

    for (int i = 0; i < len; i++) {
        vector[i] = rand() % 10;
        //std::cout << vector[i] << " ";
    }

    auto threads = new std::thread[threadNumber];

    auto start_time = std::chrono::high_resolution_clock::now();

    long long sum = 0;
    int num_in_thread = len / threadNumber;
    int last_thread_num = num_in_thread;

    if (len % threadNumber != 0) {
        num_in_thread++;
        last_thread_num = len - num_in_thread * (len - 1);
    }

    for (int i = 0; i < threadNumber - 1; i++) {
        threads[i] = std::thread(threadFunc, vector, i * num_in_thread, num_in_thread, std::ref(sum));
    }

    threads[threadNumber - 1] =
        std::thread(threadFunc, vector, (threadNumber - 1) * num_in_thread, num_in_thread, std::ref(sum));

    for (int i = 0; i < threadNumber; i++) {
        threads[i].join();
    }

    float vector_norm = sqrt(sum);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::cout << "Norm: " << vector_norm << std::endl;
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() * 1e-9 << std::endl << std::endl;
}

void singleThreadNorm(long vector_len) {
    long long sum = 0;
    int* vector = new int[vector_len];
    for (int i = 0; i < vector_len; i++) {
        vector[i] = rand() % 10;
        //std::cout << vector[i] << " ";
    }
    auto start_time = std::chrono::high_resolution_clock::now();
    threadFunc(vector, 0, vector_len, sum);
    double vector_norm = sqrt(sum);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::cout << "Norm: " << vector_norm << std::endl;
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() * 1e-9 << std::endl << std::endl;
}

int main()
{
    srand(time(0));
    long vector_len = 100000000;

    singleThreadNorm(vector_len);
    get_vector_norm(vector_len, 3);
    get_vector_norm(vector_len, 6);
    get_vector_norm(vector_len, 12);
    get_vector_norm(vector_len, 24);
    get_vector_norm(vector_len, 48);
    get_vector_norm(vector_len, 96);
    get_vector_norm(vector_len, 192);
}