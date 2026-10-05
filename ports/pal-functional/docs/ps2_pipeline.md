## Function Register

| PS2 Offset       | Ghidra Name      | PC Name            | File                          | Notes |
|------------------|------------------|--------------------|-------------------------------|-------|
| 0x0011b3d0       | FUN_0011b3d0     | kernel_cache_sync  | src/core/sys/kernel_cache.c   | __cache libc. No-op on PC + fence. |
| (internal)       | ProcessCache     | (inline)           | src/core/sys/kernel_cache.c   | c0,k,c0 → fence. |
| (internal)       | kernel_system_sync_guard   | kernel_system_sync_guard   | same file | no-op |
| (internal)       | kernel_system_sync_release | kernel_system_sync_release | same file | no-op |

| PS2 Offset       | Ghidra Name      | PC Name                    | File                                 | Notes |
|------------------|------------------|----------------------------|--------------------------------------|-------|
| 0x0011b5f0       | FUN_0011b5f0     | kernel_enable_interrupts   | src/core/sys/kernel_interrupts.c     | _EnableIntc + sync 0. No-op on PC. |
| (internal)       | _EnableIntc      | (inline in kernel_enable_interrupts) | same file | Status.IE → no-op. |

The offsets are PAL (`SCES_516.07`) addresses.
