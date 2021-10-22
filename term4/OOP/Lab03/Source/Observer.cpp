#include "Observer.hpp"

namespace Lab03 {

    void Subject::RegisterObserver(IObserver* observer) {
        auto observerIt = std::find(m_Observers.begin(), m_Observers.end(), observer);

        if (observerIt == m_Observers.end())
            m_Observers.push_back(observer);
    }

    void Subject::UnregisterObserver(IObserver* observer) {
        auto observerIt = std::find(m_Observers.begin(), m_Observers.end(), observer);

        if (observerIt != m_Observers.end())
            m_Observers.erase(observerIt);

        delete observer;
    }

    void Subject::NotifyObservers(const void* data) {
        for (auto& observer : m_Observers)
            observer->OnNotify(data);
    }

} // namespace Lab03