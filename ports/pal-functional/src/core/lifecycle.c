#include "core/lifecycle.h"
#include <stdint.h>

#define CLEANUP_TABLE_MAX   128
void (*g_cleanup_table[CLEANUP_TABLE_MAX + 1])(void) = { 0 };
/* [0] = memoized count (N). In the ELF the writer (IOP) sets it to N;
 *   -1 was only the "dynamic scan" mode used when the count was unknown. */

static int cleanup_count = 0;

int cleanup_register(void (*fn)(void))
{
	if (!fn || cleanup_count >= CLEANUP_TABLE_MAX)
		return -1;

	cleanup_count++;
	g_cleanup_table[cleanup_count] = fn;   /* slot 1-based */
	g_cleanup_table[0] = (void (*)(void))(intptr_t)cleanup_count;    /* memoized: clean LIFO, no scan */
	return cleanup_count;
}

void run_cleanup_callbacks(void)
{
	int count = (int)(intptr_t)g_cleanup_table[0];   /* N (memoized) */
	int i;

	/* [Faithful to the asm] If the header is -1 by any path (the ELF's scan mode),
	 *   reproduce the dynamic count, BUT skip the null entry
	 *   (the jalr-to-0 quirk that would segfault on PC). */
	if (count == -1)
	{
		count = 0;
		for (i = 1; g_cleanup_table[i] != 0; i++)
			count = i;
	}

	for (i = count; i >= 1; --i)
		if (g_cleanup_table[i] != 0)        /* <- plugs the asm's off-by-one */
			g_cleanup_table[i]();
}

int g_cleanup_done = 0;   /* DAT_0014186c */

void cleanup_run_once(void)
{
	if (g_cleanup_done == 0)
	{
		g_cleanup_done = 1;        /* set BEFORE cleaning up (anti re-entrancy) */
		run_cleanup_callbacks();
	}
	/* if already 1, does nothing: idempotent */
}
