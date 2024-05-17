#pragma once
#include "Queue.hpp"
#include <array>
#include <chrono>
#include <thread>

namespace iqt
{
    class Runner
    {
    private:
        unsigned int timer = 0;

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
    };

    class Controller
    {
    private:
        std::array<Runner *, 4> runners;
        std::array<std::thread, 4> threads;

    public:
        Controller()
        {
            runners[0] = new Runner(new iqt::SimpleQueue("queueS1"));
            runners[1] = new Runner(new iqt::DelayedQueue("queueD1"));
            runners[2] = new Runner(new iqt::DelayedQueue("queueD2"));
            runners[3] = new Runner(new iqt::DelayedQueue("queueD3"));

            runners[1]->queue->AddTask(new iqt::Task("TaskD1", "queueD1", 10, "TaskS1", "queueS1", 5, 1));
            runners[2]->queue->AddTask(new iqt::Task("TaskD2", "queueD2", 10, "TaskS2", "queueS1", 6, 2));
            runners[3]->queue->AddTask(new iqt::Task("TaskD3", "queueD3", 10, "TaskS3", "queueS1", 7, 1));
        }
        ~Controller()
        {
            for (int i = 0; i < 4; i++)
            {
                delete runners[i];
            }
        }

        void ExecuteRunners()
        {
            for (int i = 0; i < 4; i++)
            {
                if (runners[i]->RunnedTask == nullptr)
                {
                    runners[i]->RunnedTask = runners[i]->queue->GetTask();

                    if (runners[i]->RunnedTask != nullptr)
                    {
                        threads[i] = std::thread(&iqt::Task::Run, runners[i]->RunnedTask);
                        threads[i].detach();
                    }
                }
            }
        }

        void GetResults()
        {
            for (int i = 0; i < 4; i++)
            {
                if (runners[i]->RunnedTask != nullptr)
                {
                    if (runners[i]->RunnedTask->isDone)
                    {
                        for (int j = 0; j < 4; j++)
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

        void Update()
        {

            for (int i = 0; i < 4; i++)
            {
                if (runners[i]->RunnedTask == nullptr)
                {
                    runners[i]->RunnedTask = runners[i]->queue->GetTask();

                    if (runners[i]->RunnedTask != nullptr)
                    {
                        threads[i] = std::thread(&iqt::Task::Run, runners[i]->RunnedTask);
                        threads[i].detach();
                    }
                }
                else
                {
                    if (runners[i]->RunnedTask->isDone)
                    {
                        for (int j = 0; j < 4; j++)
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
            while (true)
            {
                ExecuteRunners();
                GetResults();
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }
    };
};
