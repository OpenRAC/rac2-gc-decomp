# English



# Español

## Registro de funciones

| Offset PS2       | Nombre Ghidra    | Nombre PC          | Archivo                       | Notas |
|------------------|------------------|--------------------|-------------------------------|-------|
| 0x0011b3d0       | FUN_0011b3d0     | kernel_cache_sync  | src/core/sys/kernel_cache.c   | __cache libc. No-op en PC + fence. |
| (interna)        | ProcessCache     | (inline)           | src/core/sys/kernel_cache.c   | c0,k,c0 → fence. |
| (interna)        | kernel_system_sync_guard   | kernel_system_sync_guard   | ibídem | no-op |
| (interna)        | kernel_system_sync_release | kernel_system_sync_release | ibídem | no-op |

| Offset PS2       | Nombre Ghidra    | Nombre PC                  | Archivo                              | Notas |
|------------------|------------------|----------------------------|--------------------------------------|-------|
| 0x0011b5f0       | FUN_0011b5f0     | kernel_enable_interrupts   | src/core/sys/kernel_interrupts.c     | _EnableIntc + sync 0. No-op en PC. |
| (interna)        | _EnableIntc      | (inline en kernel_enable_interrupts) | ibídem | Status.IE → no-op. |
