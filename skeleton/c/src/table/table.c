/*
 * table.c -- symbol table
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <util/memory.h>
#include <util/errors.h>
#include <stdarg.h>
#include "table.h"

SymbolTable *newTable(SymbolTable *upperLevel) {
    SymbolTable *table = new(SymbolTable);

    table->bintree = NULL;
    table->upperLevel = upperLevel;
    return table;
}

static void enterPredefinedTypes(SymbolTable *table) {
    enter(table, newIdentifier("int"), newTypeEntry(intType));
}

static ParameterTypeList *paramList(int count, ...) {
    va_list ap;

    ParameterTypeList *list = emptyParameterTypeList();

    va_start(ap, count);
    for (int i = 0; i < count; ++i) {
        ParameterType *pt = va_arg(ap, ParameterType*);
        list = newParameterTypeList(pt, list);
    }
    va_end(ap);

    ParameterTypeList *reversedList = emptyParameterTypeList();
    for (int i = 0; i < count; ++i) {
        reversedList = newParameterTypeList(list->head, reversedList);
        list = list->tail;
    }

    return reversedList;
}

static void enterPredefinedProcs(SymbolTable *table) {
    /* printi(i: int) */
    enter(table, newIdentifier("printi"), newPredefinedProcEntry(paramList(1, newPredefinedParameterType(intType, false, 0)), 4));

    /* printc(i: int) */
    enter(table, newIdentifier("printc"), newPredefinedProcEntry(paramList(1, newPredefinedParameterType(intType, false, 0)), 4));

    /* readi(ref i: int) */
    enter(table, newIdentifier("readi"), newPredefinedProcEntry(paramList(1, newPredefinedParameterType(intType, true, 0)), 4));

    /* readc(ref i: int) */
    enter(table, newIdentifier("readc"), newPredefinedProcEntry(paramList(1, newPredefinedParameterType(intType, true, 0)), 4));

    /* exit() */
    enter(table, newIdentifier("exit"), newPredefinedProcEntry(emptyParameterTypeList(), 0));

    /* time(ref i: int) */
    enter(table, newIdentifier("time"), newPredefinedProcEntry(paramList(1, newPredefinedParameterType(intType, true, 0)), 4));

    /* clearAll(color: int) */
    enter(table, newIdentifier("clearAll"), newPredefinedProcEntry(paramList(1, newPredefinedParameterType(intType, false, 0)), 4));

    /* setPixel(x: int, y: int, color: int) */
    enter(table, newIdentifier("setPixel"), newPredefinedProcEntry(paramList(3,
                                                                             newPredefinedParameterType(intType, false, 0),
                                                                             newPredefinedParameterType(intType, false, 4),
                                                                             newPredefinedParameterType(intType, false, 8)), 12));

    /* drawLine(x1: int, y1: int, x2: int, y2: int, color: int) */
    enter(table, newIdentifier("drawLine"), newPredefinedProcEntry(paramList(5,
                                                                             newPredefinedParameterType(intType, false, 0),
                                                                             newPredefinedParameterType(intType, false, 4),
                                                                             newPredefinedParameterType(intType, false, 8),
                                                                             newPredefinedParameterType(intType, false, 12),
                                                                             newPredefinedParameterType(intType, false, 16)), 20));

    /* drawCircle(x0: int, y0: int, radius: int, color: int) */
    enter(table, newIdentifier("drawCircle"), newPredefinedProcEntry(paramList(4,
                                                                               newPredefinedParameterType(intType, false, 0),
                                                                               newPredefinedParameterType(intType, false, 4),
                                                                               newPredefinedParameterType(intType, false, 8),
                                                                               newPredefinedParameterType(intType, false, 12)), 16));
}

SymbolTable *initializeGlobalTable(void) {
    SymbolTable *table = newTable(NULL);

    enterPredefinedTypes(table);
    enterPredefinedProcs(table);
    return table;
}

Entry *enter(SymbolTable *table, Identifier *name, Entry *entry) {
    Bintree *newtree;
    Bintree *current;
    Bintree *previous;

    //Initialize new bin tree
    newtree = new(Bintree);
    newtree->key = name;
    newtree->entry = entry;
    newtree->left = NULL;
    newtree->right = NULL;

    if (table->bintree == NULL) {
        table->bintree = newtree;
    } else {
        current = table->bintree;
        while (1) {
            if (current->key->stamp == name->stamp) {
                /* symbol already in table */
                return NULL;
            }
            previous = current;
            if (current->key->stamp > name->stamp) {
                current = current->left;
            } else {
                current = current->right;
            }
            if (current == NULL) {
                if (previous->key->stamp > name->stamp) {
                    previous->left = newtree;
                } else {
                    previous->right = newtree;
                }
                break;
            }
        }
    }
    return entry;
}

static Entry *lookupBintree(Bintree *bintree, Identifier *key) {
    while (bintree != NULL) {
        if (bintree->key->stamp == key->stamp) {
            return bintree->entry;
        }
        if (bintree->key->stamp > key->stamp) {
            bintree = bintree->left;
        } else {
            bintree = bintree->right;
        }
    }
    return NULL;
}

Entry *lookup(SymbolTable *table, Identifier *name) {
    Entry *entry;

    while (table != NULL) {
        entry = lookupBintree(table->bintree, name);
        if (entry != NULL) {
            return entry;
        }
        table = table->upperLevel;
    }
    return NULL;
}

void showEntry(Entry *entry) {
    switch (entry->kind) {
        case ENTRY_KIND_TYPE:
            printf("type: ");
            showType(entry->u.typeEntry.type);
            break;
        case ENTRY_KIND_VAR:
            printf("var: ");
            if (entry->u.varEntry.isRef) {
                printf("ref ");
            }
            showType(entry->u.varEntry.type);
            break;
        case ENTRY_KIND_PROC:
            printf("proc: ");
            showParameterTypeList(entry->u.procEntry.parameterTypes);
            break;
        default:
            fprintf(stderr, "Internal Error: Unknown entry_kind '%d'.", (int) entry->kind);
            assert(false);
    }
}

static int countBintree(Bintree *bintree) {
    if (bintree == NULL) return 0;

    return 1 + countBintree(bintree->left) + countBintree(bintree->right);
}

static Bintree **flattenBintree(Bintree *bintree, Bintree **array) {
    if (bintree == NULL) return array;

    (*array) = bintree;
    array = flattenBintree(bintree->left, array + 1);
    array = flattenBintree(bintree->right, array);
    return array;
}

static int compareEntries(void const *left, void const *right) {
    Bintree *a = *((Bintree **) left);
    Bintree *b = *((Bintree **) right);
    return strcmp(a->key->string, b->key->string);
}

static void showBintree(Bintree *bintree) {
    // Flatten and sort to ensure defined order
    int entryCount = countBintree(bintree);
    Bintree **array = (Bintree **) malloc(entryCount * sizeof(Entry *));
    flattenBintree(bintree, array);

    qsort(array, entryCount, sizeof(Entry *), &compareEntries);

    if (entryCount == 0) printf("    <empty>\n");
    else {
        for (int i = 0; i < entryCount; ++i) {
            Bintree *keyValuePair = array[i];

            printf("    %-15s --> ", keyValuePair->key->string);
            showEntry(keyValuePair->entry);
            printf("\n");
        }
    }

    free(array);
}

void showTable(SymbolTable *table) {
    int level = 0;

    while (table != NULL) {
        printf("  level %d\n", level);
        showBintree(table->bintree);
        table = table->upperLevel;
        level++;
    }
}