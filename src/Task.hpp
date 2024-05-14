#pragma once
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace iqt
{
    std::string GetTimeNow()
    {
        std::time_t currentTime = std::time(nullptr);
        std::tm *localTime = std::localtime(&currentTime);
        std::ostringstream oss;
        oss << std::setw(2) << std::setfill('0') << localTime->tm_hour << ":"
            << std::setw(2) << std::setfill('0') << localTime->tm_min << ":"
            << std::setw(2) << std::setfill('0') << localTime->tm_sec;
        return oss.str();
    };

    class Task
    {
    private:
        std::string dTaskName, dTaskQueue, sTaskName, sTaskQueue, currentQueue;
        unsigned int dTaskDelay, sTaskDelay;
        unsigned int sTaskPriority;

    public:
        Task(std::string _dTaskName, std::string _dTaskQueue, unsigned int _dTaskDelay, std::string _sTaskName, std::string _sTaskQueue, unsigned int _sTaskDelay, unsigned int _sTaskPriority)
        {
            dTaskName = _dTaskName;
            dTaskQueue = _dTaskQueue;
            dTaskDelay = _dTaskDelay;
            sTaskName = _sTaskName;
            sTaskQueue = _sTaskQueue;
            sTaskDelay = _sTaskDelay;
            sTaskPriority = _sTaskPriority;
        };

        void SetCurrentQueue(std::string _currentQueue)
        {
            currentQueue = _currentQueue;
        }

        std::string GetCurrentQueue()
        {
            return currentQueue;
        };

        std::string GetSimpleTaskName()
        {
            return sTaskName;
        };

        std::string GetSimpleQueueName()
        {
            return sTaskQueue;
        }

        unsigned int GetSimpleTaskDelay()
        {
            return sTaskDelay;
        };

        unsigned int GetSimpleTaskPriority()
        {
            return sTaskPriority;
        };

        std::string GetDelayedTaskName()
        {
            return dTaskName;
        };

        std::string GetDelayedQueueName()
        {
            return dTaskQueue;
        }

        unsigned int GetDelayedTaskDelay()
        {
            return dTaskDelay;
        };

        std::string GetDeleyedTaskData()
        {
            return GetTimeNow() + " : " + GetCurrentQueue() + " : " + GetDelayedTaskName() + " - " + std::to_string(GetDelayedTaskDelay());
        }

        std::string GetSimpleTaskData()
        {
            return GetTimeNow() + " : " + GetCurrentQueue() + " : " + GetSimpleTaskName() + " - " + std::to_string(GetSimpleTaskDelay());
        }

        void LogStartDeleyedTask()
        {
            std::cout << GetDeleyedTaskData() << " : " << "created" << std::endl;
        };

        void LogEndDeleyedTask()
        {
            std::cout << GetDeleyedTaskData() << " : (" << GetSimpleTaskData() << ") : " << "pushed" << std::endl;
        };

        void LogStartSimpleTask()
        {
            std::cout << GetSimpleTaskData() << " : " << "running..." << std::endl;
        };

        void LogEndSimpleTask()
        {
            std::cout << GetSimpleTaskData() << " : " << "complited" << std::endl;
        };
    };
}