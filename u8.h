#ifndef _ARENA_U8_
#define _ARENA_U8_

#include "basic.h"

boolean u8_is(u8 u, u8 a);
boolean u8_isspace(u8 u);
boolean u8_isalpha(u8 u);
boolean u8_isnum(u8 u);
boolean u8_isalphanum(u8 u);


#ifdef ARENA_U8_USE

inline boolean u8_is(u8 u, u8 a) {
    return u == a;
}

inline boolean u8_isspace(u8 u) {
    return u == ' '  || u == '\t' || u == '\n' || u == '\r' || u == '\v' || u == '\f';
}

inline boolean u8_isalpha(u8 u) {
    return (u >= 'a' && u <= 'z') || (u >= 'A' && u <= 'Z');
}

inline boolean u8_isnum(u8 u) {
    return (u >= '0' && u <= '9');
}

inline boolean u8_isalphanum(u8 u) {
    return u8_isnum(u) || u8_isalpha(u);
}

#endif

#endif
