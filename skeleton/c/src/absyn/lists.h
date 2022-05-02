/*
 * lists.h -- Lists structures part of the abstract syntax
 */

#ifndef SPL_LISTS_H
#define SPL_LISTS_H

#include "_declarations.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct global_declaration_list {
    bool isEmpty;
    GlobalDeclaration *head;
    struct global_declaration_list *tail;
} GlobalDeclarationList;

typedef struct parameter_list {
    bool isEmpty;
    ParameterDeclaration *head;
    struct parameter_list *tail;
} ParameterDeclarationList;

typedef struct variable_declaration_list {
    bool isEmpty;
    VariableDeclaration *head;
    struct variable_declaration_list *tail;
} VariableDeclarationList;

typedef struct statement_list {
    bool isEmpty;
    Statement *head;
    struct statement_list *tail;
} StatementList;

typedef struct expression_list {
    bool isEmpty;
    Expression *head;
    struct expression_list *tail;
} ExpressionList;

/**
 * @return An empty list for global declarations.
 */
GlobalDeclarationList *emptyGlobalDeclarationList(void);

/**
 * Creates a new GlobalDeclarationList by prepending a new head to an existing list.
 * Does not modify the existing list.
 * @param head The head of the new list.
 * @param tail The tail of the new list.
 * @return A list containing the new head as well as the old list as its tail.
 */
GlobalDeclarationList *newGlobalDeclarationList(GlobalDeclaration *head, GlobalDeclarationList *tail);

/**
 * @return An empty list for variables.
 */
VariableDeclarationList *emptyVariableList(void);

/**
 * Creates a new VariableDeclarationList by prepending a new head to an existing list.
 * Does not modify the existing list.
 * @param head The head of the new list.
 * @param tail The tail of the new list.
 * @return A list containing the new head as well as the old list as its tail.
 */
VariableDeclarationList *newVariableList(VariableDeclaration *head, VariableDeclarationList *tail);

/**
 * @return An empty list for parameters.
 */
ParameterDeclarationList *emptyParameterList(void);

/**
 * Creates a new ParameterDeclarationList by prepending a new head to an existing list.
 * Does not modify the existing list.
 * @param head The head of the new list.
 * @param tail The tail of the new list.
 * @return A list containing the new head as well as the old list as its tail.
 */
ParameterDeclarationList *newParameterList(ParameterDeclaration *head, ParameterDeclarationList *tail);

/**
 * Returns an empty list for statements.
 */
StatementList *emptyStatementList(void);

/**
 * Creates a new StatementList by prepending a new head to an existing list.
 * Does not modify the existing list.
 * @param head The head of the new list.
 * @param tail The tail of the new list.
 * @return A list containing the new head as well as the old list as its tail.
 */
StatementList *newStatementList(Statement *head, StatementList *tail);

/**
 * @return An empty list for expressions.
 */
ExpressionList *emptyExpressionList(void);

/**
 * Creates a new ExpressionList by prepending a new head to an existing list.
 * Does not modify the existing list.
 * @param head The head of the new list.
 * @param tail The tail of the new list.
 * @return A list containing the new head as well as the old list as its tail.
 */
ExpressionList *newExpressionList(Expression *head, ExpressionList *tail);

void printGlobalDeclarationList(FILE *out, int indentation, bool isFirst, GlobalDeclarationList *list);

void printParameterDeclarationList(FILE *out, int indentation, bool isFirst, ParameterDeclarationList *list);

void printVariableDeclarationList(FILE *out, int indentation, bool isFirst, VariableDeclarationList *list);

void printStatementList(FILE *out, int indentation, bool isFirst, StatementList *list);

void printExpressionList(FILE *out, int indentation, bool isFirst, ExpressionList *list);

#endif //SPL_LISTS_H
