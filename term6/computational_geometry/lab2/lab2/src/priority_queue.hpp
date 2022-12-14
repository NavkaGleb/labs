#pragma once

#include <ostream>
#include <vector>
#include <memory>

namespace cg_lab2
{

    template<typename T>
    class PriorityQueue
    {
    public:
        bool isEmpty() const
        {
            return m_Elements.empty();
        }

        std::unique_ptr<T> pop()
        {
            swap(0, m_Elements.size() - 1);
            auto top = std::move(m_Elements.back());
            m_Elements.pop_back();
            siftDown(0);
            return top;
        }

        void push(std::unique_ptr<T> elem)
        {
            elem->index = m_Elements.size();
            m_Elements.emplace_back(std::move(elem));
            siftUp(m_Elements.size() - 1);
        }

        void update(std::size_t i)
        {
            const int parent{ getParent(i) };

            if (parent >= 0 && *m_Elements[parent] < *m_Elements[i])
            {
                siftUp(i);
            }
            else
            {
                siftDown(i);
            }
        }

        void remove(std::size_t i)
        {
            swap(i, m_Elements.size() - 1);
            m_Elements.pop_back();

            if (i < m_Elements.size())
            {
                update(i);
            }
        }

    private:
        std::vector<std::unique_ptr<T>> m_Elements;

        int getParent(int i) const
        {
            return (i + 1) / 2 - 1;
        }

        std::size_t getLeftChild(std::size_t i) const
        {
            return 2 * (i + 1) - 1;
        }

        std::size_t getRightChild(std::size_t i) const
        {
            return 2 * (i + 1);
        }

        void siftDown(std::size_t i)
        {
            const std::size_t left{ getLeftChild(i) };
            const std::size_t right{ getRightChild(i) };

            std::size_t j{ i };

            if (left < m_Elements.size() && *m_Elements[j] < *m_Elements[left])
            {
                j = left;
            }

            if (right < m_Elements.size() && *m_Elements[j] < *m_Elements[right])
            {
                j = right;
            }

            if (j != i)
            {
                swap(i, j);
                siftDown(j);
            }
        }

        void siftUp(std::size_t i)
        {
            const int parent{ getParent(i) };

            if (parent >= 0 && *m_Elements[parent] < *m_Elements[i])
            {
                swap(i, parent);
                siftUp(parent);
            }
        }

        inline void swap(std::size_t i, std::size_t j)
        {
            auto tmp = std::move(m_Elements[i]);
            m_Elements[i] = std::move(m_Elements[j]);
            m_Elements[j] = std::move(tmp);
            m_Elements[i]->index = i;
            m_Elements[j]->index = j;
        }
    };

} // namespace cg_lab2