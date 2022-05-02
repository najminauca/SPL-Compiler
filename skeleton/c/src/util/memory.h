/*
 * memory.h -- memory management
 */

#ifndef SPL_MEMORY_H
#define SPL_MEMORY_H

#define new(cls) (cls*) allocate(sizeof(cls))

void *allocate(unsigned size);
void release(void *p);

#endif /* SPL_MEMORY_H */
