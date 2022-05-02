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
    if (stackLayout->outgoingAreaSize == INT_MIN) return false;

    //TODO (assignment 5): Implement this function properly
    return false;
}

int getFrameSize(StackLayout *stackLayout) {
    if (stackLayout->localVarAreaSize == INT_MIN || stackLayout->outgoingAreaSize == INT_MIN) return INT_MIN;

    //TODO (assignment 5): Implement this function properly
    return INT_MIN;
}

int getOldFramePointerOffSet(StackLayout *stackLayout) {
    if (stackLayout->outgoingAreaSize == INT_MIN) return INT_MIN;

    //TODO (assignment 5): Implement this function properly
    return INT_MIN;
}

int getOldReturnAddressOffset(StackLayout *stackLayout) {
    if (stackLayout->localVarAreaSize == INT_MIN) return INT_MIN;

    //TODO (assignment 5): Implement this function properly
    return INT_MIN;
}

