#include <stdio.h>
#include <stdlib.h>
struct process
{
    int p_id;
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
    printf("Enter file name: ");
    scanf("%s", filename);
    FILE* fp = fopen(filename, "r");
}