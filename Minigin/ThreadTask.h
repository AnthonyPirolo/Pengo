// ThreadTask.h
#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>

class ThreadTask
{
public:
    ThreadTask();
    ~ThreadTask();

    void Enqueue(const std::function<void()>& task);
    void Stop();

private:
    void ThreadLoop();

    std::thread m_Thread;
    std::mutex m_Mutex;
    std::condition_variable m_Condition;
    std::queue<std::function<void()>> m_Tasks;
    bool m_Running;
};
