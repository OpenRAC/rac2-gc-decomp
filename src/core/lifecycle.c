#include "core/lifecycle.h"

#define CLEANUP_TABLE_MAX   128
void (*g_cleanup_table[CLEANUP_TABLE_MAX + 1])(void) = { 0 };
/* [0] = count memoizado (N). En el ELF el writer (IOP) lo pone a N;
 *   el -1 era solo el modo "scan dinámico" que usaba cuando no lo sabía. */

static int cleanup_count = 0;

int cleanup_register(void (*fn)(void))
{
	if (!fn || cleanup_count >= CLEANUP_TABLE_MAX)
		return -1;

	cleanup_count++;
	g_cleanup_table[cleanup_count] = fn;   /* slot 1-based */
	g_cleanup_table[0] = cleanup_count;    /* memoizado: LIFO limpio, sin scan */
	return cleanup_count;
}

void run_cleanup_callbacks(void)
{
	int count = g_cleanup_table[0];   /* N (memoizado) */
	int i;

	/* [Fiel al asm] Si por alguna vía el header está a -1 (modo scan del ELF),
	 *   reproducimos el conteo dinámico, PERO saltando la entrada nula
	 *   (el quirk del jalr a 0 que en PC sería segfault). */
	if (count == -1)
	{
		count = 0;
		for (i = 1; g_cleanup_table[i] != 0; i++)
			count = i;
	}

	for (i = count; i >= 1; --i)
		if (g_cleanup_table[i] != 0)        /* <- tapón del off-by-one del asm */
			g_cleanup_table[i]();
}

int g_cleanup_done = 0;   /* DAT_0014186c */

void cleanup_run_once(void)
{
	if (g_cleanup_done == 0)
	{
		g_cleanup_done = 1;        /* set ANTES de limpiar (anti-re-entrancia) */
		run_cleanup_callbacks();
	}
	/* si ya = 1, no hace nada: idempotente */
}
