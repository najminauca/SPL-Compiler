/*
 * errors.h -- error reporting
 */

#ifndef SPL_ERRORS_H
#define SPL_ERRORS_H

#include <table/identifier.h>
#include <absyn/position.h>

#include <types/types.h>

/**
 * Displays an error to the user and aborts execution.
 *
 * Calling this function will exit the program with an exit code of 1.
 *
 * @param fmt A format string used to display the error message.
 * @param ... Additional parameters used by the format string.
 */
_Noreturn void error(char *fmt, ...);

/**
 * A function used to mark specific code blocks as "not implemented".
 * Calls to this function have to be removed from said code blocks and replaced
 * by correctly working code as part of the different assignments.
 */
_Noreturn void notImplemented();

_Noreturn void syntaxError(Position position, char *token);
_Noreturn void undefinedType(Position position, Identifier *name);
_Noreturn void notAType(Position position, Identifier *name);
_Noreturn void redeclarationAsType(Position position, Identifier *name);
_Noreturn void mustBeAReferenceParameter(Position position, Identifier *name);
_Noreturn void redeclarationAsProcedure(Position position, Identifier *name);
_Noreturn void redeclarationAsParameter(Position position, Identifier *name);
_Noreturn void redeclarationAsVariable(Position position, Identifier *name);
_Noreturn void assignmentHasDifferentTypes(Position position, Type *lhs, Type *rhs);
_Noreturn void assignmentRequiresIntegers(Position position, Type *actual);
_Noreturn void ifConditionMustBeBoolean(Position position, Type *actual);
_Noreturn void whileConditionMustBeBoolean(Position position, Type *actual);
_Noreturn void undefinedProcedure(Position position, Identifier *name);
_Noreturn void callOfNonProcedure(Position position, Identifier *name);
_Noreturn void argumentTypeMismatch(Position position, Identifier *name, int argumentIndex, Type *expected, Type *actual);
_Noreturn void argumentMustBeAVariable(Position position, Identifier *name, int argumentIndex);
_Noreturn void tooFewArguments(Position position, Identifier *name);
_Noreturn void tooManyArguments(Position position, Identifier *name);
_Noreturn void operatorDifferentTypes(Position position, Type *lhs, Type *rhs);
_Noreturn void comparisonNonInteger(Position position, Type *actual);
_Noreturn void arithmeticOperatorNonInteger(Position position, Type *actual);
_Noreturn void undefinedVariable(Position position, Identifier *name);
_Noreturn void notAVariable(Position position, Identifier *name);
_Noreturn void indexingNonArray(Position position);
_Noreturn void indexingWithNonInteger(Position position);
_Noreturn void mainIsMissing();
_Noreturn void mainIsNotAProcedure();
_Noreturn void mainMustNotHaveParameters();
_Noreturn void illegalApostrophe(Position position);
_Noreturn void illegalCharacter(Position position, char character);

_Noreturn void registerOverflow();

#endif //SPL_ERRORS_H
