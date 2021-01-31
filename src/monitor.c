#define _GNU_SOURCE
#define _POSIX_SOURCE

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h> 
#include <pthread.h>
#include <signal.h>
// #include <unistd.h>
// #include <stdlib.h>


#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>

#include "monitor.h"
#include "executor.h"
#include "logger.h"

bool check_config(struct config* conf);
void *process_timeout(void *);

void monitor(struct config *conf, struct result *res){
   FILE *log_fp = log_open(conf->log_path);

   // check user previlage
   uid_t uid = getuid();
   if(uid != 0) ERROR_EXIT(ROOT_REQUIRED);

   // check config
   if(!check_config(conf)) ERROR_EXIT(INVALID_CONFIG);

   // record start time
   struct timeval start_time, end_time;
   gettimeofday(&start_time, NULL);

   pid_t child_pid;
   child_pid = fork();


   // check cloning failure
   if (child_pid < 0) {
      ERROR_EXIT(FORK_FAILED);
   } else if (child_pid == 0) {
      // inside child
      execute(log_fp, conf);
   } else if (child_pid > 0) {
      // inside parent
      // create thread to kill the child on timeout
      pthread_t thread_id = 0;
      if(conf->max_real_time != UNLIMITED){
         struct timeout_monitor mon;
         mon.pid = child_pid;
         mon.timeout = conf->max_real_time;
         if(pthread_create(&thread_id, NULL, process_timeout, (void *)&mon) != 0) {
            kill(child_pid, SIGKILL);
            ERROR_EXIT(PTHREAD_FAILED);
         }
      }

      // process status
      int status;
      struct rusage resource_usage;

      // wait process to terminate
      if(wait4(child_pid, &status, WSTOPPED, &resource_usage) == -1){
         ERROR_EXIT(WAIT_FAILED);
      }

      // save real_time
      gettimeofday(&end_time, NULL);
      int time_taken;
      time_taken = end_time.tv_sec * 1000 + end_time.tv_usec / 1000; 
      time_taken = time_taken - (start_time.tv_sec * 1000 - start_time.tv_usec / 1000); 
      res->real_time = (int      )time_taken;


      // save cpu and memory 
      // res->cpu_time = (resource_usage.ru_utime.tv_sec * 1e6) + (resource_usage.ru_utime.tv_usec * 1e-6);
      res->cpu_time = resource_usage.ru_utime.tv_sec * 1000 +
                                       resource_usage.ru_utime.tv_usec / 1000;
      res->memory = resource_usage.ru_maxrss * 1024;

      // cancel thread
      if(pthread_cancel(thread_id) != 0){
         // printf("Thread cancel failed\n");
      }


      // if child was terminated by a signal
      if(WIFSIGNALED(status)){
         res->signal = WTERMSIG(status);

         // if signaled by process_timeout or by setrlimit
         if (res->signal == SIGKILL) {
            if (conf->max_real_time != UNLIMITED && res->real_time > conf->max_real_time) {
               res->result = REAL_TIME_LIMIT_EXCEEDED;
            }
            if (conf->max_cpu_time != UNLIMITED && res->cpu_time > conf->max_cpu_time) {
               res->result = CPU_TIME_LIMIT_EXCEEDED;
            }
         }
         // if signaled by setrlimit
         else if(res->signal == SIGXCPU){
            res->result = CPU_TIME_LIMIT_EXCEEDED;
         }
         // if signaled by user
         else if (res->signal == SIGUSR1) {
            res->result = SYSTEM_ERROR;
         }
         // check segementation fault
         else if (res->signal == SIGSEGV) {
            if(conf->max_memory != UNLIMITED && res->memory > conf->max_memory) {
               res->result = MEMORY_LIMIT_EXCEEDED;
            } else {
               res->result = RUNTIME_ERROR;
            }
         } 
         // check erroneous arithmetic operation
         else if (res->signal == SIGFPE) {
            res->result = RUNTIME_ERROR;
         }
         else {
            res->result = RUNTIME_ERROR;
         }
      } else {
         if(WIFEXITED(status)){
            res->exitcode = WEXITSTATUS(status);
            if(res->exitcode == 0) {
               if (conf->max_cpu_time != UNLIMITED && res->cpu_time > conf->max_cpu_time) {
                  res->result = CPU_TIME_LIMIT_EXCEEDED;
               }
               if (conf->max_real_time != UNLIMITED && res->real_time > conf->max_real_time) {
                  res->result = REAL_TIME_LIMIT_EXCEEDED;
               }
               if (conf->max_memory != UNLIMITED && res->memory > conf->max_memory) {
                  res->result = MEMORY_LIMIT_EXCEEDED;
               }               
            } else {
               res->result = RUNTIME_ERROR;
            }
         } else {
            res->result = SYSTEM_ERROR;
         }
      }
   }
}

void *process_timeout(void *mon){
   struct timeout_monitor *monit = mon;
   if(sleep((monit->timeout + 1000) / 1000) != 0){
      kill(monit->pid, SIGKILL);
      return NULL;
   }
   kill(monit->pid, SIGKILL);
   return NULL;
}

bool check_config(struct config *conf){
   if(conf->max_real_time < 1 && conf->max_real_time != UNLIMITED) return false;
   if(conf->max_cpu_time < 1 && conf->max_cpu_time != UNLIMITED) return false;
   if(conf->max_memory < 1 && conf->max_memory != UNLIMITED) return false;
   if(conf->max_output_size < 1 && conf->max_output_size != UNLIMITED) return false;
   if(conf->max_process_number < 1 && conf->max_process_number != UNLIMITED) return false;
   if(conf->max_stack < 1) return false;
   return true;
}