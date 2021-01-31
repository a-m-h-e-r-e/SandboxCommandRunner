#ifndef COMMAND_RUNNER_MONITOR_H
#define COMMAND_RUNNER_MONITOR_H

#include <sys/types.h>
#include <stdio.h>


// software info
#define VERSION 0x010000
#define AUTHOR "Name: Amir Kheiru Email: heiruamir@gmail.com"

#define UNLIMITED  -1
#define MAX_ARGS 256
#define MAX_ENV 256

enum {
   SUCCESS = 0,
   ROOT_REQUIRED = -1,
   INVALID_CONFIG = -2,
   FORK_FAILED = -3,
   PTHREAD_FAILED = -4,
   WAIT_FAILED = -5,
   SETRLIMIT_FIALED = -6,
   LOAD_SECCOMP_FAILED = -7,
   EXECVE_FAILED = -8,
};
struct config {
   int max_real_time;
   int max_cpu_time;
   long max_memory;
   long max_stack;
   long max_output_size;
   int max_process_number;
   char *exe_path;
   char *input_path;
   char *output_path;
   char *log_path;
   char *error_path;
   char *args[MAX_ARGS];
   char *env[MAX_ENV];
   uid_t uid;
   gid_t gid;
   char *seccomp_rule;
};
struct timeout_monitor{
   pid_t pid;
   int timeout;
};


enum {
   WRONG_ANSWER = -1,
   REAL_TIME_LIMIT_EXCEEDED = 2,
   CPU_TIME_LIMIT_EXCEEDED = 3,
   MEMORY_LIMIT_EXCEEDED = 4,
   RUNTIME_ERROR = 5,
   SYSTEM_ERROR = 6
};
struct result {
   int real_time;
   int cpu_time;
   long memory;
   int signal;
   int result;
   int exitcode;
};

void monitor(struct config *, struct result *);

#endif // COMMAND_RUNNER_MONITOR_H