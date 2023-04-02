#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 100

struct process
{
    char id[50];
    int arrival_time;
    int burst_time;
    int remaining_time;
    int turnaround_time;
    int waiting_time;
};

int main()
{
    FILE *input_file;
    struct process processes[MAX_PROCESSES];
    int num_processes = 0, time_slice = 0;
    float avg_turnaround_time = 0.0, avg_waiting_time = 0.0;
    int i, j, time = 0, completed = 0, shortest;

    // Open the input file for reading
    input_file = fopen("proc1.txt", "r");

    if (input_file == NULL)
    {
        printf("Error opening input file\n");
        return 1;
    }

    // Read the process data from the file
    while (fscanf(input_file, "%s %d %d", processes[num_processes].id,
                  &processes[num_processes].burst_time, &processes[num_processes].arrival_time) != EOF)
    {
        processes[num_processes].remaining_time = processes[num_processes].burst_time;
        num_processes++;
    }

    // Read the time slice from the file
    // fscanf(input_file, "%d", &time_slice);
    // Read the time slice from the file
    printf("Time slice: ");
    scanf("%d", &time_slice);

    fclose(input_file);

    // Run the SRTF algorithm
    while (completed < num_processes)
    {
        shortest = -1;
        for (i = 0; i < num_processes; i++)
        {
            if (processes[i].arrival_time <= time && processes[i].remaining_time > 0)
            {
                if (shortest == -1 || processes[i].remaining_time < processes[shortest].remaining_time)
                {
                    shortest = i;
                }
            }
        }
        if (shortest == -1)
        {
            time++;
        }
        else
        {
            if (processes[shortest].remaining_time <= time_slice)
            {
                time += processes[shortest].remaining_time;
                processes[shortest].remaining_time = 0;
            }
            else
            {
                time += time_slice;
                processes[shortest].remaining_time -= time_slice;
            }
            if (processes[shortest].remaining_time == 0)
            {
                completed++;
                processes[shortest].turnaround_time = time - processes[shortest].arrival_time;
                processes[shortest].waiting_time = processes[shortest].turnaround_time - processes[shortest].burst_time;
                avg_turnaround_time += processes[shortest].turnaround_time;
                avg_waiting_time += processes[shortest].waiting_time;
            }
        }
    }

    // Print the results
    printf("Process\t\tTurnaround Time\n");
    for (i = 0; i < num_processes; i++)
    {
        printf("%s\t\t%d\n", processes[i].id, processes[i].turnaround_time);
    }
    avg_turnaround_time /= num_processes;
    avg_waiting_time /= num_processes;
    printf("Average Turnaround Time: %.2f\n", avg_turnaround_time);
    printf("Average Waiting Time: %.2f\n", avg_waiting_time);

    return 0;
}