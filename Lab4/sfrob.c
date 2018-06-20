/*
NAME: Animesh Mangu
UID: 804-771-005
TA: Guangyu Zhou
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Print Errors
void errorMessage(int type);
// fromcmp Container Function
int frobber(const void *a, const void *b);
// frobcmp Function
int frobcmp(char const *a, char const *b); 

void errorMessage(int type) {
  switch(type) {
    case 1:
      fprintf(stderr, "ERROR: Can't Allocate Memory.\n");
      break;
    case 2: 
      fprintf(stderr, "ERROR: Can't Read Input.\n");
      break;
    case 3:
      fprintf(stderr, "ERROR: Can't Read Output.\n");
      break;
    default:
      break;
  }
  exit(1);
}

int frobcmp(char const *a, char const *b) {
  int a_value = 0;
  int b_value = 0;
  
  while (!!(strncmp(&a[a_value], " ", 1)) &&
	 !!(strncmp(&b[b_value], " ", 1))) {
    if ((a[a_value] ^ 42) >
	(b[b_value] ^ 42)) {
      return 1;
    } else if ((b[b_value] ^ 42) >
	       (a[a_value] ^ 42)) {
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
  
  return 0;
}

int frobber(const void *a, const void *b) {
  return frobcmp(*(char**)a, *(char**)b);
}

// Main Execution
int main(void) {
  int num_words = 0;
  int num_chars = 0;
  
  char** words = (char**)malloc(sizeof(char*));
  char* input_word = (char*)malloc(sizeof(char));
  
  char input = getchar();
  if (ferror(stdin) != 0) {
    errorMessage(2);
  }
  
  char future_input = getchar();
  if (ferror(stdin) != 0) {
    errorMessage(2);
  }
     
  while (input != EOF) {
    input_word[num_chars] = input;
    char* check = (char*)realloc(input_word,
		 (2 + num_chars) * sizeof(char));
    
    if (check == NULL) {
      free(input_word);
      errorMessage(1);
    }
    
    num_chars++;
    input_word = check;

    if (input == ' ') {
      words[num_words] = input_word;
            
      char** checker = (char**)realloc(words,
		       (2 + num_words) * sizeof(char*));
      if (checker == NULL) {
	free(words);
        errorMessage(1);
      }
      num_words++;
      num_chars = 0;
      
      words = checker;
      input_word = NULL;
      input_word = (char*)malloc(sizeof(char));
    } if (input == ' ' &&
	future_input == EOF) {
      break;
    } else if (input == ' ' &&
	       future_input == ' ') {
      while (input == ' ') {
	input = getchar();
	if (ferror(stdin) != 0) {
	  errorMessage(2);
	}
      }
      future_input = getchar();
      if (ferror(stdin) != 0) {
	errorMessage(2);
      }
      continue;
    } else if (future_input == EOF) {
      input = ' ';
      continue;
    }

    input = future_input;
    future_input = getchar();
    if (ferror(stdin) != 0) {
      errorMessage(2);
    }
  }
 
  qsort(words, num_words, sizeof(char*), frobber);
    
  for (int i = 0; i < num_words; i++) {
    for (int j = 0;; j++) {
      putchar(words[i][j]);
      if (ferror(stdin) != 0) {
	errorMessage(3);
      } else if (words[i][j] == ' ') {
	break;
      }
    }
  }
    
  for (int i = 0; i < num_words; i++) {
    free(words[i]);
  }
  free(words);
    
  return 0;
}

