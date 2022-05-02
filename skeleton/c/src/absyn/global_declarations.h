/*
 * global_declarations.h -- abstract syntax for global declarations
 */

#ifndef SPL_GLOBAL_DECLARATIONS_H
#define SPL_GLOBAL_DECLARATIONS_H

#include "_declarations.h"
#include "position.h"
#include <table/identifier.h>
#include <stdio.h>

/**
 * Used to differentiate between global declaration kinds.
 */
typedef enum {
    DECLARATION_TYPEDECLARATION,
    DECLARATION_PROCEDUREDECLARATION
} global_declaration_kind;

/**
 * This struct represents a global declaration in SPL.
 *
 * Global declarations are all declarations done in the global scope.
 * There are two kinds of global declarations:
 *
 * 1. Type declarations representing the declaration of a new type. When declaring a new type it is necessary
 * to provide an identifier for the newly declared type and a type expression describing the type.
 *
 * 2. Procedure declarations representing the declaration of a new procedure. When declaring a procedure
 * an identifier has to be provided, that is used as the identifier of the declaration.
 * Additionally a procedure declaration requires a list of parameters, a list of local variables and
 * a list of statements in the body of the procedure.
 */
typedef struct global_declaration {
    Position position;
    global_declaration_kind kind;
    Identifier *name;
    union {
        struct {
            TypeExpression *typeExpression;
        } typeDeclaration;
        struct {
            ParameterDeclarationList *parameters;
            VariableDeclarationList *variables;
            StatementList *body;
        } procedureDeclaration;
    } u;
} GlobalDeclaration;

/**
 * Creates a new node representing a type declaration.
 * @param line The position of the declaration in the source code.
 * @param name The declarations identifier.
 * @param ty The type expression associated with this declaration.
 * @return A pointer to a newly created node.
 */
GlobalDeclaration *newTypeDeclaration(Position position, Identifier *name, TypeExpression *ty);

/**
 * Creates a new node representing a procedure declaration.
 * @param line The position of the declaration in the source code.
 * @param name The procedures identifier.
 * @param params The procedures parameter list.
 * @param decls The procedures local variables.
 * @param body The statements in the body of the procedure.
 * @return A pointer to a newly created node.
 */
GlobalDeclaration *newProcedureDeclaration(Position position, Identifier *name, ParameterDeclarationList *params, VariableDeclarationList *decls, StatementList *body);

void printGlobalDeclaration(FILE *out, int intendation, GlobalDeclaration *self);

#endif //SPL_GLOBAL_DECLARATIONS_H
