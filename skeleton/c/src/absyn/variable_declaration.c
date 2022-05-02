/*
 * variable_declaration.c -- abstract syntax for variable declarations
 */

#include "variable_declaration.h"
#include "type_expressions.h"
#include <util/memory.h>
#include <absyn/printing/printing_helpers.h>

VariableDeclaration *newVariableDeclaration(Position position, Identifier *name, TypeExpression *ty) {
    VariableDeclaration *node = new(VariableDeclaration);
    node->position = position;
    node->name = name;
    node->typeExpression = ty;
    return node;
}

void printVariableDeclaration(FILE *out, int indentation, VariableDeclaration *self) {
    indent(out, indentation);

    if (self == NULL) {
        fprintf(out, "NULL");
        return;
    }

    fprintf(out, "VariableDeclaration(\n");
    printIdentifier(out, indentation + INDENTATION_INCREMENT, self->name);
    fprintf(out, ",\n");
    printTypeExpression(out, indentation + INDENTATION_INCREMENT, self->typeExpression);
    fprintf(out, ")");
}

