/*
 * codegen.h -- ECO32 code generator
 */

#ifndef SPL_CODEGEN_H_
#define SPL_CODEGEN_H_

#include <absyn/absyn.h>
#include <table/table.h>
#include <stdio.h>

/**
 * This function is used to generate the assembly code for the compiled program.
 * This code is emitted via the functions provided by codeprint.h .
 *
 * @param program The program for which the assembly code has to be produced.
 * @param globalTable The symbol table for the current program.
 * @param outFile The file pointer where the output has to be emitted to.
 * @param ershovOptimization Whether the ershov register optimization should be used (--ershov).
 */
void genCode(Program *program, SymbolTable *globalTable, FILE *outFile, bool ershovOptimization);
void genProcedure(GlobalDeclaration * glob_dec, SymbolTable * table, FILE * out, int reg);
void genStatement(Statement * statement, SymbolTable * table, FILE * out, int reg);
void genVariable(Variable * variable, SymbolTable * table, FILE * out, int reg);
void genExpression(Expression * expression, SymbolTable * table, FILE * out, int reg, int lab);
void genBinaryExpression(Expression * expression, SymbolTable * table, FILE * out, int reg, int lab);
#endif /* SPL_CODEGEN_H_ */
