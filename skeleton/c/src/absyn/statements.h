/*
 * statements.h -- abstract syntax for statements
 */

#ifndef _STATEMENT_H
#define _STATEMENT_H

#include <table/identifier.h>
#include "_declarations.h"
#include "position.h"
#include <stdio.h>

/**
 * Used to differentiate between statement kinds.
 */
typedef enum {
    STATEMENT_EMPTYSTATEMENT,
    STATEMENT_COMPOUNDSTATEMENT,
    STATEMENT_ASSIGNSTATEMENT,
    STATEMENT_IFSTATEMENT,
    STATEMENT_WHILESTATEMENT,
    STATEMENT_CALLSTATEMENT
} statement_kind;

/**
 * This structs represent a statement in SPL.
 *
 * There exist many kinds of statements which, may all occur in a procedures body in SPL:
 *
 * 1. Empty statements are statements with no effect.
 *
 * 2. Compound statements are statements that combine different statements into a single one.
 * These are used whenever it is semantically necessary for many statements to act as one, for example the
 * body of a while-statement may only hold a single statement, which can be a compound statement
 * containing many statements itself.
 *
 * 3. Assign statements evaluates its right hand side expression and stores the value in the
 * variable on the left hand side of the assignment operator.
 *
 * 4. If-statements consisting of two branches and a conditional expression. Which of the two branches
 * is executed depends on the value of the condition.
 *
 * 5. While-statements consisting of a conditional expression and a body. The statement in the body is executed
 * until the condition evaluates to false.
 *
 * 6. Call statements represent a call of another procedure in SPL.
 * Whenever a procedure is called, additionally to the called procedures name, a list of expressions
 * has to be provided, whose types match the parameters of the called procedure.
 * Those expressions are then evaluated and passed as arguments to the called procedure, which is executed.
 * The execution of the current procedure is halted until the called procedure returns.
 */
typedef struct statement {
    Position position;
    statement_kind kind;
    union {
        struct {
            int dummy;        /* empty struct not allowed in C */
        } emptyStatement;
        struct {
            struct statement_list *statements;
        } compoundStatement;
        struct {
            Variable *target;
            Expression *value;
        } assignStatement;
        struct {
            Expression *condition;
            struct statement *thenPart;
            struct statement *elsePart;
        } ifStatement;
        struct {
            Expression *condition;
            struct statement *body;
        } whileStatement;
        struct {
            Identifier *procedureName;
            struct expression_list *arguments;
        } callStatement;
    } u;
} Statement;

/**
 * Creates a new node representing an empty statement.
 * @param line The position of the statement in the source code.
 * @return A pointer to a newly created node.
 */
Statement *newEmptyStatement(Position position);

/**
 * Creates a new node representing a compound statement.
 * @param line The position of the statement in the source code.
 * @param stms The list of statements that this statement combines.
 * @return A pointer to a newly created node.
 */
Statement *newCompoundStatement(Position position, StatementList *stms);

/**
 * Creates a new node representing an assignment.
 * @param line The position of the statement in the source code.
 * @param var The variable where the value is assigned to.
 * @param exp The value to be assigned.
 * @return A pointer to a newly created node.
 */
Statement *newAssignStatement(Position position, Variable *var, Expression *exp);

/**
 * Creates a new node representing an if-statement.
 * @param line The position of the statement in the source code.
 * @param condition The expression deciding which branch to execute.
 * @param thenPart The executed statement if the expression evaluates to true.
 * @param elsePart The executed statement if the expression evaluates to false.
 * @return A pointer to a newly created node.
 */
Statement *newIfStatement(Position position, Expression *condition, Statement *thenPart, Statement *elsePart);

/**
 * Creates a new node representing a while-statement.
 * @param line The position of the statement in the source code.
 * @param condition The expression used to determine whether the while-loop should continue.
 * @param body The statement executed until the condition evaluates to false.
 * @return A pointer to a newly created node.
 */
Statement *newWhileStatement(Position position, Expression *condition, Statement *body);

/**
 * Creates a new node representing a procedure call.
 * @param line The position of the call in the source code.
 * @param name The identifier of the called procedure.
 * @param arguments The list of expressions, whose values will be passed to the procedure.
 * @return A pointer to a newly created node.
 */
Statement *newCallStatement(Position position, Identifier *name, ExpressionList *arguments);

void printStatement(FILE *out, int indentation, Statement *self);

#endif //_STATEMENT_H
