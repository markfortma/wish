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

static
char *PATH[] = {
  "/bin"
};

void wish_execute(FILE *script){
  char *line;
  size_t linelen;
  ssize_t nbytes;
  while(!feof(script)){
    nbytes = getline(&line, &linelen, script);
    if(line[0] == '#'){
      /* skip lines that start with # */
      continue;
    }
    
  } 
}

int main(int argc, char *argv[]){
  if(2 == argc){
      FILE *in = fopen(argv[1], "r");
      wish_execute(in);
      fclose(in);
  } else if(1 == argc){
    wish_execute(stdin);
  }
  else{
      fprintf(stderr, "Usage: %s script.sh\n", argv[0]);
  }
    return EXIT_SUCCESS;
}
