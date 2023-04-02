#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
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
int compare_priority(const void* a, const void* b) {
    const Process* p1 = (const Process*) a;
    const Process* p2 = (const Process*) b;
    return p1->priority - p2->priority;
}

// Function to simulate shortest job with preemption scheduling
void shortest_job_with_preemption(Process* processes, int num_processes, int time_slice) {
    int total_time = 0;
    Process* current_process = NULL;
    int i;
    int num_completed = 0;

    while (num_completed < num_processes) {
        // Add all processes that have arrived to the priority queue
        for (i = 0; i < num_processes; i++) {
            if (processes[i].arrival_time == total_time) {
                processes[i].remaining_time = processes[i].execution_time;
                processes[i].start_time = -1;
                processes[i].end_time = -1;
                processes[i].wait_time = 0;
            }
        }

        // Find the process with the shortest remaining time
        current_process = NULL;
        for (i = 0; i < num_processes; i++) {
            if (processes[i].remaining_time > 0) {
                if (current_process == NULL || processes[i].remaining_time < current_process->remaining_time) {
                    current_process = &processes[i];
                }
            }
        }

        if (current_process == NULL) {
            total_time++;
            continue;
        }

        // Record start time of process
        if (current_process->start_time == -1) {
            current_process->start_time = total_time;
        }

        // Execute process for a time slice or until completion
        if (current_process->remaining_time <= time_slice) {
            total_time += current_process->remaining_time;
            current_process->remaining_time = 0;
            current_process->end_time = total_time;
            num_completed++;
        }
        else {
            total_time += time_slice;
            current_process->remaining_time -= time_slice;
        }

        // Update wait time for all other processes
        for (i = 0; i < num_processes; i++) {
            if (&processes[i] != current_process && processes[i].remaining_time > 0) {
                processes[i].wait_time += time_slice;
            }
        }
    }

    // Print results for each process
    for (i = 0; i < num_processes; i++) {
        printf("%s: Turnaround Time = %d, Wait Time = %d\n", processes[i].name, processes[i].end_time - processes[i].arrival_time, processes[i].wait_time);
    }

    // Calculate and print average turnaround time
    float avg_turnaround_time = 0;
    for (i = 0; i < num_processes; i++) {
        avg_turnaround_time += (float) (processes[i].end_time - processes[i].arrival_time) / num_processes;
    }
    printf("Average Turnaround Time = %.2f\n", avg_turnaround_time);
}

// Main function to read input file and run scheduling algorithm
int main() {
    char filename[50];
    printf("Enter filename: ");
    scanf("%s", filename);

    // Open input file
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    // Read number of processes from first line of file
    int num_processes;
    fscanf(file, "%d", &num_processes);

    // Allocate array of processes
    Process* processes = malloc(num_processes * sizeof(Process));
    if (processes == NULL) {
        printf("Error allocating memory\n");
        return 1;
    }

    // Read each process from file
    int i;
    for (i = 0; i < num_processes; i++) {
        fscanf(file, "%s %d %d %d", processes[i].name, &processes[i].arrival_time, &processes[i].execution_time, &processes[i].priority);
        processes[i].remaining_time = 0;
        processes[i].start_time = -1;
        processes[i].end_time = -1;
        processes[i].wait_time = 0;
    }

    // Close input file
    fclose(file);

    // Sort processes by priority
    qsort(processes, num_processes, sizeof(Process), compare_priority);

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
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

int main() {
    char names[MAX_SIZE][50];   // array to store names
    int ages[MAX_SIZE];         // array to store ages
    int count = 0;              // counter for number of names read

    FILE *fp;                   // file pointer
    fp = fopen("input.txt", "r");

    // check if file opened successfully
    if (fp == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    // read in names and ages from file
    while (fscanf(fp, "%s %d", names[count], &ages[count]) == 2) {
        count++;
    }

    // print out the names and ages
    for (int i = 0; i < count; i++) {
        printf("%s is %d years old.\n", names[i], ages[i]);
    }

    // close the file
    fclose(fp);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>

struct process {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completed;
    int completion_time;
    int waiting_time;
    int turnaround_time;
};

int main() {
    FILE *input_file = fopen("input.txt", "r");
    if (input_file == NULL) {
        printf("Failed to open input file\n");
        return 1;
    }

    int n, time_slice;
    fscanf(input_file, "%d %d", &n, &time_slice);

    struct process *processes = malloc(n * sizeof(struct process));
    for (int i = 0; i < n; i++) {
        fscanf(input_file, "%d %d %d", &processes[i].pid, &processes[i].arrival_time, &processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].completed = 0;
        processes[i].completion_time = 0;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
    }

    fclose(input_file);

    int current_time = 0;
    int completed = 0;
    int time_slice_remaining = 0;

    while (completed < n) {
        int shortest_process_index = -1;
        int shortest_remaining_time = __INT_MAX__;
        for (int i = 0; i < n; i++) {
            if (!processes[i].completed && processes[i].arrival_time <= current_time && processes[i].remaining_time < shortest_remaining_time) {
                shortest_process_index = i;
                shortest_remaining_time = processes[i].remaining_time;
            }
        }

        if (shortest_process_index == -1) {
            printf("CPU idle from time %d to %d\n", current_time, current_time + 1);
            current_time++;
            time_slice_remaining = 0;
            continue;
        }

        struct process *p = &processes[shortest_process_index];

        if (p->remaining_time > time_slice) {
            p->remaining_time -= time_slice;
            time_slice_remaining = time_slice;
        } else {
            time_slice_remaining = p->remaining_time;
            p->remaining_time = 0;
            p->completed = 1;
            p->completion_time = current_time + time_slice_remaining;
            p->waiting_time = p->completion_time - p->arrival_time - p->burst_time;
            p->turnaround_time = p->completion_time - p->arrival_time;
            printf("Process %d completed at time %d\n", p->pid, p->completion_time);
            completed++;
        }

        for (int i = 0; i < n; i++) {
            if (i != shortest_process_index && !processes[i].completed && processes[i].arrival_time <= current_time) {
                processes[i].waiting_time += time_slice_remaining;
            }
        }

        current_time += time_slice_remaining;
    }

    float total_waiting_time = 0.0;
    float total_turnaround_time = 0.0;

    for (int i = 0; i < n; i++) {
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
    }

    float avg_waiting_time = total_waiting_time / n;
    float avg_turnaround_time = total_turnaround_time / n;

    printf("Average
