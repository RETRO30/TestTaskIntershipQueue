#pragma once
#include <string>
#include "Queue.hpp"

namespace iqt
{
    class Task
    {
    protected:
        std::string name;
        unsigned int delay;
        iqt::Queue *pQueue;

    public:
        Task(std::string _name, unsigned int _delay, iqt::Queue *_queue) : name(_name), delay(_delay), pQueue(_queue){};
        virtual ~Task()
        {
            delete pQueue;
        };
        virtual void run() = 0;
    };

    class DelayTask : public Task
    {
    public:
        DelayTask(int delay) : delay(delay) {}

    private:
        int delay;
    };
}