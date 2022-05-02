/*
 * types.c -- type representation
 */

#include <stdio.h>
#include <string.h>
#include <util/memory.h>
#include <util/list.h>
#include <assert.h>
#include <limits.h>
#include "types.h"

Type *newPrimitiveType(char *printName, int byteSize) {
    Type *type;

    type = new(Type);
    type->kind = TYPE_KIND_PRIMITIVE;
    type->u.primitiveType.printName = printName;
    type->byteSize = byteSize;
    return type;
}

Type *newArrayType(int size, Type *baseType) {
    Type *type;

    type = new(Type);
    type->kind = TYPE_KIND_ARRAY;
    type->u.arrayType.size = size;
    type->u.arrayType.baseType = baseType;
    type->byteSize = size * baseType->byteSize;
    return type;
}

void typeAsString(char *buffer, int length, Type *type) {
    switch (type->kind) {
        case TYPE_KIND_PRIMITIVE:
            snprintf(buffer, length, "%s", type->u.primitiveType.printName);
            break;
        case TYPE_KIND_ARRAY: {
            int bytesWritten = snprintf(buffer, length, "array [%d] of ", type->u.arrayType.size);
            buffer += bytesWritten;
            length -= bytesWritten;
            typeAsString(buffer, length, type->u.arrayType.baseType);
        } break;
        default:
            fprintf(stderr, "Internal Error: Unknown type_kind '%d'.", (int) type->kind);
            assert(false);
    }
}

void showType(Type *type) {
    char buffer[1024];
    typeAsString(buffer, sizeof buffer, type);
    printf("%s", buffer);
}

Type *intType = NULL;
Type *boolType = NULL;

ParameterType *newParameterType(Type *type, bool isReference) {
    ParameterType *parameterType = new(ParameterType);
    parameterType->type = type;
    parameterType->isRef = isReference;
    parameterType->offset = INT_MIN;
    return parameterType;
}

ParameterType *newPredefinedParameterType(Type *type, bool isRef, int offset) {
    ParameterType *pt = newParameterType(type, isRef);
    pt->offset = offset;
    return pt;
}

void showParameterTypeList(ParameterTypeList *parameterTypeList) {
    printf("(");
    while (!parameterTypeList->isEmpty) {
        if (parameterTypeList->head->isRef) {
            printf("ref ");
        }
        showType(parameterTypeList->head->type);
        parameterTypeList = parameterTypeList->tail;
        if (!parameterTypeList->isEmpty) {
            printf(", ");
        }
    }
    printf(")");
}

DEFINE_EMPTY_LIST(emptyParameterTypeList, ParameterTypeList)

DEFINE_LIST_CONSTRUCTOR(newParameterTypeList, ParameterTypeList, ParameterType)

