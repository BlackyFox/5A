#include <stdio.h>
void main(int argc, char **argv){
  int bytes;
  printf("%s%n\n", argv[1], &bytes);
  printf("You input %d characters\n", bytes);
}