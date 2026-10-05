#ifndef LIFECYCLE_H
#define LIFECYCLE_H

/*
 * List of engine shutdown callbacks (LIFO).
 *   g_cleanup_table[0]  = header (-1 = "zero-terminated list")
 *   g_cleanup_table[1..N] = pointers to cleanup functions
 *   g_cleanup_table[N+1]  = 0 (sentinel)
 *
 * On PS2 it was filled by the IOP loader (another binary, via SIF/RPC).
 * On PC we fill it ourselves: every resource that is created registers
 * its destructor here, and run_cleanup_callbacks() runs them in reverse on exit.
 */
extern void (*g_cleanup_table[])(void);

/* Registers a destructor. Returns the assigned slot, or -1 if the array is full. */
int  cleanup_register(void (*fn)(void));

/* Runs every registered callback, from last to first (LIFO).
 *   With an empty table (BSS=0) it is a safe no-op. */
void run_cleanup_callbacks(void);

/* "Has the cleanup already run?" flag (DAT_0014186c).
 *   0 = pending, 1 = the destructors have already run. */
extern int g_cleanup_done;

/* Runs run_cleanup_callbacks() only ONCE.
 *   The guard is set BEFORE running (re-entrancy protection:
 *   a callback that calls cleanup_run_once again does not re-enter). */
void cleanup_run_once(void);

#endif /* LIFECYCLE_H */
