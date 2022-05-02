//
// list.h -- Helper Macros for lists
//

#ifndef SPL_LIST_H
#define SPL_LIST_H

#include "memory.h"
#include <stdbool.h>

/**
 * Helper macro to define list structures.
 * Not used in most places it could be used to make the library code easier to understand for students.
 *
 * @param list_tag The struct tag of the list struct.
 * @param list_type The name of the list struct.
 * @param element_type The type of a single List element.
 */
#define DECLARE_LIST_STRUCT(list_tag, list_type, element_type)                                      \
    typedef struct list_tag {                                                                       \
        element_type *head;                                                                         \
        list_type *tail;                                                                            \
        bool isEmpty;                                                                               \
    } list_type;

/**
 * Helper macro to define list constructors.
 * @param name The name of the constructor (e.g. emptyExpressionList)
 * @param list_type The name of the list type (e.g. ExpressionList)
 */
#define DEFINE_EMPTY_LIST(name, list_type)                                                          \
    list_type* name (void) {                                                                        \
        list_type *nil = new(list_type);                                                            \
        nil->head = NULL;                                                                           \
        nil->tail = NULL;                                                                           \
        nil->isEmpty = true;                                                                        \
                                                                                                    \
        return nil;                                                                                 \
    }

/**
 * Helper macro to define list constructors.
 * @param name The name of the constructor (e.g. expressionListSize)
 * @param list_type The name of the list type (e.g. ExpressionList)
 * @param element_type Type of the list's elements (e.g. Expression)
 */
#define DEFINE_LIST_CONSTRUCTOR(name, list_type, element_type)                                      \
    list_type* name(element_type *head, list_type *tail) {                                          \
        list_type *node = new(list_type);                                \
        node->isEmpty = false;                                                                      \
        node->head = head;                                                                          \
        node->tail = tail;                                                                          \
        return node;                                                                                \
    }

/**
 * Helper macro to define a function to calculate the size of a list.
 *
 * @param name The name of the defined function (e.g. newExpressionList)
 * @param list_type The name of the list type (e.g. ExpressionList)
 */
#define DEFINE_LIST_SIZE(name, list_type)                                                           \
    int name(list_type *list) {                                                                     \
        if(list->isEmpty) return 0;                                                                 \
        return 1 + name(list->tail);                                                                \
    }

/**
 * Helper macro that defines a function that is able to copy the contents of a list into a continuous memory stretch (an array).
 * NOTE: The memory area needs to be big enough to contain all elements, this function is susceptible to buffer overflows!
 *
 * @param name The name of the defined function (e.g. expressionListToArray)
 * @param list_type The name of the list type (e.g. ExpressionList)
 * @param element_type Type of the list's elements (e.g. Expression)
 */
#define DEFINE_LIST_TO_ARRAY(name, list_type, element_type)                                         \
    void name(element_type** array, list_type *list){                                               \
        if(list->isEmpty) return;                                                                   \
        array[0] = list->head;                                                                      \
        name(array + 1, list->tail);                                                                \
    }                                                                                               \

/**
 * Helper macro that defines a function that is able to retrieve an element from a linked list with an index.
 *
 * @param name The name of the defined function (e.g. expressionListGet)
 * @param list_type The name of the list type (e.g. ExpressionList)
 * @param element_type Type of the list's elements (e.g. Expression)
 */
#define DEFINE_LIST_GET(name, list_type, element_type)                                              \
    element_type* name(list_type *list, int index) {                                                \
        if(index == 0) return list->head;                                                           \
        return name(list->tail, index - 1);                                                         \
    }

/**
 * Helper macro that defines a function that is able to retrieve an element from a linked list with an index.
 *
 * @param name The name of the defined function (e.g. expressionListGet)
 * @param list_type The name of the list type (e.g. ExpressionList)
 * @param element_type Type of the list's elements (e.g. Expression)
 */
#define DEFINE_LIST_APPEND(name, list_type, element_type, empty_list)                               \
    void name(list_type *list, element_type *value) {                                               \
        if (!list->isEmpty) name(list->tail, value);                                                \
        else {                                                                                      \
            list->head = value;                                                                     \
            list->tail = empty_list;                                                                \
            list->isEmpty = false;                                                                  \
        }                                                                                           \
    }

#endif //SPL_LIST_H
