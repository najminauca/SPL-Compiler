/*
 * entry.h -- entries for the symbol table
 */

#ifndef _ENTRY_H_
#define _ENTRY_H_

#include <table/identifier.h>
#include <phases/_05_varalloc/stack_layout.h>
#include <types/types.h>

/**
 * Used to differentiate between entry kinds.
 */
typedef enum {
    ENTRY_KIND_TYPE,
    ENTRY_KIND_VAR,
    ENTRY_KIND_PROC,
} entry_kind;

typedef struct table SymbolTable;

/**
 * Represents a table entry for a declaration in SPL.
 *
 * There are three kinds of entries, that can be stored in a table.
 *
 * 1. Type Entries, that represent a type-declaration and store a type
 *
 * 2. Variable Entries, that represent variable and parameter-declarations.
 * Please note that parameters of a procedure will also need to be entered as VariableEntries, there is no separate ParameterEntry.
 * Parameters also have an associated ParameterType, that needs to be added to the associated procedure's ProcedureEntry.
 * A Variable Entry stores a type and whether the variable is a reference.
 *
 * 3. Procedure Entries, that represent a procedure-declaration.
 * A Procedure Entry stores a list of types for the procedures parameters and the procedures local table.
 *
 */
typedef struct {
    entry_kind kind;
    union {
        struct {
            Type *type;
        } typeEntry;
        struct {
            Type *type;
            bool isRef;
            int offset;        /* filled in by variable allocator */
        } varEntry;
        struct {
            ParameterTypeList *parameterTypes;
            SymbolTable *localTable;
            StackLayout *stackLayout;
        } procEntry;
    } u;
} Entry;

/**
 * Creates a new Type Entry representing a type-declaration in SPL.
 * @param type The "meaning" of the type-declaration.
 * It is determined by the type of the type expression on the right hand side of the declaration.
 * @return A pointer to a newly created Entry in memory.
 */
Entry *newTypeEntry(Type *type);

/**
 * Creates a new Variable Entry representing a variable-declaration in SPL.
 * @param type The semantic type of the variable. It is determined through the type of the declaration.
 * @param isRef Whether or not the variable is declared as a reference.
 * @return A pointer to a newly created Entry in memory.
 */
Entry *newVarEntry(Type *type, bool isRef);

/**
 * Creates a new Procedure Entry representing a procedure-declaration in SPL.
 * @param parameterTypes A list of types representing the procedures parameters.
 * @param localTable The local table for the declared procedure.
 * @return A pointer to a newly created Entry in memory.
 */
Entry *newProcEntry(ParameterTypeList *parameterTypes, SymbolTable *localTable);

/**
 * This function is used to create procedure entries that already contain a value for the argument area size.
 * This is used for predefined functions where no variable allocations takes place.
 *
 * @param parameterTypes A list of types for the procedures parameters.
 * @param argumentAreaSize The size in bytes required on the stack to hold all of this procedures parameters.
 * @return A pointer to a newly created Entry for a predefined procedure.
 */
Entry *newPredefinedProcEntry(ParameterTypeList *parameterTypes, int argumentAreaSize);

#endif