/*
 * statements.c -- abstract syntax for statements
 */

#include "statements.h"
#include <absyn/printing/printing_helpers.h>
#include <util/memory.h>
#include "lists.h"
#include "variables.h"
#include "expressions.h"

Statement *newStatement(Position position, statement_kind kind) {
    Statement *node = new(Statement);

    node->position = position;
    node->kind = kind;
    return node;
}

Statement *newEmptyStatement(Position position) {
    Statement *node = newStatement(position, STATEMENT_EMPTYSTATEMENT);
    return node;
}

Statement *newCompoundStatement(Position position, StatementList *stms) {
    Statement *node = newStatement(position, STATEMENT_COMPOUNDSTATEMENT);
    node->u.compoundStatement.statements = stms;
    return node;
}

Statement *newAssignStatement(Position position, Variable *var, Expression *exp) {
    Statement *node = newStatement(position, STATEMENT_ASSIGNSTATEMENT);
    node->u.assignStatement.target = var;
    node->u.assignStatement.value = exp;
    return node;
}

Statement *newIfStatement(Position position, Expression *condition, Statement *thenPart, Statement *elsePart) {
    Statement *node = newStatement(position, STATEMENT_IFSTATEMENT);
    node->u.ifStatement.condition = condition;
    node->u.ifStatement.thenPart = thenPart;
    node->u.ifStatement.elsePart = elsePart;
    return node;
}

Statement *newWhileStatement(Position position, Expression *condition, Statement *body) {
    Statement *node = newStatement(position, STATEMENT_WHILESTATEMENT);
    node->u.whileStatement.condition = condition;
    node->u.whileStatement.body = body;
    return node;
}

Statement *newCallStatement(Position position, Identifier *name, ExpressionList *arguments) {
    Statement *node = newStatement(position, STATEMENT_CALLSTATEMENT);
    node->u.callStatement.procedureName = name;
    node->u.callStatement.arguments = arguments;
    return node;
}

static void printEmptyStatement(FILE *out) {
    fprintf(out, "EmptyStatement()");
}

static void printCompoundStatement(FILE *out, int indentation, Statement *self) {
    fprintf(out, "CompoundStatement(");
    printStatementList(out, indentation + INDENTATION_INCREMENT, true, self->u.compoundStatement.statements);
    fprintf(out, ")");
}

static void printAssignStatement(FILE *out, int indentation, Statement *self) {
    fprintf(out, "AssignStatement(\n");
    printVariable(out, indentation + INDENTATION_INCREMENT, self->u.assignStatement.target);
    fprintf(out, ",\n");
    printExpression(out, indentation + INDENTATION_INCREMENT, self->u.assignStatement.value);
    fprintf(out, ")");
}

static void printIfStatement(FILE *out, int indentation, Statement *self) {
    fprintf(out, "IfStatement(\n");
    printExpression(out, indentation + INDENTATION_INCREMENT, self->u.ifStatement.condition);
    fprintf(out, ",\n");
    printStatement(out, indentation + INDENTATION_INCREMENT, self->u.ifStatement.thenPart);
    fprintf(out, ",\n");
    printStatement(out, indentation + INDENTATION_INCREMENT, self->u.ifStatement.elsePart);
    fprintf(out, ")");
}

static void printWhileStatement(FILE *out, int indentation, Statement *self) {
    fprintf(out, "WhileStatement(\n");
    printExpression(out, indentation + INDENTATION_INCREMENT, self->u.whileStatement.condition);
    fprintf(out, ",\n");
    printStatement(out, indentation + INDENTATION_INCREMENT, self->u.whileStatement.body);
    fprintf(out, ")");
}

static void printCallStatement(FILE *out, int indentation, Statement *self) {

    fprintf(out, "CallStatement(\n");
    printIdentifier(out, indentation + INDENTATION_INCREMENT, self->u.callStatement.procedureName);
    fprintf(out, ",\n");

    indent(out, indentation + INDENTATION_INCREMENT);
    fprintf(out, "Arguments(");
    printExpressionList(out, indentation + 2 * INDENTATION_INCREMENT, true, self->u.callStatement.arguments);
    fprintf(out, "))");
}

void printStatement(FILE *out, int indentation, Statement *self) {
    indent(out, indentation);
    if (self == NULL) {
        fprintf(out, "NULL");
        return;
    }

    switch (self->kind) {
        case STATEMENT_ASSIGNSTATEMENT:
            printAssignStatement(out, indentation, self);
            break;
        case STATEMENT_CALLSTATEMENT:
            printCallStatement(out, indentation, self);
            break;
        case STATEMENT_COMPOUNDSTATEMENT:
            printCompoundStatement(out, indentation, self);
            break;
        case STATEMENT_EMPTYSTATEMENT:
            printEmptyStatement(out);
            break;
        case STATEMENT_IFSTATEMENT:
            printIfStatement(out, indentation, self);
            break;
        case STATEMENT_WHILESTATEMENT:
            printWhileStatement(out, indentation, self);
            break;
        default:
            printUnknownKind(out, self->kind);
    }
}