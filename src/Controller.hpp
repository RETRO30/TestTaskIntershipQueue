#pragma once
#include "Queue.hpp"
#include <array>
#include <chrono>
#include <thread>

namespace iqt
{
    class Runner
    {
    public:
        iqt::Queue *queue;
        iqt::Task *RunnedTask = nullptr;

        Runner(Queue *queue)
        {
            this->queue = queue;
        };

        ~Runner()
        {
            delete queue;
            delete RunnedTask;
        }

        void SetTask(iqt::Task *task)
        {
            RunnedTask = task;
        }

        void Run()
        {
            while (true)
            {
                if (RunnedTask != nullptr)
                {
                    if (!RunnedTask->isDone)
                    {
                        RunnedTask->Run();
                    }
                }
                else
                {
                    RunnedTask = queue->GetTask();
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }
    };

    class Controller
    {
    private:
        const unsigned int Size = 4;
        std::array<Runner *, 4> runners;
        std::array<std::thread, 4> threads;

    public:
        Controller()
        {
            runners[0] = new Runner(new iqt::SimpleQueue("queueS1"));
            runners[1] = new Runner(new iqt::DelayedQueue("queueD1"));
            runners[2] = new Runner(new iqt::DelayedQueue("queueD2"));
            runners[3] = new Runner(new iqt::DelayedQueue("queueD3"));

            runners[1]->queue->AddTask(new iqt::Task("TaskD1", "queueD1", 2, "TaskS1", "queueS1", 2, 1));
            runners[2]->queue->AddTask(new iqt::Task("TaskD2", "queueD2", 2, "TaskS2", "queueS1", 2, 2));
            runners[3]->queue->AddTask(new iqt::Task("TaskD3", "queueD3", 4, "TaskS3", "queueS1", 15, 3));
        }
        ~Controller()
        {
            for (int i = 0; i < Size; i++)
            {
                delete runners[i];
            }
        }

        void RunThreads()
        {
            for (int i = 0; i < Size; i++)
            {
                threads[i] = std::thread(&iqt::Runner::Run, std::ref(runners[i]));
                threads[i].detach();
            }
        }

        void GetResults()
        {
            for (int i = 0; i < Size; i++)
            {
                if (runners[i]->RunnedTask != nullptr)
                {
                    if (runners[i]->RunnedTask->isDone)
                    {
                        for (int j = 0; j < Size; j++)
                        {
                            if (runners[i]->RunnedTask->GetNextQueue() == runners[j]->queue->GetQueueName())
                            {
                                runners[i]->RunnedTask->isDone = false;
                                runners[j]->queue->AddTask(runners[i]->RunnedTask);
                                runners[i]->RunnedTask = nullptr;
                                break;
                            }
                        }
                    }
                }
            }
        }

        void Run()
        {
            RunThreads();
            while (true)
            {
                GetResults();
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }
    };
};
