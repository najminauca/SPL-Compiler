/*
 * procedurebodycheck.h -- semantic analysis
 */

#ifndef SPL_PROCEDUREBODYCHECK_H_
#define SPL_PROCEDUREBODYCHECK_H_

#include <absyn/absyn.h>
#include <table/table.h>

/**
 * This function is used to check if the currently compiled SPL program is semantically valid.
 * Every statement and expression has to be checked, to ensure that every type is correct.
 *
 * Types calculated in this function can be stored in the type field of Expressions, Variables and TypeExpressions.
 *
 * @param program The program to be checked for semantic correctness.
 * @param globalTable The symbol table for the current program.
 */
void checkProcedures(Program *program, SymbolTable *globalTable);

#endif /* SPL_PROCEDUREBODYCHECK_H_ */
