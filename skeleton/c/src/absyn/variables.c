/*
 * variables.c -- abstract syntax for variables
 */

#include <stddef.h>
#include <util/memory.h>
#include <absyn/printing/printing_helpers.h>
#include "variables.h"
#include "expressions.h"

static Variable *newVariable(Position position, variable_kind kind) {
    Variable *node = new(Variable);

    node->position = position;
    node->kind = kind;
    node->dataType = NULL;
    return node;
}

Variable *newNamedVariable(Position position, Identifier *name) {
    Variable *node = newVariable(position, VARIABLE_NAMEDVARIABLE);
    node->u.namedVariable.name = name;
    return node;
}

Variable *newArrayAccess(Position position, Variable *var, Expression *index) {
    Variable *node = newVariable(position, VARIABLE_ARRAYACCESS);
    node->u.arrayAccess.array = var;
    node->u.arrayAccess.index = index;
    return node;
}

static void printNamedVariable(FILE *out, int indentation, Variable *self) {
    fprintf(out, "NamedVariable(\n");
    printIdentifier(out, indentation + INDENTATION_INCREMENT, self->u.namedVariable.name);
    fprintf(out, ")");
}

static void printArrayAccess(FILE *out, int indentation, Variable *self) {
    fprintf(out, "ArrayAccess(\n");
    printVariable(out, indentation + INDENTATION_INCREMENT, self->u.arrayAccess.array);
    fprintf(out, ",\n");
    printExpression(out, indentation + INDENTATION_INCREMENT, self->u.arrayAccess.index);
    fprintf(out, ")");
}

void printVariable(FILE *out, int indentation, Variable *self) {
    indent(out, indentation);

    if (self == NULL) {
        fprintf(out, "NULL");
        return;
    }

    switch (self->kind) {
        case VARIABLE_NAMEDVARIABLE:
            printNamedVariable(out, indentation, self);
            break;
        case VARIABLE_ARRAYACCESS:
            printArrayAccess(out, indentation, self);
            break;
        default:
            printUnknownKind(out, self->kind);
    }
}