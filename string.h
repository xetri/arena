// string are prefered to be immutable. Any modification during runtime may lead to unexpected result. Deal with caution.

#ifndef _ARENA_STRING_
#define _ARENA_STRING_

#include "basic.h"
#include "u8.h"

typedef struct string string;
struct string {
    cstr str;
    i32 len;
    boolean is;

    string (*from)(cstr);
    void   (*free)(string);
    string (*clone)(string);
    string (*reverse)(string);
    string (*append)(string, string);
    string (*repeat)(string, i32);
    string (*slice)(string, i32, i32);
    i32    (*count)(string, string);
    i32    (*find)(string, string);
    i32    (*findAt)(string, string, i32);
    string (*replace)(string, string, string);
    string (*replaceAll)(string, string, string);
    string (*replaceAt)(string, string, string, i32);
    string (*sort)(string);
    i8     (*at)(string, i32);
    string (*upper)(string);
    string (*lower)(string);
    string (*capitalize)(string);
    string (*swapcase)(string);
    string (*trim)(string);
    string (*ltrim)(string);
    string (*rtrim)(string);

    boolean (*starts)(string, string);
    boolean (*ends)(string, string);
    boolean (*has)(string, string);
    boolean (*isspace)(string);
    boolean (*isalpha)(string);
    boolean (*isnum)(string);
    boolean (*isalphanum)(string);
    boolean (*isidentifier)(string);
    boolean (*cmp)(string, string);
    boolean (*lt)(string, string);
    boolean (*gt)(string, string);
    boolean (*lte)(string, string);
    boolean (*gte)(string, string);
    boolean (*eq)(string, string);
};

#define __ARENA_STRING_DEFINE(_str, _len, _is) ((string){ \
    .str = (cstr)_str, \
    .len = _len, \
    .is = _is, \
    .from = string_from, \
    .free = string_free, \
    .clone = string_clone, \
    .reverse = string_reverse, \
    .append = string_append, \
    .repeat = string_repeat, \
    .slice = string_slice, \
    .count = string_count, \
    .find = string_find, \
    .findAt = string_findAt, \
    .replace = string_replace, \
    .replaceAll = string_replaceAll, \
    .replaceAt = string_replaceAt, \
    .sort = string_sort, \
    .at = string_at, \
    .upper = string_upper, \
    .lower = string_lower, \
    .capitalize = string_capitalize, \
    .swapcase = string_swapcase, \
    .trim = string_trim, \
    .ltrim = string_ltrim, \
    .rtrim = string_rtrim, \
    .starts = string_starts, \
    .ends = string_ends, \
    .has = string_has, \
    .isspace = string_isspace, \
    .isalpha = string_isalpha, \
    .isnum = string_isnum, \
    .isalphanum = string_isalphanum, \
    .isidentifier = string_isidentifier, \
    .cmp = string_cmp, \
    .lt = string_lt, \
    .gt = string_gt, \
    .lte = string_lte, \
    .gte = string_gte, \
    .eq = string_eq, \
})

#define SLIT(lit) __ARENA_STRING_DEFINE(lit, (sizeof(lit) - 1), False)
#define Str() SLIT("")
#define String(s) string_from(s)

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

#ifdef ARENA_STRING_USE

#ifndef ARENA_U8_USE
#define ARENA_U8_USE
#include "u8.h"
#endif

inline string string_new(i32 len) {
    string s =  __ARENA_STRING_DEFINE(Arena_Alloc((len + 1) * sizeof(char)), len, True);
    return s;
}

string string_from(cstr str) {
    i32 len;
    for (len = 0; str[len] != 0; len++);
    string s = __ARENA_STRING_DEFINE(null, len, True);

    s.str = Arena_Alloc((len + 1) * sizeof(char));
    {
    i32 i;
    for (i = 0; i < s.len; i++) s.str[i] = str[i];
    s.str[s.len] = null;
    }

    return s;
}

void string_free(string s) {
    if (s.is == True) {
        Arena_Free(s.str);
        s.str = "";
        s.len = 0;
        s.is = False;
    }
}

string string_clone(string s) {
	if (s.len == 0) return SLIT("");

    string s1 = string_new(s.len);
	{
    i32 i;
    for (i = 0; i < s.len; i++) {
        s1.str[i] = s.str[i];
    }
    s1.str[s1.len] = null;
	}

    return s1;
}

string string_reverse(string s) {
    if (s.len == 0) return SLIT("");

    string s1 = string_new(s.len);

    {
    i32 i, j = 0;
    for (i = s.len; i > 0; i--, j++) {
        s1.str[j] = s.str[i - 1];
    }
    s1.str[s1.len] = null;
    }

    return s1;
}

string string_append(string s, string a) {
    if (s.len == 0) return string_clone(a);
    if (a.len == 0) return string_clone(s);

    i32 len = (s.len + a.len);
    string s1 = string_new(len);

    {
    i32 i = 0;
    for (; i < s.len; i++) {
        s1.str[i] = s.str[i];
    }
    
    i32 j = 0;
    for (; j < a.len; j++) {
        s1.str[i + j] = a.str[j];
    }
    
    s1.str[i+j] = null;
    }

    return s1;
}

string string_repeat(string s, i32 count) {
    if (count == 0 || s.len == 0) return SLIT("");
    if (count == 1) return string_clone(s);

    i32 len = s.len * count;
    string s1 = string_new(len);

    {
    i32 i;
    for (i = 0; i < count; i++) {
        i32 j;
        for (j = 0; j < s.len; j++) {
            s1.str[j + (i * s.len)] = s.str[j];
        }
    }
    s1.str[len] = null;
    }

    return s1;
}

string string_slice(string s, i32 from, i32 to) {
    if (s.len == 0) return SLIT("");

    if (from > to) {
        from += to;
        to   -= from;
        from -= to;
    }

    i32 len = (to - from) + 1;
    string s1 = string_new(len);
    {
    i32 i = 0, j = from;
    for (; i < len; i++, j++) {
        s1.str[i] = s.str[j];
    }
    s1.str[i] = null;
    }

    return s1;
}

boolean string_range_eq_from(string s, string sub, i32 start) {
    i32 i;
    for (i = 0; i < sub.len; i++) {
        if (s.str[start + i] != sub.str[i]) return False;
    }
    return True;
}

i32 string_count(string s, string sub) {
    if (s.len == 0 && sub.len == 0) return 1;
    if (s.len == 0 || sub.len == 0 || sub.len > s.len) return 0;

    i32 count = 0;
    {
    i32 i;
    for (i = 0; (i + sub.len) <= s.len; ) {
        if (string_range_eq_from(s, sub, i)) {
            count++;
            i += sub.len;
            continue;
        }
        i++;
    }
    }

    return count;
}

i32 string_find(string s, string sub) {
    if (s.len == 0 || sub.len == 0 || sub.len > s.len) return -1;

    {
    i32 i;
    for (i = 0; (i + sub.len) <= s.len; i++) {
        if (string_range_eq_from(s, sub, i)) return i;
    }
    }

    return -1;
}

i32 string_findAt(string s, string sub, i32 n) {
    if (s.len == 0 || sub.len == 0 || sub.len > s.len || n <= 0) return -1;

    {
    i32 count = 0;
    i32 i;
    for (i = 0; (i + sub.len) <= s.len;) {
        if (string_range_eq_from(s, sub, i)) {
            count++;
            if (count == n) return i;
        }
        i++;
    }
    }

    return -1;
}

string string_replace(string s, string sub, string rep) {
    if (string_eq(s, sub) == True) return string_clone(rep);
    if (s.len == 0) return SLIT("");
    if (sub.len == 0) return string_clone(s);

    i32 pos = string_find(s, sub);
    if (pos == -1) return string_clone(s);

    i32 len = s.len - sub.len + rep.len;
    string repstr = string_new(len);
    {
    i32 i = 0, j = 0;
    for (; i < pos; i++) repstr.str[i] = s.str[i];
    for (; j < rep.len; j++) repstr.str[pos + j] = rep.str[j];
    for (i += rep.len, j = pos + sub.len; i < s.len; i++, j++) repstr.str[i] = s.str[j];
    repstr.str[repstr.len] = null;
    }

    return repstr;
}

string string_replaceAll(string s, string sub, string rep) {
    i32 count = string_count(s, sub);

    if (count <= 0) return string_clone(s);
    if (count == 1) return string_replace(s, sub, rep);
    if (string_eq(s, sub) == True) return string_clone(rep);
    if (s.len == 0) return SLIT("");
    if (sub.len == 0) return string_clone(s);

    i32 len = s.len - (count * sub.len) + (count * rep.len);
    string repstr = string_new(len);

    {
    i32 poss = 0, posr = 0;
    i32 occures, j;
    for (occures = 1; occures <= count; occures++) {
        i32 i = string_findAt(s, sub, occures);

        for (;poss < i;) repstr.str[posr++] = s.str[poss++];
        for (j = 0; j < rep.len; j++) repstr.str[posr++] = rep.str[j];

        poss += sub.len;
    }
    for (;poss < s.len;) repstr.str[posr++] = s.str[poss++];

    repstr.str[repstr.len] = null;
    }

    return repstr;
}

string string_replaceAt(string s, string sub, string rep, i32 n) {
    i32 pos = string_findAt(s, sub, n);

    if (n <= 0 || pos == -1) return string_clone(s);
    if (n == 1) return string_replace(s, sub, rep);
    if (string_eq(s, sub) == True) return string_clone(rep);
    if (s.len == 0) return SLIT("");
    if (sub.len == 0) return string_clone(s);

    i32 len = s.len - sub.len + rep.len;
    string repstr = string_new(len);

    {
    i32 i, j;
    for (i = 0; i < pos; i++) repstr.str[i] = s.str[i];
    for (j = 0; j < rep.len; j++) repstr.str[pos + j] = rep.str[j];
    for (i += rep.len, j = pos + sub.len; i < s.len; i++, j++) repstr.str[i] = s.str[j];
    repstr.str[repstr.len] = null;
    }


    return repstr;
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

inline i8 string_at(string s, i32 index) {
    i32 i = index;
    {
    while (i < 0) i += s.len;
    }
    return s.str[i];
}

string string_upper(string s) {
    string s1 = string_clone(s);

    {
    i32 i;
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
    i32 i;
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
    i32 i;
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

    i32 i;
    for (i = 0; i < s.len; i++) {
        if(!u8_isspace(s.str[i])) break;
    }

    i32 j;
    for (j = s.len; j > 0; j--) {
        if(!u8_isspace(s.str[j])) break;
    }

    return string_slice(s, i, j);
}

string string_ltrim(string s) {
    if (s.len == 0) return SLIT("");

    i32 i;
    for (i = 0; i < s.len; i++) {
        if(!u8_isspace(s.str[i])) break;
    }

    if (i == 0) return string_clone(s);
    return string_slice(s, i, s.len - 1);
}

string string_rtrim(string s) {
    if (s.len == 0) return SLIT("");

    i32 i;
    for (i = s.len; i > 0; i--) {
        if(!u8_isspace(s.str[i])) break;
    }

    if (i == s.len) return string_clone(s);
    return string_slice(s, 0, i - 1);
}

boolean string_starts(string s, string start) {
    if (start.len > s.len) return False;

    {
    i32 i;
    for (i = 0; i < start.len; i++) {
        if (s.str[i] != start.str[i]) return False;
    }
    }

    return True;
}

boolean string_ends(string s, string end) {
    if (end.len > s.len) return False;

    {
    i32 i;
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
    i32 i;
    string a;
    for (i = 0; (i + sub.len) <= s.len; i++) {
        a = string_slice(s, i, i + sub.len - 1);
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
    i32 i;
    for (i = 0; i < s.len; i++) {
        if (!u8_isspace(s.str[i])) return False;
    }
    }
    return True;
}

boolean string_isalpha(string s) {
    {
    i32 i;
    for (i = 0; i < s.len; i++) {
        if (!u8_isalpha(s.str[i])) return False;
    }
    }
    return True;
}
boolean string_isnum(string s) {
    {
    i32 i;
    for (i = 0; i < s.len; i++) {
        if (!u8_isnum(s.str[i])) return False;
    }
    }
    return True;
}

boolean string_isalphanum(string s) {
    {
    i32 i;
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
    i32 i;
    for (i = 1; i < s.len; i++) {
        if (!u8_isalphanum(s.str[i]) && s.str[i] != '_') return False;
    }
    }

    return True;
}

boolean string_cmp(string s, string a) {
	i32 min_len = (s.len < a.len ? (s.len) : (a.len));

    {
    i32 i;
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
    return string_cmp(s, a) == -1;
}

inline boolean string_gt(string s, string a) {
    return string_cmp(s, a) == 1;
}

inline boolean string_lte(string s, string a) {
    return string_cmp(s, a) < 1;
}

inline boolean string_gte(string s, string a) {
    return string_cmp(s, a) > -1;
}

inline boolean string_eq(string s, string a) {
    return string_cmp(s, a) == 0;
}

#endif

#endif
