#ifndef CORE_RENDER_GL_LOADER_H
#define CORE_RENDER_GL_LOADER_H

/*
 * OpenGL entry points resolved at run time through SDL_GL_GetProcAddress.
 *
 * Include this header instead of <GL/gl.h> or <SDL_opengl.h>. It keeps the port
 * free of a link-time OpenGL library (Windows' opengl32 only exports GL 1.1 and
 * Linux distributions do not always ship the GL development files) and replaces
 * the GLAD loader the first draft expected but never added.
 *
 * Call rac2_gl_load() once after SDL_GL_CreateContext() and before any gl* call.
 */

#include <SDL.h>
#include <SDL_opengl.h>   /* GL types and enums; the prototypes are redirected below */

#ifndef APIENTRY
#define APIENTRY
#endif

/* X(return type, name, parameter list) for every GL function the port calls. */
#define RAC2_GL_FUNCTIONS(X) \
    X(void, glActiveTexture, (GLenum texture)) \
    X(void, glAttachShader, (GLuint program, GLuint shader)) \
    X(void, glBindAttribLocation, (GLuint program, GLuint index, const GLchar* name)) \
    X(void, glBindBuffer, (GLenum target, GLuint buffer)) \
    X(void, glBindTexture, (GLenum target, GLuint texture)) \
    X(void, glBindVertexArray, (GLuint array)) \
    X(void, glBlendFunc, (GLenum sfactor, GLenum dfactor)) \
    X(void, glBufferData, (GLenum target, GLsizeiptr size, const void* data, GLenum usage)) \
    X(void, glClear, (GLbitfield mask)) \
    X(void, glClearColor, (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)) \
    X(void, glClearDepthf, (GLfloat depth)) \
    X(void, glClearStencil, (GLint s)) \
    X(void, glColorMask, (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)) \
    X(void, glCompileShader, (GLuint shader)) \
    X(GLuint, glCreateProgram, (void)) \
    X(GLuint, glCreateShader, (GLenum type)) \
    X(void, glCullFace, (GLenum mode)) \
    X(void, glDeleteBuffers, (GLsizei n, const GLuint* buffers)) \
    X(void, glDeleteProgram, (GLuint program)) \
    X(void, glDeleteShader, (GLuint shader)) \
    X(void, glDeleteTextures, (GLsizei n, const GLuint* textures)) \
    X(void, glDeleteVertexArrays, (GLsizei n, const GLuint* arrays)) \
    X(void, glDepthFunc, (GLenum func)) \
    X(void, glDisable, (GLenum cap)) \
    X(void, glDrawArrays, (GLenum mode, GLint first, GLsizei count)) \
    X(void, glDrawElements, (GLenum mode, GLsizei count, GLenum type, const void* indices)) \
    X(void, glDrawElementsBaseVertex, (GLenum mode, GLsizei count, GLenum type, const void* indices, GLint basevertex)) \
    X(void, glEnable, (GLenum cap)) \
    X(void, glEnableVertexAttribArray, (GLuint index)) \
    X(void, glFinish, (void)) \
    X(void, glGenBuffers, (GLsizei n, GLuint* buffers)) \
    X(void, glGenTextures, (GLsizei n, GLuint* textures)) \
    X(void, glGenVertexArrays, (GLsizei n, GLuint* arrays)) \
    X(void, glGenerateMipmap, (GLenum target)) \
    X(void, glGetProgramInfoLog, (GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog)) \
    X(void, glGetProgramiv, (GLuint program, GLenum pname, GLint* params)) \
    X(void, glGetShaderInfoLog, (GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog)) \
    X(void, glGetShaderiv, (GLuint shader, GLenum pname, GLint* params)) \
    X(const GLubyte*, glGetString, (GLenum name)) \
    X(GLint, glGetUniformLocation, (GLuint program, const GLchar* name)) \
    X(void, glLinkProgram, (GLuint program)) \
    X(void, glScissor, (GLint x, GLint y, GLsizei width, GLsizei height)) \
    X(void, glShaderSource, (GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length)) \
    X(void, glTexImage2D, (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels)) \
    X(void, glTexParameteri, (GLenum target, GLenum pname, GLint param)) \
    X(void, glUniform1f, (GLint location, GLfloat v0)) \
    X(void, glUniform1i, (GLint location, GLint v0)) \
    X(void, glUniform2f, (GLint location, GLfloat v0, GLfloat v1)) \
    X(void, glUniformMatrix4fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)) \
    X(void, glUseProgram, (GLuint program)) \
    X(void, glVertexAttribPointer, (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)) \
    X(void, glViewport, (GLint x, GLint y, GLsizei width, GLsizei height))

#define RAC2_GL_DECLARE(ret, name, params) \
    typedef ret (APIENTRY* rac2_pfn_##name) params; \
    extern rac2_pfn_##name rac2_##name;
RAC2_GL_FUNCTIONS(RAC2_GL_DECLARE)
#undef RAC2_GL_DECLARE

/* Loads every entry point above from the current context. Returns 1 when all
 * were found, 0 otherwise (SDL_GetError() names the missing one). */
int rac2_gl_load(void);

#define glActiveTexture rac2_glActiveTexture
#define glAttachShader rac2_glAttachShader
#define glBindAttribLocation rac2_glBindAttribLocation
#define glBindBuffer rac2_glBindBuffer
#define glBindTexture rac2_glBindTexture
#define glBindVertexArray rac2_glBindVertexArray
#define glBlendFunc rac2_glBlendFunc
#define glBufferData rac2_glBufferData
#define glClear rac2_glClear
#define glClearColor rac2_glClearColor
#define glClearDepthf rac2_glClearDepthf
#define glClearStencil rac2_glClearStencil
#define glColorMask rac2_glColorMask
#define glCompileShader rac2_glCompileShader
#define glCreateProgram rac2_glCreateProgram
#define glCreateShader rac2_glCreateShader
#define glCullFace rac2_glCullFace
#define glDeleteBuffers rac2_glDeleteBuffers
#define glDeleteProgram rac2_glDeleteProgram
#define glDeleteShader rac2_glDeleteShader
#define glDeleteTextures rac2_glDeleteTextures
#define glDeleteVertexArrays rac2_glDeleteVertexArrays
#define glDepthFunc rac2_glDepthFunc
#define glDisable rac2_glDisable
#define glDrawArrays rac2_glDrawArrays
#define glDrawElements rac2_glDrawElements
#define glDrawElementsBaseVertex rac2_glDrawElementsBaseVertex
#define glEnable rac2_glEnable
#define glEnableVertexAttribArray rac2_glEnableVertexAttribArray
#define glFinish rac2_glFinish
#define glGenBuffers rac2_glGenBuffers
#define glGenTextures rac2_glGenTextures
#define glGenVertexArrays rac2_glGenVertexArrays
#define glGenerateMipmap rac2_glGenerateMipmap
#define glGetProgramInfoLog rac2_glGetProgramInfoLog
#define glGetProgramiv rac2_glGetProgramiv
#define glGetShaderInfoLog rac2_glGetShaderInfoLog
#define glGetShaderiv rac2_glGetShaderiv
#define glGetString rac2_glGetString
#define glGetUniformLocation rac2_glGetUniformLocation
#define glLinkProgram rac2_glLinkProgram
#define glScissor rac2_glScissor
#define glShaderSource rac2_glShaderSource
#define glTexImage2D rac2_glTexImage2D
#define glTexParameteri rac2_glTexParameteri
#define glUniform1f rac2_glUniform1f
#define glUniform1i rac2_glUniform1i
#define glUniform2f rac2_glUniform2f
#define glUniformMatrix4fv rac2_glUniformMatrix4fv
#define glUseProgram rac2_glUseProgram
#define glVertexAttribPointer rac2_glVertexAttribPointer
#define glViewport rac2_glViewport

#endif /* CORE_RENDER_GL_LOADER_H */
