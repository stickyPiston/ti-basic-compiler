#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  fseek(fp, 0, SEEK_END);
  int filesize = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  char *content = malloc(filesize);
  fread(content, filesize, 1, fp);

  fclose(fp);
  fp = NULL;

  int index = 0;
  while (index < filesize - 1) {
    char firstChar = content[index];
    if (firstChar == '-' && content[index + 1] == '>') { // Arrow
      writeOut(0x04); index++;
    } else if (isalpha(firstChar)) { // Identifier
      if (strncmp(content + index, "log(", 4) == 0) { writeOut(0x0c); continue; }
      if (strncmp(content + index, "round(", 6) == 0) { writeOut(0x21); continue; }
      if (strncmp(content + index, "max(", 4) == 0) { writeOut(0x91); continue; }
      if (strncmp(content + index, "min(", 4) == 0) { writeOut(0xa1); continue; }
      if (strncmp(content + index, "randM(", 5) == 0) { writeOut(0x02); continue; }
      if (strncmp(content + index, "mean(", 5) == 0) { writeOut(0x12); continue; }
      if (strncmp(content + index, "solve(", 6) == 0) { writeOut(0x22); continue; }
      if (strncmp(content + index, "seq(", 4) == 0) { writeOut(0x32); continue; }
      if (strncmp(content + index, "fnInt(", 6) == 0) { writeOut(0x42); continue; }
      if (strncmp(content + index, "nDeriv(", 7) == 0) { writeOut(0x52); continue; }
      if (strncmp(content + index, "fMin(", 5) == 0) { writeOut(0x72); continue; }
      if (strncmp(content + index, "fMax(", 5) == 0) { writeOut(0x82); continue; }
      if (strncmp(content + index, "i", 1) == 0) { writeOut(0xc2); continue; }
      if (strncmp(content + index, "or", 2) == 0) { writeOut(0xc3); continue; }
      if (strncmp(content + index, "xor", 3) == 0) { writeOut(0xd3); continue; }
      if (strncmp(content + index, "and", 3) == 0) { writeOut(0x04); continue; }

      writeOut(firstChar); continue;
    } else if (isdigit(firstChar)) { // Number
      writeOut(firstChar);
    } else if (firstChar == ':' || firstChar == ';') { // delimiter
      writeOut(0x3e);
    } else if (firstChar == ' ') { // space
      writeOut(0x29);
    } else if (firstChar == '\n') {
      writeOut(0x3f);
    } else if (firstChar == '-') {
      writeOut(0x17);
    } else if (firstChar == '+') {
      writeOut(0x07);
    } else if (firstChar == '*') {
      writeOut(0x28);
    } else if (firstChar == '/') {
      writeOut(0x17);
    } else if (firstChar == '<') {
      writeOut(0xb6);
    } else if (firstChar == '>') {
      writeOut(0xc6);
    } else if (firstChar == '=') {
      writeOut(0xa6);
    } else if (firstChar == '!') {
      writeOut(0xb8);
    } else if (firstChar == '^') {
      writeOut(0x0f);
    } else if (firstChar == '[') {
      writeOut(0x06);
    } else if (firstChar == ']') {
      writeOut(0x07);
    } else if (firstChar == '(') {
      writeOut(0x10);
    } else if (firstChar == ')') {
      writeOut(0x11);
    } else if (firstChar == '{') {
      writeOut(0x08);
    } else if (firstChar == '}') {
      writeOut(0x09);
    } else {
      fprintf(stderr, "Unrecognised token: %c\n", firstChar);
      abort();
    }

  }

  FILE *outFile = fopen("out.8xp", "w");
  if (outFile == NULL) {
    fprintf(stderr, "Couldn't open out.8xp");
    abort();
  }

  unsigned char header[] = { 0x2a, 0x2a, 0x54, 0x49, 0x38, 0x33, 0x46, 0x2a, 0x1a, 0x0a, 0x0a };
  fwrite(header, sizeof(header), 1, outFile);

  char *comment = calloc(42, 1);
  strcpy(comment, "Made using stickyPiston/ti-basic-compiler");
  fwrite(comment, 42, 1, outFile);

  programLength += 2;
  unsigned char varEntry[] = { 0x0d, 0x00, programLength, programLength >> 8,  0x05, 0x50, 0x52, 0x4f, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, programLength, programLength >> 8, 0x04, 0x00 };

  for (int i = 0; i < sizeof(varEntry); i++) dataSum += varEntry[i];

  uint16_t dataLength = sizeof(varEntry) + programLength;

  fwrite(&dataLength, 2, 1, outFile);
  fwrite(varEntry, sizeof(varEntry), 1, outFile);
  fwrite(program, programLength - 2, 1, outFile);
  
  uint16_t checksum = dataSum & 0xffff;
  fwrite(&checksum, 2, 1, outFile);

  fclose(outFile);

  return 0;
}
