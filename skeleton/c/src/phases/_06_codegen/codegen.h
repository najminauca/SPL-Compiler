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

#endif /* SPL_CODEGEN_H_ */
