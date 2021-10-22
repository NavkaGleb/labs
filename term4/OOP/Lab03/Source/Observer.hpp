#pragma once

namespace Lab03 {

    ///
    /// @brief Used to define an **Observer**
    ///
    class IObserver {
    public:
        ///
        /// @brief Default Constructor
        ///
        IObserver() = default;

        ///
        /// @brief Default Destructor
        ///
        virtual ~IObserver() = default;

    public:
        ///
        /// @brief Need to be defined in derived class
        ///
        /// @param data which send **Subject**
        ///
        virtual void OnNotify(const void* data) = 0;

    }; // class IObserver

    template <typename T>
    concept ObserverConcept = std::is_base_of_v<IObserver, T>;

    ///
    /// @brief Used to define the class to be **Observed**
    ///
    class Subject {
    public:
        ///
        /// @brief Default Constructor
        ///
        Subject() = default;

        ///
        /// @brief Default Constructor
        ///
        virtual ~Subject() = default;

    public:
        ///
        /// @brief Add new observer to observer list
        ///
        /// @param observer
        ///
        void RegisterObserver(IObserver* observer);

        ///
        /// @brief Remove observer from observer list
        ///
        /// @param observer
        ///
        void UnregisterObserver(IObserver* observer);

        ///
        /// @brief Notify all observers
        ///
        /// @param data which observers will receive
        ///
        void NotifyObservers(const void* data);

    private:
        std::vector<IObserver*> m_Observers; /// <! All Observers

    }; // class Subject

} // namespace Lab03