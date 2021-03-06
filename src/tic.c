#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

#include "tables/functions.h"
#include "tables/strings.h"
#include <tic/tic.h>

static unsigned char *program = NULL;
static uint16_t programLength = 0;
static long long dataSum = 0;

// CLI arguments
extern char *inputFile;
extern char *outputFile;
extern char programName[9];
extern bool ignoreWhitespace;
extern Language lang;

int readFile(char **out) {
  FILE *fp = fopen(inputFile, "r");
  if (fp == NULL) {
    fprintf(stderr, "Could not open %s\n", inputFile);
    abort();
  } 

  // Read all bytes from the source file
  fseek(fp, 0, SEEK_END);
  int filesize = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  *out = malloc(filesize);
  fread(*out, filesize, 1, fp);

  fclose(fp);
  
  return filesize;
}

void compile() {
  char *content = NULL;
  int filesize = readFile(&content);
  int index = 0; 
  TokenType previousTokenType = TOKEN_NONE;
  while (index < filesize - 1) {
    int isParsed = 0;
    for (int i = 0; stringTable[i][lang] != NULL; i++) {
      if (strncmp(stringTable[i][lang], content + index, strlen(stringTable[i][lang])) == 0) {
        uint16_t opcode = functionTable[i].opcode;
        if (strcmp("-", stringTable[i][lang]) == 0 && (previousTokenType == TOKEN_OPERATOR || previousTokenType == TOKEN_OTHER)) {
          opcode = 0xb0;
        }

        if (strcmp(" ", stringTable[i][lang]) == 0 && ignoreWhitespace) {
          isParsed = 1;
          index++;
          break;
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
        index += strlen(stringTable[i][lang]);
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

  FILE *outFile = fopen(outputFile, "w");
  if (outFile == NULL) {
    fprintf(stderr, "Couldn't open %s", outputFile);
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
  unsigned char varEntry[] = { 0x0d, 0x00, programLength, programLength >> 8,  0x05, programName[0], programName[1], programName[2], programName[3], programName[4], programName[5], programName[6], programName[7], 0x00, 0x00, programLength, programLength >> 8, 0x04, 0x00 };

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

void disassemble() {
  unsigned char *bytes = NULL;
  int filesize = readFile((char **)&bytes);

  for (int i = 74; i < filesize - 2; i++) {
    for (int j = 0; stringTable[j][lang] != NULL; j++) {
      if (bytes[i] == 0x5c || bytes[i] == 0x5d || bytes[i] == 0x5e || bytes[i] == 0x60 || bytes[i] == 0x61 || bytes[i] == 0xaa || bytes[i] == 0xef || bytes[i] == 0x62 || bytes[i] == 0x63 || bytes[i] == 0x7e || bytes[i] == 0xbb) {
        uint16_t twobytevalue = bytes[i] << 8;
        twobytevalue |= bytes[++i];
        if (functionTable[j].opcode == twobytevalue) {
          printf("%s", stringTable[j][lang]);
        }
      } else {
        if (functionTable[j].opcode == bytes[i]) {
          if (strcmp(stringTable[j][lang], "\n") != 0)
            printf("%s", stringTable[j][lang]);
        }
      }
    }
  }

  puts("");
}
