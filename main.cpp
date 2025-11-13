#include "threadpool.hpp"
#include "uniqueptr.hpp"
#include "vec.hpp"

#include <iostream>
#include <ostream>
#include <thread>
#include <chrono>

int main() {
    std::cout << "hello" << std::endl;

    auto f1 = [](){
        std::cout << "hello func 1\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    };
    
    auto f2 = [](const std::string& str){
        std::cout << "hello func: " << str << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    };
    

    ThreadPool pool(2);
    pool.addJob(f1);
    pool.addJob(f2, "hello");
    pool.addJob(f1);
    pool.addJob(f1);

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    return 0;
}