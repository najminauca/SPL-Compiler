/*
 * procedurebodycheck.c -- semantic analysis
 */

#include <string.h>
#include <assert.h>
#include <util/errors.h>
#include <absyn/absyn.h>
#include <types/types.h>
#include <table/table.h>
#include "procedurebodycheck.h"

void checkWhile(Statement * statement, SymbolTable * table) {
    Type * condition = checkExpression(statement->u.whileStatement.condition, table);
    if(condition != boolType) {
        whileConditionMustBeBoolean(statement->position, condition);
    }
    checkEachStatement(statement->u.whileStatement.body, table);
}

void checkCall(Statement * statement, SymbolTable * table) {
    Entry * procedure = lookup(table, statement->u.callStatement.procedureName);

    if(procedure == NULL) {
        undefinedProcedure(statement->position, statement->u.callStatement.procedureName);
    }
    if(procedure->kind != ENTRY_KIND_PROC) {
        callOfNonProcedure(statement->position, statement->u.callStatement.procedureName);
    }
    int count = 0;
    ParameterTypeList * parameters = procedure->u.procEntry.parameterTypes;
    ExpressionList * args = statement->u.callStatement.arguments;
    while(!parameters->isEmpty) {
        if(args->isEmpty) {
            tooFewArguments(args->head->position, statement->u.callStatement.procedureName);
        }
        count = count + 1;
        Type * expected = parameters->head->type;
        Type * actual = checkExpression(args->head, table);
        if(expected != actual) {
            argumentTypeMismatch(args->head->position, statement->u.callStatement.procedureName, 1, expected, actual);
        }
        if(parameters->head->isRef && args->head->kind != EXPRESSION_VARIABLEEXPRESSION) {
            argumentMustBeAVariable(args->head->position, statement->u.callStatement.procedureName, count);
        }
        parameters = parameters->tail;
        args = args->tail;
    }
    if(!args->isEmpty) {
        tooManyArguments(args->head->position, statement->u.callStatement.procedureName);
    }
}

void checkAssign(Statement * statement, SymbolTable * table) {
    Variable * target = statement->u.assignStatement.target;
    Expression * value = statement->u.assignStatement.value;
    Type * varType = checkVarExp(target, table);
    Type * expType = checkExpression(value, table);
    if(varType != expType) {
        assignmentHasDifferentTypes(statement->position, varType, expType);
    }
    if(expType != intType) {
        assignmentRequiresIntegers(statement->position, expType);
    }
}

void checkIf(Statement * statement, SymbolTable * table) {
    Type * condition = checkExpression(statement->u.ifStatement.condition, table);
    if(condition != boolType) {
        ifConditionMustBeBoolean(statement->position, condition);
    }
    checkEachStatement(statement->u.ifStatement.thenPart, table);
    checkEachStatement(statement->u.ifStatement.elsePart, table);
}

Type * checkVarExp(Variable * variable, SymbolTable * table) {
    Entry * name;
    Type * arr;
    if(variable->kind == VARIABLE_ARRAYACCESS) {
        arr = checkVarExp(variable->u.arrayAccess.array, table);
        if (arr->kind != TYPE_KIND_ARRAY) {
            indexingNonArray(variable->position);
        }
        Type *varType = checkExpression(variable->u.arrayAccess.index, table);
        if (varType != intType) {
            indexingWithNonInteger(variable->position);
        }
        variable->dataType = arr->u.arrayType.baseType; //save datatype to variable
        return arr->u.arrayType.baseType;
    }
    else if(variable->kind == VARIABLE_NAMEDVARIABLE) {
        name = lookup(table, variable->u.namedVariable.name);
        if (name == NULL) {
            undefinedVariable(variable->position, variable->u.namedVariable.name);
        }
        if (name->kind != ENTRY_KIND_VAR) {
            notAVariable(variable->position, variable->u.namedVariable.name);
        }
        variable->dataType = name->u.varEntry.type; //save datatype to variable
        return name->u.varEntry.type;
    }
    return NULL;
}

Type * checkOperand(Expression * expression, SymbolTable * table) {
    Type * left = checkExpression(expression->u.binaryExpression.leftOperand, table);
    Type * right = checkExpression(expression->u.binaryExpression.rightOperand, table);
    if(left != right) {
        operatorDifferentTypes(expression->position, left, right);
    }
    int op = expression->u.binaryExpression.operator;
    if(op == ABSYN_OP_ADD || op == ABSYN_OP_SUB || op == ABSYN_OP_MUL || op == ABSYN_OP_DIV) {
        if (left != intType || right != intType) {
            arithmeticOperatorNonInteger(expression->position, left);
        }
        return left;
    }
    else if(op == ABSYN_OP_EQU || op == ABSYN_OP_NEQ || op == ABSYN_OP_GRE || op == ABSYN_OP_GRT || op == ABSYN_OP_LSE || op == ABSYN_OP_LST) {
        if(left != intType || right != intType) {
            comparisonNonInteger(expression->position, left);
        }
        return boolType;
    }
    return NULL;
}

Type * checkExpression(Expression * expression, SymbolTable * table) {
    if(expression->kind == EXPRESSION_BINARYEXPRESSION) return checkOperand(expression, table);
    else if(expression->kind == EXPRESSION_INTLITERAL) return intType;
    else if(expression->kind == EXPRESSION_VARIABLEEXPRESSION) return checkVarExp(expression->u.variableExpression.variable, table);
}

void checkEachStatement(Statement * statement, SymbolTable * table) {
    if(statement->kind == STATEMENT_ASSIGNSTATEMENT) checkAssign(statement, table);
    else if(statement->kind == STATEMENT_CALLSTATEMENT) checkCall(statement, table);
    else if(statement->kind == STATEMENT_COMPOUNDSTATEMENT) checkCompound(statement->u.compoundStatement.statements, table);
    else if(statement->kind == STATEMENT_EMPTYSTATEMENT);
    else if(statement->kind == STATEMENT_IFSTATEMENT) checkIf(statement, table);
    else if(statement->kind == STATEMENT_WHILESTATEMENT) checkWhile(statement, table);
}

void checkCompound(StatementList * statementList, SymbolTable * table) {
    StatementList * statements = statementList;
    while(!statements->isEmpty) {
        checkEachStatement(statements->head, table);
        statements = statements->tail;
    }
}

void checkProcedures(Program *program, SymbolTable *globalTable) {
    //TODO (assignment 4b): Check all procedure bodies for semantic errors
    GlobalDeclarationList * globalDeclarationList = program->declarations;
    while(!globalDeclarationList->isEmpty) {
        if(globalDeclarationList->head->kind == DECLARATION_PROCEDUREDECLARATION) {
            Entry * proc = lookup(globalTable, globalDeclarationList->head->name);
            checkCompound(globalDeclarationList->head->u.procedureDeclaration.body, proc->u.procEntry.localTable);
        }
        globalDeclarationList = globalDeclarationList->tail;
    }
}
