/*
 * codegen.c -- ECO32 code generator
 */

#include "codegen.h"

#include <util/errors.h>
#include <assert.h>
#include "codeprint.h"
#include <phases/_05_varalloc/stack_layout.h>

#define FIRST_REGISTER 8
#define LAST_REGISTER 23

int label = 0;
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
    GlobalDeclarationList * globalDeclarationList = program->declarations;
    while(!globalDeclarationList->isEmpty) {
        if(globalDeclarationList->head->kind == DECLARATION_PROCEDUREDECLARATION) {
            genProcedure(globalDeclarationList->head, globalTable, out, FIRST_REGISTER);
        }
        globalDeclarationList = globalDeclarationList->tail;
    }
    //TODO (assignment 6): generate eco32 assembler code for the spl program
}

void genProcedure(GlobalDeclaration * glob_dec, SymbolTable * table, FILE * out, int reg) {
    Entry * proc = lookup(table, glob_dec->name);
    int offsetOldFp = getOldFramePointerOffSet(proc->u.procEntry.stackLayout);
    int framesize = getFrameSize(proc->u.procEntry.stackLayout);
    //Prolog
    emit(out, "\t.export %s", glob_dec->name->string);
    emitLabel(out, glob_dec->name->string);
    commentRRI(out, "sub", 29, 29, framesize, "sp <- sp - frame size");
    commentRRI(out, "stw", 25, 29, offsetOldFp, "fp alt speichern relativ zu sp");
    commentRRI(out, "add", 25, 29, framesize, "fp neu <- sp neu + framesize");
    if(!isLeafProcedure(proc->u.procEntry.stackLayout)) {
        commentRRI(out, "stw", 31, 25, getOldReturnAddressOffset(proc->u.procEntry.stackLayout), "return adr speichern relativ zu fp");
    }
    StatementList * body = glob_dec->u.procedureDeclaration.body;
    while(!body->isEmpty) {
        genStatement(body->head, proc->u.procEntry.localTable, out, reg);
        body = body->tail;
    }
    //Epilog
    if(!isLeafProcedure(proc->u.procEntry.stackLayout)) {
        commentRRI(out, "ldw", 31, 25, getOldReturnAddressOffset(proc->u.procEntry.stackLayout), "wiederherstellen return");
    }
    commentRRI(out, "ldw", 31, 29, offsetOldFp, "wiederherstellen fp");
    commentRRI(out, "add", 29, 29, framesize, "release frame (sp <- sp + framesize)");
    commentR(out, "jr", 31, "return");
}

void genIntLiteral(Expression * expression, SymbolTable * table, FILE * out, int reg) {
    emitRRI(out, "add", reg, 0, expression->u.intLiteral.value);
}

void genBinaryExpressionArith(Expression * expression, SymbolTable * table, FILE * out, int reg, int lab) {
    genExpression(expression->u.binaryExpression.leftOperand, table, out, reg, lab);
    int right = reg + 1;
    if(right > LAST_REGISTER) {
        registerOverflow();
    }
    genExpression(expression->u.binaryExpression.rightOperand, table, out, right, lab);
    switch(expression->u.binaryExpression.operator) {
        case ABSYN_OP_ADD :
            emitRRR(out, "add", reg, reg, right);
            break;
        case ABSYN_OP_SUB :
            emitRRR(out, "sub", reg, reg, right);
            break;
        case ABSYN_OP_MUL :
            emitRRR(out, "mul", reg, reg, right);
            break;
        case ABSYN_OP_DIV :
            emitRRR(out, "div", reg, reg, right);
            break;
        default :
            genBinaryExpressionComp(expression, table, out, reg, lab);
    }
}

void genNamedVariable(Variable * variable, SymbolTable * table, FILE * out, int reg) {
    Entry * var = lookup(table, variable->u.namedVariable.name);
    emitRRI(out, "add", reg, 25, var->u.varEntry.offset);
    if(var->u.varEntry.isRef) {
        emitRRI(out, "ldw", reg, reg, 0);
    }
}

void genVariable(Variable * variable, SymbolTable * table, FILE * out, int reg) {
    Entry * var;
    switch(variable->kind) {
        case VARIABLE_NAMEDVARIABLE :
            genNamedVariable(variable, table, out, reg);
            break;
        case VARIABLE_ARRAYACCESS : ;
            genArrayAccess(variable, table, out, reg);
    }
}

void genExpression(Expression * expression, SymbolTable * table, FILE * out, int reg, int lab) {
    switch(expression->kind) {
        case EXPRESSION_INTLITERAL :
            emitRRI(out, "add", reg, 0, expression->u.intLiteral.value);
            break;
        case EXPRESSION_VARIABLEEXPRESSION :
            genVariable(expression->u.variableExpression.variable, table, out, reg);
            emitRRI(out, "ldw", reg, reg, 0);
            break;
        case EXPRESSION_BINARYEXPRESSION :
            genBinaryExpressionArith(expression, table, out, reg, lab);
    }
}

void genAssign(Statement * statement, SymbolTable * table, FILE * out) {
    //add $8, $25, offset
    genVariable(statement->u.assignStatement.target, table, out, 8);
    //add $9, $0, value
    genExpression(statement->u.assignStatement.value, table, out, 9, 0);
    //stw $9, $8, 0
    emitRRI(out, "stw", 9, 8, 0);
}

void genArrayAccess(Variable * variable, SymbolTable * table, FILE * out, int reg) {
    int temp = reg;
    genVariable(variable->u.arrayAccess.array, table, out, reg);
    reg++;
    genExpression(variable->u.arrayAccess.index, table, out, reg, 0);   //no label
    reg++;
    emitRRI(out, "add", reg, 0, variable->dataType->byteSize);
    emitRRL(out, "bgeu", reg - 1, reg, "index error");
    emitRRI(out, "mul", reg - 1, reg - 1, variable->dataType->byteSize);
    emitRRR(out, "add", temp, temp, reg - 1);
}

void genBinaryExpressionComp(Expression * expression, SymbolTable * table, FILE * out, int reg, int lab) {
    switch(expression->u.binaryExpression.operator) {
        case ABSYN_OP_EQU :
            emitRRL(out, "bne", reg, reg + 1, "L%d", lab);
            break;
        case ABSYN_OP_NEQ :
            emitRRL(out, "beq", reg, reg + 1, "L%d", lab);
            break;
        case ABSYN_OP_GRT :
            emitRRL(out, "ble", reg, reg + 1, "L%d", lab);
            break;
        case ABSYN_OP_GRE :
            emitRRL(out, "blt", reg, reg + 1, "L%d", lab);
            break;
        case ABSYN_OP_LST :
            emitRRL(out, "bge", reg, reg + 1, "L%d", lab);
            break;
        case ABSYN_OP_LSE :
            emitRRL(out, "bgt", reg, reg + 1, "L%d", lab);
    }
}

void genWhile(Statement * statement, SymbolTable * table, FILE * out, int reg) {
    label++;
    int loopL = label;
    label++;
    int falseL = label;
    emitLabel(out, "L%d", loopL); //L0:
    genExpression(statement->u.whileStatement.condition, table, out, reg, label);
    genStatement(statement->u.whileStatement.body, table, out, reg);
    emitJump(out, "L%d", loopL); //jump L0
    emitLabel(out, "L%d", falseL); //L1:
}

void genIf(Statement * statement, SymbolTable * table, FILE * out, int reg) {
    label++;
    int trueL = label;
    int falseL;
    genExpression(statement->u.ifStatement.condition, table, out, reg, label);
    genStatement(statement->u.ifStatement.thenPart, table, out, reg);
    if(statement->u.ifStatement.elsePart->kind != STATEMENT_EMPTYSTATEMENT) {
        label++;
        falseL = label;
        emitJump(out, "L%d", falseL); //jump L1
    }
    emitLabel(out, "L%d", trueL);    //L0:
    if(statement->u.ifStatement.elsePart->kind != STATEMENT_EMPTYSTATEMENT) {
        genStatement(statement->u.ifStatement.elsePart, table, out, reg);
        emitJump(out, "L%d", trueL); //L1:
    }
}

void genCall(Statement * statement, SymbolTable * table, FILE * out) {
    Entry * call = lookup(table, statement->u.callStatement.procedureName);
    ExpressionList * args = statement->u.callStatement.arguments;
    ParameterTypeList * params = call->u.procEntry.parameterTypes;
    int count = 0;
    while(!args->isEmpty) {
        if(params->head->isRef) {
            genVariable(args->head->u.variableExpression.variable, table, out, 8);
        } else {
            genExpression(args->head, table, out, 8, 0);
        }
        commentRRI(out, "stw", 8, 29, params->head->offset, "Save arg %d", count);
        count++;
        args = args->tail;
        params = params->tail;
    }
}

void genStatement(Statement * statement, SymbolTable * table, FILE * out, int reg) {
    switch(statement->kind) {
        case STATEMENT_ASSIGNSTATEMENT :
            genAssign(statement, table, out);
            break;
        case STATEMENT_WHILESTATEMENT :
            genWhile(statement, table, out, reg);
            break;
        case STATEMENT_IFSTATEMENT :
            genIf(statement, table, out, reg);
            break;
        case STATEMENT_CALLSTATEMENT :
            genCall(statement, table, out);
    }
}

