# Arena

A lightweight C utility library that provides essential modules including memory arenas, dynamic arrays, string manipulation, and UTF-8 character handling. This library is designed with modularity and simplicity in mind, targeting performance-critical applications or minimal runtime environments.

```c
#define ARENA_USE
#include "arena/arena.h"
```

## String

Available in [string.h](./string.h) header file.
It exposes some useful string helper functions for making program safe and effectively easy to understand and execute.

```c
struct string {
    cstr str;
    i32 len;
    boolean is;
};
```

#### Example

```c
// Create an string instance
string s = String("A String");

// Clone a string (clones it into new instance of string)
string s1 = string_clone(s);

// To access the literal of that string
s1.str
// To access length of that string
s1.len

// Reverse a string
string reversedString = string_reverse(s1); // gnirtS A

// Creates an instance of string without allocation of memory
string m = SLIT("Literal"); // no need to be freed later

// Append two strings
string a = SLIT("A");
string b = SLIT("B");
string ab = string_append(a, b); // AB

// The string must be freed after the use of strings.
string_free(a);
string_free(b);
```

### Available string helper functions:
```c
string string_new(i32 len);
string string_from(cstr str);
void   string_free(string s);
string string_clone(string s);
string string_reverse(string s);
string string_append(string s, string a);
string string_repeat(string s, i32 count);
string string_slice(string s, i32 from, i32 to);
i32    string_count(string s, string sub);
i32    string_find(string s, string sub);
i32    string_findAt(string s, string sub, i32 n);
string string_replace(string s, string sub, string rep);
string string_replaceAll(string s, string sub, string rep);
string string_replaceAt(string s, string sub, string rep, i32 n);
string string_sort(string s);
i8     string_at(string s, i32 index);
string string_upper(string s);
string string_lower(string s);
string string_capitalize(string s);
string string_swapcase(string s);
string string_trim(string s);
string string_ltrim(string s);
string string_rtrim(string s);
boolean string_starts(string s, string start);
boolean string_ends(string s, string end);
boolean string_has(string s, string sub);
boolean string_isspace(string s);
boolean string_isalpha(string s);
boolean string_isnum(string s);
boolean string_isalphanum(string s);
boolean string_isidentifier(string s);
boolean string_cmp(string s, string a);
boolean string_lt(string s, string a);
boolean string_gt(string s, string a);
boolean string_lte(string s, string a);
boolean string_gte(string s, string a);
boolean string_eq(string s, string a);    
```

## Array

```c
struct array {
    voidptr data;
    usize len;
    usize cap;
    usize esize;
    boolean is;
};
```

#### Example

```c
array A = Array(i32, 1, 2, 3, 4, 5);

i32 i;
for (i = 0; i < A.len; i++) { 
    printf("%d ", ((int*)(A.data))[i]);
    // OR alts for this arena library
    // printf("%d ", (cast(i32*, A.data))[i]);
}

array_free(A); // Free the array memory
```

#### Currently creation and access of array functions are only available. More helper functions will be coming soon.

### Note: You can also contribute to it. I will personally inspect it
