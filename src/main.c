#include "argtable3.h" // manually included to the project
#include "monitor.h"

#define INTEGER_PLACE_HOLDER "<n>"
#define STRING_PLACE_HOLDER "<str>"

/* global arg_xxx structs */
struct arg_lit *verb, *help, *version, *author;
struct arg_int *max_cpu_time, *max_real_time, *max_memory, *max_stack, 
               *max_output_size, *max_process_number, *uid, *gid;
struct arg_str *exe_path, *input_path, *output_path, *error_path, 
               *args, *env, *log_path, *seccomp_rule;

struct arg_end *end; 

int main(int argc, char *argv[]){

   /* the global arg_xxx structs are initialised within the argtable */
   void *argtable[] = {
      
      help = arg_litn(NULL, "help", 0, 1, "Display this information"),
      version = arg_litn(NULL, "version", 0, 1, "Display command runner version information"),
      author = arg_litn(NULL, "author", 0, 1, "Displays author information"),

      max_cpu_time = arg_intn(NULL, "max_cpu_time", INTEGER_PLACE_HOLDER, 0, 1, "Maximum CPU time in (ms)"),
      max_real_time = arg_intn(NULL, "max_real_time", INTEGER_PLACE_HOLDER, 0, 1, "Maximum real time in (ms)"),
      max_memory = arg_intn(NULL, "max_memory", INTEGER_PLACE_HOLDER, 0, 1, "Maximum memory in (byte)"),
      max_stack = arg_intn(NULL, "max_stack", INTEGER_PLACE_HOLDER, 0, 1, "Maximum stack in (byte default 16M)"),
      max_process_number = arg_intn(NULL, "max_process_number", INTEGER_PLACE_HOLDER, 0, 1, "Maximum process number"),
      max_output_size = arg_intn(NULL, "max_output_size", INTEGER_PLACE_HOLDER, 0, 1, "Maximum Output Size (byte)"),


      exe_path = arg_str1(NULL, "exe_path", STRING_PLACE_HOLDER, "Excutable file Path"),
      input_path = arg_strn(NULL, "input_path", STRING_PLACE_HOLDER, 0, 1, "Input file Path"),
      output_path = arg_strn(NULL, "output_path", STRING_PLACE_HOLDER, 0, 1, "Output file Path"),
      error_path = arg_strn(NULL, "error_path", STRING_PLACE_HOLDER, 0, 1, "Error file path"),

      log_path = arg_strn(NULL, "log_path", STRING_PLACE_HOLDER, 0, 1, "Log Path"),
      seccomp_rule = arg_strn(NULL, "seccomp_rule", STRING_PLACE_HOLDER, 0, 1, "Seccomp Rule Name"),

      args = arg_strn(NULL, "args", STRING_PLACE_HOLDER, 0, 255, "Arg"),
      env = arg_strn(NULL, "env", STRING_PLACE_HOLDER, 0, 255, "Env"),

      uid = arg_intn(NULL, "uid", INTEGER_PLACE_HOLDER, 0, 1, "UID (default 65534)"),
      gid = arg_intn(NULL, "gid", INTEGER_PLACE_HOLDER, 0, 1, "GID (default 65534)"),

      end = arg_end(20),
   };

   int exitcode = 0;
   char progname[] = "libcommandrunner.so";

   int nerrors;
   nerrors = arg_parse(argc, argv, argtable);

   if(help->count > 0){
      printf("Usage: %s\n\n", progname);
      arg_print_syntax(stdout, argtable, "\n\n");
      printf("Options: \n");
      arg_print_glossary(stdout, argtable, "  %-25s %s\n");
      exitcode = 0;
      goto exit;
   }

   if (version->count > 0) {
      printf("Version: %d.%d.%d\n", (VERSION >> 16) & 0xff, (VERSION >> 8) & 0xff, VERSION & 0xff);
      goto exit;
   }

   if(author->count > 0){
      printf("Author: %s", AUTHOR);
      goto exit;
   }

   if(nerrors > 0){
      arg_print_errors(stdout, end, progname);
      printf("Try '%s --help' for more information. \n", progname);
      exitcode = 1;
      goto exit;
   }

   struct config conf;
   struct result res = {0, 0, 0, 0, 0, 0};


   if(max_cpu_time->count > 0){
      conf.max_cpu_time = *max_cpu_time->ival;
   } else {
      conf.max_cpu_time = UNLIMITED;
   }
   
   if(max_real_time->count > 0){
      conf.max_real_time = *max_real_time->ival;
   } else {
      conf.max_real_time = UNLIMITED;
   }

   if(max_memory->count > 0){
      conf.max_memory = *max_memory->ival;
   } else {
      conf.max_memory = UNLIMITED;
   }

   if(max_stack->count > 0){
      conf.max_stack = *max_stack->ival;
   } else {
      conf.max_stack = 16 * 1024 * 1024; // default stack 16MB
   }

   if(max_process_number->count > 0){
      conf.max_process_number = *max_process_number->ival;
   } else {
      conf.max_process_number = UNLIMITED;
   }

   if (max_output_size->count > 0) {
      conf.max_output_size = (long) *max_output_size->ival;
   } else {
      conf.max_output_size = UNLIMITED;
   }


   conf.exe_path = (char *)*exe_path->sval;

   if(input_path->count > 0){
      conf.input_path = (char *)input_path->sval[0];
   } else {
      conf.input_path = "/dev/stdin";
   }

   if(output_path->count > 0){
      conf.output_path = (char *)output_path->sval[0];
   } else {
      conf.output_path = "/dev/stdout";
   }

   if(error_path->count > 0){
      conf.error_path = (char *)error_path->sval[0];
   } else {
      conf.error_path = "/dev/stderr";
   }

   conf.args[0] = conf.exe_path; // same as exe_path
   int ind = 1;
   if(args->count > 0){
      for(; ind < args->count + 1; ind++){
         conf.args[ind] = (char *)args->sval[ind - 1];
      }
   }
   conf.args[ind] = NULL; // end must always be NULL

   ind = 0;

   if(env->count > 0){
      for(; ind < env->count; ind++){
         conf.env[ind] = (char *)env->sval[ind];
      }
   }
   conf.env[ind] = NULL; // end must always be NULL

   if(log_path->count > 0) {
      conf.log_path = (char *)log_path->sval[0];
   } else {
      conf.log_path = "commandrunner.log";
   }

   if (seccomp_rule->count > 0) {
      conf.seccomp_rule = (char *)seccomp_rule->sval[0];
   } else {
      // load the default NULL means compiling
      conf.seccomp_rule = NULL; 
   }

   if (uid->count > 0) {
      conf.uid = (uid_t)*(uid->ival);
   }
   else {
      conf.uid = 65534;
   }
   if(gid->count > 0) {
      conf.gid = (gid_t)*(gid->ival);
   }
   else {
      conf.gid = 65534;
   }

   // printf("boom");
   monitor(&conf, &res);

   // returned result
   printf("{\n"
      "    \"cpu_time\": %d,\n"
      "    \"real_time\": %d,\n"
      "    \"memory\": %ld,\n"
      "    \"signal\": %d,\n"
      "    \"exit_code\": %d,\n"
      "    \"result\": %d\n"
      "}",
      res.cpu_time,
      res.real_time,
      res.memory,
      res.signal,
      res.exitcode,
      res.result);



   exit:
      /* deallocate each non-null entry in argtable[] */
      arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
      return exitcode;
}