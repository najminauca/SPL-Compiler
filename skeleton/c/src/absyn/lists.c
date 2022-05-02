/*
 * lists.c -- Lists structures part of the abstract syntax
 */

#include <stddef.h>
#include <util/memory.h>
#include <util/list.h>
#include "lists.h"
#include "global_declarations.h"
#include "statements.h"
#include "parameter_declaration.h"
#include "variable_declaration.h"
#include "expressions.h"
#include "printing/printing_helpers.h"

DEFINE_EMPTY_LIST(emptyGlobalDeclarationList, GlobalDeclarationList)

DEFINE_EMPTY_LIST(emptyParameterList, ParameterDeclarationList)

DEFINE_EMPTY_LIST(emptyVariableList, VariableDeclarationList)

DEFINE_EMPTY_LIST(emptyStatementList, StatementList)

DEFINE_EMPTY_LIST(emptyExpressionList, ExpressionList)

DEFINE_LIST_CONSTRUCTOR(newGlobalDeclarationList, GlobalDeclarationList, GlobalDeclaration)

DEFINE_LIST_CONSTRUCTOR(newParameterList, ParameterDeclarationList, ParameterDeclaration)

DEFINE_LIST_CONSTRUCTOR(newVariableList, VariableDeclarationList, VariableDeclaration)

DEFINE_LIST_CONSTRUCTOR(newStatementList, StatementList, Statement)

DEFINE_LIST_CONSTRUCTOR(newExpressionList, ExpressionList, Expression)

#define DEFINE_LIST_PRINTING_FUNCTION(name, list_type, element_printing_function)                   \
    void name (FILE *out, int indentation, bool isFirst, list_type *list) {                         \
        if (list != NULL && list->isEmpty) return;                                                  \
                                                                                                    \
        if (isFirst) fprintf(out, "\n");                                                            \
        else fprintf(out, ",\n");                                                                   \
                                                                                                    \
        if (list == NULL) {                                                                         \
            indent(out, indentation);                                                               \
            if (isFirst) fprintf(out, "LIST_NULL");                                                 \
            else fprintf(out, "TAIL_NULL");                                                         \
            return;                                                                                 \
        }                                                                                           \
                                                                                                    \
        element_printing_function(out, indentation, list->head);                                    \
        name(out, indentation, false, list->tail);                                                  \
    }

DEFINE_LIST_PRINTING_FUNCTION(printGlobalDeclarationList, GlobalDeclarationList, printGlobalDeclaration)

DEFINE_LIST_PRINTING_FUNCTION(printVariableDeclarationList, VariableDeclarationList, printVariableDeclaration)

DEFINE_LIST_PRINTING_FUNCTION(printParameterDeclarationList, ParameterDeclarationList, printParameterDeclaration)

DEFINE_LIST_PRINTING_FUNCTION(printStatementList, StatementList, printStatement)

DEFINE_LIST_PRINTING_FUNCTION(printExpressionList, ExpressionList, printExpression)
