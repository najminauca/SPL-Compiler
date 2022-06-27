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
Entry *varEntry;
Entry *typeEntry;
Entry *procEntry;

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
    Entry *entry;

    intType = newPrimitiveType("int", INT_BYTE_SIZE);
    boolType = newPrimitiveType("boolean", BOOL_BYTE_SIZE);

    globalTable = initializeGlobalTable();

    //TODO (assignment 4a): Initialize a symbol table with all predefined symbols and fill it with user-defined symbols
    notImplemented();
}

//Check each type expression
Type * checkType(TypeExpression *typeExpression, SymbolTable *table) {
    switch(typeExpression->kind) {
        case TYPEEXPRESSION_NAMEDTYPEEXPRESSION :
            if(lookup(table, typeExpression->u.namedTypeExpression.name) == NULL) {
                undefinedType(typeExpression->position, typeExpression->u.namedTypeExpression.name);
            }
            if(lookup(table,typeExpression->u.namedTypeExpression.name)->kind != ENTRY_KIND_TYPE) {
                notAType(typeExpression->position, typeExpression->u.namedTypeExpression.name);
            }
            return typeExpression->dataType;
        case TYPEEXPRESSION_ARRAYTYPEEXPRESSION :
            return newArrayType(typeExpression->u.arrayTypeExpression.arraySize, checkType(typeExpression->u.arrayTypeExpression.baseType, table));
    }
}

//Check each variable declaration
void checkVariable(VariableDeclaration *variableDeclaration, SymbolTable *table) {
    if(lookup(table, variableDeclaration->name) != NULL) {
        redeclarationAsVariable(variableDeclaration->position, variableDeclaration->name);
    } else if(lookup(table, variableDeclaration->name)->u.varEntry.type->kind != ENTRY_KIND_VAR) {
        notAVariable(variableDeclaration->position, variableDeclaration->name);
    } else {
        varEntry = newVarEntry(checkType(variableDeclaration->typeExpression, table), false);
    }
}

//Check each parameter declaration
ParameterTypeList * checkParam(ParameterDeclarationList *parameterDeclarationList, SymbolTable *table) {
    Entry *param;
    if(parameterDeclarationList->isEmpty) {
        return emptyParameterTypeList();
    } else {
        if(lookup(table, parameterDeclarationList->head->name) != NULL) {
            redeclarationAsParameter(parameterDeclarationList->head->position, parameterDeclarationList->head->name);
        } else if(lookup(table, parameterDeclarationList->head->name)->u.varEntry.type->kind == TYPE_KIND_ARRAY) {
            notAType(parameterDeclarationList->head->position, parameterDeclarationList->head->name);
        } else {
                newVarEntry(checkType(parameterDeclarationList->head->typeExpression, table), parameterDeclarationList->head->isReference);
            }
        }
    }
}

void checkGlobalDec(GlobalDeclaration *glob_dec, SymbolTable *table) {
    switch(glob_dec->kind) {
        case DECLARATION_TYPEDECLARATION :
            if(lookup(table, glob_dec->name) != NULL) {
                //error
                redeclarationAsType(glob_dec->position, glob_dec->name);
            }
            typeEntry = newTypeEntry(checkType(glob_dec->u.typeDeclaration.typeExpression, table));
            break;
        case DECLARATION_PROCEDUREDECLARATION :
            if(lookup(table, glob_dec->name) != NULL) {
                //error
                redeclarationAsProcedure(glob_dec->position, glob_dec->name);
            } else if(lookup(table, glob_dec->name) == NULL) {
                localTable = newTable(table);
                VariableDeclarationList  *variableDeclarationList = glob_dec->u.procedureDeclaration.variables;
                while(!variableDeclarationList->isEmpty) {
                    checkVariable(variableDeclarationList->head, localTable);
                    variableDeclarationList = variableDeclarationList->tail;
                }
                procEntry = newProcEntry(checkParam(glob_dec->u.procedureDeclaration.parameters, localTable), localTable);
            }
    }
}