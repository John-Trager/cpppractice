#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>
#include <ostream>
#include <vector>
#include <thread>
#include <atomic>
#include <functional>
#include <deque>
#include <chrono>

class ThreadPool {
public:
ThreadPool(size_t numThreads) {
    for (size_t i=0; i < numThreads; i++) {
        d_threads.push_back(std::thread(&ThreadPool::executor, this));
    }
}

~ThreadPool() {
    d_done = true;
    d_jobCv.notify_all();
    for(auto& t : d_threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

template<typename Func, typename ...Args> // may add concept to make sure func works fine
void addJob(Func&& func, Args&&... args) {
    // encapsulate the function
    // could also encapsulate the func further in std::packaged_task so we could return an std::future
    // this would mean we have to make the packaged_task(func) a shared ptr so it doesn't go out of scope
    // before we run it in the executor
    auto executorFunc = [func, ...args = std::forward<Args>(args)](){
        func(args...);
    };
    
    {
        std::unique_lock<std::mutex> lock(d_jobLock);
        d_scheduledJobs.emplace_back(executorFunc);
    }
    d_jobCv.notify_one();
};

private:

void executor() {
    while(!d_done) {
        std::unique_lock<std::mutex> lock(d_jobLock);
        d_jobCv.wait(lock, [this]() { return !d_scheduledJobs.empty() || d_done; });

        if (!d_scheduledJobs.empty() && !d_done) {
            auto func = d_scheduledJobs.front();
            d_scheduledJobs.pop_front();
            lock.unlock();
            func();
        }
    }
};

std::atomic<bool> d_done;

std::vector<std::thread> d_threads;
std::deque<std::function<void()>> d_scheduledJobs;
std::condition_variable d_jobCv;
std::mutex d_jobLock;
};

// int main() {
//     std::cout << "hello" << std::endl;

//     auto f1 = [](){
//         std::cout << "hello func 1\n";
//         std::this_thread::sleep_for(std::chrono::seconds(1));
//     };
    
//     auto f2 = [](const std::string& str){
//         std::cout << "hello func: " << str << "\n";
//         std::this_thread::sleep_for(std::chrono::seconds(1));
//     };
    

//     ThreadPool pool(1);
//     pool.addJob(f1);
//     pool.addJob(f2, "hello");
//     pool.addJob(f1);
//     pool.addJob(f1);

//     std::this_thread::sleep_for(std::chrono::milliseconds(2000));

//     return 0;
// }