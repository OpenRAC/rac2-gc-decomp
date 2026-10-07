// include/core/types.h
#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stddef.h>

// Standard PlayStation 2 data types (Emotion Engine), as fixed-width types so that
// every header that repeats one of these typedefs through <stdint.h> agrees with it
typedef uint8_t   u8;    // unsigned 8 bits (0 to 255)
typedef uint16_t  u16;   // unsigned 16 bits
typedef uint32_t  u32;   // unsigned 32 bits (memory addresses)
typedef uint64_t  u64;   // unsigned 64 bits

typedef int8_t    s8;    // signed 8 bits
typedef int16_t   s16;   // signed 16 bits
typedef int32_t   s32;   // signed 32 bits
typedef int64_t   s64;   // signed 64 bits

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
