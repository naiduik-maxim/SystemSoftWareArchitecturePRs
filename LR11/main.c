#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define PRIORITY_HIGH   0
#define PRIORITY_MED    1
#define PRIORITY_LOW    2
#define MAX_PRIORITIES  3

static void die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

static long parse_long(const char *s, const char *what) {
    char *end;
    errno = 0;
    long v = strtol(s, &end, 10);
    if (errno != 0 || end == s || *end != '\0') {
        fprintf(stderr, "Invalid %s: %s\n", what, s);
        exit(EXIT_FAILURE);
    }
    return v;
}

static void subscriber(void) {
    sigset_t mask;
    sigemptyset(&mask);

    for (int i = 0; i < MAX_PRIORITIES; i++) {
        sigaddset(&mask, SIGRTMIN + i);
    }

    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
        die("sigprocmask");
    }

    printf("PID: %ld\n", (long)getpid());
    printf("Listening Priorities: 0 (High) -> 1 (Med) -> 2 (Low)\n");
    printf("Waiting for messages...\n\n");

    while (1) {
        siginfo_t info;
        int sig = sigwaitinfo(&mask, &info);
        
        if (sig == -1) {
            if (errno == EINTR) continue;
            die("sigwaitinfo");
        }

        int priority = sig - SIGRTMIN;
        int value = info.si_value.sival_int;

        printf("[SUBSCRIBER] Processed Priority: %d | Value: %d | From PID: %ld\n", 
               priority, value, (long)info.si_pid);

        sleep(1); 
        
        if (value < 0) {
            printf("Shutdown signal received. Exiting...\n");
            break;
        }
    }
}

static void publisher(pid_t target_pid, int argc, char *argv[]) {
    if ((argc - 3) % 2 != 0) {
        fprintf(stderr, "Error: Arguments must be in pairs <priority> <value>\n");
        exit(EXIT_FAILURE);
    }

    printf("Sending messages to PID %ld...\n\n", (long)target_pid);

    for (int i = 3; i < argc; i += 2) {
        int priority = (int)parse_long(argv[i], "priority");
        int value    = (int)parse_long(argv[i+1], "value");

        if (priority < 0 || priority >= MAX_PRIORITIES) {
            fprintf(stderr, "[ERROR] Invalid priority %d. Allowed: 0 (High), 1 (Med), 2 (Low). Skipping...\n", priority);
            continue;
        }

        union sigval val;
        val.sival_int = value;
        
        if (sigqueue(target_pid, SIGRTMIN + priority, val) == -1) {
            perror("sigqueue failed");
        } else {
            printf("[PUBLISHER] Sent Priority: %d | Value: %d\n", priority, value);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "  %s sub\n", argv[0]);
        fprintf(stderr, "  %s pub <PID> <prio1> <val1> [<prio2> <val2> ...]\n", argv[0]);
        fprintf(stderr, "Example:\n  %s pub 12345 2 100 0 500 1 300\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "sub") == 0) {
        subscriber();
    } else if (strcmp(argv[1], "pub") == 0) {
        if (argc < 5) {
            fprintf(stderr, "Error: Missing arguments for publisher.\n");
            fprintf(stderr, "Try: %s pub <PID> <priority> <value>\n", argv[0]);
            return EXIT_FAILURE;
        }
        pid_t pid = (pid_t)parse_long(argv[2], "PID");
        publisher(pid, argc, argv);
    } else {
        fprintf(stderr, "Unknown command: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}