#include <stdio.h>
#include <stdlib.h>

// Define Process structure
typedef struct Process
{
    int pid;
    int priority;
    int burst_time;
    int wait_time;
} Process;

// Define Ready Queue
typedef struct ReadyQueue
{
    Process *queue[100];
    int front;
    int rear;
} ReadyQueue;

// Define IO Queue
typedef struct IOQueue
{
    Process *queue[100];
    int front;
    int rear;
} IOQueue;

// Function to add a process to the ready queue
void add_to_ready_queue(ReadyQueue *rq, Process *p)
{
    rq->queue[rq->rear] = p;
    rq->rear++;
}

// Function to add a process to the IO queue
void add_to_io_queue(IOQueue *ioq, Process *p)
{
    ioq->queue[ioq->rear] = p;
    ioq->rear++;
}

// Function to remove a process from the ready queue
Process *remove_from_ready_queue(ReadyQueue *rq)
{
    Process *p = rq->queue[rq->front];
    rq->front++;
    return p;
}

// Function to remove a process from the IO queue
Process *remove_from_io_queue(IOQueue *ioq)
{
    Process *p = ioq->queue[ioq->front];
    ioq->front++;
    return p;
}

// Function to simulate aging mechanism
void age_process(ReadyQueue *rq)
{
    for (int i = rq->front; i < rq->rear; i++)
    {
        rq->queue[i]->priority++;
    }
}

// Function to simulate context-switch time penalty
void penalize_process(Process *p, int cst)
{
    p->wait_time += cst;
}

// Function to simulate the priority-based operating system
void simulate_priority_based_os(Process *processes, int num_processes, int cst)
{
    ReadyQueue rq;
    rq.front = 0;
    rq.rear = 0;

    IOQueue ioq;
    ioq.front = 0;
    ioq.rear = 0;

    // Add processes to ready queue
    for (int i = 0; i < num_processes; i++)
    {
        add_to_ready_queue(&rq, &processes[i]);
    }

    // Simulate priority-based operating system
    while (rq.front < rq.rear || ioq.front < ioq.rear)
    {
        if (rq.front < rq.rear)
        {
            // Remove process from ready queue
            Process *p = remove_from_ready_queue(&rq);

            // Execute process
            for (int i = 0; i < p->burst_time; i++)
            {
                // Simulate context-switch time penalty
                penalize_process(p, cst);

                // Simulate aging mechanism
                age_process(&rq);

                // Check if process requires I/O
                if (i == (p->burst_time / 2))
                {
                    add_to_io_queue(&ioq, p);
                    break;
                }
            }

            // Add process back to ready queue if not complete
            if (p->burst_time > 0)
            {
                add_to_ready_queue(&rq, p);
            }
        }
        else
        {
            // Remove process from IO queue
            Process *p = remove_from_io_queue(&ioq);

            // Execute process
            for (int i = 0; i < p->burst_time; i++)
            {
                // Simulate context-switch time penalty
                penalize_process(p, cst);

                // Simulate aging mechanism
                age_process(&rq);
            }

            // Add process back to ready queue
            add_to_ready_queue(&rq, p);
        }
    }

    // Display results
    printf("Simulation Results:\n");
    printf("Ready Queue:\n");
    for (int i = rq.front; i < rq.rear; i++)
    {
        printf("Process %d (Priority: %d, Burst Time: %d, Wait Time: %d)\n",
               rq.queue[i]->pid, rq.queue[i]->priority, rq.queue[i]->burst_time, rq.queue[i]->wait_time);
    }
    printf("\nIO Queue:\n");
    for (int i = ioq.front; i < ioq.rear; i++)
    {
        printf("Process %d (Priority: %d, Burst Time: %d, Wait Time: %d)\n",
               ioq.queue[i]->pid, ioq.queue[i]->priority, ioq.queue[i]->burst_time, ioq.queue[i]->wait_time);
    }
}

int main()
{
    // Define processes
    Process processes[3];
    processes[0].pid = 1;
    processes[0].priority = 1;
    processes[0].burst_time = 8;
    processes[1].pid = 2;
    processes[1].priority = 2;
    processes[1].burst_time = 4;
    processes[2].pid = 3;
    processes[2].priority = 3;
    processes[2].burst_time = 6;

    // Simulate priority-based operating system
    simulate_priority_based_os(processes, 3, 2);

    return 0;
}