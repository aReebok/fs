#ifndef _TALLOC
#define _TALLOC

// Replacement for malloc that stores the pointers allocated. It should store
// the pointers in a linked list 
// talloc stand for tree alloc - this is a simple linked list for now
void *talloc(size_t size);

// Free all pointers allocated by talloc, as well as whatever memory
// allocated in lists to hold those pointers.
void tfree();

// Replacement for the C function "exit", that consists of two lines: it calls
// tfree before calling exit. It's useful to have later on; if an error happens,
// you can exit your program, and all memory is automatically cleaned up.
void texit(int status);

#endif