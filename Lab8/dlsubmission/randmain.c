#include "randcpuid.h"
#include <stdlib.h> 
#include <stdio.h>
#include <errno.h> 
#include <dlfcn.h> 
#include <stdbool.h>

static bool
writebytes (unsigned long long x, int nbytes) {
  int ndigits = nbytes * 2;
  do {
    if (putchar ("0123456789abcdef"[x & 0xf]) < 0)
      return false;
    x >>= 4;
    ndigits--;
  } while (0 < ndigits);

  return 0 <= putchar ('\n');
}

int main(int argc, char **argv) {
  bool valid = false;
  long long nbytes;
  
  if (argc == 2) {
    char *endptr;
    errno = 0;
    nbytes = strtoll(argv[1], &endptr, 10);
    
    if (errno)
      perror (argv[1]);
    else
      valid = !*endptr && 0 <= nbytes;
  }
  
  if (!valid) {
    fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
    return 1;
  }

  if (nbytes == 0)
    return 0;

  void* library; 
  unsigned long long (*rand64)(void);

  if(rdrand_supported()) {
    library = dlopen("./randlibhw.so", RTLD_NOW);
      
    if(library == NULL) {
      printf("Could not open randlibhw.so. %s\n", dlerror());
      exit(1);
    }

    rand64 = dlsym(library, "hardware_rand64");
    if(dlerror()) {
      printf("Could not find hardware_rand64. %s\n", dlerror());
      exit(1);
    }
  } else {
    library = dlopen("./randlibsw.so", RTLD_NOW);
    
    if(library == NULL) {
      printf("Could not open randlibsw.so. %s\n", dlerror());
      exit(1);
    }
    
    rand64 = dlsym(library, "software_rand64");
    if(dlerror()) {
      printf("Could not find  software_rand64. %s\n", dlerror());
      exit(1);
    }
  }
  
  int wordsize = sizeof rand64();
  int output_errno = 0;
  
  do {
    unsigned long long x = rand64();
    int outbytes = nbytes < wordsize ? nbytes : wordsize;
    if (!writebytes (x, outbytes)) {
      output_errno = errno;
      break;
    }
      
    nbytes -= outbytes;
  } while (0 < nbytes);

  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno) {
    errno = output_errno;
    perror("output");
    return 1;
  }
  
  dlclose(library);
  return 0;
}
