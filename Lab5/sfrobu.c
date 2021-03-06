/*
  NAME: Animesh Mangu
  UID: 804-771-005
  TA: Guangyu Zhou
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

struct stat buffer;
bool caseSense = false;

// Print Errors
void errorMessage(int type);
// fromcmp Container Function
int frobber(const void *a, const void *b);
// Decrypt When Flag Set
unsigned char decrypt(char a);
// frobcmp Function
int frobcmp(char const *a, char const *b); 

void errorMessage(int type) {
  switch(type) {
  case 1:
    fprintf(stderr, "ERROR: Can't Allocate Memory.\n");
    break;
  case 2: 
    fprintf(stderr, "ERROR: Can't Read I/O.\n");
    break;
  case 3:
    fprintf(stderr, "ERROR: Incorrect Line Number.");
    break;
  case 4:
    fprintf(stderr, "ERROR: Incorrect Command Arguments.");
    break;
  default:
    break;
  }
  exit(1);
}

unsigned char decrypt(char a) {
  a = a ^ 42;
  if (caseSense) {
    a = toupper((unsigned char)a);
  }
  
  return a;
}

int frobcmp (char const *a, char const *b) {
  int a_value = 0;
  int b_value = 0;
  
  while (!!(strncmp(&a[a_value], " ", 1)) &&
	 !!(strncmp(&b[b_value], " ", 1))) {
    if (decrypt(a[a_value]) >
	decrypt(b[b_value])) {
      return 1;
    } else if (decrypt(a[a_value]) <
	       decrypt(b[b_value])) {
      return -1;
    } else {
      a_value++;
      b_value++;
    }
  }
  
  if (!!(strncmp(&a[a_value], " ", 1)))
    return 1;
  else if (!!(strncmp(&b[b_value], " ", 1)))
    return -1;
  else
    return 0;
}

int frobber(const void *a, const void *b) {
  return frobcmp(*(char**)a, *(char**)b);
}

int main(int argc, char *argv[]) {
  if(argc > 2) {
    errorMessage(4);
  } else if (argc == 2) {
    const char *optionCheck = argv[1];
    if (strlen(optionCheck) != 2)
      errorMessage(4);
    else if (optionCheck[0] == '-' &&
	     optionCheck[1] == 'f')
      caseSense = true;
    else
      errorMessage(4);
  }
  
  char currentChar;
  char* secondInput, ** lineBuffer;
  char readBuffer[2];
    
  int bufferSize = 0;
  int lineNum = 0;
  int lineSize = 0;
  int isEOF = 0;
    
  int (* cmp) (const void*, const void*) = &frobber;
  int returnValue = fstat(STDIN_FILENO, &buffer);

  if(returnValue < 0)
    errorMessage(2);
  
  int fileSize = buffer.st_size + 1;
  char* input = (char*) malloc(sizeof(char) * fileSize);

  if(input == NULL)
    errorMessage(1);
    
  char* line = input;
                                                         
  while (!isEOF) {
    returnValue = read(STDIN_FILENO, readBuffer, 1);

    if(returnValue < 0)
      errorMessage(2);
    
    currentChar = readBuffer[0];
    int isSpace = currentChar == ' ';
    isEOF = !returnValue;

    if (isSpace && !lineSize)
      continue;
        
    if (bufferSize == fileSize) {
      fileSize *= 2;
      secondInput = (char*) realloc(input,
		     sizeof(char) * fileSize);
      
      if(secondInput == NULL)
	errorMessage(1);
      
      input = secondInput;
    }
        
    if (!isEOF) {
      input[bufferSize++] = currentChar;
      lineSize++;
      
      if (!isSpace)
	continue;
    } else {
      if (!bufferSize) {
	free(input);
	return 0;         
      }

      if (input[bufferSize-1] != ' ')
	input[bufferSize++] = ' ';
      if (!lineSize)
	break;
    }

    lineSize = 0;
    lineNum++;
  }
  
  lineBuffer = (char**) malloc(sizeof(char*) * lineNum);
  if(lineBuffer == NULL)
    errorMessage(1);

  char* liner = input;
  int i = 0;
  int num = 0;
  for (i, num; i < bufferSize; i++) {
    if (input[i] == ' ') {
      lineBuffer[num++] = liner;
      liner = input + i + 1;
    }
  }
    
  if (lineNum != num)
    errorMessage(3);
    
  qsort(lineBuffer, lineNum, sizeof(char*), cmp);
    
  for (int i = 0; i < lineNum; i++) {
    int ret = 0;
    for (;;) {
      ret = write(STDOUT_FILENO, lineBuffer[i], 1);

      if(ret < 0)
	errorMessage(2);

      if (*lineBuffer[i]++ == ' ')
	break;
    }
  }
    
  free(lineBuffer);
  free(input);
  
  return 0;
}
