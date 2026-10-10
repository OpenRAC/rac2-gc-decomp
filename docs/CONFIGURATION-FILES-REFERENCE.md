# Configuration Files Reference

This document explains the purpose of each JSON configuration file in the project.

## File Structure Overview

The project uses **JSON files for configuration and metadata**, not for runtime data. All JSON files are committed to the repository and serve as:

1. **Reference data** for the matching decompilation workflow
2. **Metadata** for build tools and verification scripts
3. **State tracking** for decompilation progress

## Key Configuration Files

### 1. Campaign Register (`config/campaign-register.json`) - **20MB**

**Purpose:** The main authority for campaign task assignments, decisions, and experiment history.

**Contains:**
- Task assignments and ownership
- Experiment history and trial results
- lot (batch) decisions
- Campaign queue state

**Why it's large:** This file tracks all decompilation work across the entire project, including:
- Every task assignment
- Trial results (successful/failed)
- Ruling decisions
- Evidence references

**Action:** Keep as-is. This is the source of truth for task assignments.

---

### 2. Source Layout (`config/source-layout.json`) - **5.9MB**

**Purpose:** Maps source fragments to original object boundaries.

**Contains:**
- `boot_modules` - Array of boot module definitions with:
  - `name` - Module identifier
  - `fragment` - Path to the .cfrag file
  - `boundary_evidence` - Description of how the boundary was determined

**Why it's large:** Contains all 27+ boot modules with detailed boundary evidence.

**Action:** Keep as-is. This is essential for build configuration.

---

### 3. Candidate Catalog (`config/candidate-catalog.json`) - **64KB**

**Purpose:** Catalog of candidate functions with their byte signatures and evidence.

**Contains:**
- Function names and addresses
- Evidence strings explaining how each function was identified
- SHA256 hashes of original and candidate code

**Structure:**
```json
{
  "evidence": "Description of how this catalog was generated",
  "externals": {
    "FUN_0011B580": 1261172,
    "memcpy": 1234567
  }
}
```

**Action:** Keep as-is. Essential for function identification.

---

### 4. Level Catalog (`config/level-catalog.json`) - **516KB**

**Purpose:** Catalog of level-specific functions and their properties.

**Contains:**
- Per-level function listings
- Evidence for each level's functions
- Region-specific information

**Action:** Keep as-is. Important for level-specific decompilation.

---

### 5. Progress Scope (`config/progress-scope.json`) - **32KB**

**Purpose:** Defines the scope of progress tracking.

**Contains:**
- Progress boundaries
- Scope definitions
- Region-specific settings

**Action:** Keep as-is. Used for progress reporting.

---

### 6. Boot Sections (`config/boot-sections.json`) - **16KB**

**Purpose:** Defines boot module section layout.

**Contains:**
- Section names and addresses
- Boot module boundaries
- Memory layout information

**Action:** Keep as-is. Critical for boot module builds.

---

### 7. Overlays (`config/overlays.json`) - **4KB**

**Purpose:** Lists all level overlays with their SHA256 checksums.

**Contains:**
- `levels` array with:
  - `level` - Level identifier
  - `sha256` - Checksum of the level overlay

**Example:**
```json
{
  "target": "SCUS_972.68",
  "levels": [
    {
      "level": "0_aranos_tutorial",
      "sha256": "c486f497920118066ed02076f540a5fad6811ce874d014a3a2bb18cb77714d11"
    }
  ]
}
```

**Action:** Keep as-is. Used for overlay verification.

---

### 8. Target (`config/target.json`) - **4KB**

**Purpose:** Defines the target platform and build configuration.

**Contains:**
- Target executable identifier
- Build configuration
- Platform-specific settings

**Action:** Keep as-is. Essential for build targeting.

---

## Function Catalog Directories

### `config/function-catalog/*.ndjson.gz`

**Purpose:** Per-level function catalogs with detailed metadata.

**Format:** Newline-delimited JSON (NDJSON) with gzip compression.

**Contains per entry:**
- Function name and address
- Evidence strings
- Byte signatures
- Category classifications

**Action:** Keep as-is. Used by build tools for function identification.

---

### `config/function-evidence/*.json.gz`

**Purpose:** Evidence files for specific function types.

**Examples:**
- `boot-bindings.json.gz` - Boot module binding evidence
- `pointer-arguments.json.gz` - Pointer argument evidence

**Action:** Keep as-is. Used for evidence verification.

---

### `config/level-candidates/*.json`

**Purpose:** Per-level candidate function listings.

**Contains:**
- Level-specific candidate functions
- Evidence strings
- SHA256 hashes

**Action:** Keep as-is. Used for level-specific builds.

---

### `config/level-g8/*.json`

**Purpose:** Level candidates with g8 (non-matchable) status.

**Contains:**
- G8-level candidates
- Non-matchable function information

**Action:** Keep as-is. Used for progress reporting.

---

### `config/level-native/*.json`

**Purpose:** Level candidates for native (host) builds.

**Contains:**
- Native build candidates
- Host-specific configuration

**Action:** Keep as-is. Used for native builds.

---

### `config/regions/*.json`

**Purpose:** Region-specific configuration.

**Contains:**
- Region definitions
- Region-specific symbol mappings

**Action:** Keep as-is. Used for multi-region builds.

---

## Summary Table

| File | Size | Purpose | Keep? |
|------|------|---------|-------|
| campaign-register.json | 20MB | Task assignments & experiment history | **YES** - Source of truth |
| source-layout.json | 5.9MB | Source fragment to object mapping | **YES** - Build config |
| candidate-catalog.json | 64KB | Function candidate catalog | **YES** - Function ID |
| level-catalog.json | 516KB | Level-specific function catalog | **YES** - Level builds |
| progress-scope.json | 32KB | Progress tracking scope | **YES** - Progress |
| boot-sections.json | 16KB | Boot module sections | **YES** - Boot config |
| overlays.json | 4KB | Level overlay checksums | **YES** - Overlay verification |
| target.json | 4KB | Build target definition | **YES** - Target config |
| function-catalog/*.ndjson.gz | ~2MB total | Per-level function metadata | **YES** - Function ID |
| function-evidence/*.json.gz | ~50KB total | Function evidence | **YES** - Evidence |
| level-candidates/*.json | ~100KB total | Per-level candidates | **YES** - Level builds |
| level-g8/*.json | ~100KB total | Non-matchable candidates | **YES** - Progress |
| level-native/*.json | ~100KB total | Native build candidates | **YES** - Native builds |
| regions/*.json | ~10KB total | Region configuration | **YES** - Multi-region |

---

## Deduplication Recommendations

### Can Be Consolidated:

1. **Function Catalog** - 25+ NDJSON files could be consolidated into a single catalog with level identifiers
2. **Evidence Files** - Multiple evidence files could be merged into a single evidence index

### Should Keep Separate:

1. **Campaign Register** - Must remain as single source of truth
2. **Source Layout** - Essential for build configuration
3. **Progress Scope** - Required for progress reporting

### Can Be Compressed Further:

- Consider zstd compression for larger NDJSON files
- Consider database format for frequently queried data

---

## Next Steps

1. Import symbol addresses from Going Native (completed)
2. Consolidate function catalog files if needed
3. Create evidence index file if needed
4. Document the relationship between files
