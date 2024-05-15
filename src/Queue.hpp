#pragma once
#include <string>
#include <queue>
#include "Task.hpp"

namespace iqt
{
    struct SimpleComparator
    {
        bool operator()(iqt::Task *s1, iqt::Task *s2)
        {
            return s1->GetSimpleTaskPriority() > s2->GetSimpleTaskPriority();
        }
    };

    class Queue
    {
    protected:
        std::string queueName;

    public:
        Queue(std::string name)
        {
            queueName = name;
        }
        std::string GetQueueName()
        {
            return queueName;
        }
        virtual void AddTask(iqt::Task *task) {}
        virtual iqt::Task *GetTask()
        {
            return nullptr;
        };
    };

    class SimpleQueue : public Queue
    {
    private:
        std::priority_queue<iqt::Task *, std::vector<iqt::Task *>, SimpleComparator> queue;

    public:
        SimpleQueue(std::string name) : Queue(name){};

        ~SimpleQueue()
        {
            for (int i = 0; i < queue.size(); i++)
            {
                delete queue.top();
                queue.pop();
            }
        }

        void AddTask(iqt::Task *task)
        {
            task->SetCurrentQueue(GetQueueName());
            queue.push(task);
        };

        iqt::Task *GetTask()
        {
            if (queue.size() > 0)
            {
                iqt::Task *task = queue.top();
                queue.pop();
                return task;
            }
            return nullptr;
        };
    };

    class DelayedQueue : public Queue
    {
    private:
        std::queue<iqt::Task *> queue;

    public:
        DelayedQueue(std::string name) : Queue(name){};

        ~DelayedQueue()
        {
            for (int i = 0; i < queue.size(); i++)
            {
                delete queue.front();
                queue.pop();
            }
        }

        void AddTask(iqt::Task *task)
        {
            task->SetCurrentQueue(GetQueueName());
            queue.push(task);
        };

        iqt::Task *GetTask()
        {
            if (queue.size() > 0)
            {
                iqt::Task *task = queue.front();
                queue.pop();
                return task;
            }
            return nullptr;
        };
    };
}