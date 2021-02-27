/*

* API (macro, no type check)
CO_BEGIN      : ...
CO_END        : ...
CO_YIELD      : ...
CO_RETURN     : ...
CO_STATE(CO)  : ...

CO_THIS                 : the parameter name of coroutine function.
CO_DECLARE(NAME, ...)   : declare a coroutine.
CO_DEFINE (NAME)        : define a declared coroutine which not defined.
CO_MAKE   (NAME, ...)   : coroutine maker.
NAME_func               : coroutine function name, made by CO_DECLARE(NAME), e.g. Nat_func

*/
#ifndef MOXITREL_COGO_YIELD_H_
#define MOXITREL_COGO_YIELD_H_

#if defined(__GNUC__)
#   include "yield_label_value.h"
#else
#   include "yield_case.h"
#endif

#include "utils.h"

// COGO_DECLARE(NAME, ...): define a coroutine named <NAME>
//
// * Example
//
// COGO_DECLARE(Point, cogo_yield_t co_gen, int x, int y, int z):
//
//  typedef struct {
//      cogo_yield_t co_yield;
//      int x;
//      int y;
//      int z;
//  } Point;
//  void Point_func(Point* CO_THIS)
//
#define COGO_COMMA_static               ,
#define COGO_COMMA_extern               ,
#define COGO_REMOVE_LINKAGE_static
#define COGO_REMOVE_LINKAGE_extern
#define COGO_DECLARE(NAME, ...)         COGO_DECLARE1(COGO_ARG_COUNT(COGO_COMMA_##NAME), NAME, __VA_ARGS__)
#define COGO_DECLARE1(...)              COGO_DECLARE2(__VA_ARGS__)
#define COGO_DECLARE2(N, ...)           COGO_DECLARE_##N(__VA_ARGS__)
#define COGO_DECLARE_1(NAME, ...)       /* NAME: Type */                    \
    typedef struct NAME NAME;                                               \
    struct NAME {                                                           \
        COGO_MAP(;, COGO_ID, __VA_ARGS__);                                  \
    };                                                                      \
    CO_DEFINE(NAME)
#define COGO_DECLARE_2(NAME, ...)       /* NAME: static Type */             \
    typedef struct COGO_REMOVE_LINKAGE_##NAME COGO_REMOVE_LINKAGE_##NAME;   \
    struct COGO_REMOVE_LINKAGE_##NAME {                                     \
        COGO_MAP(;, COGO_ID, __VA_ARGS__);                                  \
    };                                                                      \
    CO_DEFINE(NAME)


#define CO_DECLARE(NAME, ...)                                               \
    COGO_IFNIL(__VA_ARGS__)(                                                \
        COGO_DECLARE(NAME, cogo_yield_t cogo_yield),                        \
        COGO_DECLARE(NAME, cogo_yield_t cogo_yield, __VA_ARGS__)            \
    )

#define CO_DEFINE(NAME)                 \
    void NAME##_func(void* CO_THIS)

#define CO_MAKE(NAME, ...)              \
    ((NAME){                            \
        .cogo_yield = {.cogo_pc = 0},   \
        __VA_ARGS__                     \
    })

#endif // MOXITREL_COGO_YIELD_H_