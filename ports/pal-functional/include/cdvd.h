#ifndef CDVD_H
#define CDVD_H

extern int g_CdvdNcmdInitialized;
extern int g_CdvdCurrentCommand;

/**
 * @brief Initializes the emulated file-reading system for PC.
 * @param mode Original PS2 initialization mode.
 * @return 1 on success, 0 on failure.
 */
int sceCdInit(int mode);

// ... Keep the previous declarations (sceCdInit, etc.) ...

/**
 * @brief Stops the virtual spindle motor of the PC disc reader.
 * @return Always returns 0 for compatibility with the original kernel.
 */
int sceCdStop(void);

/**
 * @brief Reads a block of simulated data sectors from the extracted folder on PC.
 * @param sector_start First logical sector (Logical Sector Number).
 * @param sector_count Number of 2048-byte sectors to read.
 * @param dest_buffer Destination pointer in the game's RAM.
 * @param mode_struct Structure holding the read-mode flags.
 * @return 1 if the transfer started successfully, 0 on failure.
 */
int sceCdRead(unsigned int sector_start, int sector_count, unsigned int dest_buffer, unsigned char* mode_struct);

#endif // CDVD_H
