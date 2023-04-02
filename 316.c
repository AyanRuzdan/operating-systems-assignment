#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char name[10];
    int arrival_time;
    int execution_time;
    int remaining_time;
    int priority;
    int start_time;
    int end_time;
    int wait_time;
} Process;

// Function to compare processes by priority
int compare_priority(const void *a, const void *b)
{
    const Process *p1 = (const Process *)a;
    const Process *p2 = (const Process *)b;
    return p1->priority - p2->priority;
}

// Function to simulate shortest job with preemption scheduling
void shortest_job_with_preemption(Process *processes, int num_processes, int time_slice)
{
    int total_time = 0;
    Process *current_process = NULL;
    int i;
    int num_completed = 0;

    while (num_completed < num_processes)
    {
        // Add all processes that have arrived to the priority queue
        for (i = 0; i < num_processes; i++)
        {
            if (processes[i].arrival_time == total_time)
            {
                processes[i].remaining_time = processes[i].execution_time;
                processes[i].start_time = -1;
                processes[i].end_time = -1;
                processes[i].wait_time = 0;
            }
        }

        // Find the process with the shortest remaining time
        current_process = NULL;
        for (i = 0; i < num_processes; i++)
        {
            if (processes[i].remaining_time > 0)
            {
                if (current_process == NULL || processes[i].remaining_time < current_process->remaining_time)
                {
                    current_process = &processes[i];
                }
            }
        }

        if (current_process == NULL)
        {
            total_time++;
            continue;
        }

        // Record start time of process
        if (current_process->start_time == -1)
        {
            current_process->start_time = total_time;
        }

        // Execute process for a time slice or until completion
        if (current_process->remaining_time <= time_slice)
        {
            total_time += current_process->remaining_time;
            current_process->remaining_time = 0;
            current_process->end_time = total_time;
            num_completed++;
        }
        else
        {
            total_time += time_slice;
            current_process->remaining_time -= time_slice;
        }

        // Update wait time for all other processes
        for (i = 0; i < num_processes; i++)
        {
            if (&processes[i] != current_process && processes[i].remaining_time > 0)
            {
                processes[i].wait_time += time_slice;
            }
        }
    }

    // Print results for each process
    for (i = 0; i < num_processes; i++)
    {
        printf("%s: Turnaround Time = %d, Wait Time = %d\n", processes[i].name, processes[i].end_time - processes[i].arrival_time, processes[i].wait_time);
    }

    // Calculate and print average turnaround time
    float avg_turnaround_time = 0;
    for (i = 0; i < num_processes; i++)
    {
        avg_turnaround_time += (float)(processes[i].end_time - processes[i].arrival_time) / num_processes;
    }
    printf("Average Turnaround Time = %.2f\n", avg_turnaround_time);
}

// Main function to read input file and run scheduling algorithm
int main()
{
    char filename[50];
    printf("Enter filename: ");
    scanf("%s", filename);

    // Open input file
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return 1;
    }

    // Read number of processes from first line of file
    int num_processes;
    fscanf(file, "%d", &num_processes);

    // Allocate array of processes
    Process *processes = malloc(num_processes * sizeof(Process));
    if (processes == NULL)
    {
        printf("Error allocating memory\n");
        return 1;
    }

    // Read each process from file
    int i;
    for (i = 0; i < num_processes; i++)
    {
        fscanf(file, "%s %d %d", processes[i].name, &processes[i].arrival_time, &processes[i].execution_time);
        processes[i].remaining_time = 0;
        processes[i].start_time = -1;
        processes[i].end_time = -1;
        processes[i].wait_time = 0;
    }

    // Close input file
    fclose(file);

    // Prompt for time slice
    int time_slice;
    printf("Enter time slice: ");
    scanf("%d", &time_slice);

    // Run shortest job with preemption scheduling algorithm
    shortest_job_with_preemption(processes, num_processes, time_slice);

    // Free memory allocated for processes
    free(processes);

    return 0;
}