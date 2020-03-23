#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct {
  unsigned long size, resident, share, text, lib, data, dt;
} statm_t;

/**
 * This program is not designed to me senseable.
 * It tests how much memory it can allocate before being killed.
 * It does therefore not free any memory.
 * Please be careful.
 * */
int main(int argc, char **argv) {
  int block_size_mb = 1024;
  if (argc > 1) {
    block_size_mb = atoi(argv[1]);
  }
  int block_size_byte = 1048576 * block_size_mb;

  pid_t pid = getpid();
  printf("The current pid is %d\n", pid);
  printf("The used block_size in Mib is %d\n", block_size_mb);

  statm_t result;
  FILE *f = fopen("/proc/self/statm", "r");
  if (!f) {
    perror("could not open statm");
    abort();
  }

  char *current_pointer = 0;
  unsigned long current_size = sizeof(char) * block_size_byte;
  // for (int i = 0; i < 10; i++) {
  while (1) {
    current_pointer = realloc(current_pointer, current_size);
    if (current_pointer == 0) {
      perror("malloc failed");
      abort();
    }
    sleep(1);

    if (7 != fscanf(f, "%ld %ld %ld %ld %ld %ld %ld", &result.size,
                    &result.resident, &result.share, &result.text, &result.lib,
                    &result.data, &result.dt)) {
      perror("could not read line or line was malformed");
      abort();
    }
    printf("Current memory in GB %f\n", (double)result.size / 1024.0 / 1024.0);
    printf("%ld %ld %ld %ld %ld %ld \n", result.resident, result.share,
           result.text, result.lib, result.data, result.dt);
    fseek(f, 0, 0);
    current_size += sizeof(char) * block_size_byte;
  }
}
