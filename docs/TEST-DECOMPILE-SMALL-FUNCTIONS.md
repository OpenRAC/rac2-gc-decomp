# Test Decompile - Small Functions

## Small Functions (0-100 bytes) to Test

Based on `scripts/function_size_rank.py` analysis:

1. **func_002FCFC8** - 40 bytes
   - File: `going-decompiled/src/usa/text/1FCF48.cpp`
   - Purpose: Select scene arena region
   - Category: small
   - Status: Already decompiled (matched)

2. **func_002FD020** - 32 bytes  
   - File: `going-decompiled/src/usa/text/1FCF48.cpp`
   - Purpose: Derive render task list
   - Category: small
   - Status: Already decompiled (matched)

3. **func_002FFCE0** - 32 bytes
   - File: `going-decompiled/src/usa/text/1FFBA0.cpp`
   - Purpose: Clear sound pool slot
   - Category: small
   - Status: Already decompiled (matched)

4. **func_00300118** - 16 bytes
   - File: `going-decompiled/src/usa/text/1FFBA0.cpp`
   - Purpose: Empty/no-op leaf
   - Category: small
   - Status: Already decompiled (matched)

5. **func_002FFCE0** - 32 bytes
   - File: `going-decompiled/src/usa/text/1FFBA0.cpp`
   - Purpose: Clear sound pool slot
   - Category: small
   - Status: Already decompiled (matched)

## Function Size Ranking Verification

```bash
# Run function size rank to verify categorization
python3 scripts/function_size_rank.py --format compact --limit 10

# Expected output format:
# func_002FCFC8 size 40 small
# func_002FD020 size 32 small  
# func_002FFCE0 size 32 small
# func_00300118 size 16 small
# func_002FFCE0 size 32 small
```

## Build System Testing

```bash
# Run the build system test
./test_build_system.sh

# Expected: Check baserom, build tools, directory structure
```

## Workflow for New Contributors

1. **Discover functions**: `python3 scripts/function_size_rank.py --category small`
2. **Analyze in Ghidra**: Set boundaries, rename variables
3. **Check size**: Verify with `function_size_rank.py`
4. **Implement**: Create C file with proper INCLUDE_ASM
5. **Build**: `REGION=usa sh tools/ee/build.sh usa`
6. **Verify**: Byte-exact match required
7. **Test**: Run `tests/test_function_size_rank_simple.py`

## Tools Summary

- **scripts/function_size_rank.py**: Function analysis and categorization
- **tools/ee/build.sh**: Main matching build system
- **tools/ee/ee_cc1.sh**: EE compiler wrapper  
- **tools/ee/asm_unit.sh**: Assemble with VU0 fixup
- **tools/ee/s136os_splice.sh**: SN compiler splicing
- **test_build_system.sh**: Build verification script
- **tests/test_function_size_rank_simple.py**: Unit tests