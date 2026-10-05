// include/core/ee_memory.h
#ifndef RAC2_CORE_EE_MEMORY_H
#define RAC2_CORE_EE_MEMORY_H

#include <stdint.h>

/*
 * Emulated Emotion Engine main memory.
 *
 * The reconstructed functions still read and write the game's globals at
 * their PS2 addresses (for example *(u32*)0x00141C1C). On PC those addresses
 * are not mapped, so every such access goes through EE_ADDR(), which places it
 * inside a 32 MB buffer that mirrors the console's RAM. The KSEG and uncached
 * mirrors (0x20000000 | addr) fold onto the same bytes, as on hardware.
 *
 * ee_memory_load_boot_elf() copies the loadable segments of the user's own
 * boot executable (orig/<RAC2_BOOT_SERIAL>) into this buffer, so the static
 * data the functions expect (strings, tables, initialized globals) sits at
 * its original address. The addresses in the sources are PAL addresses: the
 * USA executable loads, but its data lives at different addresses.
 */

#define EE_RAM_SIZE  0x02000000u   /* 32 MB retail console */

#if defined(PLATFORM_PS2)
#define EE_ADDR(addr) ((void*)(uintptr_t)(addr))
#else
extern uint8_t g_ee_ram[EE_RAM_SIZE];
#define EE_ADDR(addr) ((void*)(g_ee_ram + ((uintptr_t)(addr) & (EE_RAM_SIZE - 1u))))
#endif

/* Loads the PT_LOAD segments of a 32-bit little-endian MIPS ELF into the
 * emulated RAM. Returns the number of segments loaded, or -1 if the file is
 * missing or is not such an ELF. */
int ee_memory_load_boot_elf(const char* path);

#endif // RAC2_CORE_EE_MEMORY_H
