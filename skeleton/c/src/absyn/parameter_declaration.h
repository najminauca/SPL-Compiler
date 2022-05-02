/*
 * parameter_declaration.h -- abstract syntax for parameter declarations
 */

#ifndef SPL_PARAMETER_DECLARATION_H
#define SPL_PARAMETER_DECLARATION_H

#include "_declarations.h"
#include "position.h"
#include <table/identifier.h>
#include <stdbool.h>

/**
 * This struct represents the local declaration of a parameter in SPL.
 *
 * Parameter are declared in the parameter list of a procedure. They combine an identifier with
 * a type expression, expressing the parameters type and additionally have to store
 * whether the parameter is passed as a reference.
 * Parameters are only visible in the scope of their procedure.
 */
typedef struct parameter_declaration {
    Position position;
    Identifier *name;
    TypeExpression *typeExpression;
    bool isReference;
} ParameterDeclaration;

/**
 * Creates a new node representing the declaration of a parameter in the head of a procedure.
 * @param line The position of the declaration in the source code.
 * @param name The identifier of the declared parameter.
 * @param ty The type expression used to express the parameters type.
 * @param isRef A boolean value used to represent whether the parameter is passed as a reference.
 * @return A pointer to a newly created node.
 */
ParameterDeclaration *newParameterDeclaration(Position position, Identifier *name, TypeExpression *ty, bool isRef);

void printParameterDeclaration(FILE *out, int indentation, ParameterDeclaration *self);

#endif //SPL_PARAMETER_DECLARATION_H
