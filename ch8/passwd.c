#include <errno.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void fatal(char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

struct passwd *my_getpwd(char *name) {
  struct passwd *pwd;
  while ((pwd = getpwent()) != NULL) {
    if (strcmp(pwd->pw_name, name) == 0) {
      break;
    }
  }
  endpwent();
  return pwd;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "%s <username>\n", argv[0]);
    exit(1);
  }

  struct passwd *pwd = my_getpwd(argv[1]);
  if (pwd != NULL) {
    printf("Hello! %s (%d)\n"
           "  shell: %s\n"
           "  dir: %s\n",
           pwd->pw_name, pwd->pw_uid, pwd->pw_shell, pwd->pw_dir);

  } else {
    printf("Password entry not found: %s\n", argv[1]);
  }
  return EXIT_SUCCESS;
}
