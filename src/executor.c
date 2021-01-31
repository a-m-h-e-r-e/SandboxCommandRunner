#define _DEFAULT_SOURCE
#define _POSIX_SOURCE
#define _GNU_SOURCE             /* See feature_test_macros(7) */

#include <stdlib.h>
#include <fcntl.h>              /* Obtain O_* constant definitions */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <stdarg.h>
#include <dlfcn.h>




#include <sys/resource.h>
#include <sys/types.h>


#include "executor.h"
#include "monitor.h"
#include "logger.h"
#include "rules/seccomp_rules.h"

void close_file(FILE *fp) {
   if(fp != NULL) 
      fclose(fp);
}

void execute(FILE *log_fp, struct config *conf){
   FILE *input_file = NULL, *output_file = NULL, *error_file = NULL;

   // set stack limit
   if (conf->max_stack != UNLIMITED) {
      struct rlimit max_stack;
      max_stack.rlim_cur = max_stack.rlim_max = (rlim_t)(conf->max_stack);
      if (setrlimit(RLIMIT_STACK, &max_stack) != 0){
         EXECUTOR_ERROR_EXIT(SETRLIMIT_FIALED);
      }
   }

   // set memory limit
   if (conf->max_memory != UNLIMITED) {
      struct rlimit max_memory;
      max_memory.rlim_cur = max_memory.rlim_max = (rlim_t)(conf->max_memory);
      if (setrlimit(RLIMIT_AS, &max_memory) != 0) {
         EXECUTOR_ERROR_EXIT(SETRLIMIT_FIALED);
      }
   }

   // set cpu time limit
   if (conf->max_cpu_time !=  UNLIMITED) {
      struct rlimit max_cpu_time;
      max_cpu_time.rlim_cur = max_cpu_time.rlim_max = (rlim_t)((conf->max_cpu_time + 1000) / 1000);
      if (setrlimit(RLIMIT_CPU, &max_cpu_time) != 0) {
         EXECUTOR_ERROR_EXIT(SETRLIMIT_FIALED);
      }
   }

   // set output size limit
   if (conf->max_output_size != UNLIMITED) {
      struct rlimit max_output_size;
      max_output_size.rlim_cur = max_output_size.rlim_max = (rlim_t)(conf->max_output_size);
      if (setrlimit(RLIMIT_FSIZE, &max_output_size) != 0) {
         EXECUTOR_ERROR_EXIT(SETRLIMIT_FIALED);
      }
   }

   // set processor number limit  
   if (conf->max_process_number != UNLIMITED) {
      struct rlimit max_process_number;
      max_process_number.rlim_cur = max_process_number.rlim_max = (rlim_t)(conf->max_process_number);
      if (setrlimit(RLIMIT_NPROC, &max_process_number) != 0) {
         EXECUTOR_ERROR_EXIT(SETRLIMIT_FIALED);
      }
   }

   // redirect file -> stdin
   if (conf->input_path != NULL) {
      input_file = fopen(conf->input_path, "r");
      if (input_file == NULL) {
         EXECUTOR_ERROR_EXIT(DUP2_FAILED);
      } else {
         if(dup2(fileno(input_file), fileno(stdin)) == -1) {
            EXECUTOR_ERROR_EXIT(DUP2_FAILED);
         }
      }
   }

   // redirect stdout -> file
   if (conf->output_path != NULL) {
      output_file = fopen(conf->output_path, "w");
      if (output_file == NULL) {
         EXECUTOR_ERROR_EXIT(DUP2_FAILED);
      } else {
         if(dup2(fileno(output_file), fileno(stdout)) == -1) {
            EXECUTOR_ERROR_EXIT(DUP2_FAILED);
         } else {
         }
      }
   }

   // redirect stderr -> file
   if (conf->error_path != NULL) {
      if (conf->output_path != NULL && strcmp(conf->output_path, conf->error_path) == 0) {
         error_file = output_file;
      } else {
         error_file = fopen(conf->error_path, "w");
         if (error_file == NULL) {
            EXECUTOR_ERROR_EXIT(DUP2_FAILED);
         } else {
            if(dup2(fileno(error_file), fileno(stderr)) == -1) {
               EXECUTOR_ERROR_EXIT(DUP2_FAILED);
            }
         }
      }
   }

   // load seccomp rule
   if(conf->seccomp_rule != NULL) {
      if(strcmp(conf->seccomp_rule, "general") == 0) {
         if(general_rule(conf) != 0) {
            EXECUTOR_ERROR_EXIT(LOAD_SECCOMP_FAILED);
         }
      } else if(strcmp(conf->seccomp_rule, "cpp") == 0) {

      } else {
         EXECUTOR_ERROR_EXIT(INVALID_CONFIG);
      }
   }

   // execute
  if (execve(conf->exe_path, conf->args, conf->env) == -1) {
   // if this is executed then execve has failed
   EXECUTOR_ERROR_EXIT(EXECVE_FAILED);
  }
   


}