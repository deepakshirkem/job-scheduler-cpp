#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class ThreadSafeQueue
{
    public:
        void push(T item)
        {
            std::lock_guard<std::mutex> lock(mutex);
            queue.push(item);
            cond.notify_one();
        }

        bool tryPop(T& item)
        {
            std::lock_guard<std::mutex> lock(mutex);
            if(queue.empty()) return false;
            item = queue.front();
            queue.pop();
            return true;
        }

        void waitPop(T& item)
        {
            std::unique_lock<std::mutex> lock(mutex);
            cond.wait(lock, [this]{ return !queue.empty();});
            item = queue.front();
            queue.pop();
        }

    private:
        std::queue<T> queue;
        std::mutex mutex;
        std::condition_variable cond;
};