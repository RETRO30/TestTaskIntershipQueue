#pragma once
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <thread>
#include <chrono>

namespace iqt
{
    std::string GetTimeNow()
    {
        auto now = std::chrono::system_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now);
        auto now_tm = *std::localtime(&now_time_t);
        auto duration = now.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 10000;

        std::ostringstream oss;
        oss << std::put_time(&now_tm, "%H:%M:%S")
            << ":" << std::setw(5) << std::setfill('0') << millis.count();

        return oss.str();
    };

    class Task
    {
    private:
        std::string dTaskName, dTaskQueue, sTaskName, sTaskQueue, currentQueue;
        unsigned int dTaskDelay, sTaskDelay;
        unsigned int sTaskPriority;

    public:
        bool isDone = false;

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

        bool isDelayed()
        {
            return GetCurrentQueue() == dTaskQueue;
        }

        bool isSimple()
        {
            return GetCurrentQueue() == sTaskQueue;
        }

        void Run()
        {
            if (isSimple())
            {
                RunSimpleTask();
            }
            else if (isDelayed())
            {
                RunDelayedTask();
            }
        }

        void RunDelayedTask()
        {
            LogStartDeleyedTask();
            std::this_thread::sleep_for(std::chrono::seconds(dTaskDelay));
            LogEndDeleyedTask();
            isDone = true;
        }

        void RunSimpleTask()
        {
            LogStartSimpleTask();
            std::this_thread::sleep_for(std::chrono::seconds(sTaskDelay));
            LogEndSimpleTask();
            isDone = true;
        }

        void SetCurrentQueue(std::string _currentQueue)
        {
            currentQueue = _currentQueue;
        }

        std::string GetCurrentQueue()
        {
            return currentQueue;
        };

        std::string GetNextQueue()
        {
            if (isDelayed())
            {
                return sTaskQueue;
            }
            else
            {
                return dTaskQueue;
            }
        }

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

        unsigned int GetDelay()
        {
            if (isDelayed())
            {
                return GetDelayedTaskDelay();
            }
            else
            {
                return GetSimpleTaskDelay();
            }
        }

        std::string GetDeleyedTaskData()
        {
            return GetTimeNow() + " : " + GetDelayedQueueName() + " : " + GetDelayedTaskName() + " - " + std::to_string(GetDelayedTaskDelay());
        }

        std::string GetSimpleTaskData()
        {
            return GetTimeNow() + " : " + GetSimpleQueueName() + " : " + GetSimpleTaskName() + " - " + std::to_string(GetSimpleTaskDelay());
        }

        void LogStartDeleyedTask()
        {
            std::cout << GetDeleyedTaskData() + " : " + "created\n";
        };

        void LogEndDeleyedTask()
        {
            std::cout << GetDeleyedTaskData() + " : (" + GetSimpleTaskData() + ") : " << "pushed\n";
        };

        void LogStartSimpleTask()
        {
            std::cout << GetSimpleTaskData() + " : " + "running...\n";
        };

        void LogEndSimpleTask()
        {
            std::cout << GetSimpleTaskData() + " : " + "complited\n";
        };
    };
}