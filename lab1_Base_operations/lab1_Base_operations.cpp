#include <thread>
#include <iostream>
#include <chrono>

void threadFunc(const int* numbers, const int startIndex, const int length, int& sum) {
    int cursum = 0;
    for (int i = startIndex; i < startIndex + length; i++) {
        cursum += numbers[i] * numbers[i];
    }
    sum += cursum;//this one must be synchronized
    
}

int main()
{
    srand(time(0));
    int len;
    std::cout << "Enter length of your vector: ";
    std::cin >> len;
    int* vector = new int[len];

    for (int i = 0; i < len; i++) {
        vector[i] = rand() % 10;
        std::cout << vector[i] << std::endl;
    }

    int threadNumber;
    std::cout << "Enter number of threads: ";
    std::cin >> threadNumber;
    auto threads = new std::thread[threadNumber];

    auto start_time = std::chrono::high_resolution_clock::now();

    int sum = 0;
    int num_in_thread = len/ threadNumber;
    int last_thread_num = num_in_thread;

    if (len % threadNumber != 0) {
        num_in_thread++;
        last_thread_num = len - num_in_thread * (len - 1);
    }

    for (int i = 0; i < threadNumber - 1; i++) {
        threads[i] = std::thread(threadFunc, vector, i * num_in_thread, num_in_thread, std::ref(sum));
    }

    threads[threadNumber - 1] = 
        std::thread(threadFunc, vector, (len - 1) * num_in_thread, num_in_thread, std::ref(sum));

    for (int i = 0; i < threadNumber; i++) {
        threads[i].join();
    }

    float vector_norm = sqrt(sum);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::cout << "Norm: " << vector_norm;
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
}