/*
 *  gl_loader.c – resolves the OpenGL entry points of gl_loader.h at run time.
 */

#include "core/render/gl/gl_loader.h"

#define RAC2_GL_DEFINE(ret, name, params) rac2_pfn_##name rac2_##name = NULL;
RAC2_GL_FUNCTIONS(RAC2_GL_DEFINE)
#undef RAC2_GL_DEFINE

int rac2_gl_load(void)
{
    int complete = 1;
#define RAC2_GL_RESOLVE(ret, name, params) \
    rac2_##name = (rac2_pfn_##name)SDL_GL_GetProcAddress(#name); \
    if (rac2_##name == NULL) { \
        SDL_SetError("OpenGL entry point %s is unavailable", #name); \
        complete = 0; \
    }
    RAC2_GL_FUNCTIONS(RAC2_GL_RESOLVE)
#undef RAC2_GL_RESOLVE
    return complete;
}
