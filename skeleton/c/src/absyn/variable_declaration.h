/*
 * variable_declaration.h -- abstract syntax for variable declarations
 */

#ifndef _VARIABLE_DECLARATION_H
#define _VARIABLE_DECLARATION_H

#include "_declarations.h"
#include "position.h"
#include <table/identifier.h>
#include <stdio.h>

/**
 * This struct represents the local declaration of a variable in SPL.
 *
 * Variables are declared inside a procedure and combine an identifier with a type expression,
 * expressing the variables type.
 * Variables are only visible in the local scope of their procedure.
 */
typedef struct variable_declaration {
    Position position;
    Identifier *name;
    TypeExpression *typeExpression;
} VariableDeclaration;

/**
 * Creates a new node representing the declaration of a local variable in a procedures body.
 * @param line The position of the declaration in the source code.
 * @param name The identifier of the declared local variable.
 * @param ty The type expression used to express the type of the local variable.
 * @return A pointer to a newly created node.
 */
VariableDeclaration *newVariableDeclaration(Position position, Identifier *name, TypeExpression *ty);

void printVariableDeclaration(FILE *out, int indentation, VariableDeclaration *self);

#endif //_VARIABLE_DECLARATION_H
