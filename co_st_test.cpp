#include <assert.h>
#include "co_st.h"
#include "gtest/gtest.h"

CO_DECLARE(static Recv, co_chan_t* c, co_msg_t msgNext)
{
CO_BEGIN:

    CO_CHAN_READ(((Recv*)CO_THIS)->c, &((Recv*)CO_THIS)->msgNext);

CO_END:;
}

CO_DECLARE(static Send, co_chan_t* c, co_msg_t msg)
{
CO_BEGIN:

    CO_CHAN_WRITE(((Send*)CO_THIS)->c, &((Send*)CO_THIS)->msg);

CO_END:;
}

CO_DECLARE(static Entry, Recv recv1, Send send1)
{
CO_BEGIN:

    CO_START(&((Entry*)CO_THIS)->recv1);
    CO_START(&((Entry*)CO_THIS)->send1);

CO_END:;
}

TEST(Chan, ReadWrite)
{
    auto c0 = CO_CHAN_MAKE(0);
    auto entry = CO_MAKE(Entry,
            CO_MAKE(Recv, &c0),
            CO_MAKE(Send, &c0));
    co_run(&entry);
    EXPECT_EQ(&entry.send1.msg, entry.recv1.msgNext.next);
}
