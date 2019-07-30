#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct part32 {
  char magic[4];
  char num_parts[4];
  unsigned int offset;
  unsigned int size;
  char name[32];
  char type[32];
  char pad1[9];
  char media;
  char pad[422]; //512-..
};

struct part64 {
  char magic[4];
  char num_parts[4];
  unsigned long offset;
  unsigned long size;
  char name[32];
  char type[32];
  char pad[420]; //512-..
};


int main(int argc, char ** argv) {
  int min_args = 5;
  if (argc < min_args) {
    printf("Usage: %s MP Ext2 'Root 1' size [offset]\n", argv[0]);
    return 1;
  }

  int narg = 1;
  int calc_total = 1;
  unsigned long long  total = 0;
  if (argv[narg][0] == '-') {
    narg = 2;
    calc_total = 0;
    min_args++;
  }

  char safe_name[32];
  FILE *fp;

  part32 nw;

  memset(&nw, 0, 512);

  strcpy(nw.magic, argv[narg++]);
  strcpy(nw.num_parts, "\016");

  strcpy(nw.type, argv[narg++]);
  strcpy(nw.name, argv[narg++]);
  nw.size = atol(argv[narg++]);

  if (argc > min_args) 
    nw.offset = atol(argv[narg++]);
  else
    nw.offset = 0;

  char mfs_names[] = "MFS media";
  if (0 == strncmp(nw.name, mfs_names, strlen(mfs_names)))
    nw.media = '\001';

  if (calc_total)
    total = (unsigned long long) nw.size + (unsigned long long) nw.offset;
  printf("%llu\n", total);

  strcpy(safe_name, nw.name);
  for (int s=0; s<strlen(safe_name); s++) {
    if (' ' == safe_name[s] || '/' == safe_name[s]) {
     safe_name[s] = '_';
    }
  }
  fp = fopen(safe_name, "w");
  fwrite(&nw, 1, sizeof(nw), fp);
  fclose(fp);
  
  return 0;
}
