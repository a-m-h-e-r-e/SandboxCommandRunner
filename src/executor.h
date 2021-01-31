#ifndef COMMNAD_RUNNER_EXECUTOR_H
#define COMMNAD_RUNNER_EXECUTOR_H

#include <string.h>

#include "monitor.h"
#include "logger.h"

#define EXECUTOR_ERROR_EXIT(error_code)\
    {\
        LOG_ERROR_FORMAT(log_fp, "Error: System errno: %s; Internal errno: "#error_code, strerror(errno)); \
        close_file(input_file); \
        if (output_file == error_file) { \
            close_file(output_file); \
        } else { \
            close_file(output_file); \
            close_file(error_file);  \
        } \
        raise(SIGUSR1);  \
        exit(EXIT_FAILURE); \
    }



void execute(FILE *log_fp, struct config *);


#endif // COMMNAD_RUNNER_EXECUTOR_H