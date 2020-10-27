#include "Task.h"
#include <iostream>

namespace Ng {

    // constructor / destructor
    Task::Task() : _priority(Priority::None), _time(nullptr), _date(nullptr), _done(false) {
        this->_creationDateTime = QDateTime::currentDateTime();
    }

    Task::Task(const QString& name, Priority priority)
        : _name(name), _priority(priority), _time(nullptr), _date(nullptr), _done(false) {
        this->_creationDateTime = QDateTime::currentDateTime();
    }

    Task::Task(QString&& name, Priority priority)
        : _name(name), _priority(priority), _time(nullptr), _date(nullptr), _done(false) {
        this->_creationDateTime = QDateTime::currentDateTime();
    }

    Task::~Task() {
        delete this->_time;
        delete this->_date;
        std::cout << "destructor" << std::endl;
    }

    // accessors
    const QString& Task::name() const { return this->_name; }

    const Task::Priority& Task::priority() const { return this->_priority; }

    QTime* Task::time() const { return this->_time; }

    QDate* Task::date() const { return this->_date; }

    const QString& Task::description() const { return this->_description; }

    const QDateTime& Task::creationDateTime() const { return this->_creationDateTime; }

    bool Task::done() const { return this->_done; }

    // modifiers
    void Task::setName(const QString& name) { this->_name = name; }

    void Task::setPriority(const Priority& priority) { this->_priority = priority; }

    void Task::setTime(const QTime& time) { this->_time = new QTime(time); }

    void Task::setDate(const QDate& date) { this->_date = new QDate(date); }

    void Task::setDescription(const QString& description) { this->_description = description; }

    void Task::setDone(bool done) { this->_done = done; }

    // operators
    bool Task::operator<(const Task& other) const {
        if (this->_done != other._done)
            return this->_done < other._done;

        if (this->_date && other._date && this->_date != other._date)
            return this->_date > other._date;

        if (this->_date && !other._date)
            return true;

        if (!this->_date && other._date)
            return false;

        if (this->_time && other._time && this->_time != other._time)
            return this->_date > other._date;

        if (this->_time && !other._time)
            return true;

        if (!this->_time && other._time)
            return false;

        if (this->_priority != other._priority)
            return this->_priority > other._priority;

        return this->_creationDateTime < other._creationDateTime;
    }

} // namespace Ng


