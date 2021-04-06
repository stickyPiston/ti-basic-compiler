#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "functions.h"
#include <tic/tic.h>

static unsigned char *program = NULL;
static uint16_t programLength = 0;
static long long dataSum = 0;

extern char *inputFile;

void runTic() {
  FILE *fp = fopen(inputFile, "r");
  if (fp == NULL) {
    fprintf(stderr, "Could not open %s\n", inputFile);
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
  TokenType previousTokenType = TOKEN_NONE;
  while (index < filesize - 1) {
    int isParsed = 0;
    for (int i = 0; functionTable[i].name != NULL; i++) {
      if (strncmp(functionTable[i].name, content + index, strlen(functionTable[i].name)) == 0) {
        uint16_t opcode = functionTable[i].opcode;
        if (strcmp("-", functionTable[i].name) == 0 && (previousTokenType == TOKEN_OPERATOR || previousTokenType == TOKEN_OTHER)) {
          opcode = 0xb0;
        }

        if (programLength == 0) {
          if (functionTable[i].opcodeType == OPCODE_1BYTE) {
            program = malloc(1);
            program[programLength++] = (unsigned char)(opcode & 0xff);
          } else {
            program = malloc(2);
            program[programLength++] = (unsigned char)(opcode >> 8 & 0xff);
            program[programLength++] = (unsigned char)(opcode & 0xff);
          }
        } else {
          if (functionTable[i].opcodeType == OPCODE_1BYTE) {
            program = realloc(program, ++programLength);
            program[programLength - 1] = (unsigned char)(opcode & 0xff);
          } else {
            programLength += 2;
            program = realloc(program, programLength);
            program[programLength - 2] = (unsigned char)(opcode >> 8 & 0xff);
            program[programLength - 1] = (unsigned char)(opcode & 0xff);
          }
        }
        dataSum += opcode;
        index += strlen(functionTable[i].name);
        previousTokenType = functionTable[i].tokenType;
        isParsed = 1;
        break;
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
}
