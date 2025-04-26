#include "ThreadTask.h"

ThreadTask::ThreadTask()
    : m_Running(true)
{
    m_Thread = std::thread(&ThreadTask::ThreadLoop, this);
}

ThreadTask::~ThreadTask()
{
    Stop();
}

void ThreadTask::Enqueue(const std::function<void()>& task)
{
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_Tasks.push(task);
    }
    m_Condition.notify_one();
}

void ThreadTask::Stop()
{
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_Running = false;
    }
    m_Condition.notify_one();
    if (m_Thread.joinable())
        m_Thread.join();
}

void ThreadTask::ThreadLoop()
{
    while (true)
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(m_Mutex);
            m_Condition.wait(lock, [this]() { return !m_Tasks.empty() || !m_Running; });

            if (!m_Running && m_Tasks.empty())
                break;

            task = m_Tasks.front();
            m_Tasks.pop();
        }
        if (task)
            task();
    }
}
