#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PROCESSES 100
#define MAX_PRIORITY 5

typedef struct Process
{
    int pid;             // process ID
    int arrival_time;    // arrival time
    int priority;        // priority level
    int cpu_burst;       // CPU burst time
    int waiting_time;    // waiting time
    int turnaround_time; // turnaround time
} Process;

void initialize_processes(Process *p, int n)
{
    srand(time(NULL));
    for (int i = 0; i < n; i++)
    {
        p[i].pid = i;
        p[i].arrival_time = rand() % 20;
        p[i].priority = rand() % MAX_PRIORITY + 1;
        p[i].cpu_burst = rand() % 20 + 1;
        p[i].waiting_time = 0;
        p[i].turnaround_time = 0;
    }
}

void priority_sort(Process *p, int n)
{
    // sort processes based on priority level
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (p[i].priority < p[j].priority)
            {
                // swap processes
                Process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
}

void simulate(Process *p, int n, int time_limit)
{
    int current_time = 0;
    int completed = 0;
    while (completed < n && current_time < time_limit)
    {
        priority_sort(p, n);
        int selected = -1;
        for (int i = 0; i < n; i++)
        {
            if (p[i].arrival_time <= current_time && p[i].cpu_burst > 0)
            {
                selected = i;
                break;
            }
        }
        if (selected == -1)
        {
            // no process available, move to next time unit
            current_time++;
        }
        else
        {
            // process selected for execution
            p[selected].cpu_burst--;
            if (p[selected].cpu_burst == 0)
            {
                // process completed
                completed++;
                p[selected].turnaround_time = current_time - p[selected].arrival_time + 1;
            }
            for (int i = 0; i < n; i++)
            {
                if (i != selected && p[i].arrival_time <= current_time && p[i].cpu_burst > 0)
                {
                    // increment waiting time of other processes
                    p[i].waiting_time++;
                }
            }
            current_time++;
        }
    }
}

void fcfs(Process *p, int n, int time_limit)
{

    float waiting_time = 0, turnaround_time = 0;
    int completion_time[n];
    // completion_time[0] = p[0].arrival_time + p[0].cpu_burst;
    // completion_time[0] = p[0].cpu_burst;
    // p[0].turnaround_time = completion_time[0] - p[0].arrival_time;
    // p[0].waiting_time = p[0].turnaround_time - p[0].cpu_burst;
    // waiting_time = p[0].waiting_time;
    // turnaround_time = p[0].turnaround_time;
    completion_time[0] = p[0].cpu_burst;
    p[0].turnaround_time = completion_time[0] - p[0].arrival_time;
    p[0].waiting_time = p[0].turnaround_time - p[0].cpu_burst;
    waiting_time = p[0].waiting_time;
    turnaround_time = p[0].turnaround_time;
    for (int i = 1; i < n; i++)
    {
        completion_time[i] = completion_time[i - 1] + p[i].cpu_burst;
        p[i].turnaround_time = completion_time[i] - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].cpu_burst;
        waiting_time += p[i].waiting_time;
        turnaround_time += p[i].turnaround_time;
    }

    printf("\n<--------------FCFS--------------->\n");
    printf("Process\tArrival Time\tBurst Time\tCompletion Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", i + 1, p[i].arrival_time, p[i].cpu_burst, completion_time[i], completion_time[i] - p[i].arrival_time - p[i].cpu_burst, completion_time[i] - p[i].arrival_time);
    }
    printf("\nAverage Waiting Time: %.2f", waiting_time / n);
    printf("\nAverage Turnaround Time: %.2f", turnaround_time / n);
}
void sjf(Process *p, int n)
{
    int temp;
    int completion_time[n];
    float waiting_time = 0, turnaround_time = 0;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (p[i].cpu_burst > p[j].cpu_burst)
            {
                temp = p[i].cpu_burst;
                p[i].cpu_burst = p[j].cpu_burst;
                p[j].cpu_burst = temp;
                temp = p[i].arrival_time;
                p[i].arrival_time = p[j].arrival_time;
                p[j].arrival_time = temp;
            }
    completion_time[0] = p[0].arrival_time + p[0].cpu_burst;
    p[0].turnaround_time = completion_time[0] - p[0].arrival_time;
    p[0].waiting_time = 0;
    for (int i = 1; i < n; i++)
    {
        if (completion_time[i - 1] < p[i].arrival_time)
        {
            completion_time[i] = p[i].arrival_time + p[i].cpu_burst;
        }
        else
        {
            completion_time[i] = completion_time[i - 1] + p[i].cpu_burst;
        }
        p[i].turnaround_time = completion_time[i] - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].cpu_burst;
    }
    // calculate waiting time and turnaround time
    for (int i = 0; i < n; i++)
    {
        turnaround_time += p[i].turnaround_time;
        waiting_time += p[i].waiting_time;
    }
    printf("\n<------------------------------sjf--------------------------------------------------->");
    printf("\nProcess\tArrival Time\tBurst Time\tCompletion Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", i + 1, p[i].arrival_time, p[i].cpu_burst, completion_time[i], p[i].waiting_time, p[i].turnaround_time);
    }
    printf("\nAverage Waiting Time: %.2f", waiting_time / n);
    printf("\nAverage Turnaround Time: %.2f", turnaround_time / n);
}
void print_results(Process *p, int n)
{
    printf("PID\tArrival\tPriority\tBurst Time\tWaiting Time\tTurnaround Time\n");
    int total_waiting_time = 0, total_turnaround_time = 0;
    for (int i = 0; i < n; i++)
    {
        total_waiting_time += p[i].waiting_time;
        total_turnaround_time += p[i].turnaround_time;
        printf("%d\t%d\t%d\t\t%d\t\t%d\t\t%d\n", p[i].pid, p[i].arrival_time, p[i].priority, p[i].cpu_burst, p[i].waiting_time, p[i].turnaround_time);
    }
    printf("\nAverage Waiting Time: %.2f\n", (float)total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_turnaround_time / n);
}

int main()
{
    int time_limit = 100;
    printf("Enter number of processes: ");
    int n;
    scanf("%d", &n);
    Process processes[n];
    initialize_processes(processes, n);
    simulate(processes, n, time_limit);
    print_results(processes, n);
    fcfs(processes, n, time_limit);
    sjf(processes, n);
    return 0;
}