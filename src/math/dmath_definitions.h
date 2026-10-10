#pragma once

#include "../utils/ddefines.h"

#if !defined(DMATH_DEFINTIONS_NO_LINEAR_ALGEBRA)

// ================================================
// ================================================
//
//                 Linear Algebra
//
// ================================================
// ================================================

typedef struct vec_t 
{
    union {
        u32 n, count, dimention, dim, d;
    };
    f32 as_arr[];
} vec_t;

typedef union vec2_t {
    f32 as_arr[ 2 ];
    union { f32 x, u, s, r; };
    union { f32 y, v, t, g; };
} vec2_t;

typedef union vec3_t {
    f32 as_arr[ 3 ];
    union { f32 x, u, r, s; };
    union { f32 y, v, g, t; };
    union { f32 z, w, b, p; };
} vec3_t;

typedef union vec4_t {
    f32 as_arr[ 4 ];
    union { f32 x, r, s; };
    union { f32 y, g, t; };
    union { f32 z, b, p, width;  };
    union { f32 w, a, q, height; };
} vec4_t;


typedef struct matrix_t 
{
    // Dimensions
    u32 rows, cols;
    f32 entries[];
} matrix_t;

typedef union mat2x2_t 
{
    f32 as_arr [ 4 ]; // a11, a12, a21, a22
    struct {
        f32 R1 [ 2 ];
        f32 R2 [ 2 ];
    };
    f32 a11, a12, 
        a21, a22;
} mat2x2_t;

typedef union mat3x3_t 
{
    f32 as_arr [ 9 ]; // a11, a12, a13, a21, a22, a23, a31, a32, a33
    struct {
        f32 R1 [ 3 ];
        f32 R2 [ 3 ];
        f32 R3 [ 3 ];
    };
    f32 a11, a12, a13, 
        a21, a22, a23, 
        a31, a32, a33;
} mat3x3_t;

typedef union mat4x4_t
{
    f32 as_arr [ 16 ]; // a11, a12, a13, a14, a21, a22, a23, a24, a31, a32, a33, a34, a41, a42, a43, a44
    struct {
        f32 R1 [ 4 ];
        f32 R2 [ 4 ];
        f32 R3 [ 4 ];
        f32 R4 [ 4 ];
    };
    f32 a11, a12, a13, a14, 
        a21, a22, a23, a24, 
        a31, a32, a33, a34, 
        a41, a42, a43, a44;
} mat4x4_t;

#endif

#if !defined(DMATH_DEFINITIONS_NO_COMPLEX)

typedef union complex_t
{
    f32 as_arr[ 2 ];
    union { f32 x, a; };
    union { f32 y, b; };
} complex_t;

typedef union quaternion_t
{
    f32 as_arr[ 4 ];
    union { f32 x, a; };
    union { f32 y, b; };
    union { f32 z, c; };
    union { f32 w, d; };
} quaternion_t;

#endif

#if !defined(DMATH_DEFINITIONS_NO_GEOMETRY)

typedef union rectangle_t {
    f32 as_arr[ 5 ]; // x, y, w, h, r
    union {
        vec2_t origin;
        struct {
            f32 x, y;
        };
    };
    union {
        vec2_t scale;
        struct {
            f32 width, height;
        };
        struct {
            f32 w, h;
        };
    };
    union {
        f32 angle, r, rotation;
    };
} rectangle_t;

typedef union circle_t {
    f32 as_arr[ 3 ]; // x, y, r
    union {
        vec2_t center;
        struct {
            f32 x, y;
        };
        struct {
            f32 a, b;
        };
    };
    union {
        f32 r, radius;
    };
} circle_t;

typedef union line2D_t
{
    f32 as_arr[ 4 ]; // x1, y1, x2, y2
    union {
        vec2_t p1, start, s;
        struct {
            f32 x1, y1;
        };
        struct {
            f32 a1, b1;
        };
    };
    union {
        vec2_t p2, end, e;
        struct {
            f32 x2, y2;
        };
        struct {
            f32 a2, b2;
        };
    };
} line2D_t;

typedef union line3D_t
{
    f32 as_arr[ 6 ]; // x1, y1, z1, x2, y2, z2
    union {
        vec3_t p1, start, s;
        struct {
            f32 x1, y1, z1;
        };
        struct {
            f32 a1, b1, c1;
        };
    };
    union {
        vec3_t p2, end, e;
        struct {
            f32 x2, y2, z2;
        };
        struct {
           f32 a2, b2, c2;
        };
    };
} line3D_t;

typedef union ray2D_t
{
    f32 as_arr[ 4 ]; // origin x, origin y, direction x, direction y
    union {
        vec2_t origin, point, p, o;
        struct {
            f32 x, y;
        };
    };
    union {
        vec2_t direction, dir, d;
        struct {
            f32 direction_x, direction_y;
        };
        struct {
            f32 dir_x, dir_y;
        };
        struct {
            f32 dx, dy;
        };
    };
} ray2D_t;

typedef union ray3D_t
{
    f32 as_arr[ 6 ]; // origin x, origin y, origin z, direction x, direction y, direction z
    union {
        vec3_t origin, point, p, o;
        struct {
            f32 x, y, z;
        };
    };
    union {
        vec3_t direction, dir, d;
        struct {
            f32 direction_x, direction_y, direction_z;
        };
        struct {
            f32 dir_x, dir_y, dir_z;
        };
        struct {
            f32 dx, dy, dz;
        };
    };
} ray3D_t;

typedef union box_t
{
    f32 as_arr[ 9 ]; // origin x, origin y, origin z, width, height, depth, roll pitch yaw
    union {
        vec3_t origin, o, point, p;
        struct {
            f32 x, y, z;
        };
        struct {
            f32 a, b, c;
        };
        struct {
            f32 origin_x, origin_y, origin_z;
        };
        struct {
            f32 ox, oy, oz;
        };
    };
    union {
        vec3_t scale, size, s, dimensions, dims;
        struct {
            f32 w, h, d;
        };
        struct {
            f32 width ,height, depth;
        };
        struct {
            f32 sx, sy, sz;
        };
        struct {
            f32 scale_x, scale_y, scale_z;
        };
    };
    union {
        vec3_t orientation, rotation, rot, r;
        struct {
            f32 rx, ry, rz;
        };
        struct {
            f32 roll, pitch, yaw;
        };
    };
} box_t;

typedef union sphere_t
{
    f32 as_arr[ 4 ]; // center x, center y, center z, radius
    union {
        vec3_t center, c, M, origin, o;
        struct {
            f32 x, y, z;
        };
        struct {
            f32 a, b, c;
        };
        struct {
            f32 cx, cy, cz;
        };
        struct {
            f32 center_x, center_y, center_z;
        };
    };
    union {
        f32 radius, r;
    };
} sphere_t;

#endif