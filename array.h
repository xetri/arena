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

#define Array(typ, ...) ((array) { \
    .data = (typ[]) {__VA_ARGS__}, \
    .len = sizeof((typ[]) {__VA_ARGS__}) / sizeof(typ), \
    .cap = sizeof((typ[]) {__VA_ARGS__}) / sizeof(typ), \
    .esize = sizeof(typ), \
    .is = False \
})

array array_new(i32 elemsize, i32 cap);
usize array_len(array a);
void array_free(array a);
array array_clone(array a);
void array_clear(array* a);

#ifdef ARENA_ARRAY_USE

inline array array_new(i32 elemsize, i32 cap) {
    return (array){
        .data = Arena_Alloc(elemsize * cap),
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
        if (a.data) Arena_Free(a.data);
        a.data = null;
        a.len = 0;
        a.cap = 0;
        a.is = False;
    }
}

array array_clone(array a) {
    array a1 = array_new(a.esize, a.cap);
    a1.len = a.len;

    u8* src = cast(u8*, a.data);
    u8* dst = cast(u8*, a1.data);

    for (usize i = 0; i < a.len * a.esize; i++) {
        dst[i] = src[i];
    }

    return a1;
}

inline void array_clear(array* a) {
    array a1 = array_new(a->esize, a->cap);
    array_free(*a);
    *a = a1;
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
