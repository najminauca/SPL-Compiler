/*
 * expressions.h -- abstract syntax for expressions
 */

#ifndef SPL_EXPRESSION_H
#define SPL_EXPRESSION_H

#include "_declarations.h"
#include "position.h"
#include <types/types.h>
#include <stdio.h>

/**
 * Used to differentiate between expression kinds.
 */
typedef enum {
    EXPRESSION_BINARYEXPRESSION,
    EXPRESSION_INTLITERAL,
    EXPRESSION_VARIABLEEXPRESSION
} expression_kind;

/**
 * This enum represents the possible operators for binary expressions in SPL.
 */
typedef enum {
    ABSYN_OP_EQU,
    ABSYN_OP_NEQ,
    ABSYN_OP_LST,
    ABSYN_OP_LSE,
    ABSYN_OP_GRT,
    ABSYN_OP_GRE,
    ABSYN_OP_ADD,
    ABSYN_OP_SUB,
    ABSYN_OP_MUL,
    ABSYN_OP_DIV
} BinaryOperator;

/**
 * @param operator An operator
 * @return Whether this operator is arithmetic (+, -, *, /)
 */
bool isOperatorArithmetic(BinaryOperator operator);

/**
 * @param operator An operator
 * @return Whether this operator is a comparison (=, !=, <, <=, >, >=)
 */
bool isOperatorComparison(BinaryOperator operator);

/**
 * Flips an operator if it is a comparison operator.
 * @param operator The operator to flip.
 * @return The flipped comparison operator
 */
BinaryOperator flipComparisonOperator(BinaryOperator operator);

/**
 * This struct represents an expression in SPL.
 *
 * Expressions are used as a value assigned to a variable or in the condition of conditional statements in SPL.
 * Every expression has a semantic type that has to be set in phase 4.
 * There are three kinds of expressions:
 *
 * 1. Binary expressions combining a left and a right expression with an operator.
 * The type of binary expressions is dependent on the kind of the operator.
 *
 * 2. Integer literals representing a literal value from the source code.
 * Every time a number is used in an SPL program (decimal, hexadecimal or as a character), this is represented as an
 * integer literal.
 *
 * 3. Variable expressions representing the value of a variable.
 * The semantic type of a variable expression depends on the type of its variable.
 */
typedef struct expression {
    Position position;
    expression_kind kind;
    Type *dataType;
    union {
        struct {
            BinaryOperator operator;
            struct expression *leftOperand, *rightOperand;
        } binaryExpression;
        struct {
            int value;
        } intLiteral;
        struct {
            struct variable *variable;
        } variableExpression;
    } u;
} Expression;

/**
 * Creates a new node representing an expression combining two expressions with an operator.
 * @param line The position of the expression in the source code.
 * @param op The operator used in this expression.
 * @param left The operand on the left hand side of the operator.
 * @param right The operand on the right hand side of the operator.
 * @return A pointer to a newly created node.
 */
Expression *newBinaryExpression(Position position, BinaryOperator op, Expression *left, Expression *right);

/**
 * Creates a new expression-node representing the value of a variable.
 * @param line The position of the variable in the source code.
 * @param var The variable whose value is used as a value for this expression.
 * @return A pointer to a newly created node.
 */
Expression *newVariableExpression(Position position, Variable *var);

/**
 * Creates a new node representing an integer literal.
 * @param line The position of the literal in the source code.
 * @param val The value the literal holds.
 * @return A pointer to a newly created node.
 */
Expression *newIntLiteral(Position position, int val);

void printExpression(FILE *out, int indentation, Expression *expression);

#endif //SPL_EXPRESSION_H
