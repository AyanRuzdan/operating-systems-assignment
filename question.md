# You need to program the Shortest Job with Pre-emption scheduling algorithm reading data from file.

**Aim**: The program will read from a file containing a list of processes with relevant data for the process. The program will simulate the execution of the processes. It will print out the time taken by each process to complete (turnaround time) and the wait time and compute the average turnaround time.

**Inputs:** A filename from the keyboard, read the file for the relevant data. You may assume that a swap (context switching time) is small enough that it can be ignored.

The file containing the information on the processes will have each process on a separate line. The processes will be in the file in the order in which they arrive at the OS.

Each line will have a process name that will be a string.

Followung the name will be the arrival time of the process. The arrival time will be in reference to the previous process.

Following this will be the total execution time.

**Outputs:** A prompt for which file is to be read. A prompt for time for a time-slice. A list of each processes and the time it took for it to complete. Then an average time for processes to complete.
