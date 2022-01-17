#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define READ  0
#define WRITE 1

int main() {

  pid_t pid;
  int   fd[2];

   char *const cmd1[]={"cat","/etc/hosts",NULL};
   char *const cmd2[]={"sort",NULL};
  if (pipe(fd) == -1) {
	perror("Creating pipe");
	exit(EXIT_FAILURE);
  }

  switch(pid = fork()) {

  case 0:

	close(fd[WRITE]);
	dup2(fd[READ], STDIN_FILENO);
	// Execute sort
	execvp(cmd2[0],cmd2);

  case -1:
	perror("fork() failed)");
	exit(EXIT_FAILURE);

  default:
	
	// Close the pipe read descriptor.
	close(fd[READ]);
	// Redirect STDOUT to write to the pipe.
	dup2(fd[WRITE], STDOUT_FILENO);
	// Execute cat /etc/hosts
	execvp(cmd1[0],cmd1);
  }
}
