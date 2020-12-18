#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void fatal(char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

// Returns true if s is a number else false
bool is_number(char *s) {
  for (int i = 0; i < s[i]; i++) {
    if (isdigit(s[i]) == false) {
      return false;
    }
  }

  return true;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "%s <dir>\n", argv[0]);
    exit(1);
  }

  DIR *proc = opendir(argv[1]);
  if (!proc) {
    fatal("open");
  }

  struct dirent *dir;
  puts("PID\tCMD");
  while ((dir = readdir(proc)) != NULL) {
    if (dir->d_type == DT_DIR) {
      if (is_number(dir->d_name)) {
        const size_t pbuf_size = 1024;
        char path[pbuf_size];
        snprintf(path, pbuf_size, "%s/%s/cmdline", argv[1], dir->d_name);
        char cmd[4096];
        FILE *cmdf = fopen(path, "r");
        if (!cmdf) {
          fprintf(stderr, "* file %s doesn't exist *\n", path);
          continue;
        }
        fgets(cmd, 4096, cmdf);
        fclose(cmdf);
        printf("%s\t%s\n", dir->d_name, cmd);
      }
    }
  }

  closedir(proc);
  return EXIT_SUCCESS;
}
