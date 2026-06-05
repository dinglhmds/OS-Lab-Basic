#ifndef SCHEDULER_H
#define SCHEDULER_H

#define MAX_NAME_LEN 16
#define MAX_PROCS 64

typedef struct {
    int pid;
    char name[MAX_NAME_LEN];
    float arrival_time;
    float burst_time;
    int priority;
    float start_time;
    float finish_time;
    float turnaround_time;
    float waiting_time;
    float remaining_time;
    int completed;
} PCB;

typedef struct {
    int pid;
    float start;
    float end;
} GanttSlot;

/* Input */
void input_processes(PCB procs[], int *n);
void load_test_case(PCB procs[], int *n, int case_id);

/* Algorithms */
void fcfs(PCB procs[], int n, GanttSlot gantt[], int *gantt_len);
void sjf_nonpreemptive(PCB procs[], int n, GanttSlot gantt[], int *gantt_len);
void sjf_preemptive(PCB procs[], int n, GanttSlot gantt[], int *gantt_len);
void rr(PCB procs[], int n, float quantum, GanttSlot gantt[], int *gantt_len);
void priority_nonpreemptive(PCB procs[], int n, GanttSlot gantt[], int *gantt_len);
void priority_preemptive(PCB procs[], int n, GanttSlot gantt[], int *gantt_len);

/* Output */
void print_results(const PCB procs[], int n);
void print_gantt(const GanttSlot gantt[], int len);
void print_comparison(const PCB procs[], int n, const char *algo_name);

/* Utility */
void reset_procs(PCB procs[], int n);
void copy_procs(PCB dst[], const PCB src[], int n);

#endif /* SCHEDULER_H */
