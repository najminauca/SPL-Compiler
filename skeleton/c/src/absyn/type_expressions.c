/*
 * type_expressions.c -- abstract syntax for type expressions
 */

#include "type_expressions.h"
#include <util/memory.h>
#include <stddef.h>
#include <absyn/printing/printing_helpers.h>

static TypeExpression *newTypeExpression(Position position, type_expression_kind kind) {
    TypeExpression *node =new(TypeExpression);

    node->position = position;
    node->kind = kind;
    node->dataType = NULL;
    return node;
}

TypeExpression *newNamedTypeExpression(Position position, Identifier *name) {
    TypeExpression *node = newTypeExpression(position, TYPEEXPRESSION_NAMEDTYPEEXPRESSION);
    node->u.namedTypeExpression.name = name;
    return node;
}

TypeExpression *newArrayTypeExpression(Position position, int size, TypeExpression *ty) {
    TypeExpression *node = newTypeExpression(position, TYPEEXPRESSION_ARRAYTYPEEXPRESSION);
    node->u.arrayTypeExpression.arraySize = size;
    node->u.arrayTypeExpression.baseType = ty;
    return node;
}

static void printNamedTypeExpression(FILE *out, int indentation, TypeExpression *self) {
    fprintf(out, "NamedTypeExpression(\n");
    printIdentifier(out, indentation + INDENTATION_INCREMENT, self->u.namedTypeExpression.name);
    fprintf(out, ")");
}

static void printArrayTypeExpression(FILE *out, int indentation, TypeExpression *self) {
    fprintf(out, "ArrayTypeExpression(\n");
    printTypeExpression(out, indentation + INDENTATION_INCREMENT, self->u.arrayTypeExpression.baseType);
    fprintf(out, ",\n");
    printInteger(out, indentation + INDENTATION_INCREMENT, self->u.arrayTypeExpression.arraySize);
    fprintf(out, ")");
}

void printTypeExpression(FILE *out, int indentation, TypeExpression *self) {
    indent(out, indentation);

    if (self == NULL) {
        fprintf(out, "NULL");
        return;
    }

    switch (self->kind) {
        case TYPEEXPRESSION_ARRAYTYPEEXPRESSION:
            printArrayTypeExpression(out, indentation, self);
            break;
        case TYPEEXPRESSION_NAMEDTYPEEXPRESSION:
            printNamedTypeExpression(out, indentation, self);
            break;
        default:
            printUnknownKind(out, self->kind);
    }
}