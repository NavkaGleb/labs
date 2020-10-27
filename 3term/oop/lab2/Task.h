#pragma once

#include <QObject>
#include <QSet>
#include <QString>
#include <QDateTime>

namespace Ng {

    class Task {
    public:
        // enums
        enum class Priority { None = 0, Low = 1, Middle, High };

        // constructor / destructor
        Task();
        Task(const QString& name, Priority priority = Priority::None);
        Task(QString&& name, Priority priority = Priority::None);
        virtual ~Task();

        // accessors
        const QString& name() const;
        QTime* time() const;
        QDate* date() const;
        const Priority& priority() const;
        const QDateTime& creationDateTime() const;
        bool done() const;

        // modifiers
        void setName(const QString& name);
        void setTime(const QTime& time);
        void setDate(const QDate& date);
        void setPriority(const Priority& priority);
        void setDone(bool done);

        // operators
        bool operator<(const Task& other) const;

    private:       
        // fields
        QString _name;
        Priority _priority;
        QTime* _time;
        QDate* _date;
        QDateTime _creationDateTime;
        QSet<QString> _tags;
        bool _done;

    }; // class Task

} // namespace ng
