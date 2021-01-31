#ifndef COMMNAD_RUNNER_LOGGER_H
#define COMMNAD_RUNNER_LOGGER_H

// LOG levels
#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARNING 1
#define LOG_LEVEL_STATUS 2
#define LOG_LEVEL_DEUBUG 3



FILE *log_open(const char *);

void log_close(FILE *);

void log_write(int log_level, const char *filename, const int line_number, const FILE *log_fp, const char *, ...  );




#define LOG_ERROR_FORMAT(log_fp, msg...) log_write(LOG_LEVEL_ERROR, __FILE__, __LINE__, log_fp, ##msg)
#define LOG_WARNING_FORMAT(log_fp, msg...) log_write(LOG_LEVEL_WARNING, __FILE__, __LINE__, log_fp, ##msg)
#define LOG_STATUS_FORMAT(log_fp, msg...) log_write(LOG_LEVEL_STATUS, __FILE__, __LINE__, log_fp, ##msg)
#define LOG_DEBUG_FORMAT(log_fp, msg...) log_write(LOG_LEVEL_DEBUG, __FILE__, __LINE__, log_fp, ##msg)

#define LOG_ERROR(error_code) LOG_ERROR_FORMAT(log_fp, "Error: "#error_code)
#define LOG_WARNING(error_code) LOG_WARNING_FORMAT(log_fp, "Error: "#error_code)
#define LOG_STATUS(error_code) LOG_STATUS_FORMAT(log_fp, "Error: "#error_code)
#define LOG_DEBUG(error_code) LOG_DEBUG_FORMAT(log_fp, "Error: "#error_code)

#define ERROR_EXIT(error_code) \
   { \
      LOG_ERROR(error_code); \
      res->result = error_code; \
      log_close(log_fp); \
      return; \
   }
#endif // COMMNAD_RUNNER_LOGGER_H