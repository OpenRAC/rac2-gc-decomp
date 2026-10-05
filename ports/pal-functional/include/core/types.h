// include/types.h
#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stddef.h>

// Standard PlayStation 2 data types (Emotion Engine)
typedef unsigned char      u8;   // unsigned 8 bits (0 to 255)
typedef unsigned short     u16;  // unsigned 16 bits
typedef unsigned int       u32;  // unsigned 32 bits (memory addresses)
typedef unsigned long long u64;  // unsigned 64 bits

typedef signed char        s8;   // signed 8 bits
typedef signed short       s16;  // signed 16 bits
typedef signed int         s32;  // signed 32 bits
typedef signed long long   s64;  // signed 64 bits

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;
typedef int64_t   i64;
typedef float     f32; // standard 32-bit floating point
typedef double    f64;

typedef int       bool_t;
#define TRUE  1
#define FALSE 0

// Common vector math structures of the Insomniac engine
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
