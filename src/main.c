#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "functions.h"

#define writeOut(value) do { \
    if (programLength == 0) { \
      program = malloc(1); \
      program[programLength++] = value; \
    } else { \
      program = realloc(program, ++programLength); \
      program[programLength - 1] = value; \
    } \
    dataSum += value; \
    index++; \
  } while (0)

unsigned char *program = NULL;
uint16_t programLength = 0;
long long dataSum = 0;

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "The ti-basic compiler requires 1 argument");
    abort();
  }

  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "Could not open %s\n", argv[1]);
    abort();
  } 

  // Read all bytes from the source file
  fseek(fp, 0, SEEK_END);
  int filesize = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  char *content = malloc(filesize);
  fread(content, filesize, 1, fp);

  fclose(fp);
  fp = NULL;

  int index = 0;
  while (index < filesize - 1) {
    int isParsed = 0;
    for (int i = 0; functionTable[i].name != NULL; i++) {
      if (strncmp(functionTable[i].name, content + index, strlen(functionTable[i].name)) == 0) {
        isParsed = 1;
        writeOut(functionTable[i].opCode);
        index += strlen(functionTable[i].name) - 1;
      }
    }
    if (!isParsed) {
      fprintf(stderr, "Unrecognised token %c", *(content + index));
      abort();
    }
  }

  FILE *outFile = fopen("out.8xp", "w");
  if (outFile == NULL) {
    fprintf(stderr, "Couldn't open out.8xp");
    abort();
  }

  // Append header to file
  unsigned char header[] = { 0x2a, 0x2a, 0x54, 0x49, 0x38, 0x33, 0x46, 0x2a, 0x1a, 0x0a, 0x0a };
  fwrite(header, 11, 1, outFile);

  char *comment = calloc(42, 1);
  strcpy(comment, "Made using stickyPiston/ti-basic-compiler");
  fwrite(comment, 42, 1, outFile);

  // Append program and program information to file
  programLength += 2;
  unsigned char varEntry[] = { 0x0d, 0x00, programLength, programLength >> 8,  0x05, 0x50, 0x52, 0x4f, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, programLength, programLength >> 8, 0x04, 0x00 };

  for (int i = 0; i < sizeof(varEntry); i++) dataSum += varEntry[i];

  uint16_t dataLength = sizeof(varEntry) + programLength;

  fwrite(&dataLength, 2, 1, outFile);
  fwrite(varEntry, sizeof(varEntry), 1, outFile);
  fwrite(program, programLength - 2, 1, outFile);
  
  // Append checksum to file
  uint16_t checksum = dataSum & 0xffff;
  fwrite(&checksum, 2, 1, outFile);

  fclose(outFile);

  return 0;
}
