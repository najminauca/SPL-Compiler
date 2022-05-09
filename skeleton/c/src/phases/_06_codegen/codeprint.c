/*
 * codeprint.h -- Support function to generate ECO32 assembly
 */

#include "codeprint.h"

#include <stdarg.h>

void emit(FILE *out, const char *format, ...) {
    va_list ap;

    va_start(ap, format);
    vfprintf(out, format, ap);
    fprintf(out, "\n");
    va_end(ap);
}

void emitImport(FILE *out, char *id) {
    fprintf(out, "\t.import\t%s\n", id);
}

void emitRRI(FILE *out, const char *opcode, int reg1, int reg2, int value) {
    fprintf(out, "\t%s\t$%d,$%d,%d\n", opcode, reg1, reg2, value);
}

void commentRRI(FILE *out, const char *opcode, int reg1, int reg2, int value, const char *commentFormat, ...) {
    va_list ap;

    fprintf(out, "\t%s\t$%d,$%d,%d", opcode, reg1, reg2, value);

    va_start(ap, commentFormat);
    fprintf(out, "\t\t; ");
    vfprintf(out, commentFormat, ap);
    fprintf(out, "\n");
    va_end(ap);
}

void emitR(FILE *out, const char *opcode, int reg) {
    fprintf(out, "\t%s\t$%d\n", opcode, reg);
}

void commentR(FILE *out, const char *opcode, int reg, const char *commentFormat, ...) {
    va_list ap;

    fprintf(out, "\t%s\t$%d", opcode, reg);

    va_start(ap, commentFormat);
    fprintf(out, "\t\t\t; ");
    vfprintf(out, commentFormat, ap);
    fprintf(out, "\n");
    va_end(ap);
}

void emitRRR(FILE *out, const char *opcode, int reg1, int reg2, int reg3) {
    fprintf(out, "\t%s\t$%d,$%d,$%d\n", opcode, reg1, reg2, reg3);
}

void commentRRR(FILE *out, const char *opcode, int reg1, int reg2, int reg3, const char *commentFormat, ...) {
    va_list ap;

    fprintf(out, "\t%s\t$%d,$%d,$%d", opcode, reg1, reg2, reg3);

    va_start(ap, commentFormat);
    fprintf(out, "\t\t; ");
    vfprintf(out, commentFormat, ap);
    fprintf(out, "\n");
    va_end(ap);
}

void emitRRL(FILE *out, const char *opcode, int reg1, int reg2, const char *labelFormat, ...) {
    va_list ap;
    // print("\t%s\t$%d,$%d,%s\n", opcode, reg1, reg2, label)
    va_start(ap, labelFormat);
    fprintf(out, "\t%s\t$%d,$%d,", opcode, reg1, reg2);
    vfprintf(out, labelFormat, ap);
    fprintf(out, "\n");
    va_end(ap);
}

void emitLabel(FILE *out, const char *labelFormat, ...) {
    va_list ap;
    // print("%s:\n", label)
    va_start(ap, labelFormat);
    vfprintf(out, labelFormat, ap);
    fprintf(out, ":\n");
    va_end(ap);
}

void emitJump(FILE *out, const char *labelFormat, ...) {
    va_list ap;
    // print("\tj\t%s\n", label)
    va_start(ap, labelFormat);
    fprintf(out, "\tj\t");
    vfprintf(out, labelFormat, ap);
    fprintf(out, "\n");
    va_end(ap);
}

void emitSS(FILE *out, const char *s1, const char *s2) {
    fprintf(out, "\t%s\t%s\n", s1, s2);
}
