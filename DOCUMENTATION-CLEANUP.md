# Documentation Cleanup

## Analysis of docs/ folder

Total: 84 .md files

### Files Created in This PR
- LLM.md (new - AI workflow)
- SOURCE-LAYOUT.md
- CONTRIBUTOR-RESERVATIONS.md
- CAMPAIGN-WORKFLOW.md
- PR-DESCRIPTIONS.md
- CONTINUE.md
- CONTRIBUTOR-QUICKSTART.md
- SDK-PROFILE-QUALIFICATION.md
- BOOT-SHARED-CODE-VERIFICATION.md
- REGIONS.md
- RAC1-TO-RAC2.md
- PROTOTYPE-BUILDS.md
- MERGE-QUEUE.md
- COMPILER-NOTES.md
- COMMUNITY-ENGINE-REFERENCE.md
- LEVEL-INTEGRATION-PLAN.md
- LEVEL-NATIVE-C.md
- LEVEL-ARCHIVE-FORMAT.md
- MOBY-DISPATCH-TABLES.md
- ASSERT-MESSAGE-NAMES.md
- ENGINE-SYMBOL-NAMES.md
- NORMALIZED-FAMILY-WORKFLOW.md
- NORMALIZED-FAMILY-VALIDATION.md
- RAC1-DECOMP-FINDINGS.md
- RAC1-FP-BIT-EVIDENCE.md
- DUPLICATION-DATA-GP-AUDIT.md
- SMALL-DATA-UNIT-EVIDENCE.md
- SDK-SYSBIT-EVIDENCE.md
- SDK-TRACK-IPU-EVIDENCE.md
- SDK-CPR8-EVIDENCE.md
- SDK-RESTART-DMA-EVIDENCE.md
- GP-CALLER-EVIDENCE.md
- GLOBAL-CODE-REUSE.md
- GLOBAL-UNIQUE-CODE.md
- SCALAR-LEAF-PAIR-EVIDENCE.md
- KNOWN-BOOT-SHARED-PLACEMENTS-EVIDENCE.md
- KNOWN-SCALAR-AND-STATE-REUSE-EVIDENCE.md
- SHARED-FAMILY-BATCH-EVIDENCE.md
- STATE-BYTE-DISPATCH204-EVIDENCE.md
- RESIDENT-WRAPPER-REUSE-EVIDENCE.md
- VU-MICROPROGRAMS.md
- UPDATE-MOBY775-76-EVIDENCE.md
- QWEN-BACKLOG-CLOSEOUT.md
- QWEN-RECOVERY-41K-EVIDENCE.md
- NATIVE-SCALAR-41K-EVIDENCE.md
- NATIVE-PARENT-40K-EVIDENCE.md
- NATIVE-GP-46K-EVIDENCE.md
- MPEG-DUAL-PRIME-EVIDENCE.md
- LOCAL-GP-PROOF.md
- PR54-RESIDENT-CELL-CLEAR-EVIDENCE.md
- PCSX2-VALIDATION.md
- ALL C LOT FILES (FIRST-C-LOT through TWENTY-SEVENTH-C-LOT)

### Redundant Files (Should Be Summarized/Shortened)

#### Lot Files (47 files)
The 47 C lot files (FIRST-C-LOT.md through TWENTY-SEVENTH-C-LOT.md) contain:
- Historical campaign progress
- Trial results
- Function completion status

**Recommendation:** These should be summarized into a single "CAMPAIGN-PROGRESS.md" that:
- Shows current coverage percentage
- Links to detailed progress in progress/ directory
- Mentions major milestones
- Provides summary statistics only

#### Evidence Files (21 files)
Files like *-EVIDENCE.md contain:
- Specific technical findings
- Proof data
- Measurement results

**Recommendation:** These should be shortened to:
- One sentence summary of the finding
- Link to detailed proof data in progress/ or build artifacts
- Remove raw measurement data (already in tracked files)

#### Workflow Files (5 files)
- CAMPAIGN-WORKFLOW.md (comprehensive)
- NORMALIZED-FAMILY-WORKFLOW.md (detailed)
- CONTINUE.md (concise)
- START-HERE.md (comprehensive setup guide)
- MERGE-QUEUE.md (short)

**Recommendation:** Keep these as-is - they are essential workflow documentation

### Files That Should Be Removed/Archived

1. **QWEN-BACKLOG-CLOSEOUT.md** - Internal note about task completion
2. **UPDATE-MOBY775-76-EVIDENCE.md** - Single function evidence, already in proof
3. **QWEN-RECOVERY-41K-EVIDENCE.md** - Technical investigation, already resolved

### Summary Recommendations

**Keep (Essential):**
- START-HERE.md - New contributor entry point
- CONTINUE.md - Ongoing workflow
- CAMPAIGN-WORKFLOW.md - Core process
- SOURCE-LAYOUT.md - Source organization
- CONTRIBUTOR-QUICKSTART.md - Quick start guide
- CONTRIBUTOR-RESERVATIONS.md - Task claiming
- PR-DESCRIPTIONS.md - PR template
- BUILD.md - Build system (new)
- LLM.md - AI workflow (new)
- COMPILER-NOTES.md - Compiler info
- REGIONS.md - Region documentation
- RAC1-TO-RAC2.md - Migration guide
- NORMALIZED-FAMILY-WORKFLOW.md - Shared code workflow

**Shorten (Reduce to 1-2 sentences + link):**
- All *-EVIDENCE.md files - Summarize finding, link to proof
- All C LOT files - Consolidate into CAMPAIGN-PROGRESS.md

**Remove:**
- QWEN-BACKLOG-CLOSEOUT.md - Internal note
- UPDATE-MOBY775-76-EVIDENCE.md - Already in proof
- QWEN-RECOVERY-41K-EVIDENCE.md - Resolved technical finding
