# Ratchet & Clank 2: Going Commando (PS2) Decompilation

[![Website](https://img.shields.io/badge/Website-openrac.dev-ff8a00?logo=googlechrome&logoColor=white)](https://openrac.dev)
[![Discord](https://img.shields.io/badge/Discord-Join%20Community-5865F2?logo=discord&logoColor=white)](https://discord.gg/Sfd2B54PDG)
[![Progress report](https://github.com/OpenRAC/rac2-gc-decomp/actions/workflows/progress.yml/badge.svg)](https://github.com/OpenRAC/rac2-gc-decomp/actions/workflows/progress.yml)
[![Code](https://decomp.dev/OpenRAC/rac2-gc-decomp.svg?mode=shield&label=Code&measure=matched_code_percent)](https://decomp.dev/OpenRAC/rac2-gc-decomp)
[![Functions](https://decomp.dev/OpenRAC/rac2-gc-decomp.svg?mode=shield&label=Functions&measure=matched_functions)](https://decomp.dev/OpenRAC/rac2-gc-decomp)

A work-in-progress **matching decompilation** of *Ratchet & Clank 2: Going Commando* (Insomniac Games, 2003) for the PlayStation 2 (`SCUS_972.68`, USA v1.01), part of the **[OpenRAC](https://openrac.dev)** initiative.

The objective is to produce C/C++ source code that, when compiled with the original toolchain, generates a byte-identical copy of the retail executable. Matched code is then refactored toward readable, idiomatic C++ with accurate types and naming, using matching builds as continuous regression tests.

> [!NOTE]
> This repository contains **no game assets, retail executables, or disassembly**. To build, you must provide your own legally obtained copy of the game. Please review [`LEGAL.md`](LEGAL.md) before contributing.

---

## Progress

Decompilation progress is tracked live on **[openrac.dev](https://openrac.dev)** and **[decomp.dev/OpenRAC/rac2-gc-decomp](https://decomp.dev/OpenRAC/rac2-gc-decomp)**.

| Version | Region | Target ID | Code Matched | Functions Matched |
|---|---|---|---|---|
| v1.01 | USA (NTSC-U) | `SCUS_972.68` | [![](https://decomp.dev/OpenRAC/rac2-gc-decomp.svg?mode=shield&label=Code&measure=matched_code_percent)](https://decomp.dev/OpenRAC/rac2-gc-decomp) | [![](https://decomp.dev/OpenRAC/rac2-gc-decomp.svg?mode=shield&label=Functions&measure=matched_functions)](https://decomp.dev/OpenRAC/rac2-gc-decomp) |

Every function links at its original retail address. Functions not yet decompiled are built from disassembly, ensuring the full binary always links and matches retail byte-for-byte outside in-progress functions. For level overlays and breakdown details, see [decomp.dev](https://decomp.dev/OpenRAC/rac2-gc-decomp) and [`docs/OVERLAYS.md`](docs/OVERLAYS.md).

---

## Quick Start

### Prerequisites
- **Linux & macOS**: [Docker](https://www.docker.com/) or [Podman](https://podman.io/) (uses our prebuilt Wine container via GitHub Container Registry).
- **Windows**: Git Bash, Python 3.10+, and community toolchain mirrors.

### Setup & Build

1. **Clone the repository:**
   ```bash
   git clone https://github.com/OpenRAC/rac2-gc-decomp.git
   cd rac2-gc-decomp
   ```
   *(On Windows, keep the directory path short to avoid path length limits in the legacy toolchain's `make`.)*

2. **Provide your original executable:**
   Copy `SCUS_972.68` from your disc image into `baserom/`:
   ```bash
   # Expected SHA-1: [add when available]
   cp /path/to/SCUS_972.68 baserom/SCUS_972.68
   ```

3. **Install dependencies and fetch toolchains:**
   ```bash
   # Windows (native):
   pip install -r requirements.txt
   bash scripts/setup_asm.sh
   git clone https://github.com/AngheloAlf/SN-Systems-ProDG_for_PS2_3.01 toolchain/sn-prodg-3.01
   git clone https://github.com/AngheloAlf/sce_ps2_sdk_24 toolchain/sn-prodg-24

   # Linux & macOS (via container wrapper):
   bash scripts/docker/run.sh bash scripts/setup_asm.sh
   git clone https://github.com/AngheloAlf/SN-Systems-ProDG_for_PS2_3.01 toolchain/sn-prodg-3.01
   git clone https://github.com/AngheloAlf/sce_ps2_sdk_24 toolchain/sn-prodg-24
   ```

4. **Build and verify:**
   ```bash
   # Windows (native):
   bash scripts/build_sn.sh

   # Linux & macOS (via container):
   bash scripts/docker/run.sh bash scripts/build_sn.sh
   ```

For detailed documentation on the toolchain and container setup:
- [`docs/TOOLCHAIN.md`](docs/TOOLCHAIN.md) – Compiler and assembler configurations
- [`docs/BUILD_FIDELITY.md`](docs/BUILD_FIDELITY.md) – What the build reproduces of retail's toolchain, what it models, and the rules that keep it honest
- [`docs/CONTAINERS.md`](docs/CONTAINERS.md) – Docker/Podman container workflow

---

## Contributing

Contributions are warmly welcome! Whether you are interested in decompiling functions, researching engine quirks, or improving documentation:

- See [`CONTRIBUTING.md`](CONTRIBUTING.md) for contribution guidelines and rules.
- See [`docs/WORKFLOW.md`](docs/WORKFLOW.md) for the step-by-step function matching guide.
- **New contributors**: Once you have a non-stripped ELF, use `python scripts/function_size_rank.py --category small --limit 10` to find small functions ideal for first contributions.
- **All contributors**: See [`docs/FUNCTION-CATEGORIES.md`](docs/FUNCTION-CATEGORIES.md) for our size-based task categorization system.
- Join our **[Discord Community](https://discord.gg/Sfd2B54PDG)** to discuss progress, ask questions, and collaborate!

---

## Credits

This project builds upon years of dedicated reverse-engineering research and tooling by the community:

- **GFI (Game Fuckery Inc.)** – Special thanks to the GFI Discord community for years of reverse engineering, game research, and technical insights that made this decompilation possible.
- **[RC2-Going-Decompiled](https://github.com/Promises/RC2-Going-Decompiled)** by Promises – Comprehensive matching build system with period-correct ee-gcc, VU0 macro fixup, and ASM mirroring. Our matching build toolchain (`tools/ee/`) is based on this project.
- **[rac1-decomp](https://github.com/OpenRAC/rac1-decomp)** by Lynder063, OpenRAC contributors – Matching decompilation of the first *Ratchet & Clank*. Invaluable reference for function pairing, struct definitions, symbol names, and engine insights ([`docs/SIBLING_DECOMPS.md`](docs/SIBLING_DECOMPS.md)).
- **[rac3-uya-decomp](https://github.com/OpenRAC/rac3-uya-decomp)** by vetusmagnus – Matching decompilation of *Ratchet & Clank: Up Your Arsenal*. Foundation for SN Systems compiler flag discoveries and build setup.
- **[Wrench](https://github.com/chaoticgd/wrench)** by chaoticgd – Ratchet & Clank PS2 modding tools and asset format specifications ([`tools/extract/README.md`](tools/extract/README.md)).

---

## License

- Code written for this project is licensed under the [MIT License](LICENSE).
- *Ratchet & Clank* is a registered trademark of Sony Interactive Entertainment. This project is not affiliated with or endorsed by Sony or Insomniac Games.
