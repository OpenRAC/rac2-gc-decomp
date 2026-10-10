# LLM Workflow for Ratchet & Clank 2 Decompilation

This document outlines the workflow that AI assistants should follow when
contributing to the rac2-gc-decomp project.

## Overview

The rac2-gc-decomp project is a **matching decompilation** of *Ratchet & Clank 2:
Going Commando* (PS2, 2003). The goal is to produce C source that, when compiled,
reassembles byte-for-byte to the original retail executable.

## Prerequisites

### Legal Requirements

1. **You must legally own a copy** of the game (disc or PSN ISO)
2. **Extract your own copy** of the executable (`SCUS_972.68` for USA v2.00)
3. **Place it in `baserom/`** - this is gitignored and never committed

### Toolchain Setup

1. **Install dependencies**:
   ```bash
   pip install -r requirements.txt
   ```

2. **Set up the build environment**:
   - Install SN Systems ProDG ee-gcc 2.9-ee-991111b/r4
   - Set up Docker with colima for x86 Windows binary support
   - Or use a cross-compiler toolchain

## Workflow Steps

### 1. Discover Functions to Decompile

Use the function size ranking tool to find suitable functions:

```bash
# Rank all functions by size
python scripts/function_size_rank.py --format compact | head -20

# Get small functions (good for beginners)
python scripts/function_size_rank.py --category small --limit 10

# Get functions of specific size range
python scripts/function_size_rank.py --min-size 100 --max-size 300
```

### 2. Analyze Function

For each target function:

1. **Extract disassembly** using splat
2. **Analyze in Ghidra**:
   - Set function boundaries
   - Rename variables
   - Document logic
   - Extract pseudocode

3. **Check function size** (verify with function_size_rank.py):
   ```bash
   python3 scripts/function_size_rank.py --format compact | grep func_XXXXXX
   # Expected: func_XXXXXX size N [small|medium|big]
   ```

### 3. Verify Function Size

```bash
# Get exact function size
python scripts/function_size_rank.py --format compact | grep "func_00123456"
```

### 4. Create Decompile Unit

1. **Identify the region** (USA/EU) and **segment** (text/cod/lit)
2. **Check existing units** in `going-decompiled/src/`
3. **Create new unit** if needed:
   - Create directory: `going-decompiled/src/usa/text/`
   - Create C file: `func_00123456.c`
   - Add `INCLUDE_ASM` for non-matching parts

### 5. Implement Decompile

```c
#include "common.h"

// Function header from Ghidra analysis
void func_00123456(int param1, int param2) {
    // Implement logic from disassembly
    // Match register usage and stack layout
}
```

### 6. Build and Verify

```bash
# Run the build system
REGION=usa sh tools/ee/build.sh usa

# Check for byte-exact match
# Expected: "MATCH: byte-identical .rom"
```

### 7. Handle Build Errors

If the build doesn't match:

1. **Check symbol addresses** in `symbol_addrs/usa/symbol_addrs.txt`
2. **Verify compiler flags** in `tools/ee/build.sh` GFLAG overrides
3. **Inspect disassembly** for misidentified boundaries
4. **Check VU0 fixup** for VU0 macro issues

## Function Categorization

### Small Functions (0-100 bytes)

Good for beginners. Examples:
- Simple math helpers
- Getters/setters
- Small state transitions

**Verification**: Use `function_size_rank.py` to verify categorization:
```bash
python3 scripts/function_size_rank.py --format compact | grep func_XXXXXX
# Expected: func_XXXXXX size N small
```

### Medium Functions (101-500 bytes)

Moderate complexity. Examples:
- UI element handlers
- Game state machines
- Helper functions with multiple cases

### Big Functions (501+ bytes)

Complex functions. Examples:
- Main game loops
- Level loaders
- Cutscene systems

## Testing Strategy

### Unit Testing

Create test files in `tests/`:

```python
# tests/test_func_00123456.py
def test_function_boundaries():
    """Verify function starts at correct address"""
    pass

def test_size_verification():
    """Verify function size matches expected"""
    pass
```

### Build Verification

```bash
# Verify build process
sh tools/ee/build.sh usa

# Check byte-exact match
cmp extracted/usa/SCUS_972.68.rom going-decompiled/build/usa/SCUS_972.68.rom
```

## Function Size Ranking Verification

### Using function_size_rank.py

The `scripts/function_size_rank.py` script provides comprehensive function analysis:

```bash
# Rank all functions by size
python3 scripts/function_size_rank.py --format compact | head -20

# Filter by category
python3 scripts/function_size_rank.py --category small --limit 10

# Filter by size range
python3 scripts/function_size_rank.py --min-size 100 --max-size 300

# Get JSON output for automation
python3 scripts/function_size_rank.py --format json

# Get CSV output for spreadsheets
python3 scripts/function_size_rank.py --format csv
```

### Compact Format

The compact format is ideal for scripting:
```
func_001163A0 size 64 small
func_00116580 size 128 medium
func_00116780 size 256 medium
```

### Test Coverage

Run unit tests for the function size ranking:
```bash
python3 tests/test_function_size_rank_simple.py
```

Expected: 100% pass rate for categorization and filtering.

## Common Patterns

### Small-Data Access (-G8)

Functions accessing `sdata` must use `-G8`:

```bash
# Add to build.sh GFLAG case
*/usa/text/XXXXXX.c) GFLAG="-G8";;
```

### VU0 Macro Handling

VU0 (COP2) uses special registers Q and ACC:

- Q → `$Q`
- ACC → `$ACC`

The `vu0_fixup.sed` handles this automatically.

### Include ASM Stubs

For non-matching parts:

```c
#define INCLUDE_ASM
#include "asm/usa/text/func_00123456.s"
```

## Troubleshooting

### Build Fails

1. Check baserom SHA-1 matches expected
2. Verify toolchain binaries have correct SHA-256
3. Clean build artifacts: `rm -rf going-decompiled/build/*`

### Match Not Found

1. Verify symbol addresses
2. Check compiler flags match original
3. Inspect disassembly for boundary errors

### Symbol Errors

1. Add to `symbol_addrs/usa/symbol_addrs.txt`
2. Run build to regenerate undefined symbols

## Resources

- **Architecture Manual**: `docs/DECOMPILED-ARCHITECTURE-MANUAL.md`
- **Function Categories**: `docs/FUNCTION-CATEGORIES.md`
- **Build System**: `BUILD.md`
- **RAC2-Going-Decompiled**: https://github.com/Promises/RC2-Going-Decompiled

## Quality Checklist

Before submitting:

- [ ] Function size verified with `function_size_rank.py`
- [ ] Build produces byte-exact match
- [ ] Symbol addresses documented
- [ ] Compiler flags correct (-G0 vs -G8)
- [ ] VU0 macros fixed
- [ ] No copyrighted material committed
- [ ] Test cases pass
