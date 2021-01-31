#ifndef COMMAND_RUNNER_SECCOMP_RULES
#define COMMAND_RUNNER_SECCOMP_RULES

#include "../monitor.h"

int general_rule(struct config *);
int cpp_rule(struct config *);


#endif // COMMAND_RUNNER_SECCOMP_RULES