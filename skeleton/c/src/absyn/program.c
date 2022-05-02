/*
 * program.c -- abstract syntax for programs
 */

#include <util/memory.h>
#include "program.h"

Program *newProgram(Position position, GlobalDeclarationList *declarations) {
    Program *program = new(Program);
    program->position = position;
    program->declarations = declarations;
    return program;
}

void printProgram(FILE *out, int indentation, Program *program) {
    indent(out, indentation);

    if (program == NULL) {
        fprintf(out, "NULL");
        return;
    }

    fprintf(out, "Program(");
    printGlobalDeclarationList(out, indentation + INDENTATION_INCREMENT, true, program->declarations);
    fprintf(out, ")\n");

    fflush(out);
}
