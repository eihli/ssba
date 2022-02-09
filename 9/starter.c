#define _GNU_SOURCE
#include <sched.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define STACK_SIZE 65536

struct child_config {
  int argc;
  char **argv;
};

/* Entry point for child after `clone` */
int child(void *arg) {
  struct child_config *config = arg;
  if (execvpe(config->argv[0], config->argv, NULL)) {
    fprintf(stderr, "execvpe failed %m.\n");
    return -1;
  }
  return 0;
}


int main(int argc, char**argv) {
  struct child_config config = {0};
  int flags = 0;
  pid_t child_pid = 0;

  // Prepare child configuration
  config.argc = argc - 1;
  config.argv = &argv[1];

  // Allocate stack for child
  char *stack = 0;
  if (!(stack = malloc(STACK_SIZE))) {
    fprintf(stderr, "Malloc failed");
    exit(1);
  }


  FILE *f;
  if (!(f = fopen("/sys/fs/cgroup/pids/ilario/cgroup.procs", "w"))) {
    perror("Error opening cgroup.procs");
    return EXIT_FAILURE;
  }

  if ((fprintf(f, "%d", 0)) < 0) {
    perror("Error writing to cgroup.procs");
    return EXIT_FAILURE;
  }

  if (fclose(f) != 0) {
    perror("Error closing cgroup.procs");
    return EXIT_FAILURE;
  }


  // Clone parent, enter child code
  if ((child_pid = clone(child, stack + STACK_SIZE, flags | SIGCHLD | CLONE_NEWPID | CLONE_NEWUSER | CLONE_NEWNET, &config)) == -1) {
    fprintf(stderr, "Clone failed");
    exit(2);
  }

  printf("%d\n", child_pid);


  wait(NULL);
  
  return 0;
}
