#include <table/entry.h>
#include <table/table.h>
#include <limits.h>
#include <stddef.h>

static Entry *newEntry(entry_kind kind) {
    Entry *entry = new(Entry);
    entry->kind = kind;

    return entry;
}

Entry *newTypeEntry(Type *type) {
    Entry *entry = newEntry(ENTRY_KIND_TYPE);
    entry->u.typeEntry.type = type;
    return entry;
}

Entry *newVarEntry(Type *type, bool isRef) {
    Entry *entry = newEntry(ENTRY_KIND_VAR);
    entry->u.varEntry.type = type;
    entry->u.varEntry.isRef = isRef;
    entry->u.varEntry.offset = INT_MIN;
    return entry;
}

Entry *newProcEntry(ParameterTypeList *parameterTypes, SymbolTable *localTable) {
    Entry *entry = newEntry(ENTRY_KIND_PROC);
    entry->u.procEntry.parameterTypes = parameterTypes;
    entry->u.procEntry.localTable = localTable;
    entry->u.procEntry.stackLayout = newStackLayout();
    return entry;
}

Entry *newPredefinedProcEntry(ParameterTypeList *parameterTypes, int argumentAreaSize) {
    Entry *entry = newProcEntry(parameterTypes, NULL);
    entry->u.procEntry.parameterTypes = parameterTypes;
    entry->u.procEntry.stackLayout->argumentAreaSize = argumentAreaSize;
    return entry;
}