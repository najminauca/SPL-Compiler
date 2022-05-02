#include "position.h"

Position newPosition(int line, int column) {
    Position pos;
    pos.line = line;
    pos.column = column;
    return pos;
}

Position ERROR_POSITION = {-1, -1};