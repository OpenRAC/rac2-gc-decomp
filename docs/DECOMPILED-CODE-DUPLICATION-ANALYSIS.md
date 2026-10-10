# Code Duplication Analysis

## Overview

This document analyzes the codebase for duplication and provides recommendations.

## File Counts

| Category | Count |
|----------|-------|
| C/C++ source files | 103 |
| Documentation (.md) | 85 |
| Python scripts | ~50+ |
| Shell scripts | ~10+ |
| JSON configs | 170+ |
| Total lines of C/C++ | 137,450 |

## Key Directories

### Source Code
- `src/` - Hand-written C source (4 files, 1,374 lines)
- `candidates/` - Generated standalone units (boot.c, 40 level units)
- `going-decompiled/` - Empty (gitignored per LEGAL.md)
- `ports/pal-functional/` - External port (8 files)

### Build/Toolchain
- `tools/ee/` - Matching build toolchain (15 scripts)
- `scripts/` - Build/test scripts (~50+ Python scripts)

### Configuration
- `config/` - 170+ JSON config files
- `symbol_addrs/` - Symbol address definitions

### Progress Tracking
- `progress/` - JSON progress tracking files
- `candidates.json` - Candidate functions
- `integration.json` - Integrated code info

## Duplication Issues

### 1. Documentation (85 files)

**Issue:** Too many documentation files with overlapping content

**Recommendation:**
- Consolidate 25 C-LOT files → `docs/CAMPAIGN-PROGRESS.md`
- Consolidate 20 EVIDENCE files → `docs/EVIDENCE-SUMMARY.md`
- Keep 10 core documentation files

### 2. Configuration Files (170+ JSON)

**Issue:** Multiple JSON files with overlapping data

**Recommendation:**
- `config/campaign-register.json` - Single source of truth
- `config/candidate-catalog.json` - Candidate catalog
- Consolidate overlapping config files

### 3. Build Scripts

**Issue:** Potential overlap between `scripts/` and `tools/ee/`

**Recommendation:**
- `tools/ee/` - Canonical matching build system
- `scripts/` - Helper scripts, campaign workflow
- Remove duplicate functionality

### 4. Progress Tracking

**Issue:** Multiple JSON files tracking progress

**Recommendation:**
- `progress/report.json` - Current status (source of truth)
- `progress/integration.json` - Integrated code info
- Consolidate overlapping progress files

## Symbol Map Import Plan

### Current Symbol Location
- `symbol_addrs/usa/symbol_addrs.txt` - Manual additions only
- `symbol_addrs/eu/symbol_addrs.txt` - Manual additions only

### Import Source
- `progress/candidates.json` - Contains function definitions
- `progress/integration.json` - Contains matched functions
- `config/source-layout.json` - Contains source fragment mappings

### Import Strategy
1. Read function addresses from `progress/integration.json`
2. Map to symbol names from `config/candidate-catalog.json`
3. Generate `symbol_addrs/usa/symbol_addrs.txt`
4. Generate `symbol_addrs/eu/symbol_addrs.txt`

## Deduplication Actions

### High Priority
1. **Import symbol addresses** from progress into symbol_addrs/
2. **Consolidate documentation** (85 → 10-15 core docs)
3. **Review build scripts** for overlaps

### Medium Priority
4. **Consolidate config files** (170 → 20-30)
5. **Clean up empty directories** (going-decompiled/)

### Low Priority
6. **Standardize naming conventions**
7. **Remove debug/legacy files**

## Recommendations

1. **Keep as-is:**
   - `tools/ee/` - Complete matching build toolchain
   - `progress/` - JSON progress tracking (source of truth)
   - `symbol_addrs/` - Manual symbol additions

2. **Consolidate:**
   - Documentation (85 → 15 core docs)
   - Config files (170 → 30 core files)

3. **Import:**
   - Symbol addresses from progress into symbol_addrs/

4. **Remove:**
   - Empty directories
   - Duplicate build scripts

## Next Steps

1. Import symbol addresses from `progress/integration.json`
2. Create consolidated documentation
3. Review and remove duplicate build scripts
4. Clean up empty directories
