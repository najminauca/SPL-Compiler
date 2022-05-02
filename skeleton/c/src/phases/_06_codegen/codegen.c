/*
 * codegen.c -- ECO32 code generator
 */

#include "codegen.h"

#include <util/errors.h>
#include <assert.h>
#include "codeprint.h"

#define FIRST_REGISTER 8
#define LAST_REGISTER 23

/**
 * Emits needed import statements, to allow usage of the predefined functions and sets the correct settings
 * for the assembler.
 * @param out The file pointer where the output has to be emitted to.
 */
void assemblerProlog(FILE *out) {
    emitImport(out, "printi");
    emitImport(out, "printc");
    emitImport(out, "readi");
    emitImport(out, "readc");
    emitImport(out, "exit");
    emitImport(out, "time");
    emitImport(out, "clearAll");
    emitImport(out, "setPixel");
    emitImport(out, "drawLine");
    emitImport(out, "drawCircle");
    emitImport(out, "_indexError");

    emit(out, "");
    emit(out, "\t.code");
    emit(out, "\t.align\t4");
}

void genCode(Program *program, SymbolTable *globalTable, FILE *out, bool ershovOptimization) {
    assemblerProlog(out);

    //TODO (assignment 6): generate eco32 assembler code for the spl program

    notImplemented();
}
