/*
 * Experiment 2: Shortest-Job-First (SJF) CPU Scheduling (Non-preemptive)
 * Compile : gcc exp2_sjf.c -o sjf
 * Run     : ./sjf
 */
#include <stdio.h>

int main() {
    int n, i, j, temp;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    int pid[n], at[n], bt[n], ct[n], tat[n], wt[n], done[n];

    for (i = 0; i < n; i++) {
        pid[i] = i + 1;
        printf("Enter Arrival Time and Burst Time for P%d: ", pid[i]);
        scanf("%d %d", &at[i], &bt[i]);
        done[i] = 0;
    }

    int completed = 0, current_time = 0;
    float total_wt = 0, total_tat = 0;

    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");

    while (completed < n) {
        int idx = -1;
        int min_bt = 1e9;
        for (i = 0; i < n; i++) {
            if (!done[i] && at[i] <= current_time) {
                if (bt[i] < min_bt) {
                    min_bt = bt[i];
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            /* No process has arrived yet, jump time forward */
            current_time++;
            continue;
        }

        current_time += bt[idx];
        ct[idx] = current_time;
        tat[idx] = ct[idx] - at[idx];
        wt[idx] = tat[idx] - bt[idx];
        done[idx] = 1;
        completed++;

        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               pid[idx], at[idx], bt[idx], ct[idx], tat[idx], wt[idx]);

        total_wt += wt[idx];
        total_tat += tat[idx];
    }

    printf("\nAverage Waiting Time    = %.2f\n", total_wt / n);
    printf("Average Turnaround Time = %.2f\n", total_tat / n);
    return 0;
}
