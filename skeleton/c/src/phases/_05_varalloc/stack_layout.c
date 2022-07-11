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
    if (stackLayout->outgoingAreaSize == -1) return true;   //-1 means no procedure call
    //TODO (assignment 5): Implement this function properly
    return false;
}

int getFrameSize(StackLayout *stackLayout) {
    int old = isLeafProcedure(stackLayout) ? 4 : 2 * 4; //if no procedure call = 4 (old fp) | otherwise = 4 + 4 (old fp + return address)
    int var = (stackLayout->localVarAreaSize != 0) ? stackLayout->localVarAreaSize : 0; //if empty = 0
    int out = (stackLayout->outgoingAreaSize != -1) ? stackLayout->outgoingAreaSize : 0;    //if empty = 0
    //TODO (assignment 5): Implement this function properly
    return var + out + old;
}

int getOldFramePointerOffSet(StackLayout *stackLayout) {
    if (stackLayout->outgoingAreaSize == -1) return 0;  //SP + 0
    //TODO (assignment 5): Implement this function properly
    return stackLayout->outgoingAreaSize + 4;   //4 = Old return address
}

int getOldReturnAddressOffset(StackLayout *stackLayout) {
    int retOff = (stackLayout->localVarAreaSize != 0) ? -(stackLayout->localVarAreaSize + 8) : -8; //minus so the offset is correct in --vars
    //TODO (assignment 5): Implement this function properly
    return retOff;
}

