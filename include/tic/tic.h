#ifndef TIC_TIC_H
#define TIC_TIC_H

#include <inttypes.h>

typedef enum {
  OPCODE_1BYTE,
  OPCODE_2BYTE
} OpcodeType;

typedef enum {
  TOKEN_OPERATOR,
  TOKEN_FUNCTION,
  TOKEN_VAR,
  TOKEN_OTHER, // ?
  TOKEN_NONE
} TokenType;

typedef struct {
  uint16_t opcode;
  OpcodeType opcodeType;
  TokenType  tokenType;
  /* char *name; */
} TIFunction;

typedef enum {
  LANG_EN = 0,
  LANG_NL = 1,
  LANG_SE = 2
} Language;

void compile();
void disassemble();

#endif
