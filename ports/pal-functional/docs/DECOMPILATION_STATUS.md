# Ratchet & Clank 2 - Functional Decompilation

## Project Status

The project is in the initial stage of functional reconstruction.

The goal is to progressively recover the behaviour of the original
Ratchet & Clank 2 game (PS2 PAL), adapting the decompiled functions
to C for modern PC systems.

The implementation aims to keep the game's behaviour and logic as
faithful to the original as possible, replacing only the parts that
depend on PlayStation 2 hardware and APIs with suitable PC
equivalents.

---

## Recovered Infrastructure

### Portable Kernel

- [x] Portable kernel
- [x] Virtual thread system
- [x] Virtual semaphores
- File: `ps2_kernel.c`
- Reference: `[INDEX]`

This layer provides a portable representation of some PS2 kernel
services needed to run the recovered logic.

---

### Communication Bus and Storage System

- [x] Communication bus
- [x] DVD reader simulation
- [x] Memory Card system skeleton
- File: `ps2_sif.c`
- Reference: `[INDEX]`

This layer reproduces the interfaces the recovered code needs to
interact with the original communication and storage systems.

---

### Text Utilities

- [x] Accelerated text utilities
- File: `ps2_string.c`
- Reference: `[INDEX]`

Adaptation of the text manipulation utilities used by the original
code.

---

## System Logic

### Boot Sequence / Intro

- [x] Root state machine of the intro
- Function: `sys_boot_intro_state_machine`
- Reference: `[INDEX]`

This function is the main state machine used during the game's
initial sequence.

---

## Pending Systems

### Platform

- [ ] Graphics Canvas Init
- [ ] Game Frame / V-Sync / Engine Clock
- [ ] Pad Input Subsystem
- [ ] Entry point / executable

### Graphics

- [ ] Graphics system initialization
- [ ] Canvas / framebuffer
- [ ] Rendering
- [ ] Frame presentation

### Input

- [ ] Controller reading
- [ ] Button state
- [ ] Analog sticks
- [ ] DualShock 2 adaptation for PC

### Time

- [ ] Engine clock
- [ ] Frame synchronization
- [ ] V-Sync
- [ ] Delta time / timing

---

## Decompiled Functions

Recovered functions initially keep their original Ghidra name while
their purpose or structure has not been confirmed.

A function may receive a descriptive name later, once there is
enough evidence about its behaviour.

Example:

    FUN_80012340
        ↓
    update_engine_clock

Names, structures or offset meanings must not be assumed without
sufficient evidence.

---

## Documentation

Documentation of the recovered functions and systems is stored in:

    docs/

The C implementation is stored mainly in:

    src/

Shared declarations and structures are stored in:

    include/

Auxiliary tools used during the research are stored in:

    tools/

---

## Adaptation Criteria

The PS2 code is used as the reference for the original behaviour.

The PC adaptation may replace:

- PS2-specific APIs
- PS2 hardware
- the graphics system
- the input system
- timing
- kernel services

However, the game's own logic must be preserved whenever possible.

When the behaviour of a function is not yet confirmed, the
uncertainty must be documented instead of presenting an assumption
as a fact.

---

## Current Progress

### Infrastructure

- [x] `ps2_kernel.c`
- [x] `ps2_sif.c`
- [x] `ps2_string.c`

### Boot System

- [x] `sys_boot_intro_state_machine`

### Running on PC

- [ ] Graphics initialization
- [ ] Engine clock
- [ ] Input
- [ ] Entry point
- [ ] First functional boot

---

## Next Goal

Progressively implement the layers needed for the game's first
observable boot on PC.

Initially planned order:

1. Graphics Canvas Init
2. Game Frame / V-Sync / Engine Clock
3. Pad Input Subsystem
4. Entry Point / executable

The order may change according to the dependencies discovered
during decompilation.
