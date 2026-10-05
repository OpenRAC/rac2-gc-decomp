#ifndef PS2_SYNC_H
#define PS2_SYNC_H

#include <stdatomic.h>

static inline void ps2_sync(int type)
{
    (void)type;
    atomic_thread_fence(memory_order_seq_cst);
}

#endif