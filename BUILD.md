# Build System

This repository uses a matching build system based on the approach used by
RC2-Going-Decompiled. The goal is to produce a byte-identical executable from
C source and splat-generated assembly.

## Prerequisites

You need:

1. **Your legally obtained copy of the game** - The ISO or extracted ROM
2. **SN Systems ProDG toolchain** - ee-gcc 2.9-ee-991111b/r4
3. **Docker with colima** (for x86 Windows binary support)

## Directory Structure

```
going-decompiled/
├── asm/               # Splat-generated assembly (per region)
│   ├── usa/
│   └── eu/
├── build/             # Build outputs (per region)
│   ├── usa/
│   └── eu/
├── config/            # Per-region configuration
├── include/           # Public headers
├── linker_scripts/    # Generated linker scripts
├── src/               # Hand-written C source
├── symbol_addrs/      # Symbol address definitions
└── libgcc/            # GCC source for libgcc.a
```

## Building

### USA Region

```bash
# Inside the ee-build container
REGION=usa sh tools/ee/build.sh usa
```

### EU Region

```bash
REGION=eu sh tools/ee/build.sh eu
```

## Build Process

1. **Assembly** - Assembles plain .s files with VU0 fixup (Q/ACC → $Q/$ACC)
2. **Compilation** - Compiles C units with region-specific flags:
   - Default: -G0 -O2
   - Specific units: -G8 for small-data optimization
3. **Splicing** - Replaces selected functions with SN 2.95.3 output
4. **Linking** - Links with auto-defined symbols and libgcc
5. **Verification** - Compares byte-for-byte with extracted ROM

## Key Scripts

- `tools/ee/build.sh` - Main build orchestration
- `tools/ee/ee_cc1.sh` - EE C/C++ compiler wrapper
- `tools/ee/asm_unit.sh` - Assembles compiled units with VU0 fixup
- `tools/ee/s136os_splice.sh` - SN 2.95.3 compiler splicing
- `tools/ee/vu0_fixup.sed` - VU0 macro fixup for GNU-as

## Symbol Resolution

The build system automatically defines symbols:

1. **D_XXX** - Auto-symbols from splat
2. **func_XXX** - Function symbols from splat
3. **symbol_addrs/** - Hand-defined symbols
4. **libgcc** - GCC library symbols

## Compiler Flags

- **-G0** - Default (no small-data optimization)
- **-G8** - For functions accessing %gp_rel data
- **-fno-gcse** - For specific units requiring it
- **-fno-strict-aliasing** - For aliasing-sensitive units

##ASM Mirroring

The build uses a warm mirror system to avoid rebuilding nonmatchings for every
unit (~3 min per unit over slow mounts). The mirror is fingerprinted by its
inputs (.s files, vu0_fixup.sed, macro.inc, asm_unit.sh) so changes select
a new mirror path.

## Troubleshooting

### Build fails with "byte-exact" mismatch

1. Check that your baserom matches the expected SHA-1
2. Verify all toolchain binaries have the correct SHA-256
3. Check for stale build artifacts (`rm -rf going-decompiled/build/*`)

### Linker errors

- Missing symbols: Add to symbol_addrs/REGION/symbol_addrs.txt
- Section overlap: Check linker script addresses
- libgcc missing: Run `tools/ee/build_libgcc.sh` first

### VU0 macro errors

The VU0 fixup should catch most issues. Check that vu0_fixup.sed is applied
before assembly.

## References

- RC2-Going-Decompiled: https://github.com/Promises/RC2-Going-Decompiled
- Matching decompilation: https://github.com/cawka/mm-decomp
