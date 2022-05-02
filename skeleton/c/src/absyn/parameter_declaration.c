/*
 * parameter_declaration.c -- abstract syntax for parameter declarations
 */

#include <absyn/printing/printing_helpers.h>
#include <util/memory.h>
#include "parameter_declaration.h"
#include "type_expressions.h"

ParameterDeclaration *newParameterDeclaration(Position position, Identifier *name, TypeExpression *ty, bool isRef) {
    ParameterDeclaration *node = new(ParameterDeclaration);
    node->position = position;
    node->name = name;
    node->typeExpression = ty;
    node->isReference = isRef;
    return node;
}

void printParameterDeclaration(FILE *out, int indentation, ParameterDeclaration *self) {
    indent(out, indentation);

    if (self == NULL) {
        fprintf(out, "NULL");
        return;
    }

    fprintf(out, "ParameterDeclaration(\n");
    printIdentifier(out, indentation + INDENTATION_INCREMENT, self->name);
    fprintf(out, ",\n");
    printTypeExpression(out, indentation + INDENTATION_INCREMENT, self->typeExpression);
    fprintf(out, ",\n");
    printBoolean(out, indentation + INDENTATION_INCREMENT, self->isReference);
    fprintf(out, ")");
}
