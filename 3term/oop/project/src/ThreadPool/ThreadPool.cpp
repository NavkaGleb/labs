#include "ThreadPool.hpp"

#include <algorithm>
#include <random>
#include <iostream>

namespace ng {

    // constructor / destructor
    ThreadPool::ThreadPool(std::size_t count) : m_stop(false) {
        m_threads.reserve(count);

        for (std::size_t i = 0; i < m_threads.capacity(); ++i) {
            m_threads.emplace_back([&] {
                while (true) {
                    Task task;

                    {
                        std::unique_lock<std::mutex> lock(m_mutex);

                        m_cv.wait(lock, [&] { return m_stop || !m_tasks.empty(); });

                        if (m_stop && m_tasks.empty())
                            break;

                        task = std::move(m_tasks.front());
                        m_tasks.pop();
                    }

                    task();
                }
            });
        }
    }

    ThreadPool::~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_stop = true;
        }

        m_cv.notify_all();

        for (auto&& thread : m_threads)
            thread.join();
    }

} // namespace ng