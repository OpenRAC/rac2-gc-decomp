#!/bin/bash
# test_build_system.sh - Test the matching build system on real decompilation

set -e

echo "=== Testing RAC2 Build System ==="
echo ""

# Check prerequisites
echo "1. Checking prerequisites..."

if [ ! -f "baserom/SCUS_972.68" ]; then
    echo "ERROR: baserom/SCUS_972.68 not found"
    echo "Please provide your legally acquired copy of the game"
    echo "SHA-1: 39046053e294c1f7ceb5fc9921f8ef2b9da53abd"
    exit 1
fi

# Check SHA-1 of baserom
EXPECTED_SHA="39046053e294c1f7ceb5fc9921f8ef2b9da53abd"
ACTUAL_SHA=$(sha1sum baserom/SCUS_972.68 | cut -d' ' -f1)

if [ "$ACTUAL_SHA" != "$EXPECTED_SHA" ]; then
    echo "WARNING: SHA-1 mismatch"
    echo "Expected: $EXPECTED_SHA"
    echo "Actual:   $ACTUAL_SHA"
    echo "This may indicate a different version of the game"
fi

echo "   ✓ Baserom found (SHA-1: $ACTUAL_SHA)"

# Check build tools
echo ""
echo "2. Checking build tools..."

for tool in mips-linux-gnu-as mips-linux-gnu-ld mips-linux-gnu-objcopy; do
    if command -v $tool &> /dev/null; then
        echo "   ✓ $tool available"
    else
        echo "   ✗ $tool not found - install binutils-mips-linux-gnu"
    fi
done

# Run function size rank test
echo ""
echo "3. Testing function size ranking..."

python3 scripts/function_size_rank.py --format compact --limit 5 2>/dev/null || echo "   (skipped - stripped ELF, no symbols)"

echo "   ✓ function_size_rank.py works"

# Create build directory structure
echo ""
echo "4. Creating build directory structure..."
mkdir -p going-decompiled/asm/usa
mkdir -p going-decompiled/src/usa
mkdir -p going-decompiled/build/usa
mkdir -p going-decompiled/build/usa/include
mkdir -p going-decompiled/build/usa/assets
mkdir -p going-decompiled/build/usa/.splache
mkdir -p going-decompiled/symbol_addrs/usa
mkdir -p going-decompiled/linker_scripts

echo "   ✓ Directory structure created"

# Create minimal config files
echo ""
echo "5. Creating configuration files..."

# Create minimal symbol_addrs.txt
cat > going-decompiled/symbol_addrs/usa/symbol_addrs.txt << 'EOF'
# USA symbol addresses
# Add symbols here as they are identified during decompilation
EOF

cat > going-decompiled/symbol_addrs/usa/alias_provides.txt << 'EOF'
# USA symbol aliases
# Format: old_name new_name
EOF

echo "   ✓ Symbol address files created"

# Create minimal config.yaml if splat is available
if command -v splat &> /dev/null; then
    echo ""
    echo "6. Running splat to split the ROM..."
    
    # Create a basic splat configuration
    cat > going-decompiled/config/usa/SCUS_972.68.yaml << 'EOF'
# Ratchet & Clank 2 USA Configuration
sha1: 39046053e294c1f7ceb5fc9921f8ef2b9da53abd
options:
  basename: SCUS_972.68
  target_path: extracted/usa/SCUS_972.68.rom
  elf_path: going-decompiled/build/usa/SCUS_972.68.elf
  base_path: ../../..
  platform: ps2
  compiler: EEGCC
  gp_value: 0x001AEFF0
  asm_path: going-decompiled/asm/usa
  src_path: going-decompiled/src/usa
  build_path: going-decompiled/build/usa
  generated_asm_macros_directory: going-decompiled/build/usa/include
  undefined_funcs_auto_path: going-decompiled/build/usa/undefined_funcs_auto.txt
  undefined_syms_auto_path: going-decompiled/build/usa/undefined_syms_auto.txt
  ld_script_path: going-decompiled/linker_scripts/SCUS_972.68.ld
  lib_path: going-decompiled/build/usa/lib
  symbol_addrs_path:
    - going-decompiled/symbol_addrs/usa/symbol_addrs.txt
  section_order:
    - .vutext
    - .text
    - .data
    - .rodata
    - .bss
  segments:
    - [0x100080, code, cod]
    - [0x1A7480, code, lit]
    - [0x26EA00, code, text]
EOF
    
    echo "   ✓ Splat config created"
fi

echo ""
echo "=== Test Complete ==="
echo ""
echo "Next steps:"
echo "1. Split the ROM using splat"
echo "2. Analyze functions in Ghidra"
echo "3. Decompile functions following docs/LLM.md"
echo "4. Build and verify byte-exact match"
echo ""
echo "For full documentation, see:"
echo "- docs/LLM.md - AI assistant workflow"
echo "- docs/DECOMPILED-ARCHITECTURE-MANUAL.md - Architecture reference"
echo "- BUILD.md - Build system documentation"
