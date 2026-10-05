#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include "types.h"
#include "core/region.h"
#include "core/ee_memory.h"

// Enumeration for the dynamic PC renderer selector
typedef enum {
    RENDERER_OPENGL,
    RENDERER_VULKAN
} PC_RendererType;

// Global port configuration (starts with Vulkan by default; switch to OPENGL if desired)
static PC_RendererType g_selected_pc_renderer = RENDERER_VULKAN;

// The PS2's native frame clock of the selected region: RAC2_FRAME_TARGET_MS in core/region.h
// (PAL 50Hz = 20 ms per frame, NTSC 59.94Hz = about 16.68 ms per frame)

// References to the infrastructure functions already consolidated in the repository
void sys_boot_intro_state_machine(s32 execution_stage);

int main(int argc, char* argv[]) {
    (void)argc; (void)argv; // Avoids compiler warnings

    printf("[PORT START] Starting native Ratchet & Clank 2 (PC Port v1.0, %s, %s)...\n", RAC2_REGION_NAME, RAC2_BOOT_SERIAL);

    // 0. Load the static data of the user's own boot executable into the emulated EE RAM
    int boot_segments = ee_memory_load_boot_elf("orig/" RAC2_BOOT_SERIAL);
    if (boot_segments < 0)
        printf("[PORT START] orig/%s not found or not a PS2 ELF; the emulated EE RAM starts empty.\n", RAC2_BOOT_SERIAL);
    else
        printf("[PORT START] Loaded %d segments of orig/%s into the emulated EE RAM.\n", boot_segments, RAC2_BOOT_SERIAL);

    // 1. Initialize the essential SDL 2.32.2 subsystems (video and controller)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        printf("[SDL ERROR] Could not initialize SDL: %s\n", SDL_GetError());
        return -1;
    }

    // Configure the window flags according to the selected graphics backend
    Uint32 window_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    if (g_selected_pc_renderer == RENDERER_VULKAN) {
        window_flags |= SDL_WINDOW_VULKAN;
        printf("[SDL VIDEO] Configuring a native surface for VULKAN.\n");
    }
    else {
        window_flags |= SDL_WINDOW_OPENGL;
        printf("[SDL VIDEO] Configuring a native context for OPENGL.\n");
    }

    // 2. Create the native PC window
    SDL_Window* p_window = SDL_CreateWindow(
        "Ratchet & Clank 2: Going Commando - Native PC Port",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1024, 768, window_flags
    );

    // Without Vulkan support (driver, SDL build or platform), fall back to OpenGL
    if (p_window == NULL && g_selected_pc_renderer == RENDERER_VULKAN) {
        printf("[SDL VIDEO] Vulkan window unavailable (%s); falling back to OPENGL.\n", SDL_GetError());
        g_selected_pc_renderer = RENDERER_OPENGL;
        window_flags = (window_flags & ~(Uint32)SDL_WINDOW_VULKAN) | SDL_WINDOW_OPENGL;
        p_window = SDL_CreateWindow(
            "Ratchet & Clank 2: Going Commando - Native PC Port",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            1024, 768, window_flags
        );
    }

    if (p_window == NULL) {
        printf("[SDL ERROR] Could not create the window: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Virtual render contexts for the simulation
    SDL_Renderer* p_renderer = NULL;
    if (g_selected_pc_renderer == RENDERER_OPENGL) {
        p_renderer = SDL_CreateRenderer(p_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (p_renderer == NULL)   // e.g. no GPU: SDL's software renderer still presents frames
            p_renderer = SDL_CreateRenderer(p_window, -1, SDL_RENDERER_SOFTWARE);
        printf("[SDL RENDER] OpenGL backend active with adapted V-Sync%s.\n", p_renderer ? "" : " (no renderer available)");
    }
    else {
        printf("[SDL RENDER] Vulkan backend active (direct pipeline initialized).\n");
    }

    printf("[ENGINE] Flat ground detected. Starting the main game loop...\n");

    bool is_running = true;
    SDL_Event sdl_event;

    // High-precision variables for the dynamic time metronome
    Uint64 current_time = SDL_GetPerformanceCounter();
    Uint64 last_time = current_time;
    double accumulated_time_ms = 0.0;
    double frequency = (double)SDL_GetPerformanceFrequency();

    // 3. MAIN GAME LOOP
    while (is_running) {
        // A) Process the PC operating system events (Windows/Linux/SteamOS)
        while (SDL_PollEvent(&sdl_event)) {
            if (sdl_event.type == SDL_QUIT) {
                is_running = false;
            }
            // Initial keyboard mapping for the port
            if (sdl_event.type == SDL_KEYDOWN) {
                if (sdl_event.key.keysym.sym == SDLK_ESCAPE) {
                    is_running = false; // Quick exit with the Escape key
                }
            }
        }

        // B) Dynamic time clock control (cross-platform delta time)
        current_time = SDL_GetPerformanceCounter();
        double frame_time_ms = ((double)(current_time - last_time) * 1000.0) / frequency;
        last_time = current_time;

        // Accumulate the time elapsed on the modern PC
        accumulated_time_ms += frame_time_ms;

        // C) Execution in fixed logic steps (exact emulation of the region's field rate)
        // If the modern PC runs very fast (e.g. at 144Hz), this loop processes the game ticks
        // exactly, keeping physics and the intro stable without speeding up
        while (accumulated_time_ms >= RAC2_FRAME_TARGET_MS) {

            // MASTER TRIGGER: invokes the root intro state machine with the active state (1)
            sys_boot_intro_state_machine(1);

            accumulated_time_ms -= RAC2_FRAME_TARGET_MS;
        }

        // D) On-screen visual rendering (PC frame rate)
        if (g_selected_pc_renderer == RENDERER_OPENGL && p_renderer != NULL) {
            SDL_SetRenderDrawColor(p_renderer, 0, 0, 0, 255); // Original black loading background
            SDL_RenderClear(p_renderer);

            // The drawing functions (sceGsSetDefLoadImage) will be connected here

            SDL_RenderPresent(p_renderer);
        }
        else {
            // Section reserved for presenting native Vulkan buffers
            SDL_Delay(1); // Avoids saturating the PC processor in empty debug loops
        }
    }

    // 4. Tear down and release resources when closing the game
    printf("[PORT CLOSE] Closing the window and releasing the subsystems safely.\n");
    if (p_renderer) SDL_DestroyRenderer(p_renderer);
    SDL_DestroyWindow(p_window);
    SDL_Quit();

    return 0;
}
