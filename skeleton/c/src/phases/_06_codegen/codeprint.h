/*
 * codeprint.h -- Support function to generate ECO32 assembly
 */

#ifndef SPL_CODEPRINT_H
#define SPL_CODEPRINT_H

#include <stdio.h>

void emit(FILE *out, const char *format, ...);

void emitImport(FILE *out, char *id);

void emitRRI(FILE *out, const char *opcode, int reg1, int reg2, int value);

void commentRRI(FILE *out, const char *opcode, int reg1, int reg2, int value, const char *commentFormat, ...);

void emitR(FILE *out, const char *opcode, int reg);

void commentR(FILE *out, const char *opcode, int reg, const char *commentFormat, ...);

void emitRRR(FILE *out, const char *opcode, int reg1, int reg2, int reg3);

void commentRRR(FILE *out, const char *opcode, int reg1, int reg2, int reg3, const char *commentFormat, ...);

void emitRRL(FILE *out, const char *opcode, int reg1, int reg2, const char *labelFormat, ...);

void emitLabel(FILE *out, const char *labelFormat, ...);

void emitJump(FILE *out, const char *labelFormat, ...);

void emitSS(FILE *out, const char *s1, const char *s2);

#endif /* SPL_CODEPRINT_H */