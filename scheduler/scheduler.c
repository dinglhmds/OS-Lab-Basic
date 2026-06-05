#include "scheduler.h"
#include "common.h"
#include "utils.h"
#include <math.h>

void reset_procs(PCB procs[], int n) {
    for (int i = 0; i < n; i++) {
        procs[i].start_time = -1;
        procs[i].finish_time = 0;
        procs[i].turnaround_time = 0;
        procs[i].waiting_time = 0;
        procs[i].remaining_time = procs[i].burst_time;
        procs[i].completed = 0;
    }
}

void copy_procs(PCB dst[], const PCB src[], int n) {
    memcpy(dst, src, sizeof(PCB) * n);
}

void input_processes(PCB procs[], int *n) {
    printf("Enter number of processes (max %d): ", MAX_PROCS);
    if (scanf("%d", n) != 1) PANIC("Failed to read process count");
    if (*n < 1 || *n > MAX_PROCS) PANIC("Invalid process count");
    for (int i = 0; i < *n; i++) {
        printf("\n--- Process %d ---\n", i + 1);
        procs[i].pid = i + 1;
        printf("Name: ");
        if (scanf("%15s", procs[i].name) != 1) PANIC("Failed to read name");
        printf("Arrival Time: ");
        if (scanf("%f", &procs[i].arrival_time) != 1) PANIC("Failed to read arrival time");
        printf("Burst Time: ");
        if (scanf("%f", &procs[i].burst_time) != 1) PANIC("Failed to read burst time");
        printf("Priority (lower = higher): ");
        if (scanf("%d", &procs[i].priority) != 1) PANIC("Failed to read priority");
        procs[i].remaining_time = procs[i].burst_time;
        procs[i].completed = 0;
    }
}

void load_test_case(PCB procs[], int *n, int case_id) {
    switch (case_id) {
        case 1:
            *n = 4;
            procs[0] = (PCB){1, "P1", 0, 8, 2, 0,0,0,0,8,0};
            procs[1] = (PCB){2, "P2", 1, 4, 1, 0,0,0,0,4,0};
            procs[2] = (PCB){3, "P3", 2, 9, 4, 0,0,0,0,9,0};
            procs[3] = (PCB){4, "P4", 3, 5, 3, 0,0,0,0,5,0};
            break;
        case 2:
            *n = 5;
            procs[0] = (PCB){1, "A", 0, 5, 3, 0,0,0,0,5,0};
            procs[1] = (PCB){2, "B", 1, 3, 1, 0,0,0,0,3,0};
            procs[2] = (PCB){3, "C", 2, 8, 4, 0,0,0,0,8,0};
            procs[3] = (PCB){4, "D", 3, 6, 2, 0,0,0,0,6,0};
            procs[4] = (PCB){5, "E", 4, 2, 5, 0,0,0,0,2,0};
            break;
        case 3:
            *n = 3;
            procs[0] = (PCB){1, "X", 0, 10, 1, 0,0,0,0,10,0};
            procs[1] = (PCB){2, "Y", 0, 5, 2, 0,0,0,0,5,0};
            procs[2] = (PCB){3, "Z", 0, 8, 3, 0,0,0,0,8,0};
            break;
        default:
            PANIC("Unknown test case %d", case_id);
    }
    reset_procs(procs, *n);
}

static void add_gantt(GanttSlot gantt[], int *len, int pid, float start, float end) {
    if (*len > 0 && gantt[*len - 1].pid == pid) {
        gantt[*len - 1].end = end;
    } else {
        gantt[*len].pid = pid;
        gantt[*len].start = start;
        gantt[*len].end = end;
        (*len)++;
    }
}

void fcfs(PCB procs[], int n, GanttSlot gantt[], int *gantt_len) {
    reset_procs(procs, n);
    *gantt_len = 0;
    float time = 0;
    int completed = 0;
    while (completed < n) {
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (!procs[i].completed && procs[i].arrival_time <= time) {
                if (idx == -1 || procs[i].arrival_time < procs[idx].arrival_time ||
                    (procs[i].arrival_time == procs[idx].arrival_time && procs[i].pid < procs[idx].pid)) {
                    idx = i;
                }
            }
        }
        if (idx == -1) {
            time++;
            continue;
        }
        if (procs[idx].start_time < 0) procs[idx].start_time = time;
        add_gantt(gantt, gantt_len, procs[idx].pid, time, time + procs[idx].burst_time);
        time += procs[idx].burst_time;
        procs[idx].finish_time = time;
        procs[idx].turnaround_time = time - procs[idx].arrival_time;
        procs[idx].waiting_time = procs[idx].turnaround_time - procs[idx].burst_time;
        procs[idx].completed = 1;
        completed++;
    }
}

void sjf_nonpreemptive(PCB procs[], int n, GanttSlot gantt[], int *gantt_len) {
    reset_procs(procs, n);
    *gantt_len = 0;
    float time = 0;
    int completed = 0;
    while (completed < n) {
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (!procs[i].completed && procs[i].arrival_time <= time) {
                if (idx == -1 || procs[i].burst_time < procs[idx].burst_time ||
                    (procs[i].burst_time == procs[idx].burst_time && procs[i].pid < procs[idx].pid)) {
                    idx = i;
                }
            }
        }
        if (idx == -1) { time++; continue; }
        if (procs[idx].start_time < 0) procs[idx].start_time = time;
        add_gantt(gantt, gantt_len, procs[idx].pid, time, time + procs[idx].burst_time);
        time += procs[idx].burst_time;
        procs[idx].finish_time = time;
        procs[idx].turnaround_time = time - procs[idx].arrival_time;
        procs[idx].waiting_time = procs[idx].turnaround_time - procs[idx].burst_time;
        procs[idx].completed = 1;
        completed++;
    }
}

void sjf_preemptive(PCB procs[], int n, GanttSlot gantt[], int *gantt_len) {
    reset_procs(procs, n);
    *gantt_len = 0;
    float time = 0;
    int completed = 0;
    while (completed < n) {
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (!procs[i].completed && procs[i].arrival_time <= time) {
                if (idx == -1 || procs[i].remaining_time < procs[idx].remaining_time ||
                    (procs[i].remaining_time == procs[idx].remaining_time && procs[i].pid < procs[idx].pid)) {
                    idx = i;
                }
            }
        }
        if (idx == -1) { time++; continue; }
        if (procs[idx].start_time < 0) procs[idx].start_time = time;
        add_gantt(gantt, gantt_len, procs[idx].pid, time, time + 1);
        procs[idx].remaining_time -= 1;
        time++;
        if (procs[idx].remaining_time <= 0) {
            procs[idx].finish_time = time;
            procs[idx].turnaround_time = time - procs[idx].arrival_time;
            procs[idx].waiting_time = procs[idx].turnaround_time - procs[idx].burst_time;
            procs[idx].completed = 1;
            completed++;
        }
    }
}

void rr(PCB procs[], int n, float quantum, GanttSlot gantt[], int *gantt_len) {
    reset_procs(procs, n);
    *gantt_len = 0;
    int ready[MAX_PROCS];
    int front = 0, rear = 0;
    float time = 0;
    int completed = 0;
    int visited[MAX_PROCS] = {0};

    /* Enqueue initially arrived processes */
    for (int i = 0; i < n; i++) {
        if (procs[i].arrival_time <= time) {
            ready[rear++] = i;
            visited[i] = 1;
        }
    }

    while (completed < n) {
        if (front == rear) {
            time++;
            for (int i = 0; i < n; i++) {
                if (!visited[i] && procs[i].arrival_time <= time) {
                    ready[rear++] = i;
                    visited[i] = 1;
                }
            }
            continue;
        }
        int idx = ready[front++];
        if (procs[idx].start_time < 0) procs[idx].start_time = time;
        float exec = (procs[idx].remaining_time < quantum) ? procs[idx].remaining_time : quantum;
        add_gantt(gantt, gantt_len, procs[idx].pid, time, time + exec);
        time += exec;
        procs[idx].remaining_time -= exec;

        for (int i = 0; i < n; i++) {
            if (!visited[i] && procs[i].arrival_time <= time) {
                ready[rear++] = i;
                visited[i] = 1;
            }
        }

        if (procs[idx].remaining_time <= 0) {
            procs[idx].finish_time = time;
            procs[idx].turnaround_time = time - procs[idx].arrival_time;
            procs[idx].waiting_time = procs[idx].turnaround_time - procs[idx].burst_time;
            procs[idx].completed = 1;
            completed++;
        } else {
            ready[rear++] = idx;
        }
    }
}

void priority_nonpreemptive(PCB procs[], int n, GanttSlot gantt[], int *gantt_len) {
    reset_procs(procs, n);
    *gantt_len = 0;
    float time = 0;
    int completed = 0;
    while (completed < n) {
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (!procs[i].completed && procs[i].arrival_time <= time) {
                if (idx == -1 || procs[i].priority < procs[idx].priority ||
                    (procs[i].priority == procs[idx].priority && procs[i].pid < procs[idx].pid)) {
                    idx = i;
                }
            }
        }
        if (idx == -1) { time++; continue; }
        if (procs[idx].start_time < 0) procs[idx].start_time = time;
        add_gantt(gantt, gantt_len, procs[idx].pid, time, time + procs[idx].burst_time);
        time += procs[idx].burst_time;
        procs[idx].finish_time = time;
        procs[idx].turnaround_time = time - procs[idx].arrival_time;
        procs[idx].waiting_time = procs[idx].turnaround_time - procs[idx].burst_time;
        procs[idx].completed = 1;
        completed++;
    }
}

void priority_preemptive(PCB procs[], int n, GanttSlot gantt[], int *gantt_len) {
    reset_procs(procs, n);
    *gantt_len = 0;
    float time = 0;
    int completed = 0;
    while (completed < n) {
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (!procs[i].completed && procs[i].arrival_time <= time) {
                if (idx == -1 || procs[i].priority < procs[idx].priority ||
                    (procs[i].priority == procs[idx].priority && procs[i].pid < procs[idx].pid)) {
                    idx = i;
                }
            }
        }
        if (idx == -1) { time++; continue; }
        if (procs[idx].start_time < 0) procs[idx].start_time = time;
        add_gantt(gantt, gantt_len, procs[idx].pid, time, time + 1);
        procs[idx].remaining_time -= 1;
        time++;
        if (procs[idx].remaining_time <= 0) {
            procs[idx].finish_time = time;
            procs[idx].turnaround_time = time - procs[idx].arrival_time;
            procs[idx].waiting_time = procs[idx].turnaround_time - procs[idx].burst_time;
            procs[idx].completed = 1;
            completed++;
        }
    }
}

void print_results(const PCB procs[], int n) {
    printf("\n%-4s %-8s %-10s %-10s %-10s %-12s %-12s %-12s\n",
           "PID", "Name", "Arrival", "Burst", "Priority", "Finish", "Turnaround", "Waiting");
    print_table_line(90);
    float avg_tat = 0, avg_wt = 0;
    for (int i = 0; i < n; i++) {
        printf("%-4d %-8s %-10.2f %-10.2f %-10d %-12.2f %-12.2f %-12.2f\n",
               procs[i].pid, procs[i].name, procs[i].arrival_time, procs[i].burst_time,
               procs[i].priority, procs[i].finish_time, procs[i].turnaround_time, procs[i].waiting_time);
        avg_tat += procs[i].turnaround_time;
        avg_wt += procs[i].waiting_time;
    }
    print_table_line(90);
    printf("Average Turnaround Time: %.2f\n", avg_tat / n);
    printf("Average Waiting Time:    %.2f\n", avg_wt / n);
}

void print_gantt(const GanttSlot gantt[], int len) {
    printf("\nGantt Chart:\n");
    /* Top border */
    for (int i = 0; i < len; i++) {
        printf("+");
        for (int j = 0; j < 8; j++) printf("-");
    }
    printf("+\n");
    /* Process IDs */
    for (int i = 0; i < len; i++) {
        printf("|  P%-4d ", gantt[i].pid);
    }
    printf("|\n");
    /* Bottom border */
    for (int i = 0; i < len; i++) {
        printf("+");
        for (int j = 0; j < 8; j++) printf("-");
    }
    printf("+\n");
    /* Times */
    printf("%.1f", gantt[0].start);
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < 8 - (i == 0 ? 3 : 0); j++) printf(" ");
        printf("%.1f", gantt[i].end);
    }
    printf("\n");
}

void print_comparison(const PCB procs[], int n, const char *algo_name) {
    float avg_tat = 0, avg_wt = 0;
    for (int i = 0; i < n; i++) {
        avg_tat += procs[i].turnaround_time;
        avg_wt += procs[i].waiting_time;
    }
    printf("%-24s  Avg TAT: %6.2f   Avg WT: %6.2f\n", algo_name, avg_tat / n, avg_wt / n);
}
