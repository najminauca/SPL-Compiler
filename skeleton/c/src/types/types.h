/*
 * types.h -- type representation
 */

#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdbool.h>

/**
 * Used to differentiate between type kinds.
 */
typedef enum {
    TYPE_KIND_PRIMITIVE,
    TYPE_KIND_ARRAY
} type_kind;

/**
 * Represents the semantic type of expressions and variables.
 *
 * There are two kinds of Types in SPL.
 *
 * 1. Primitive Types representing the builtin types in SPL.
 * There are exactly two primitive types in SPL ("int" and "boolean") which are defined in this file.
 *
 * 2. Array Types representing the types of arrays in SPL.
 * They are constructed every time an ArrayTypeExpression is encountered in the source code.
 */
typedef struct type {
    type_kind kind;
    union {
        struct {
            char *printName;
        } primitiveType;
        struct {
            int size;
            struct type *baseType;
        } arrayType;
    } u;
    int byteSize;
} Type;

/**
 * The primitive type "int". Initialized in phase 4a.
 */
extern Type *intType;

/**
 * The primitive type "boolean". Initialized in phase 4a.
 */
extern Type *boolType;

/**
 * Creates a new primitive type representing a builtin type in SPL.
 * @param printName The name of the primitive type.
 * @param byteSize The size in bytes required to hold a value of this type.
 * @return A reference to the newly created primitive type.
 */
Type *newPrimitiveType(char *printName, int byteSize);

/**
 * Creates a new array type representing the type of an array in SPL.
 * This automatically calculates the size in byte required to hold a value of this type.
 * @param size The number of elements an array of this type can hold.
 * @param baseType The type of the arrays elements.
 * @return A reference to the newly created array type.
 */
Type *newArrayType(int size, Type *baseType);

/**
 * Fills the buffer with a human readable string for a given type.
 * @param buffer The buffer for the string.
 * @param length The buffer length.
 * @param type The type to print.
 */
void typeAsString(char *buffer, int length, Type *type);

/**
 * Prints a given type in a human readable format.
 * @param type The type to print.
 */
void showType(Type *type);

typedef struct parameter_type {
    Type *type;
    bool isRef;
    int offset;        /* filled in by variable allocator */
} ParameterType;

/**
 * Creates a new ParameterType object.
 * @param type The semantic type of the parameter.
 * @param isRef Whether the parameter is a reference parameter.
 * @return A pointer to the newly created ParameterType object.
 */
ParameterType *newParameterType(Type *type, bool isReference);

/**
 * Same as newParameterType but also allows to set an offset.
 * Exclusively used to initialize the global table with predefined procedures.
 * @param type The semantic type of the parameter.
 * @param isRef Whether the parameter is a reference parameter.
 * @param offset The parameter's offset in the stack frame.
 * @return A pointer to the newly created ParameterType object.
 */
ParameterType *newPredefinedParameterType(Type *type, bool isRef, int offset);

/**
 * Contains the information about parameters of a procedure.
 * This information is required for calling the associated procedure.
 *
 * Since a procedure may have an arbitrary amount of parameters, this information is stored in a linked list.
 */
typedef struct parameter_type_list {
    bool isEmpty;
    ParameterType *head;
    struct parameter_type_list *tail;
} ParameterTypeList;

/**
 * @return An empty list of parameter types.
 */
ParameterTypeList *emptyParameterTypeList(void);

/**
 * Creates a new ParameterTypeList by prepending a new head to an existing list.
 * Does not modify the existing list.
 * @param head The head of the new list.
 * @param tail The tail of the new list.
 * @return A list containing the new head as well as the old list as its tail.
 */
ParameterTypeList *newParameterTypeList(ParameterType *head, ParameterTypeList *tail);

/**
 * Prints a list of parameter types in a human readable format.
 * @param parameterTypeList The ParameterTypeList to print.
 */
void showParameterTypeList(ParameterTypeList *parameterTypeList);

#endif /* _TYPES_H_ */
