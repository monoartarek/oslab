/*
 * Experiment 10: Resource-Allocation Graph Algorithm (Deadlock Detection)
 * Uses the "reduction" method: an Allocation and Request matrix, plus
 * an Available resource vector, to check if the system is in a safe /
 * deadlock-free state.
 *
 * Compile : gcc exp10_raga.c -o raga
 * Run     : ./raga
 */
#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, m, i, j;

    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter number of resource types: ");
    scanf("%d", &m);

    int alloc[n][m], request[n][m];
    int available[m];
    int finish[n];

    printf("\nEnter the Allocation matrix (%d x %d):\n", n, m);
    for (i = 0; i < n; i++) {
        printf("Process P%d: ", i);
        for (j = 0; j < m; j++)
            scanf("%d", &alloc[i][j]);
        finish[i] = 0;
    }

    printf("\nEnter the Request matrix (%d x %d):\n", n, m);
    for (i = 0; i < n; i++) {
        printf("Process P%d: ", i);
        for (j = 0; j < m; j++)
            scanf("%d", &request[i][j]);
    }

    printf("\nEnter the Available vector (%d values):\n", m);
    for (j = 0; j < m; j++)
        scanf("%d", &available[j]);

    int work[m];
    for (j = 0; j < m; j++) work[j] = available[j];

    int sequence[n], seq_count = 0;
    int changed = 1;

    while (changed) {
        changed = 0;
        for (i = 0; i < n; i++) {
            if (!finish[i]) {
                int can_proceed = 1;
                for (j = 0; j < m; j++) {
                    if (request[i][j] > work[j]) {
                        can_proceed = 0;
                        break;
                    }
                }
                if (can_proceed) {
                    for (j = 0; j < m; j++)
                        work[j] += alloc[i][j];
                    finish[i] = 1;
                    sequence[seq_count++] = i;
                    changed = 1;
                }
            }
        }
    }

    int deadlock = 0;
    printf("\n--- Result ---\n");
    for (i = 0; i < n; i++) {
        if (!finish[i]) {
            deadlock = 1;
            printf("Process P%d is STUCK (part of deadlock / cycle)\n", i);
        }
    }

    if (!deadlock) {
        printf("No deadlock detected. Safe completion sequence: ");
        for (i = 0; i < seq_count; i++)
            printf("P%d ", sequence[i]);
        printf("\n");
    } else {
        printf("Deadlock DETECTED in the system!\n");
    }

    return 0;
}
