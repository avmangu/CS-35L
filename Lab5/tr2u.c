/*
NAME: Animesh Mangu
UID: 804-771-005
TA: Guangyu Zhou
*/

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void errorMessage(int type) {
  switch(type) {
    case 1:
      fprintf(stderr, "ERROR: Please enter only 2 arguments.\n");
      break;
    case 2: 
      fprintf(stderr, "ERROR: From and To arguments must be same size.\n");
      break;
    case 3:
      fprintf(stderr, "ERROR: From must not contain any duplicates.\n");
      break;
    default:
      break;
  }
  exit(1);
}

int main(int argc, char* argv[]) {
  const int arg_num = 3;
  
  if(argc != arg_num)
    errorMessage(1);
  
  char* from = argv[1];
  char* to = argv[2];

  int from_length = strlen(from);
  int to_length = strlen(to);
  
  if(from_length != to_length)
    errorMessage(2);

  for(int i = 0; i < from_length; i++) {
    for(int j = i+1; j < from_length; j++) {
      if(from[i] == from[j])
	errorMessage(3);
    }
  }

  char c[1];
  int val = read(0, c, 1);
  int attach = 0;
  while(val > 0) {
    for(int i = 0; i < from_length; i++) {
      if(from[i] == c[0]) {
	c[0] = to[i];
	write(1, c, 1);
	attach = 1;
	break;
      } else {
	attach = 0;
      }
    }

    if(!attach)
      write(1, c, 1);

    val = read(0, c, 1);
  }
  
  return 0;
}
