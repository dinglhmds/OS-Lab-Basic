#include "scheduler.h"
#include "common.h"
#include "utils.h"
#include <string.h>

static void run_all_algorithms(PCB orig[], int n) {
    PCB procs[MAX_PROCS];
    GanttSlot gantt[256];
    int gantt_len;

    printf("\n");
    print_banner("FCFS");
    copy_procs(procs, orig, n);
    fcfs(procs, n, gantt, &gantt_len);
    print_gantt(gantt, gantt_len);
    print_results(procs, n);

    printf("\n");
    print_banner("SJF (Non-Preemptive)");
    copy_procs(procs, orig, n);
    sjf_nonpreemptive(procs, n, gantt, &gantt_len);
    print_gantt(gantt, gantt_len);
    print_results(procs, n);

    printf("\n");
    print_banner("SJF (Preemptive)");
    copy_procs(procs, orig, n);
    sjf_preemptive(procs, n, gantt, &gantt_len);
    print_gantt(gantt, gantt_len);
    print_results(procs, n);

    printf("\n");
    print_banner("Round Robin (q=2)");
    copy_procs(procs, orig, n);
    rr(procs, n, 2.0f, gantt, &gantt_len);
    print_gantt(gantt, gantt_len);
    print_results(procs, n);

    printf("\n");
    print_banner("Priority (Non-Preemptive)");
    copy_procs(procs, orig, n);
    priority_nonpreemptive(procs, n, gantt, &gantt_len);
    print_gantt(gantt, gantt_len);
    print_results(procs, n);

    printf("\n");
    print_banner("Priority (Preemptive)");
    copy_procs(procs, orig, n);
    priority_preemptive(procs, n, gantt, &gantt_len);
    print_gantt(gantt, gantt_len);
    print_results(procs, n);

    printf("\n");
    print_banner("Comparison Summary");
    copy_procs(procs, orig, n); fcfs(procs, n, gantt, &gantt_len);
    print_comparison(procs, n, "FCFS");
    copy_procs(procs, orig, n); sjf_nonpreemptive(procs, n, gantt, &gantt_len);
    print_comparison(procs, n, "SJF Non-Preemptive");
    copy_procs(procs, orig, n); sjf_preemptive(procs, n, gantt, &gantt_len);
    print_comparison(procs, n, "SJF Preemptive");
    copy_procs(procs, orig, n); rr(procs, n, 2.0f, gantt, &gantt_len);
    print_comparison(procs, n, "Round Robin (q=2)");
    copy_procs(procs, orig, n); priority_nonpreemptive(procs, n, gantt, &gantt_len);
    print_comparison(procs, n, "Priority Non-Preemptive");
    copy_procs(procs, orig, n); priority_preemptive(procs, n, gantt, &gantt_len);
    print_comparison(procs, n, "Priority Preemptive");
}

static void run_tests(void) {
    PCB orig[MAX_PROCS];
    int n;
    for (int c = 1; c <= 3; c++) {
        printf("\n========== TEST CASE %d ==========\n", c);
        load_test_case(orig, &n, c);
        printf("Processes loaded: %d\n", n);
        run_all_algorithms(orig, n);
    }
}

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "test") == 0) {
        run_tests();
        return 0;
    }

    PCB procs[MAX_PROCS];
    int n = 0;
    int choice;

    print_banner("CPU Scheduler Simulator");
    printf("1. Manual Input\n");
    printf("2. Load Test Case 1\n");
    printf("3. Load Test Case 2\n");
    printf("4. Load Test Case 3\n");
    printf("Choice: ");
    if (scanf("%d", &choice) != 1) { WARN("Invalid input"); return 1; }

    if (choice == 1) {
        input_processes(procs, &n);
    } else if (choice >= 2 && choice <= 4) {
        load_test_case(procs, &n, choice - 1);
        printf("Loaded %d processes.\n", n);
    } else {
        WARN("Invalid choice.");
        return 1;
    }

    run_all_algorithms(procs, n);
    return 0;
}
