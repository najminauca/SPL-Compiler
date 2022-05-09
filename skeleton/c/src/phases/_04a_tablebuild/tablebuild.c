/*
 * tablebuild.c -- symbol table creation
 */

#include "tablebuild.h"

#include <types/types.h>
#include <absyn/absyn.h>
#include <table/table.h>
#include <stdio.h>
#include <util/errors.h>

#define BOOL_BYTE_SIZE   4    /* size of a bool in bytes */
#define INT_BYTE_SIZE    4    /* size of an int in bytes */

/**
 * Prints the local symbol table of a procedure together with a heading-line
 * NOTE: You have to call this after completing the local table to support '--tables'.
 * @param name           The name of the procedure
 * @param procedureEntry The entry of the procedure to print
 */
static void printSymbolTableAtEndOfProcedure(Identifier *name, Entry *procedureEntry) {
    printf("Symbol table at end of procedure '%s':\n", name->string);
    showTable(procedureEntry->u.procEntry.localTable);
    printf("\n");
}

SymbolTable *buildSymbolTable(Program *program, bool showSymbolTables) {
    intType = newPrimitiveType("int", INT_BYTE_SIZE);
    boolType = newPrimitiveType("boolean", BOOL_BYTE_SIZE);

    //TODO (assignment 4a): Initialize a symbol table with all predefined symbols and fill it with user-defined symbols
    notImplemented();
}
