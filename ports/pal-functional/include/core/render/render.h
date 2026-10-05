#ifndef CORE_RENDER_H
#define CORE_RENDER_H

#include "core/types.h"

/* ========================================================================
 *  Render API – agnóstica de backend.
 *  engine/ y game/ incluyen SOLO este header.
 *  Nunca incluyen <GL/gl.h> ni <vulkan/vulkan.h> directamente.
 * ======================================================================== */

 /* ----------------------------------------------------------------------
  *  Tipos opacos – el struct real vive en el .c del backend
  * ---------------------------------------------------------------------- */
typedef struct RenderHandle   RenderHandle;
typedef struct ShaderHandle   ShaderHandle;
typedef struct MeshHandle     MeshHandle;
typedef struct TextureHandle  TextureHandle;
typedef struct Camera         Camera;

/* ----------------------------------------------------------------------
 *  Backend selection
 * ---------------------------------------------------------------------- */
typedef enum {
    BACKEND_OPENGL = 0,
    BACKEND_VULKAN = 1
} RenderBackend;

/* ----------------------------------------------------------------------
 *  Flags para Render_Init
 * ---------------------------------------------------------------------- */
#define RENDER_FLAG_NONE        0x00
#define RENDER_FLAG_VSYNC       0x01
#define RENDER_FLAG_RESIZABLE   0x02
#define RENDER_FLAG_FULLSCREEN  0x04
#define RENDER_FLAG_SRGB        0x08   /* framebuffer sRGB (GL) / swapchain sRGB (VK) */

 /* ----------------------------------------------------------------------
  *  Flags para Render_BeginFrame (qué limpiar)
  * ---------------------------------------------------------------------- */
#define CLEAR_COLOR             0x01
#define CLEAR_DEPTH             0x02
#define CLEAR_STENCIL           0x04

  /* ----------------------------------------------------------------------
   *  Vertex layout (lo que el vertex shader espera)
   * ---------------------------------------------------------------------- */
typedef struct Vertex {
    f32 position[3];
    f32 normal[3];
    f32 texcoord[2];
    f32 bone_indices[4];
    f32 bone_weights[4];
} Vertex;

/* ----------------------------------------------------------------------
 *  Pixel format (para texturas)
 * ---------------------------------------------------------------------- */
typedef enum {
    PIX_RGBA8,
    PIX_RGB8,
    PIX_R8
} PixelFormat;

/* ----------------------------------------------------------------------
 *  Camera
 * ---------------------------------------------------------------------- */
typedef struct Camera {
    f32 view[16];       /* 4×4 col-major */
    f32 projection[16]; /* 4×4 col-major */
    f32 view_proj[16];  /* 4×4 col-major (pre-computed) */
    f32 position[3];
} Camera;

/* ======================================================================
 *  Lifecycle
 * ====================================================================== */

 /**
  *  Crea ventana SDL + contexto del backend (GL 3.3 / VK 1.2).
  *  Debe llamarse UNA vez al inicio.
  *  @return  RenderHandle* o NULL si falló.
  */
RenderHandle* Render_Init(RenderBackend backend, u32 width, u32 height, u32 flags);

/**
 *  Destruye todo: GL context, buffers, texturas, window, SDL.
 *  @param   h  handle de Render_Init (NULL-safe).
 */
void Render_Destroy(RenderHandle* h);

/* ======================================================================
 *  Frame
 * ====================================================================== */

void Render_BeginFrame(RenderHandle* h, u32 clear_flags, f32 r, f32 g, f32 b, f32 a);
void Render_EndFrame(RenderHandle* h);

/* ======================================================================
 *  State
 * ====================================================================== */

void Render_SetPipeline(RenderHandle* h, ShaderHandle* shader);
void Render_SetCamera(RenderHandle* h, Camera* cam);
void Render_SetTexture(RenderHandle* h, u32 unit, TextureHandle* tex);
void Render_SetUniformMat4(RenderHandle* h, const char* name, const f32* mat4);
void Render_SetUniformF32(RenderHandle* h, const char* name, f32 val);
void Render_SetUniformI32(RenderHandle* h, const char* name, i32 val);

/* ======================================================================
 *  Geometry
 * ====================================================================== */

void Render_DrawMesh(RenderHandle* h, MeshHandle* mesh);
void Render_DrawMeshRange(RenderHandle* h, MeshHandle* mesh, u32 first, u32 count);

/* ======================================================================
 *  Resource creation (llamar antes del primer BeginFrame, o durante)
 * ====================================================================== */

ShaderHandle* Render_CreateShader(RenderHandle* h, const char* vert_src, const char* frag_src);
MeshHandle* Render_CreateMesh(RenderHandle* h, const Vertex* verts, u32 vert_count,
    const u16* indices, u32 idx_count);
TextureHandle* Render_CreateTextureFromData(RenderHandle* h, const u8* data,
    u32 width, u32 height, PixelFormat fmt);
TextureHandle* Render_LoadTextureFile(RenderHandle* h, const char* path);

/* ======================================================================
 *  Resource destruction
 * ====================================================================== */

void Render_DestroyShader(RenderHandle* h, ShaderHandle* sh);
void Render_DestroyMesh(RenderHandle* h, MeshHandle* mesh);
void Render_DestroyTexture(RenderHandle* h, TextureHandle* tex);

/* ======================================================================
 *  Utility
 * ====================================================================== */

u32 Render_GetWindowWidth(RenderHandle* h);
u32 Render_GetWindowHeight(RenderHandle* h);
void Render_GetWindowPosition(RenderHandle* h, i32* x, i32* y);

#endif // CORE_RENDER_H
