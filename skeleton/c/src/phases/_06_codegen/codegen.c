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

static int label = 0;
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
    emit(out, "\t.export %s", glob_dec->name->string);  //Export before procedure
    emitLabel(out, glob_dec->name->string);
    commentRRI(out, "sub", 29, 29, framesize, "assemble frame, SP <- SP - frame size");
    commentRRI(out, "stw", 25, 29, offsetOldFp, "store old FP relative to SP");
    commentRRI(out, "add", 25, 29, framesize, "new FP <- new SP + frame size");
    if(!isLeafProcedure(proc->u.procEntry.stackLayout)) {
        commentRRI(out, "stw", 31, 25, getOldReturnAddressOffset(proc->u.procEntry.stackLayout), "store return address relative to FP");
    }
    StatementList * body = glob_dec->u.procedureDeclaration.body;
    while(!body->isEmpty) {
        genStatement(body->head, proc->u.procEntry.localTable, out, reg);
        body = body->tail;
    }
    if(!isLeafProcedure(proc->u.procEntry.stackLayout)) {
        commentRRI(out, "ldw", 31, 25, getOldReturnAddressOffset(proc->u.procEntry.stackLayout), "get return address");
    }
    commentRRI(out, "ldw", 31, 29, offsetOldFp, "get old FP");
    commentRRI(out, "add", 29, 29, framesize, "release frame (SP <- SP + frame size)");
    commentR(out, "jr", 31, "return");
}

void genBinaryExpressionArith(Expression * expression, SymbolTable * table, FILE * out, int reg, int lab) {
    genExpression(expression->u.binaryExpression.leftOperand, table, out, reg, lab);    //left operand
    int right = reg + 1;
    if(right > LAST_REGISTER) {
        registerOverflow();
    }
    genExpression(expression->u.binaryExpression.rightOperand, table, out, right, lab); //right operand
    switch(expression->u.binaryExpression.operator) {   //operation type
        case ABSYN_OP_ADD :
            commentRRR(out, "add", reg, reg, right, "add operation register %d + register %d", reg, right);
            break;
        case ABSYN_OP_SUB :
            commentRRR(out, "sub", reg, reg, right, "sub operation register %d - register %d", reg, right);
            break;
        case ABSYN_OP_MUL :
            commentRRR(out, "mul", reg, reg, right, "mul operation register %d * register %d", reg, right);
            break;
        case ABSYN_OP_DIV :
            commentRRR(out, "div", reg, reg, right, "mul operation register %d / register %d", reg, right);
            break;
        default :
            genBinaryExpressionComp(expression, table, out, reg, lab);
    }
}

void genVariable(Variable * variable, SymbolTable * table, FILE * out, int reg) {
    Entry * var;
    switch(variable->kind) {
        case VARIABLE_NAMEDVARIABLE :
            var = lookup(table, variable->u.namedVariable.name);
            commentRRI(out, "add", reg, 25, var->u.varEntry.offset, "save variable offset relative to FP into register %d", reg);
            if(var->u.varEntry.isRef) {
                commentRRI(out, "ldw", reg, reg, 0, "load reference variable value into register %d", reg);
            }
            break;
        case VARIABLE_ARRAYACCESS : ;
            genArrayAccess(variable, table, out, reg);
    }
}

void genExpression(Expression * expression, SymbolTable * table, FILE * out, int reg, int lab) {
    switch(expression->kind) {
        case EXPRESSION_INTLITERAL :
            commentRRI(out, "add", reg, 0, expression->u.intLiteral.value, "save intlit value to register %d", reg);
            break;
        case EXPRESSION_VARIABLEEXPRESSION :
            genVariable(expression->u.variableExpression.variable, table, out, reg);
            commentRRI(out, "ldw", reg, reg, 0, "load variable expression value to register %d", reg);
            break;
        case EXPRESSION_BINARYEXPRESSION :
            genBinaryExpressionArith(expression, table, out, reg, lab);
    }
}

void genAssign(Statement * statement, SymbolTable * table, FILE * out) {
    //target
    genVariable(statement->u.assignStatement.target, table, out, 8);
    //value
    genExpression(statement->u.assignStatement.value, table, out, 9, 0);
    //stw $9, $8, 0
    commentRRI(out, "stw", 9, 8, 0, "store register 8 value to register 9");
}

void genArrayAccess(Variable * variable, SymbolTable * table, FILE * out, int reg) {
    int temp = reg;
    genVariable(variable->u.arrayAccess.array, table, out, reg);
    reg += 1;
    genExpression(variable->u.arrayAccess.index, table, out, reg, 0);   //no label
    reg += 1;
    commentRRI(out, "add", reg, 0, variable->dataType->byteSize, "save component byte size into register %d", reg);
    emitRRL(out, "bgeu", reg - 1, reg, "_indexError");
    commentRRI(out, "mul", reg - 1, reg - 1, variable->dataType->byteSize, "multiply index with component byte size");
    commentRRR(out, "add", temp, temp, reg - 1, "save offset for index into register %d", temp);
}

void genBinaryExpressionComp(Expression * expression, SymbolTable * table, FILE * out, int reg, int lab) {
    switch(expression->u.binaryExpression.operator) {   //operation type
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
    label += 1;
    int loopL = label;
    label += 1;
    int falseL = label;
    emitLabel(out, "L%d", loopL); //L0:
    genExpression(statement->u.whileStatement.condition, table, out, reg, label);
    genStatement(statement->u.whileStatement.body, table, out, reg);
    emitJump(out, "L%d", loopL); //jump L0
    emitLabel(out, "L%d", falseL); //L1:
}

void genIf(Statement * statement, SymbolTable * table, FILE * out, int reg) {
    label += 1;
    int trueL = label;
    int falseL;
    genExpression(statement->u.ifStatement.condition, table, out, reg, label);
    genStatement(statement->u.ifStatement.thenPart, table, out, reg);
    if(statement->u.ifStatement.elsePart->kind != STATEMENT_EMPTYSTATEMENT) {
        label += 1;
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
        commentRRI(out, "stw", 8, 29, params->head->offset, "store argument offset relative to SP into register 8");
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

