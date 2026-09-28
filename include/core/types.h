// include/types.h
#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stddef.h>

// Tipos de datos estándar de PlayStation 2 (Emotion Engine)
typedef unsigned char      u8;   // 8 bits sin signo (0 a 255)
typedef unsigned short     u16;  // 16 bits sin signo
typedef unsigned int       u32;  // 32 bits sin signo (Direcciones de memoria)
typedef unsigned long long u64;  // 64 bits sin signo

typedef signed char        s8;   // 8 bits con signo
typedef signed short       s16;  // 16 bits con signo
typedef signed int         s32;  // 32 bits con signo
typedef signed long long   s64;  // 64 bits con signo

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;
typedef int64_t   i64;
typedef float     f32; // Punto flotante estándar de 32 bits
typedef double    f64;

typedef int       bool_t;
#define TRUE  1
#define FALSE 0

// Estructuras matemáticas vectoriales comunes en el motor de Insomniac
typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vector3;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vector4;

#endif // TYPES_H
