# Source-specific IPU DMA restart owner

The fixed attributed IPU restart source was compiled once with the owned SDK driver, compiler and assembler, stripped with the fixed pipeline, and linked with the pinned SN linker. Its complete 336-byte boot body at 0x00130C68 matches the original reference without masks or trimmed extents.

The source is MIT-licensed Lombyte code, copyright 2026 Mateusz Kłysz, from src/sdk/dma/sce_ipu_restart_dma.c. Its original source hash is d9666f6500d013b7442df964b74a35165cc90ae2855f0179afcc29313214de7b. The complete standalone source hash is 590d6a10dcc6883cd4724257300aaf6c5f5fe25e5745b60d463918039ba81bea. Only the original include is replaced with its exact two scalar typedefs, and the full MIT notice accompanies the source. Expressions, statements, types, field names, external declarations and ordering are unchanged.

The actual stripped object hash is a056d3afe14484ee659a783ca86a95d5c1bd31347557fb630e0ea41815f3de8c. It contains one complete .text function aligned to eight bytes, with exactly two R_MIPS_26 rows: offset0x7C is zero-addend JAL to SetD3Chcr at0x00130AB0; offset0x130 is a zero-addend restored-frame tail J to SetD4Chcr at0x00130B18. Both input helper symbols are global undefined NOTYPE. Neither helper is emitted or credited as new C.

The two original 100-byte setters and their transitive interrupt helpers remain opaque original code. The fixed linker assigns their exact absolute addresses. Its absolute symbols may be GLOBAL OBJECT metadata; this does not create function bodies or original data ownership. There are no additional allocated data or readonly sections, GP use, helper stubs or veneers.

The complete linked body hash is d9993319706cf0ddfe71dae0fcdfb3302b7d3773f59d15b8165514f4e4034a86, with zero differing reference bytes. The actual ELF hash is 384c8e9f09bc78b8b4c753759f36b142bc16b124aee8402aec96694aef5b662d.

This admission is limited to this exact source unit. The leaf controls, sysbit152 owner, CPR8 owner and default compiler remain separate. It does not qualify arbitrary SDK calls or tail jumps, identify original game type names, or establish DMA/interrupt hardware execution. Publication of matching credit still requires fresh current full-image gates and all existing object-owner controls.

## Completed integration

Campaign `790b5eb477624821be9ed3fcdc3c615d` passes the boot and all 27 overlay
loaded-byte and metadata gates. The independent audit verifies all 5,573
complete owned functions, readonly sections and object provenance; prior
controls and opaque helpers remain unchanged. The coherent lot adds 2,388
physical bytes across its two source families. Current physical coverage is
346,192 / 48,788,176 bytes; conservative all-members unique coverage is
89,964 / 44,451,612, measured after complete reconstruction and theorem replay.
