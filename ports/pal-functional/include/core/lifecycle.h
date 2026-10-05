#ifndef LIFECYCLE_H
#define LIFECYCLE_H

/*
 * Lista de callbacks de cierre del motor (LIFO).
 *   g_cleanup_table[0]  = header (-1 = "lista terminada en 0")
 *   g_cleanup_table[1..N] = punteros a funciones de cleanup
 *   g_cleanup_table[N+1]  = 0 (sentinela)
 *
 * En PS2 la llenaba el loader del IOP (otro binario, vía SIF/RPC).
 * En PC la llenamos nosotros: cada recurso que se crea registra aquí
 * su destructor, y run_cleanup_callbacks() los ejecuta en reversa al salir.
 */
extern void (*g_cleanup_table[])(void);

/* Registra un destructor. Devuelve el slot asignado, o -1 si el array está lleno. */
int  cleanup_register(void (*fn)(void));

/* Ejecuta todos los callbacks registrados, del último al primero (LIFO).
 *   Con la tabla vacía (BSS=0) es un no-op seguro. */
void run_cleanup_callbacks(void);

/* Flag "¿ya ejecuté el cleanup?" (DAT_0014186c).
 *   0 = pendiente, 1 = ya corrió los destructores. */
extern int g_cleanup_done;

/* Ejecuta run_cleanup_callbacks() UNA sola vez.
 *   El guard se setea ANTES de ejecutar (protección anti-re-entrancia:
 *   un callback que llame a cleanup_run_once de nuevo no re-entra). */
void cleanup_run_once(void);

#endif /* LIFECYCLE_H */
