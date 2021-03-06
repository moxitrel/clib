#include <assert.h>

#if defined(COGO_CASE)
#   include "yield_case.h"
#elif defined(COGO_LABEL_VALUE)
#   include "yield_label_value.h"
#endif

#include "yield.h"
#include "gtest/gtest.h"

CO_DECLARE(CogoYield, int v)
{
CO_BEGIN:

    ((CogoYield*)CO_THIS)->v++;
    CO_YIELD;
    ((CogoYield*)CO_THIS)->v++;

CO_END:;
}

TEST(cogo_yield_t, Yield)
{
    CogoYield cogoYield = CO_MAKE(CogoYield, 0);
    EXPECT_EQ(CO_STATE(&cogoYield), 0);     // init
    EXPECT_EQ(cogoYield.v, 0);

    CogoYield_func(&cogoYield);
    EXPECT_GT(CO_STATE(&cogoYield), 0);     // running
    EXPECT_EQ(cogoYield.v, 1);

    CogoYield_func(&cogoYield);
    EXPECT_EQ(CO_STATE(&cogoYield), -1);    // end
    EXPECT_EQ(cogoYield.v, 2);

    // noop when coroutine end
    CogoYield_func(&cogoYield);
    EXPECT_EQ(CO_STATE(&cogoYield), -1);
    EXPECT_EQ(cogoYield.v, 2);
}

CO_DECLARE(static CogoReturn, int v)
{
CO_BEGIN:

    ((CogoReturn*)CO_THIS)->v++;
    CO_RETURN;
    ((CogoReturn*)CO_THIS)->v++;

CO_END:;
}

TEST(cogo_yield_t, Return)
{
    CogoReturn cogoReturn = CO_MAKE(CogoReturn, .v=0);
    EXPECT_EQ(CO_STATE(&cogoReturn), 0);    // init
    EXPECT_EQ(cogoReturn.v, 0);

    CogoReturn_func(&cogoReturn);
    EXPECT_EQ(CO_STATE(&cogoReturn), -1);   // end
    EXPECT_EQ(cogoReturn.v, 1);
}

CO_DECLARE(Prologue, int enter, int exit);
CO_DEFINE(Prologue)
{
    Prologue* thiz = (Prologue*)CO_THIS;
    thiz->enter++;

CO_BEGIN:
    CO_YIELD;
    CO_YIELD;
CO_END:

    thiz->exit++;
}

TEST(cogo_yield_t, Prologue)
{
    Prologue prologue = {
        .enter = 0,
        .exit = 0,
    };

    while(CO_STATE(&prologue) >= 0) {
        Prologue_func(&prologue);
    }

    EXPECT_EQ(prologue.enter, 3);
    EXPECT_EQ(prologue.exit, 3);
}
