#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <tic/tic.h>
#include <tic/cli.h>

Language lang = LANG_EN;
char *inputFile        = NULL;
char programName[9]    = {0};
char *outputFile       = NULL;
bool ignoreWhitespace  = false;
bool disassembleMode   = false;

int main(int argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-o") == 0) {
      outputFile = argv[++i];
    } else if (strcmp(argv[i], "-i") == 0) {
      ignoreWhitespace = true;
    } else if (strcmp(argv[i], "-d") == 0) {
      disassembleMode = true;
    } else if (strcmp(argv[i], "-n") == 0) {
      strncpy(programName, argv[++i], 8);
    } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      openHelp(argv[0]);
      return 0;
    } else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--lang") == 0) {
      i++;
      if (strcmp(argv[i], "SE") == 0 || strcmp(argv[i], "se") == 0) lang = LANG_SE;
      if (strcmp(argv[i], "EN") == 0 || strcmp(argv[i], "en") == 0) lang = LANG_EN;
      if (strcmp(argv[i], "NL") == 0 || strcmp(argv[i], "nl") == 0) lang = LANG_NL;
    } else {
      if (argv[i][0] == '-') {
        printf("Unknown flag: %s\n", argv[i]);
        return 1;
      } else {
        inputFile = argv[i];
      }
    }
  }

  if (outputFile == NULL) outputFile = "out.8xp";
  if (inputFile == NULL) { openHelp(argv[0]); return 0;}
  if (*programName == 0) strcpy(programName, "PROG");

  if (disassembleMode) {
    disassemble();
  } else {
    compile();
  }
}

void openHelp(char *command) {
  puts("tic: TI-BASIC compiler");
  puts("Command line interface usage:");
  printf("%s <input file> <flags>\n", command);
  puts("\nAvailable flags:");
  puts(" -o: Specify the output file path");
  puts(" -i: Ignore whitespace characters, tabs and whitespaces will not be added to the final binary");
  puts(" -d: Enable disassembly mode");
  puts(" -n: Specify the name of the program when sent to the calculator");
  puts(" -h, --help: Show the help page");
  puts(" -l, --lang: Select a language, choices are: nl, en, se");
}
