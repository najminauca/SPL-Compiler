/*
 * type_expressions.h -- abstract syntax for type expressions
 */

#ifndef SPL_TYPE_EXPRESSIONS_H
#define SPL_TYPE_EXPRESSIONS_H

#include "_declarations.h"
#include "position.h"
#include <table/identifier.h>
#include <types/types.h>
#include <stdio.h>

/**
 * Used to differentiate between type expression kinds.
 */
typedef enum {
    TYPEEXPRESSION_NAMEDTYPEEXPRESSION,
    TYPEEXPRESSION_ARRAYTYPEEXPRESSION
} type_expression_kind;

/**
 * This struct represents type expressions in SPL.
 *
 * A type expression behaves like a formula representing a concrete semantic type, which has to be
 * calculated during phase 4.
 * There are two kinds of type expressions in SPL:
 *
 * 1. Named type expressions are type expressions, where the type is determined solely by its identifier.
 *
 * 2. Array type expressions which represent the type of an fixed-size array of another type expression.
 * They consist of the base type, which is the type of an arrays elements and a size, determining how many
 * of those elements an array of this type can hold.
 */
typedef struct type_expression {
    Position position;
    type_expression_kind kind;
    Type *dataType;
    union {
        struct {
            Identifier *name;
        } namedTypeExpression;
        struct {
            int arraySize;
            struct type_expression *baseType;
        } arrayTypeExpression;
    } u;
} TypeExpression;

/**
 * Creates a new node representing a named type expression.
 * @param line The position of the type expression in the source code.
 * @param name The identifier used to express this type.
 * @return A pointer to a newly created node.
 */
TypeExpression *newNamedTypeExpression(Position position, Identifier *name);

/**
 * Creates a new node representing the type expression for the type of a fixed-size array.
 * @param line The position of the type expression in the source code.
 * @param ty The type expression of the elements type.
 * @param size The number of elements an array of this type can hold.
 * @return A pointer to a newly created node.
 */
TypeExpression *newArrayTypeExpression(Position position, int size, TypeExpression *ty);

void printTypeExpression(FILE *out, int indentation, TypeExpression *self);

#endif //SPL_TYPE_EXPRESSIONS_H
