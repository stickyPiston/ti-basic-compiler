#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef struct {
  unsigned char opCode;
  char *name;
} TIFunction;

TIFunction functionTable[] = {
  {0xc0, "log("},
  {0x12, "round("},
  {0x19, "max("},
  {0x1a, "min("},
  {0x20, "randM("},
  {0x21, "mean("},
  {0x22, "solve("},
  {0x23, "seq("},
  {0x24, "fnInt("},
  {0x25, "nDeriv("},
  {0x27, "fMin("},
  {0x28, "fMax("},
  {0x3c, "or"},
  {0x3d, "xor"},
  {0x40, "and"},
  {0x13, "pxlTest("},
  {0x14, "augment("},
  {0x15, "rowSwap("},
  {0x16, "row+("},
  {0x5f, "prgm"},
  {0x64, "Radian"},
  {0x65, "Degree"},
  {0x66, "Normal"},
  {0x67, "Sci"},
  {0x68, "Eng"},
  {0x69, "Float"},
  {0x17, "*row("},
  {0x18, "*row+("},
  {0x2e, "CubicReg"},
  {0x2f, "QuartReg"},
  {0x72, "Ans"},
  {0x73, "Fix"},
  {0x74, "Horiz"},
  {0x75, "Full"},
  {0x76, "Func"},
  {0x77, "Param"},
  {0x78, "Polar"},
  {0x79, "Seq"},
  {0x7a, "IndpntAuto"},
  {0x7b, "IndpntAsk"},
  {0x7c, "DependAuto"},
  {0x7d, "DependAsk"},
  {0x04, "->"},
  {0x29, " "},

  {0x3f, "\n"},
  {0x3f, "\r"},
  {0x70, "+"},
  {0x71, "-"},
  {0x2a, "\""},
  {0x2b, ","},
  {0x06, "["},
  {0x07, "]"},
  {0x08, "{"},
  {0x09, "}"},
  {0x10, "("},
  {0x11, ")"},
  {0x6a, "="},
  {0x6b, "<"},
  {0x6c, ">"},
  {0x6d, "<="},
  {0x6e, ">="},
  {0x6f, "!="},
  {0x2d, "!"},

  // alphabetical opcodes
  {0x2c, "i"},
  {'A',  "A"},
  {'B',  "B"},
  {'C',  "C"},
  {'D',  "D"},
  {'E',  "E"},
  {'F',  "F"},
  {'G',  "G"},
  {'H',  "H"},
  {'I',  "I"},
  {'J',  "J"},
  {'K',  "K"},
  {'L',  "L"},
  {'M',  "M"},
  {'N',  "N"},
  {'O',  "O"},
  {'P',  "P"},
  {'Q',  "Q"},
  {'R',  "R"},
  {'S',  "S"},
  {'T',  "T"},
  {'U',  "U"},
  {'V',  "V"},
  {'W',  "W"},
  {'X',  "X"},
  {'Y',  "Y"},
  {'Z',  "Z"},
  {'0',  "0"},
  {'1',  "1"},
  {'2',  "2"},
  {'3',  "3"},
  {'4',  "4"},
  {'5',  "5"},
  {'6',  "6"},
  {'7',  "7"},
  {'8',  "8"},
  {'9',  "9"},

  {0,    NULL}
};

#endif