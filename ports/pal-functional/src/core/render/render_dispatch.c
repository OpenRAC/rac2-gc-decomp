/*
 *  render_dispatch.c – ruta única para engine/ y game/.
 *  Aquí se decide GL vs VK. Ningún otro archivo del proyecto
 *  incluye headers de GL o VK.
 *
 *  Uso:
 *    #include "core/render/render.h"
 *    RenderHandle* h = Render_Init(BACKEND_OPENGL, 1920, 1080, RENDER_FLAG_VSYNC);
 */

#include "core/render/render.h"
#include <stddef.h>

#if defined(RENDER_USE_GL)
#include "core/render/gl/gl_backend.h"
#endif

 /* VK headers se incluirán cuando escribamos el backend VK */
#if defined(RENDER_USE_VK)
  /* #include "core/render/vk/vk_backend.h" */
#endif

/* ========================================================================
 *  Lifecycle
 * ======================================================================== */

RenderHandle* Render_Init(RenderBackend backend, u32 width, u32 height, u32 flags)
{
#if defined(RENDER_USE_GL)
    if (backend == BACKEND_OPENGL) return GL_Init(width, height, flags);
#endif
#if defined(RENDER_USE_VK)
    if (backend == BACKEND_VULKAN) return VK_Init(width, height, flags);
#endif
    (void)backend; (void)width; (void)height; (void)flags;
    return NULL;
}

void Render_Destroy(RenderHandle* h)
{
    if (!h) return;
#if defined(RENDER_USE_GL)
    /* En fase 1 solo GL; cuando VK entre, se hace dispatch por backend */
    GL_Destroy(h);
#endif
}

/* ========================================================================
 *  Frame
 * ======================================================================== */

void Render_BeginFrame(RenderHandle* h, u32 clear_flags, f32 r, f32 g, f32 b, f32 a)
{
    if (!h) return;
    GL_BeginFrame(h, clear_flags, r, g, b, a);
}

void Render_EndFrame(RenderHandle* h)
{
    if (!h) return;
    GL_EndFrame(h);
}

/* ========================================================================
 *  State
 * ======================================================================== */

void Render_SetPipeline(RenderHandle* h, ShaderHandle* sh)
{
    if (!h) return;
    GL_SetPipeline(h, sh);
}

void Render_SetCamera(RenderHandle* h, Camera* cam)
{
    if (!h) return;
    GL_SetCamera(h, cam);
}

void Render_SetTexture(RenderHandle* h, u32 unit, TextureHandle* tex)
{
    if (!h) return;
    GL_SetTexture(h, unit, tex);
}

void Render_SetUniformMat4(RenderHandle* h, const char* name, const f32* mat4)
{
    if (!h) return;
    GL_SetUniformMat4(h, name, mat4);
}

void Render_SetUniformF32(RenderHandle* h, const char* name, f32 val)
{
    if (!h) return;
    GL_SetUniformF32(h, name, val);
}

void Render_SetUniformI32(RenderHandle* h, const char* name, i32 val)
{
    if (!h) return;
    GL_SetUniformI32(h, name, val);
}

/* ========================================================================
 *  Draw
 * ======================================================================== */

void Render_DrawMesh(RenderHandle* h, MeshHandle* mesh)
{
    if (!h) return;
    GL_DrawMesh(h, mesh);
}

void Render_DrawMeshRange(RenderHandle* h, MeshHandle* mesh, u32 first, u32 count)
{
    if (!h) return;
    GL_DrawMeshRange(h, mesh, first, count);
}

/* ========================================================================
 *  Resources
 * ======================================================================== */

ShaderHandle* Render_CreateShader(RenderHandle* h, const char* vs, const char* fs)
{
    (void)h;
    return GL_CreateShader(vs, fs);
}

MeshHandle* Render_CreateMesh(RenderHandle* h, const Vertex* v, u32 vc,
    const u16* idx, u32 ic)
{
    (void)h;
    return GL_CreateMesh(v, vc, idx, ic);
}

TextureHandle* Render_CreateTextureFromData(RenderHandle* h, const u8* data,
    u32 w, u32 h, PixelFormat fmt)
{
    (void)h;
    return GL_CreateTextureFromData(data, w, h, fmt);
}

TextureHandle* Render_LoadTextureFile(RenderHandle* h, const char* path)
{
    (void)h;
    /* Placeholder: usar stb_image aquí. Se implementa cuando lleguemos a assets. */
    (void)path;
    return NULL;
}

void Render_DestroyShader(RenderHandle* h, ShaderHandle* sh)
{
    (void)h;
    GL_DestroyShader(sh);
}

void Render_DestroyMesh(RenderHandle* h, MeshHandle* m)
{
    (void)h;
    GL_DestroyMesh(m);
}

void Render_DestroyTexture(RenderHandle* h, TextureHandle* t)
{
    (void)h;
    GL_DestroyTexture(t);
}

/* ========================================================================
 *  Window query
 * ======================================================================== */

u32 Render_GetWindowWidth(RenderHandle* h)
{
    if (!h) return 0;
    return (u32)h->width;
}

u32 Render_GetWindowHeight(RenderHandle* h)
{
    if (!h) return 0;
    return (u32)h->height;
}

void Render_GetWindowPosition(RenderHandle* h, i32* x, i32* y)
{
    (void)h; (void)x; (void)y;
}
