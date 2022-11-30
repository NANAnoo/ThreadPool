#include <iostream>
#include <functional>
#include "ThreadPool.h"
struct Timer {
    std::chrono::time_point<std::chrono::steady_clock> start, end;
    std::chrono::duration<float> duration;
    Timer()
    {
        start = std::chrono::high_resolution_clock::now();
    }
    ~Timer()
    {
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        float ms = duration.count() * 1000.0f;
        std::cout << "Timer took " << ms << "ms" << std::endl;
    }
};

int main(int, char **)
{
    nano_std::ThreadPool *pool = new nano_std::ThreadPool(10);
    std::mutex mux;
    std::vector<std::function<void(void)>> tasks;
    std::chrono::duration<float> duration;

    for (int i = 0; i < 1000; i++)
    {
        tasks.push_back([i, &mux, &duration]()
                      { 
                        mux.lock();
                        std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::high_resolution_clock::now();
                        // std::cout << "[" << std::this_thread::get_id() << "] "
                        //           << "I am " << i << std::endl;
                        std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::high_resolution_clock::now(); 
                        duration = duration + end - start;
                        mux.unlock();
                        for (int i=0; i < 1000000; i ++) {}
                                  });
    }
    {
        Timer t;
        pool->syncGroup(tasks);
        std::cout << "Tasks cost " << duration.count() * 1000 << " ms" << std::endl;
    }
    Timer t;
    for (int i = 0; i < 1000000 * 1000; i++)
    {

    }
}