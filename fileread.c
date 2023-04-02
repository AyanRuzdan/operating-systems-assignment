#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 100
struct process
{
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completed;
    int completion_time;
    int waiting_time;
    int turnaround_time;
};
int main()
{
    char filename[50];
    char p_id[MAX_SIZE][50];
    int at[MAX_SIZE];
    int bt[MAX_SIZE];
    printf("Enter file name: ");
    scanf("%s", filename);
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        printf("Failed to open file!!!\n");
        return 1;
    }
    int time_slice;
    printf("Enter time slice: ");
    scanf("%d", &time_slice);
    int count = 0;
    while (fscanf(fp, "%s %d %d", p_id[count], &at[count], &bt[count]) == 3)
        count++;
    struct process *processes = malloc(count * sizeof(struct process));
    for (int i = 0; i < count; i++)
    {
        fscanf(filename, "%s %d %d", processes[i].pid, &processes[i].arrival_time, &processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].completed = 0;
        processes[i].completion_time = 0;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
    }
    fclose(filename);
    int current_time = 0;
    int completed = 0;
    int time_slice_remaining = 0;
    while (completed < count)
    {
        int shortest_process_index = -1;
        int shortest_remaining_time = __INT_MAX__;
        for (int i = 0; i < count; i++)
        {
            if (!processes[i].completed && processes[i].arrival_time <= current_time && processes[i].remaining_time < shortest_remaining_time)
            {
                shortest_process_index = i;
                shortest_remaining_time = processes[i].remaining_time;
            }
        }
        struct process *p = &processes[shortest_process_index];
        if (p->remaining_time > time_slice)
        {
            p->remaining_time -= time_slice;
            time_slice_remaining = time_slice;
        }
        else
        {
            time_slice_remaining = p->remaining_time;
            p->remaining_time = 0;
            p->completed = 1;
            p->completion_time = current_time + time_slice_remaining;
            p->waiting_time = p->waiting_time - p->arrival_time - p->burst_time;
            p->turnaround_time = p->completion_time - p->arrival_time;
            completed++;
        }
        for (int i = 0; i < count; i++)
            if (i != shortest_process_index && !processes[i].completed && processes[i].arrival_time <= current_time)
                processes[i].waiting_time += time_slice_remaining;

        current_time += time_slice_remaining;
    }
    float total_waiting_time = 0.0;
    float total_turnaround_time = 0.0;
    for (int i = 0; i < count; i++)
    {
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
    }
    float avg_waiting_time = total_waiting_time / count;
    float avg_turnaround_time = total_turnaround_time / count;
    printf("The average waiting time is: %d\n", avg_waiting_time);
    printf("The average turnaround time is: %d\n", avg_turnaround_time);
    return 0;
}