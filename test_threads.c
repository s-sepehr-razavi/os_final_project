#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "user.h"
#include "stat.h"
#include "fcntl.h"
#include "x86.h"

#define SLEEP_TIME 100
#define NUM_THREADS 10
#define TEST_THREAD_COUNT 5
#define ITERATIONS 200

volatile int counter = 0;
lock_t* lock;

typedef struct {
    int arg;
    char threadName[30];
} ThreadArg;

void safe_str_copy(char *dest, const char *src, int max_len) {
    int i;
    for (i = 0; i < max_len - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}

void increment_and_print(ThreadArg *arg) {
    if (arg->arg) lock_acquire(lock);

    counter++;
    printf(1, "%s: %d\n", arg->threadName, counter);

    sleep(SLEEP_TIME);

    if (arg->arg) lock_release(lock);
}

void thread_func(void* arg, void* unused) {
    ThreadArg *threadArg = (ThreadArg *)arg;
    increment_and_print(threadArg);
    exit();
}

// Scheduler fairness test functions
int scheduler_test_counter = 0;
int scheduler_thread_counts[TEST_THREAD_COUNT] = {0};

void increment_thread_count(int thread_id) {
    for (int i = 0; i < ITERATIONS; i++) {
        scheduler_thread_counts[thread_id]++;
    }
}

void scheduler_test_thread_func(void *arg1, void *arg2) {
    int thread_id = *(int *) arg1;
    increment_thread_count(thread_id);
    exit();
}

// Function to count the number of fair threads
int count_fair_threads(int *scheduler_thread_counts, int thread_count) {
    int fair_count = 0;
    for (int i = 0; i < thread_count; i++) {
        if (scheduler_thread_counts[i] == ITERATIONS) {
            fair_count++;
        }
    }
    return fair_count;
}

// Function to join threads
void join_threads(int *thread_pids, int thread_count) {
    for (int i = 0; i < thread_count; i++) {
        thread_join(thread_pids[i]);
    }
}

// Function to assess and print the fairness result
int assess_fairness(int fair_count) {
    if (fair_count > 2) {
        printf(1, "Scheduler is not fair\n");
        return 0;
    }
    printf(1, "Scheduler Fairness Test Passed\n");
    return 1;
}

int scheduler_test() {
    printf(1, "\n\nScheduler Fairness Test Started\n");
    int fork_pid = fork();
    if (fork_pid == 0) {
        for (int i = 0; i < ITERATIONS; i++) {
            scheduler_test_counter++;
        }
        exit();
    }

    int thread_pids[TEST_THREAD_COUNT];
    int thread_ids[TEST_THREAD_COUNT] = {0, 1, 2, 3, 4};
    for (int i = 0; i < TEST_THREAD_COUNT; i++) {
        thread_pids[i] = thread_create(&scheduler_test_thread_func, (void *)(thread_ids + i), (void *)0);
    }
        
    wait();

    int fair_count = count_fair_threads(scheduler_thread_counts, TEST_THREAD_COUNT);
    join_threads(thread_pids, TEST_THREAD_COUNT);
    return assess_fairness(fair_count);
}

int main(int argc, char *argv[]) {
    lock_init(lock);
    ThreadArg args[NUM_THREADS];
    char *threadNames[NUM_THREADS] = {
        "Thread 1", "Thread 2", "Thread 3", "Thread 4", "Thread 5", 
        "Thread 6", "Thread 7", "Thread 8", "Thread 9", "Thread 10"
    };
    int pid[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        args[i].arg = 1;
        safe_str_copy(args[i].threadName, threadNames[i], sizeof(args[i].threadName));
        
        pid[i] = thread_create(&thread_func, &args[i], (void *)0);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_join(pid[i]);
    }

    scheduler_test(); // Call to the scheduler fairness test

    exit();
}
