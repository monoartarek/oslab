/*
 * Experiment 4: Round Robin (RR) CPU Scheduling
 * Compile : gcc exp4_rr.c -o rr
 * Run     : ./rr
 */
#include <stdio.h>

int main() {
    int n, i, tq;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    int pid[n], at[n], bt[n], rem[n], ct[n], tat[n], wt[n];

    for (i = 0; i < n; i++) {
        pid[i] = i + 1;
        printf("Enter Arrival Time and Burst Time for P%d: ", pid[i]);
        scanf("%d %d", &at[i], &bt[i]);
        rem[i] = bt[i];
    }

    printf("Enter Time Quantum: ");
    scanf("%d", &tq);

    int queue[1000], front = 0, rear = 0;
    int in_queue[n];
    for (i = 0; i < n; i++) in_queue[i] = 0;

    /* Sort by arrival time to enqueue correctly, simplest: assume arrival 0 or push as time reaches */
    int current_time = 0, completed = 0;
    int visited[n];
    for (i = 0; i < n; i++) visited[i] = 0;

    /* Enqueue all processes that have arrived at time 0 */
    for (i = 0; i < n; i++) {
        if (at[i] <= current_time && !in_queue[i]) {
            queue[rear++] = i;
            in_queue[i] = 1;
        }
    }

    printf("\nGantt Chart (PID : Start -> End)\n");

    while (completed < n) {
        if (front == rear) {
            /* queue empty, advance time to next arrival */
            current_time++;
            for (i = 0; i < n; i++) {
                if (at[i] <= current_time && !in_queue[i] && rem[i] > 0) {
                    queue[rear++] = i;
                    in_queue[i] = 1;
                }
            }
            continue;
        }

        int idx = queue[front++];
        int slice = (rem[idx] < tq) ? rem[idx] : tq;
        int start = current_time;
        current_time += slice;
        rem[idx] -= slice;

        printf("P%d : %d -> %d\n", pid[idx], start, current_time);

        /* Enqueue any new arrivals during this slice (except the one just run) */
        for (i = 0; i < n; i++) {
            if (at[i] <= current_time && !in_queue[i] && rem[i] > 0) {
                queue[rear++] = i;
                in_queue[i] = 1;
            }
        }

        if (rem[idx] > 0) {
            queue[rear++] = idx; /* re-queue, still in_queue stays 1 */
        } else {
            ct[idx] = current_time;
            tat[idx] = ct[idx] - at[idx];
            wt[idx] = tat[idx] - bt[idx];
            completed++;
        }
    }

    float total_wt = 0, total_tat = 0;
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", pid[i], at[i], bt[i], ct[i], tat[i], wt[i]);
        total_wt += wt[i];
        total_tat += tat[i];
    }

    printf("\nAverage Waiting Time    = %.2f\n", total_wt / n);
    printf("Average Turnaround Time = %.2f\n", total_tat / n);
    return 0;
}
