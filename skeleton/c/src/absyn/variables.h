/*
 * variables.h -- abstract syntax for variables
 */

#ifndef SPL_VARIABLES_H
#define SPL_VARIABLES_H

#include "_declarations.h"
#include "position.h"
#include <types/types.h>
#include <table/identifier.h>

/**
 * Used to differentiate between variable kinds.
 */
typedef enum {
    VARIABLE_NAMEDVARIABLE,
    VARIABLE_ARRAYACCESS
} variable_kind;

/**
 * This struct represents variables in SPL.
 *
 * Variables are comparable to pointers to a certain value.
 * Every variable has a semantic type, that has to be set in phase 4.
 * There are two kinds of variables in SPL:
 *
 * 1. Named variables which represent a simple variable, that is identified by its name.
 *
 * 2. Array accesses that represent an array accessed at a certain index. The accessed array itself may be any variable.
 */
typedef struct variable {
    Position position;
    variable_kind kind;
    Type *dataType;
    union {
        struct {
            Identifier *name;
        } namedVariable;
        struct {
            struct variable *array;
            struct expression *index;
        } arrayAccess;
    } u;
} Variable;

/**
 * Creates a new node representing a named variable.
 * @param line The position of the variable in the source code.
 * @param name The identifier of the variable.
 * @return A pointer to a newly created node.
 */
Variable *newNamedVariable(Position position, Identifier *name);

/**
 * Creates a new node representing an array access.
 * @param line The position of the array access in the source code.
 * @param var The variable representing the accessed array.
 * @param index The expression representing the index of the access.
 * @return A pointer to a newly created node.
 */
Variable *newArrayAccess(Position position, Variable *var, Expression *index);

void printVariable(FILE *out, int indentation, Variable *self);

#endif //SPL_VARIABLES_H
