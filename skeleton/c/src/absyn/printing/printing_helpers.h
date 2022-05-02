/*
 * printing_helpers.h -- Helpers for printing the abstract syntax tree
 */

#ifndef SPL_PRINTING_HELPERS_H
#define SPL_PRINTING_HELPERS_H

#include <stdio.h>
#include <stdbool.h>
#include <table/identifier.h>

void indent(FILE *out, int indentation);

void printUnknownKind(FILE *out, int kind);

void printInteger(FILE *out, int indentation, int integer);

void printBoolean(FILE *out, int indentation, bool boolean);

void printIdentifier(FILE *out, int indentation, Identifier *identifier);

#define INDENTATION_INCREMENT 2

#endif //SPL_PRINTING_HELPERS_H
