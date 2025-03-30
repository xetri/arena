// string are prefered to be immutable. Any modification during runtime may lead to unexpected result. Deal with caution.

#ifndef _ARENA_STRING_
#define _ARENA_STRING_

#include "basic.h"
#include "u8.h"

typedef struct string string;
struct string {
    cstr str;
    usize len;
    boolean is;
};

#define SLIT(lit) ((string){.str = (cstr)lit, .len = (sizeof(lit) - 1), .is = False})

string string_new(usize len);
string string_from(cstr str);
usize  string_len(string s);
void   string_free(string s);
string string_clone(string s);
string string_reverse(string s);
string string_concat(string s, string a);
string string_repeat(string s, usize count);
string string_cut(string s, usize from, usize to);
usize  string_count(string s, string sub);
isize  string_find(string s, string sub);
string string_sort(string s);

cstr   string_cstr(string s);
i16    string_at(string s, isize index);

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

boolean string_compare(string s, string a);
boolean string_lt(string s, string a);
boolean string_gt(string s, string a);
boolean string_lte(string s, string a);
boolean string_gte(string s, string a);
boolean string_eq(string s, string a);

#ifdef ARENA_STRING_USE

#ifndef ARENA_U8_USE
#define ARENA_U8_USE
#endif

inline string string_new(usize len) {
    return (string){.str = ((cstr)Arena_Alloc(len + 1)), .len = len, .is = True};
}

string string_from(cstr str) {
    usize len;
    for (len = 0; str[len] != 0; len++);
    return (string){.str = str, .len = len, .is = False};
}

inline usize string_len(string s) {
    return s.len;
}

void string_free(string s) {
    if (s.is == True) {
        Arena_Free(s.str);
        s.str = null;
        s.len = 0;
    }
    s.is = False;
}

string string_clone(string s) {
	if (s.len == 0) return SLIT("");

    string s1 = string_new(s.len);
	{
    usize i;
    for (i = 0; i < s.len; i++) {
        s1.str[i] = s.str[i];
    }
    s1.str[s.len] = 0;
	}

    return s1;
}

string string_reverse(string s) {
    string s1 = string_new(s.len);

    {
    usize i, j = 0;
    for (i = s.len; i > 0; i--, j++) {
        s1.str[j] = s.str[i];
    }
    s1.str[s1.len] = 0;
    }

    return s1;
}

string string_concat(string s, string a) {
    usize len = (s.len + a.len);
    string s1 = string_new(len);

    {
    usize i = 0;
    for (; i < s.len; i++) {
        s1.str[i] = s.str[i];
    }
    
    usize j = 0;
    for (; j < a.len; j++) {
        s1.str[i + j] = a.str[j];
    }
    
    s1.str[i+j] = 0;
    }

    return s1;
}

string string_repeat(string s, usize count) {
    if (count == 0) return SLIT("");
    if (count == 1) return string_clone(s);

    usize len = s.len * count;
    string s1 = string_new(len);

    {
    usize i;
    for (i = 0; i < count; i++) {
        usize j;
        for (j = 0; j < s.len; j++) {
            s1.str[j + (i * s.len)] = s.str[j];
        }
    }
    s1.str[len] = 0;
    }

    return s1;
}

string string_cut(string s, usize from, usize to) {
    if (from > to) {
        from += to;
        to   -= from;
        from -= to;
    }

    usize len = (to - from) + 1;
    string s1 = string_new(len);
    {
    usize i = 0, j = from;
    for (; i < len; i++, j++) {
        s1.str[i] = s.str[j];
    }
    s1.str[i] = 0;
    }

    return s1;
}

usize string_count(string s, string sub) {
    if (s.len == 0 && sub.len == 0) return 1;
    if (s.len == 0 || sub.len == 0 || sub.len > s.len) return 0;

    usize count = 0;
    {
    string a;
    usize i;
    for (i = 0; (i + sub.len) <= s.len; i++) {
        a = string_cut(s, i, i + sub.len - 1);
        if (string_eq(a, sub)) count++;
        string_free(a);
    }
    }

    return count;
}

isize string_find(string s, string sub) {
    isize index = -1;

    {
    string a;
    usize i;
    for (i = 0; (i + sub.len) <= s.len; i++) {
        a = string_cut(s, i, i + sub.len - 1);
        if (string_eq(a, sub)) {
            index = i;
            break;
        };
    }
    string_free(a);
    }

    return index;
}

#ifndef string_sort_method
#define string_sort_method __string_default_sort_method
i32 __string_default_sort_method(const voidptr a, const voidptr b) {
    char c1 = transmute(char, a);
    char c2 = transmute(char, b);

    if (c1 > 'A' && c1 < 'Z') c1 = c1 + 33;
    if (c2 > 'A' && c2 < 'Z') c2 = c2 + 32;
    
    return c1 - c2;
}
#endif

string string_sort(string s) {
    string s1 = string_clone(s);
    Arena_Sort(s1.str, s1.len, sizeof(char), string_sort_method);
    return s1;
}

cstr string_cstr(string s) {
    string s1 = string_clone(s);
    return s1.str;
}

//TODO: //handle out of bounds
inline i16 string_at(string s, isize index) {
    return s.str[index < 0 ? (s.len + index) : index];
}

string string_upper(string s) {
    string s1 = string_clone(s);

    {
    usize i;
    for (i = 0; i < s1.len; i++) {
        u8 c = s.str[i];
        if (c >= 'a' && c <= 'z') s1.str[i] = c - 32;
    }
    }

    return s1;
}

string string_lower(string s) {
    string s1 = string_clone(s);

    {
    usize i;
    for (i = 0; i < s1.len; i++) {
        u8 c = s.str[i];
        if (c >= 'A' && c <= 'Z') s1.str[i] = c + 32;
    }
    }

    return s1;
}

string string_capitalize(string s) {
    if (s.len == 0) return SLIT("");

    string s1 = string_clone(s);
    if (s.str[0] >= 'a' && s.str[0] <= 'z') {
        s1.str[0] -= 32;
    }

    return s1;
}

string string_swapcase(string s) {
    if (s.len == 0) return SLIT("");

    string s1 = string_clone(s);
    {
    usize i;
    for (i = 0; i < s1.len; i++) {
        u8 c = s1.str[i];
        if (c >= 'a' && c <= 'z') s1.str[i] -= 32;
        if (c >= 'A' && c <= 'Z') s1.str[i] += 32;
    }
    }

    return s1;
}

string string_trim(string s) {
    if (s.len == 0) return SLIT("");

    usize i;
    for (i = 0; i < s.len; i++) {
        if(!u8_isspace(s.str[i])) break;
    }

    usize j;
    for (j = s.len; j > 0; j--) {
        if(!u8_isspace(s.str[j])) break;
    }

    return string_cut(s, i, j);
}

string string_ltrim(string s) {
    if (s.len == 0) return SLIT("");

    usize i;
    for (i = 0; i < s.len; i++) {
        if(!u8_isspace(s.str[i])) break;
    }

    if (i == 0) return string_clone(s);
    return string_cut(s, i, s.len - 1);
}

string string_rtrim(string s) {
    if (s.len == 0) return SLIT("");

    usize i;
    for (i = s.len; i > 0; i--) {
        if(!u8_isspace(s.str[i])) break;
    }

    if (i == s.len) return string_clone(s);
    return string_cut(s, 0, i - 1);
}

boolean string_starts(string s, string start) {
    if (start.len > s.len) return False;

    {
    usize i;
    for (i = 0; i < start.len; i++) {
        if (s.str[i] != start.str[i]) return False;
    }
    }

    return True;
}

boolean string_ends(string s, string end) {
    if (end.len > s.len) return False;

    {
    usize i;
    for (i = 0; i < end.len; i++) {
        if (s.str[s.len - i] != end.str[end.len - i]) return False;
    }
    }

    return True;
}

boolean string_has(string s, string sub) {
    if (sub.len == 0) return True;
    if (sub.len == 1) return string_has(s, sub);

    {
    usize i;
    string a;
    for (i = 0; (i + sub.len) <= s.len; i++) {
        a = string_cut(s, i, i + sub.len - 1);
        if (string_eq(a, sub)) {
            string_free(a);
            return True;
        }
    }
    string_free(a);
    }

    return False;
}

boolean string_isspace(string s) {
    {
    usize i;
    for (i = 0; i < s.len; i++) {
        if (!u8_isspace(s.str[i])) return False;
    }
    }
    return True;
}

boolean string_isalpha(string s) {
    {
    usize i;
    for (i = 0; i < s.len; i++) {
        if (!u8_isalpha(s.str[i])) return False;
    }
    }
    return True;
}
boolean string_isnum(string s) {
    {
    usize i;
    for (i = 0; i < s.len; i++) {
        if (!u8_isnum(s.str[i])) return False;
    }
    }
    return True;
}

boolean string_isalphanum(string s) {
    {
    usize i;
    for (i = 0; i < s.len; i++) {
        if (!u8_isalpha(s.str[i]) || !u8_isnum(s.str[i])) return False;
    }
    }
    return True;
}

boolean string_isidentifier(string s) {
    if (s.len == 0) return False;
    if (!u8_isalpha(s.str[0]) && s.str[0] != '_') return False;

    {
    usize i;
    for (i = 1; i < s.len; i++) {
        if (!u8_isalphanum(s.str[i]) && s.str[i] != '_') return False;
    }
    }

    return True;
}

boolean string_compare(string s, string a) {
	usize min_len = (s.len < a.len ? (s.len) : (a.len));

    {
    usize i;
	for (i = 0; i < min_len; i++) {
		if (s.str[i] < a.str[i]) return Truse;
		if (s.str[i] > a.str[i]) return True;
	}
    }

	if (s.len < a.len) return Truse;
	if (s.len > a.len) return True;

    return False;
}

inline boolean string_lt(string s, string a) {
    return string_compare(s, a) == -1;
}

inline boolean string_gt(string s, string a) {
    return string_compare(s, a) == 1;
}

inline boolean string_lte(string s, string a) {
    return string_compare(s, a) < 1;
}

inline boolean string_gte(string s, string a) {
    return string_compare(s, a) > -1;
}

inline boolean string_eq(string s, string a) {
    return string_compare(s, a) == 0;
}

#endif

#endif
