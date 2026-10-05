#ifndef CORE_RENDER_GL_BACKEND_H
#define CORE_RENDER_GL_BACKEND_H

#include "core/render/render.h"
#include <SDL.h>
#include "core/render/gl/gl_loader.h"   /* GL types, enums and run-time entry points */

/* ========================================================================
 *  Internal types – expand the opaque types of render.h
 * ======================================================================== */

struct RenderHandle {
    SDL_Window* window;
    SDL_GLContext        gl_ctx;
    u32                  width;
    u32                  height;
    u32                  flags;

    /* current state */
    GLuint               current_vao;
    ShaderHandle* current_shader;
    Camera               current_cam;
    GLuint               tex_units[8];  /* tracks whether each unit has a texture loaded */
};

struct ShaderHandle {
    GLuint   program;
    GLuint   vert_shader;
    GLuint   frag_shader;
};

struct MeshHandle {
    GLuint   vao;
    GLuint   vbo;
    GLuint   ibo;
    u32      vert_count;
    u32      idx_count;
};

struct TextureHandle {
    GLuint   tex_id;
    u32      width;
    u32      height;
    PixelFormat fmt;
};

/* ========================================================================
 *  Internal functions (called from render_dispatch.c)
 * ======================================================================== */

RenderHandle* GL_Init(u32 width, u32 height, u32 flags);
void           GL_Destroy(RenderHandle* h);

void           GL_BeginFrame(RenderHandle* h, u32 clear_flags, f32 r, f32 g, f32 b, f32 a);
void           GL_EndFrame(RenderHandle* h);

void           GL_SetPipeline(RenderHandle* h, ShaderHandle* sh);
void           GL_SetCamera(RenderHandle* h, Camera* cam);
void           GL_SetTexture(RenderHandle* h, u32 unit, TextureHandle* tex);
void           GL_SetUniformMat4(RenderHandle* h, const char* name, const f32* mat4);
void           GL_SetUniformF32(RenderHandle* h, const char* name, f32 val);
void           GL_SetUniformI32(RenderHandle* h, const char* name, i32 val);

void           GL_DrawMesh(RenderHandle* h, MeshHandle* mesh);
void           GL_DrawMeshRange(RenderHandle* h, MeshHandle* mesh, u32 first, u32 count);

ShaderHandle* GL_CreateShader(const char* vert_src, const char* frag_src);
MeshHandle* GL_CreateMesh(const Vertex* verts, u32 vert_count, const u16* indices, u32 idx_count);
TextureHandle* GL_CreateTextureFromData(const u8* data, u32 w, u32 h, PixelFormat fmt);

void           GL_DestroyShader(ShaderHandle* sh);
void           GL_DestroyMesh(MeshHandle* mesh);
void           GL_DestroyTexture(TextureHandle* tex);

u32            GL_GetWindowWidth(SDL_Window* win);
u32            GL_GetWindowHeight(SDL_Window* win);
void           GL_GetWindowPosition(SDL_Window* win, i32* x, i32* y);

#endif // CORE_RENDER_GL_BACKEND_H
