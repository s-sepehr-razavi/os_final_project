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

volatile int counter = 0;
lock_t* lock;

typedef struct {
    int arg;
    char threadName[30];
} ThreadArg;

// Custom string copy function
void safe_str_copy(char *dest, const char *src, int max_len) {
    int i;
    for (i = 0; i < max_len - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0'; // Ensure null termination
}

void increment_and_print(ThreadArg *arg) {
    if (arg->arg) lock_acquire(lock);

    counter++;
    printf(1, "%s: %d \n", arg->threadName, counter);

    sleep(SLEEP_TIME);

    if (arg->arg) lock_release(lock);
}

void thread_func(void* arg, void* unused) {
    ThreadArg *threadArg = (ThreadArg *)arg;
    increment_and_print(threadArg);
    exit();
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

    exit();
}
