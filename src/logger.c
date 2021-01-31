
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>

#include "logger.h"

/*
 * Prefixes for the different logging levels
 */
#define LOG_PREFIX_ERROR "ERROR"
#define LOG_PREFIX_WARNING "WARNING"
#define LOG_PREFIX_STATUS "STATUS"
#define LOG_PREFIX_DEBUG "DEBUG"

#define log_buffer_size 8192

static const char* LOG_LEVELS[] = { LOG_PREFIX_ERROR,
                    LOG_PREFIX_WARNING,
                    LOG_PREFIX_STATUS,
                    LOG_PREFIX_DEBUG };

FILE *log_open(const char *filename) {
   FILE *log_fp = fopen(filename, "a");
   if (log_fp == NULL ) {
      fprintf(stderr, "Can not open %s\n", filename);
   }
   return log_fp;
}

void log_close(FILE *log_fp) {
   if(log_fp != NULL)
      fclose(log_fp);
}

void log_write(int log_level, const char *filename, const int line_number, const FILE *log_fp, const char *fmt, ...) {

   if(log_fp == NULL) {
      fprintf(stderr, "Can not open log_fp\n");
      return;
   }

   //  static char buffer[log_buffer_size];
    static char log_buffer[log_buffer_size];
    static char datetime[100];
    static char line_str[20];
    static time_t now;
    now = time(NULL);

    strftime(datetime, 99, "%Y-%m-%d %H:%M:%S", localtime(&now));
    // format to string
    snprintf(line_str, 19, "%d", line_number);

    // get variable args and format to log_buffer
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(log_buffer, log_buffer_size, fmt, ap);
    va_end(ap);

   int res = fprintf((FILE *)log_fp,
         "%s: [%s] [%s:%s] %s\n"
               , LOG_LEVELS[log_level]
               , datetime
               , filename
               , line_str
               , log_buffer );

   if (res == -1) {
      // print_to_syslog(LOG_LEVEL_ERROR, "Unable to write to log file!");
      fprintf(stderr, "Unable to wirte to error log file!\n");
      return;
   }

   fflush((FILE *)log_fp);

}