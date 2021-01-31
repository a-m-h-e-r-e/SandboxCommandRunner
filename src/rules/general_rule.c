#include <stdio.h>
#include <fcntl.h>
#include <seccomp.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>


// #include "seccomp_rules.h"
#include "../monitor.h"

int general_rule(struct config *conf) {
  
   scmp_filter_ctx ctx = NULL;
   ctx = seccomp_init(SCMP_ACT_KILL);

   if(!ctx) {
      printf("seccomp_init failed!\n");
      return LOAD_SECCOMP_FAILED;
   }
   
   // This white list is needed for dynamic memory allocation,
   // for stdio initialisation, and by execve
    int syscalls_whitelist[] = {SCMP_SYS(read), SCMP_SYS(fstat),
                              SCMP_SYS(mmap), SCMP_SYS(mprotect),
                              SCMP_SYS(munmap), SCMP_SYS(uname),
                              SCMP_SYS(arch_prctl), SCMP_SYS(brk),
                              SCMP_SYS(access), SCMP_SYS(exit_group),
                              SCMP_SYS(close), SCMP_SYS(readlink),
                              SCMP_SYS(sysinfo), SCMP_SYS(write),
                              SCMP_SYS(writev), SCMP_SYS(lseek),
                              SCMP_SYS(clock_gettime)};
   
   int syscalls_whitelist_length = sizeof(syscalls_whitelist) / sizeof(int);
   
   for (int i = 0; i < syscalls_whitelist_length; i++) {
      if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, syscalls_whitelist[i], 0) != 0) {
         return LOAD_SECCOMP_FAILED;
      }
   }

   // add extra rule for execve
   if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(execve), 1, SCMP_A0(SCMP_CMP_EQ, (scmp_datum_t)(conf->exe_path))) != 0) {
      return LOAD_SECCOMP_FAILED;
   }

   if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(open), 1, SCMP_CMP(1, SCMP_CMP_MASKED_EQ, O_WRONLY | O_RDWR, 0)) != 0) {
      return LOAD_SECCOMP_FAILED;
   }
   if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(openat), 1, SCMP_CMP(2, SCMP_CMP_MASKED_EQ, O_WRONLY | O_RDWR, 0)) != 0) {
      return LOAD_SECCOMP_FAILED;
   }
   

  
   if(seccomp_load(ctx) != 0) {
      printf("seccomp_load failed\n");
      return LOAD_SECCOMP_FAILED;
   }

   // clean up
   seccomp_release(ctx);
   return 0;
}



