from subprocess import Popen, PIPE
import json

VERSION="0.0.1"

UNLIMITED = -1

CommandRunner = '/usr/lib/commandrunner/libcommandrunner.so'


# commandrunner return results and errors
SUCCESS = 0
ROOT_REQUIRED = -1
INVALID_CONFIG = -2
FORK_FAILED = -3
PTHREAD_FAILED = -4
WAIT_FAILED = -5
SETRLIMIT_FIALED = -6
LOAD_SECCOMP_FAILED = -7
EXECVE_FAILED = -8

# program result 
WRONG_ANSWER = -1
REAL_TIME_LIMIT_EXCEEDED = 2
CPU_TIME_LIMIT_EXCEEDED = 3
MEMORY_LIMIT_EXCEEDED = 4
RUNTIME_ERROR = 5
SYSTEM_ERROR = 6

def run(
   max_real_time=None,
   max_cpu_time=None,
   max_memory=None,
   max_stack=None,
   max_output_size=None,
   max_process_number=None,
   exe_path=None,
   input_path=None,
   output_path=None,
   log_path=None,
   error_path=None,
   args=[],
   env=[],
   uid=None,
   gid=None,
   seccomp_rule=None,
):
	command = [CommandRunner]

	if not isinstance(args, list):
	  raise Exception("args must be a list")
	else:
	  for item in args:
	     if not isinstance(item, str):
	        raise Exception(item + " must be a string")
	     else:
	        command.append("--args="+item)

	if not isinstance(env, list):
	  raise Exception("args must be a list")
	else:
	  for item in env:
	     if not isinstance(item, str):
	        raise Exception(item + " must be a string")
	     else:
	        command.append("--env="+item)

	# setting integers
	if max_real_time is not None:
	  if not isinstance(max_real_time, int) and not None:
	     raise Exception("max_real_time must be a int")
	  else:
	     command.append("--max_real_time={}".format( max_real_time))

	if max_cpu_time is not None:
	  if not isinstance(max_cpu_time, int) and not None:
	     raise Exception("max_cpu_time must be an int")
	  else:
	     command.append("--max_cpu_time={}".format( max_cpu_time))

	if max_memory is not None:
	  if not isinstance(max_memory, int) and not None:
	     raise Exception("max_memory must be an int")
	  else:
	     command.append("--max_memory={}".format( max_memory))

	if max_stack is not None:
	  if not isinstance(max_stack, int) and not None:
	     raise Exception("max_stack must be an int")
	  else:
	     command.append("--max_stack={}".format( max_stack))

	if max_output_size is not None:
	  if not isinstance(max_output_size, int) and not None:
	     raise Exception("max_output_size must be a int")
	  else:
	     command.append("--max_output_size={}".format( max_output_size))

	if max_process_number is not None:
	  if not isinstance(max_process_number, int) and not None:
	     raise Exception("max_process_number must be a int")
	  else:
	     command.append("--max_process_number={}".format( max_process_number))

	if uid is not None:
	  if not isinstance(uid, int) and not None:
	     raise Exception("uid must be a int")
	  else:
	     command.append("--uid={}".format( uid))

	if gid is not None:
	  if not isinstance(gid, int) and not None:
	     raise Exception("gid must be a int")
	  else:
	     command.append("--gid={}".format( gid))

	# setting strings
	if exe_path is not None:
	  if not isinstance(exe_path, str):
	     raise Exception("exe_path must be a string")
	  else:
	     command.append("--exe_path=" + exe_path)
	else:
	  raise Exception("exe_path can not be empty")

	if input_path is not None:
	  if not isinstance(input_path, str):
	     raise Exception("input_path must be a string")
	  else:
	     command.append("--input_path=" + input_path)

	if output_path is not None:
	  if not isinstance(output_path, str):
	     raise Exception("output_path must be a string")
	  else:
	     command.append("--output_path=" + output_path)

	if log_path is not None:
	  if not isinstance(log_path, str):
	     raise Exception("log_path must be a string")
	  else:
	     command.append("--log_path=" + log_path)

	if error_path is not None:
	  if not isinstance(error_path, str):
	     raise Exception("error_path must be a string")
	  else:
	     command.append("--error_path=" + error_path)
	if seccomp_rule is not None:
	  if not isinstance(seccomp_rule, str):
	     raise Exception("seccomp_rule must be a string")
	  else:
	     command.append("--seccomp_rule=" + seccomp_rule)

	process = Popen(command, stdout=PIPE, stderr=PIPE)
	stdout, stderr = process.communicate()
	if stderr:
	  raise Exception("Error encountered while executing commandrunner: {}".format(stderr.decode("utf-8")))
	else:
	  result = json.loads(stdout.decode("utf-8"))
	  return result
