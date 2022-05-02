/*
 * table.h -- symbol table
 */

#ifndef _TABLE_H_
#define _TABLE_H_

#include <types/types.h>
#include <table/identifier.h>
#include "entry.h"


/**
 * A binary tree structure that aids in the implementation of the symbol table
 */
typedef struct bintree {
    Identifier *key;
    Entry *entry;
    struct bintree *left;
    struct bintree *right;
} Bintree;

/**
 * Represents a symbol table for a definition scope in SPL.
 * Every table maps identifiers to the corresponding symbols.
 */
typedef struct table {
    Bintree *bintree;
    struct table *upperLevel;
} SymbolTable;


/**
 * Constructs a local table representing a local definition scope.
 * @param upperLevel The symbol table for the surrounding scope.
 * @return A pointer to a newly constructed table in memory.
 */
SymbolTable *newTable(SymbolTable *upperLevel);

/**
 * Creates a new table and enters entries for all predefined types and procedures.
 * @return The reference to the symbol table representing the global definition scope.
 */
SymbolTable *initializeGlobalTable(void);

/**
 * Inserts a new symbol into a symbol table.
 * Returns NULL if a symbol with this name already exists in this scope.
 * @param table The table, where the symbol should be added.
 * @param name The name of the symbol that is entered.
 * @param entry The entry of the symbol.
 * @return The given entry or NULL if a symbol with the same name already exists in this scope.
 */
Entry *enter(SymbolTable *table, Identifier *name, Entry *entry);

/**
 * Looks for a symbol defined with the given name.
 * Recursively looks in outer scopes if the name is not defined in this scope.
 * @param table The symbol table of the current scope.
 * @param name The name of the symbol to lookup.
 * @return NULL if the symbol was not found, the corresponding entry otherwise.
 */
Entry *lookup(SymbolTable *table, Identifier *name);

void showEntry(Entry *entry);

/**
 * Converts a table into a human readable formatInt and prints it.
 * @param table The table to print.
 */
void showTable(SymbolTable *table);

#endif /* _TABLE_H_ */
