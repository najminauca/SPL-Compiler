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

SymbolTable *globalTable;
SymbolTable *localTable;
bool showBool;

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

//Check each type expression
Type * checkType(TypeExpression *typeExpression, SymbolTable *table) {
    Position position;
    Identifier * name;
    Entry * typeEntry;
    switch(typeExpression->kind) {
        case TYPEEXPRESSION_NAMEDTYPEEXPRESSION :
            position = typeExpression->position;
            name = typeExpression->u.namedTypeExpression.name;
            typeEntry = lookup(table, name);
            if(typeEntry == NULL) {
                undefinedType(position, name);
            }
            if(typeEntry->kind != ENTRY_KIND_TYPE) {
                notAType(position, name);
            }
            return typeEntry->u.typeEntry.type;
        case TYPEEXPRESSION_ARRAYTYPEEXPRESSION :
            return newArrayType(typeExpression->u.arrayTypeExpression.arraySize, checkType(typeExpression->u.arrayTypeExpression.baseType, table));
    }
}

//Check each variable declaration
void checkVariable(VariableDeclaration *variableDeclaration, SymbolTable *table) {
    Position position = variableDeclaration->position;
    Identifier * name = variableDeclaration->name;

    Entry * varEntry = lookup(table, name);
    if(varEntry != NULL && varEntry->u.varEntry.type->kind == ENTRY_KIND_VAR) {
        redeclarationAsVariable(position, name);
    }

    varEntry = newVarEntry(checkType(variableDeclaration->typeExpression, table), false);
    Entry * enterVar = enter(table, name, varEntry);
    if(enterVar == NULL) {
        redeclarationAsParameter(position, name);
    }
}

//Check each parameter declaration
ParameterTypeList * checkParam(ParameterDeclarationList *parameterDeclarationList, SymbolTable *table) {
    if(parameterDeclarationList->isEmpty) {
        return emptyParameterTypeList();
    }

    Position position = parameterDeclarationList->head->position;
    Identifier * name = parameterDeclarationList->head->name;
    if(lookup(table, name) != NULL) {
        redeclarationAsParameter(position, name);
    }

    Entry * param = newVarEntry(checkType(parameterDeclarationList->head->typeExpression, table), parameterDeclarationList->head->isReference);
    Entry * enterParam = enter(table, name, param);
    if(enterParam == NULL) {
        redeclarationAsParameter(position, name);
    }
    return newParameterTypeList(newParameterType(param->u.varEntry.type, param->u.varEntry.isRef), checkParam(parameterDeclarationList->tail, table));
}

void checkGlobalDec(GlobalDeclaration *glob_dec, SymbolTable *table) {
    Position position = glob_dec->position;
    Identifier * name = glob_dec->name;
    switch(glob_dec->kind) {
        case DECLARATION_TYPEDECLARATION :
            if(lookup(table, name) != NULL) {
                //type redeclaration
                redeclarationAsType(position, name);
            }

            Entry * typeEntry = newTypeEntry(checkType(glob_dec->u.typeDeclaration.typeExpression, table));
            Entry * enterType = enter(table, name, typeEntry);
            if(enterType == NULL) {
                redeclarationAsType(position, name);
            }
            break;
        case DECLARATION_PROCEDUREDECLARATION :
            if(lookup(table, name) != NULL) {
                //procedure redeclaration
                redeclarationAsProcedure(position, name);
            }

            localTable = newTable(table);
            VariableDeclarationList  *variableDeclarationList = glob_dec->u.procedureDeclaration.variables;
            while(!variableDeclarationList->isEmpty) {
                checkVariable(variableDeclarationList->head, localTable);
                variableDeclarationList = variableDeclarationList->tail;
            }

            Entry * procEntry = newProcEntry(checkParam(glob_dec->u.procedureDeclaration.parameters, localTable), localTable);
            Entry * enterProc = enter(table, name, procEntry);
            if(enterProc == NULL) {
                redeclarationAsProcedure(position, name);
            }
            if(showBool) printSymbolTableAtEndOfProcedure(glob_dec->name, procEntry);
    }
}

SymbolTable *buildSymbolTable(Program *program, bool showSymbolTables) {
    intType = newPrimitiveType("int", INT_BYTE_SIZE);
    boolType = newPrimitiveType("boolean", BOOL_BYTE_SIZE);
    showBool = showSymbolTables;
    globalTable = initializeGlobalTable();

    GlobalDeclarationList * globalDeclarationList = program->declarations;
    while(globalDeclarationList->head != NULL) {
        checkGlobalDec(globalDeclarationList->head, globalTable);
        globalDeclarationList = globalDeclarationList->tail;
    }

    Entry * main = lookup(globalTable, newIdentifier("main"));
    if(main == NULL) {
        mainIsMissing();
    } else if(main->kind != ENTRY_KIND_PROC) {
        mainIsNotAProcedure();
    } else if(!main->u.procEntry.parameterTypes->isEmpty) {
        mainMustNotHaveParameters();
    }
    return globalTable;
    //TODO (assignment 4a): Initialize a symbol table with all predefined symbols and fill it with user-defined symbols
}