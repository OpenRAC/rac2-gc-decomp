#ifndef CDVD_H
#define CDVD_H

extern int g_CdvdNcmdInitialized;
extern int g_CdvdCurrentCommand;

/**
 * @brief Inicializa el sistema de lectura de archivos emulado para PC.
 * @param mode Modo de inicialización original de la PS2.
 * @return 1 para éxito, 0 para fallo.
 */
int sceCdInit(int mode);

// ... Mantener lo anterior (sceCdInit, etc.) ...

/**
 * @brief Detiene el motor de rotación virtual del lector de discos en PC.
 * @return Siempre retorna 0 por compatibilidad con el Kernel original.
 */
int sceCdStop(void);

/**
 * @brief Lee un bloque de sectores de datos simulados desde la carpeta extraída en PC.
 * @param sector_start Sector lógico inicial (Logical Sector Number).
 * @param sector_count Cantidad de sectores de 2048 bytes a leer.
 * @param dest_buffer Puntero de destino en la memoria RAM del juego.
 * @param mode_struct Estructura con flags del modo de lectura.
 * @return 1 para éxito en el inicio de la transferencia, 0 para fallo.
 */
int sceCdRead(unsigned int sector_start, int sector_count, unsigned int dest_buffer, unsigned char* mode_struct);

#endif // CDVD_H
