#pragma once
#include "Queue.hpp"
#include <array>
#include "windows.h"

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

        void SetTimer(unsigned int timer)
        {
            this->timer = timer;
        }

        unsigned int GetTimer()
        {
            return timer;
        }

        void tick()
        {
            if (timer > 0)
            {
                timer--;
            }
        }
    };

    class Controller
    {
    private:
        std::array<Runner *, 4> runners;

    public:
        Controller()
        {
            runners[0] = new Runner(new iqt::SimpleQueue("queueS1"));
            runners[1] = new Runner(new iqt::DelayedQueue("queueD1"));
            runners[2] = new Runner(new iqt::DelayedQueue("queueD2"));
            runners[3] = new Runner(new iqt::DelayedQueue("queueD3"));

            runners[1]->queue->AddTask(new iqt::Task("TaskD1", "queueD1", 10, "TaskS1", "queueS1", 2, 1));
            runners[2]->queue->AddTask(new iqt::Task("TaskD2", "queueD2", 10, "TaskS2", "queueS1", 2, 2));
            runners[3]->queue->AddTask(new iqt::Task("TaskD3", "queueD3", 10, "TaskS3", "queueS1", 2, 1));
        }
        ~Controller()
        {
            for (int i = 0; i < 4; i++)
            {
                delete runners[i];
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
                        if (i == 0)
                        {
                            runners[i]->RunnedTask->LogStartSimpleTask();
                        }
                        else
                        {
                            runners[i]->RunnedTask->LogStartDeleyedTask();
                        }
                        if (i == 0)
                        {
                            runners[i]->SetTimer(runners[i]->RunnedTask->GetSimpleTaskDelay());
                        }
                        else
                        {
                            runners[i]->SetTimer(runners[i]->RunnedTask->GetDelayedTaskDelay());
                        }
                    }
                }
                else
                {
                    if (runners[i]->GetTimer() == 0)
                    {
                        if (i == 0)
                        {
                            runners[i]->RunnedTask->LogEndSimpleTask();
                        }
                        else
                        {
                            runners[i]->RunnedTask->LogEndDeleyedTask();
                        }
                        for (int j = 0; j < 4; j++)
                        {
                            if (runners[i]->RunnedTask->GetCurrentQueue() != runners[i]->RunnedTask->GetSimpleQueueName())
                            {
                                if (runners[j]->queue->GetQueueName() == runners[i]->RunnedTask->GetSimpleQueueName())
                                {
                                    runners[j]->queue->AddTask(runners[i]->RunnedTask);
                                    runners[i]->RunnedTask = nullptr;
                                    break;
                                }
                            }
                            else
                            {
                                if (runners[j]->queue->GetQueueName() == runners[i]->RunnedTask->GetDelayedQueueName())
                                {
                                    runners[j]->queue->AddTask(runners[i]->RunnedTask);
                                    runners[i]->RunnedTask = nullptr;
                                    break;
                                }
                            }
                        }
                    }
                }
                runners[i]->tick();
            }
        }

        void Run()
        {
            while (true)
            {
                Update();
                Sleep(1000);
            }
        }
    };
};
