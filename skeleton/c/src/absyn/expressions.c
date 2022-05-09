/*
 * expressions.c -- abstract syntax for expressions
 */

#include "expressions.h"
#include "variables.h"

#include <stddef.h>
#include <util/memory.h>
#include <absyn/printing/printing_helpers.h>
#include <assert.h>
#include <util/errors.h>

bool isOperatorArithmetic(BinaryOperator operator) {
    notImplemented(); // TODO: Implement if you need this
}

bool isOperatorComparison(BinaryOperator operator) {
    notImplemented(); // TODO: Implement if you need this
}

BinaryOperator flipComparisonOperator(BinaryOperator operator) {
    notImplemented(); // TODO: Implement if you need this
}

/**
 * This array holds the short-codes of the operators in an order corresponding to the BinaryOperator enum.
 */
const char *BINARY_OPERATOR_NAMES[] = {
        "EQU",
        "NEQ",
        "LST",
        "LSE",
        "GRT",
        "GRE",
        "ADD",
        "SUB",
        "MUL",
        "DIV"
};

Expression *newExpression(Position position, expression_kind kind) {
    Expression *node = new(Expression);

    node->position = position;
    node->kind = kind;
    node->dataType = NULL;
    return node;
}

Expression *newBinaryExpression(Position position, BinaryOperator op, Expression *left, Expression *right) {
    Expression *node = newExpression(position, EXPRESSION_BINARYEXPRESSION);
    node->u.binaryExpression.operator = op;
    node->u.binaryExpression.leftOperand = left;
    node->u.binaryExpression.rightOperand = right;
    return node;
}

Expression *newVariableExpression(Position position, Variable *var) {
    Expression *node = newExpression(position, EXPRESSION_VARIABLEEXPRESSION);
    node->u.variableExpression.variable = var;
    return node;
}

Expression *newIntLiteral(Position position, int val) {
    Expression *node = newExpression(position, EXPRESSION_INTLITERAL);
    node->u.intLiteral.value = val;
    return node;
}

static void printBinaryOperator(FILE *out, int indentation, BinaryOperator binaryOperator) {
    indent(out, indentation);
    fprintf(out, "%s", BINARY_OPERATOR_NAMES[binaryOperator]);
}

static void printBinaryExpression(FILE *out, int indentation, Expression *expression) {
    fprintf(out, "BinaryExpression(\n");
    printBinaryOperator(out, indentation + INDENTATION_INCREMENT, expression->u.binaryExpression.operator);
    fprintf(out, ",\n");

    printExpression(out, indentation + INDENTATION_INCREMENT, expression->u.binaryExpression.leftOperand);
    fprintf(out, ",\n");

    printExpression(out, indentation + INDENTATION_INCREMENT, expression->u.binaryExpression.rightOperand);
    fprintf(out, ")");
}

static void printIntLiteral(FILE *out, int indentation, Expression *expression) {
    fprintf(out, "IntLiteral(\n");
    printInteger(out, indentation + INDENTATION_INCREMENT, expression->u.intLiteral.value);
    fprintf(out, ")");
}

static void printVariableExpression(FILE *out, int indentation, Expression *expression) {
    fprintf(out, "VariableExpression(\n");
    printVariable(out, indentation + INDENTATION_INCREMENT, expression->u.variableExpression.variable);
    fprintf(out, ")");
}

void printExpression(FILE *out, int indentation, Expression *expression) {
    indent(out, indentation);
    if (expression == NULL) {
        fprintf(out, "NULL");
        return;
    }

    switch (expression->kind) {
        case EXPRESSION_BINARYEXPRESSION:
            printBinaryExpression(out, indentation, expression);
            break;
        case EXPRESSION_INTLITERAL:
            printIntLiteral(out, indentation, expression);
            break;
        case EXPRESSION_VARIABLEEXPRESSION:
            printVariableExpression(out, indentation, expression);
            break;
        default:
            printUnknownKind(out, expression->kind);
    }
}
