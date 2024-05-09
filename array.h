// cast the pointer returned by array fns, yourself.
#ifndef _ARENA_ARRAY_
#define _ARENA_ARRAY_

#include "basic.h"

typedef struct array array;
struct array {
    voidptr data;
    usize len;
    usize cap;
    usize esize;
    boolean is;
};

array array_new(usize elemsize, usize cap);
usize array_len(array a);
void array_free(array a);
array array_clone(array a);

#ifdef ARENA_ARRAY_USE

inline array array_new(usize elemsize, usize cap) {
    return (array){
        .data = ArenaAlloc(elemsize * cap),
        .len = 0,
        .cap  = cap,
        .esize = elemsize,
        .is = True,
    };
}

inline usize array_len(array a) {
    return a.len;
}

void array_free(array a) {
    if (a.is == True) {
        ArenaFree(a.data);
        a.data = null;
        a.len = 0;
        a.cap = 0;
    }
    a.is = False;
}

array array_clone(array a) {
    array a1 = array_new(a.esize, a.cap);
    a1.data = a.data;
    return a1;
}

array array_clear(array a);
inline array array_clear(array a) {
    return array_new(a.esize, a.cap);
}

array array_append(array a, voidptr elem);

array array_insert(array a, voidptr elem, usize index);

voidptr array_pop(array a);

voidptr array_shift(array a);

voidptr array_unshift(array a, voidptr elem);

array array_remove(array a, voidptr elem);

array array_reverse(array a);

array array_sort(array a);

#endif

#endif
