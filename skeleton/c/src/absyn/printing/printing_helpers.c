/*
 * printing_helpers.c -- Helpers for printing the abstract syntax tree
 */

#include "printing_helpers.h"

void indent(FILE *out, int indentation) {
    fprintf(out, "%*s", indentation, "");
}

void printUnknownKind(FILE *out, int kind) {
    fprintf(out, "UNKNOWN_KIND(%d)", kind);
}

void printInteger(FILE *out, int indentation, int integer) {
    indent(out, indentation);
    fprintf(out, "%d", integer);
}

void printBoolean(FILE *out, int indentation, bool boolean) {
    indent(out, indentation);
    fprintf(out, "%s", boolean ? "true" : "false");
}

void printIdentifier(FILE *out, int indentation, Identifier *identifier) {
    indent(out, indentation);
    fprintf(out, "%s", identifier->string);
}
