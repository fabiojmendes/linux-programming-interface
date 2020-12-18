#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char** environ;

char* my_getenv(char* name) {
  size_t len = strlen(name);
  for (int i = 0; environ[i]; i++) {
    char* var = environ[i];
    char* ptr = strchr(var, '=');
    if (ptr) {
      size_t index = ptr - var;
      if (len == index && strncmp(var, name, len) == 0) {
        return &var[len + 1];
      }
    }
  }
  return NULL;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    return EXIT_FAILURE;
  }
  char* var = my_getenv(argv[1]);
  if (var) {
    printf("%s\n", var);
  } else {
    puts("variable not found");
  }
  return EXIT_SUCCESS;
}
