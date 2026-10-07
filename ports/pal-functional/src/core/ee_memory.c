/*
 *  ee_memory.c – emulated EE RAM and boot executable loader (see core/ee_memory.h).
 */

#include "core/ee_memory.h"
#include <stdio.h>
#include <string.h>

#if !defined(PLATFORM_PS2)
_Alignas(64) uint8_t g_ee_ram[EE_RAM_SIZE];
#endif

static uint32_t read_u32(const uint8_t* p) { return p[0] | (p[1] << 8) | (p[2] << 16) | ((uint32_t)p[3] << 24); }
static uint16_t read_u16(const uint8_t* p) { return (uint16_t)(p[0] | (p[1] << 8)); }

int ee_memory_load_boot_elf(const char* path)
{
#if defined(PLATFORM_PS2)
	(void)path;
	return 0;   /* the console already loaded it */
#else
	FILE* file = fopen(path, "rb");
	uint8_t header[52], program[32];
	int loaded = 0;

	if (file == NULL)
		return -1;
	/* ELF32, little-endian, MIPS (EM_MIPS = 8) */
	if (fread(header, 1, sizeof(header), file) != sizeof(header)
		|| memcmp(header, "\177ELF\001\001", 6) != 0 || read_u16(header + 18) != 8) {
		fclose(file);
		return -1;
	}
	uint32_t phoff = read_u32(header + 28);
	uint16_t phentsize = read_u16(header + 42), phnum = read_u16(header + 44);
	for (uint16_t i = 0; i < phnum; i++) {
		if (phentsize < sizeof(program) || fseek(file, (long)(phoff + (uint32_t)i * phentsize), SEEK_SET) != 0
			|| fread(program, 1, sizeof(program), file) != sizeof(program))
			break;
		if (read_u32(program) != 1)   /* PT_LOAD */
			continue;
		uint32_t offset = read_u32(program + 4), vaddr = read_u32(program + 8) & (EE_RAM_SIZE - 1u);
		uint32_t filesz = read_u32(program + 16), memsz = read_u32(program + 20);
		if (filesz > memsz || memsz > EE_RAM_SIZE - vaddr)
			continue;
		if (fseek(file, (long)offset, SEEK_SET) != 0 || fread(g_ee_ram + vaddr, 1, filesz, file) != filesz)
			continue;
		memset(g_ee_ram + vaddr + filesz, 0, memsz - filesz);   /* .bss */
		loaded++;
	}
	fclose(file);
	return loaded;
#endif
}
