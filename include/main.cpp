#include <iostream>
#include <chrono>
#include "ThreadPool.hpp"
std::mutex print_mutex;

int main() {
    ThreadPool pool(3);
    for (int i=1; i<=8; i++) {
        pool.enqueue([i]() {
            {
                std::lock_guard<std::mutex> lock(print_mutex);
                std::cout << "mission " << i << " begin " 
                          << std::this_thread::get_id() << "\n";
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            {
                std::lock_guard<std::mutex> lock(print_mutex);
                std::cout << "mission " << i << " got finished \n";
            }
        });
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "finish";
    return 0;
}
