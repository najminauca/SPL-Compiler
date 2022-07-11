#include <phases/_05_varalloc/stack_layout.h>
#include <limits.h>

StackLayout *newStackLayout() {
    StackLayout *self = new(StackLayout);

    self->argumentAreaSize = INT_MIN;
    self->localVarAreaSize = INT_MIN;
    self->outgoingAreaSize = INT_MIN;

    return self;
}

bool isLeafProcedure(StackLayout *stackLayout) {
    if (stackLayout->outgoingAreaSize == -1) return true;

    //TODO (assignment 5): Implement this function properly
    return false;
}

int getFrameSize(StackLayout *stackLayout) {
    int old = isLeafProcedure(stackLayout) ? 4 : 2 * 4;
    int var = stackLayout->localVarAreaSize;
    int out = stackLayout->outgoingAreaSize;
    if (var == 0 || out == -1) {
        if(var != 0) {
            return var + old;
        }
        if(out != -1) {
            return out + old;
        }
        return old;
    }
    //TODO (assignment 5): Implement this function properly
    return stackLayout->localVarAreaSize + stackLayout->outgoingAreaSize + old;
}

int getOldFramePointerOffSet(StackLayout *stackLayout) {
    if (stackLayout->outgoingAreaSize == -1) return 0;
    //TODO (assignment 5): Implement this function properly
    return stackLayout->outgoingAreaSize + 4;
}

int getOldReturnAddressOffset(StackLayout *stackLayout) {
    int var = stackLayout->localVarAreaSize;
    if(var == 0) {
        return -8;
    }
    //TODO (assignment 5): Implement this function properly
    return -(var + 8);
}

