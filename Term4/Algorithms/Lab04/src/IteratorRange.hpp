#pragma once

namespace Ng {

    template <typename Iterator>
    class IteratorRange {
    public:
        IteratorRange(Iterator begin, Iterator end)
            : m_Begin(begin)
            , m_End(end) {}

        Iterator begin() { return m_Begin; }
        Iterator end() { return m_End; }

        const Iterator& begin() const { return m_Begin; }
        const Iterator& end() const { return m_End; }

    private:
        Iterator m_Begin;
        Iterator m_End;

    }; // class IteratorRange

} // namespace Ng