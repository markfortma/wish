/*
 * author    Matthew Markfort <matthew.markfort@my.metrostate.edu>
 * class     ICS-462 Operating Systems (Summer 2024), Metropolitan State University
 * date      28 May 2024
 * due       22 Jul 2024
 *
 * "wisconsin shell" (wish) is a rudimentary Linux shell for learning purposes
 *
 */

// built-in commands: exit, cd, and path
// - path overwrites PATH
// default PATH=/bin
// redirection to file also includes stderr
// allow background processes (&)

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define MAX_PATH_SIZE 16
static
char *path[MAX_PATH_SIZE] = {
  "/bin"
};

static
int path_count = 1;

void lsh_execute(FILE *in){
  char *line = NULL;
  size_t linelen = 0;
  ssize_t nbytes = 0;
  while(!feof(in)){
    if(in == stdin)
      printf("wish> ");
    nbytes = getline(&line, &linelen, in);

    // exit while if no bytes read
    if(0 == nbytes) break;
    // skip on comment lines
    if('#' == line[0]) continue;

    
  }
  /* release the line buffer */
  free(line);
}

int main(int argc, char *argv[]){
  if(2 == argc){
    FILE *in = fopen(argv[1], "r");
    lsh_execute(in);
    fclose(in);
  } else if(1 == argc){
    lsh_execute(stdin);
  } else {
    fprintf(stderr, "Usage: %s [script.sh]\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
