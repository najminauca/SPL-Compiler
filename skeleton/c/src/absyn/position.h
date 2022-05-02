#ifndef SPL_POSITION_H_
#define SPL_POSITION_H_

#include "_declarations.h"

typedef struct position {
    int line;
    int column;
} Position;

/**
 * Constructs a new position.
 * Unlike other constructors this does not create an object on the heap but instead returns a Position-value.
 * @param line A line number.
 * @param column A column number.
 * @return The constructed Position.
 */
Position newPosition(int line, int column);

extern Position ERROR_POSITION;

#endif //SPL_POSITION_H_
