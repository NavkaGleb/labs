#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include <future>
#include <iostream>

namespace ng {

    //////////////////////////////////////////////////////////////
    /// HEADER
    //////////////////////////////////////////////////////////////

    class ThreadPool {
    public:
        // aliases
        using Task = std::function<void()>;

        // constructor / destructor
        explicit ThreadPool(std::size_t count);
        virtual ~ThreadPool();

        // accessors
        [[nodiscard]] inline bool isEmpty() { return m_tasks.empty(); }
        [[nodiscard]] inline std::size_t getTasksCount() { return m_tasks.size(); }
        [[nodiscard]] inline std::size_t getThreadsCount() { return m_threads.size(); }

        // public methods
        template <typename T>
        auto enqueue(T&& task) -> std::future<decltype(task())>;

    private:
        // member data
        std::vector<std::thread> m_threads;
        std::condition_variable m_cv;
        std::mutex m_mutex;
        std::queue<Task> m_tasks;
        bool m_stop;

    }; // class ThreadPool

    //////////////////////////////////////////////////////////////
    /// SOURCE
    //////////////////////////////////////////////////////////////

    // public methods
    template <typename T>
    auto ThreadPool::enqueue(T&& task) -> std::future<decltype(task())> {

        auto wrapper = std::make_shared<std::packaged_task<decltype(task())()>>(std::forward<T>(task));

        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_tasks.emplace([=] {
                (*wrapper)();
            });
        }

        m_cv.notify_one();
        return wrapper->get_future();
    }

} // namespace ng