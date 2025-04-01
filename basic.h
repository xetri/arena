#ifndef _ARENA_BASIC_
#define _ARENA_BASIC_

typedef char i8;
typedef short int i16;
typedef int i32;
typedef long long i64;
#ifndef __SSIZE_T_TYPE
#define __SSIZE_T_TYPE long int
#endif
typedef __SSIZE_T_TYPE isize;

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;
typedef unsigned long long u64;
#ifndef __SIZE_TYPE__ 
#define __SIZE_TYPE__  unsigned long int
#endif
typedef __SIZE_TYPE__ usize;

typedef char boolean;
typedef char* cstr;
typedef char* charptr;
typedef void* voidptr;

#define True    ((boolean)1)
#define False   ((boolean)0)
#define Truse  ((boolean)-1)
#define null    0

#define fn(type, name)          type (*name)
#define cast(type, expr)        ((type)expr)
#define transmute(type, expr)   (*(type*)expr)

#ifndef Arena_Alloc
voidptr malloc(usize size);
#define Arena_Alloc malloc
#endif

#ifndef Arena_Free
void free(voidptr ptr);
#define Arena_Free free
#endif

#ifndef Arena_Sort
#define Arena_Sort qsort
void qsort(voidptr base, usize nmemb, usize size, fn(i32, compare)(const voidptr a, const voidptr b));
#endif

#endif
