#pragma once 

#include "dmath_definitions.h"
#include <math.h>
#include <assert.h> // TODO: Remove

#ifndef PI
#define PI 3.1415927F
#endif // PI

#ifndef TWO_PI
#define TWO_PI 6.2831855F
#endif // TWO_PI

#ifndef F_EPSILON
#define F_EPSILON 1.19E-7F
#endif // F_EPSILON

#if !defined(DMATH_NO_REGULAR_TYPES)

// Usage printf("My byte = 0b"BYTE8_PRINT_FMT"\n", BYTE8_TO_PRINT_FMT(my_byte));
#define BYTE8_PRINT_FMT "%c%c%c%c%c%c%c%c"
#define BYTE8_TO_PRINT_FMT(b) \
((b) & 0b10000000) ? '1' : '0', \
((b) & 0b01000000) ? '1' : '0', \
((b) & 0b00100000) ? '1' : '0', \
((b) & 0b00010000) ? '1' : '0', \
((b) & 0b00001000) ? '1' : '0', \
((b) & 0b00000100) ? '1' : '0', \
((b) & 0b00000010) ? '1' : '0', \
((b) & 0b00000001) ? '1' : '0'

// Usage printf("My byte = 0b"BYTE32_PRINT_FMT"\n", BYTE32_TO_PRINT_FMT(my_byte));
#define BYTE32_PRINT_FMT \
    BYTE8_PRINT_FMT BYTE8_PRINT_FMT BYTE8_PRINT_FMT BYTE8_PRINT_FMT
#define BYTE32_TO_PRINT_FMT(b) \
    BYTE8_TO_PRINT_FMT((u8)((b & 0xFF000000) >> 8 * 3)), \
    BYTE8_TO_PRINT_FMT((u8)((b & 0x00FF0000) >> 8 * 2)), \
    BYTE8_TO_PRINT_FMT((u8)((b & 0x0000FF00) >> 8 * 1)), \
    BYTE8_TO_PRINT_FMT((u8)((b & 0x000000FF) >> 8 * 0)) 

f32 f32_clamp(const f32 x, const f32 min_x, const f32 max_x);
f32 f32_lerp(const f32 a, const f32 b, const f32 t);

i32 i32_clamp(const i32 x, const i32 m, const i32 M);

u32 u32_pack_bytes(const u8 msb , const u8 msb2, const u8 lsb2, const u8 lsb);
u32 u32_reverse_indian(const u32 u); 

u8 u8_min(const u8 a, const u8 b);
u8 u8_max(const u8 a, const u8 b);
u8 u8_clamp(const u8 x, const u8 m, const u8 M);
u8 u8_bit_at(const u8 byte, const u8 where);
u8 u8_unpack_byte_32(const u32 bytes, const u8 where);
u8 u8_unpack_byte_64(const u64 bytes, const u8 where);

#endif // DMATH_NO_REGULAR_TYPES

#if !defined(DMATH_DEFINTIONS_NO_LINEAR_ALGEBRA)

// =========================================================
//                      Vector 2
// =========================================================

// Usage printf("My vector: " DVEC2_PRINT_FMT "\n", DVEC2_TO_PRINT_FMT(my_vector));
#define VEC2_PRINT_FMT "(%f, %f)"
#define VEC2_TO_PRINT_FMT(v) (v).x, (v).y

vec2_t * vec2_allocate(const f32 x, const f32 y, void* (*allocator)(size_t));
vec2_t vec2(const f32 x, const f32 y); 
vec2_t vec2_0(void);                                                    // return vec2(0, 0)
vec2_t vec2_1(void);                                                        // return vec2(1, 1)
vec2_t vec2_e1(void);                                           // return vec2(1, 0)
vec2_t vec2_e2(void);                                           // return vec2(0, 1)
vec2_t vec2_v(const f32 v);                                           // return vec2(val, val)
vec2_t vec2_neg(const vec2_t v);                                      // return vec2(-v.x, -v.y)
vec2_t vec2_clamp(const vec2_t v, const vec2_t m, const vec2_t M);                // return vec2(fclamp(v.x, m.x, M.x), fclamp(v.x, m.x, M.x))
vec2_t vec2_add(const vec2_t v1, const vec2_t v2);                          // return vec2(v1.x + v2.x, v1.y+ v2.y)
vec2_t vec2_sub(const vec2_t v1, const vec2_t v2);                          // return vec2(v1.x- v2.x, v1.y - v2.y)
vec2_t vec2_mul(const vec2_t v1, const vec2_t v2);                          // return vec2(v1.x*v2.x, v1.y*v2.y)
vec2_t vec2_div(const vec2_t v1, const vec2_t v2);                          // return vec2(v1.x/v2.x, v1.y/v2.y)
vec2_t vec2_add_v(const vec2_t v, const f32 val);                           // return vec2(v.x + val, v.y + val)
vec2_t vec2_sub_v(const vec2_t v, const f32 val);                           // return vec2(v.x - val, v.y - val)
vec2_t vec2_mul_v(const vec2_t v, const f32 val);                           // return vec2(v.x * val, v.y * val)
vec2_t vec2_div_v(const vec2_t v, const f32 val);                           // return vec2(v.x / val, v.y / val)
vec2_t vec2_activate(const vec2_t v, f32 (*activation_func)(f32));    // return vec2(f(v.x), f(v.y))
vec2_t vec2_normalize(const vec2_t v);                                // return v / Length(v)
vec2_t vec2_abs(const vec2_t v);                                      // return vec2(abs(v.x), abs(v.y))
vec2_t vec2_lerp_v(const vec2_t a, const vec2_t b, const f32 t);                 // return a + (b - a) * t
vec2_t vec2_slerp_v(const vec2_t a, const vec2_t b, const f32 t);                // return a * cos(vec2_angle_between(a,b) * t) + (b - a * vec2_dot(a,b)) * sin(vec2_angle_between(a,b) * t)
vec2_t vec2_lerp(const vec2_t a, const vec2_t b, const vec2_t t);                 // return a + (b - a) * t
vec2_t vec2_slerp(const vec2_t a, const vec2_t b, const vec2_t t);                // return a * cos(vec2_angle_between(a,b) * t) + (b - a * vec2_dot(a,b)) * sin(vec2_angle_between(a,b) * t)
vec2_t vec2_linear_comb(const vec2_t v, const vec2_t u, const f32 a1, const f32 a2);    // return a1 * v1 + a2 * v2
vec2_t vec2_transform(const vec2_t v, const mat2x2_t M);                    // return M * v as matrix mult
vec2_t vec2_from_angle(const f32 alpha);                              // return vec2(cos(alpha),sin(alpha))
vec2_t vec2_from_angle_and_r(const f32 alpha, const f32 radius);            // return vec2(r * cos(alpha), r * sin(alpha))
bool   vec2_is_eq(const vec2_t v, const vec2_t u);
bool   vec2_is_eq_approx(const vec2_t v, const vec2_t u, f32 epsilon);
f32    vec2_length(const vec2_t v);                                   // return sqrt(v.x * v.x + v.y * v.y)
f32    vec2_dist(const vec2_t v, const vec2_t u);                           // return length(v-u) 
f32    vec2_dot(const vec2_t v, const vec2_t u);                            // return v.x * u.x + v.y * u.y
f32    vec2_angle_between(const vec2_t v, const vec2_t u);                  // return arccos(vec2_dot(v,u)/(length(v)*length(u)))
f32    vec2_dist_sqr(const vec2_t v, const vec2_t u);                       // return dist(u,v) * dist(u,v)
f32    vec2_length_sqr(const vec2_t v);                               // return length(v) * length(v)
f32    vec2_lp_norm(const vec2_t v, const f32 p);                           // return (v.x ^ p + v.y ^ p) ^ (1/p)
f32    vec2_lp_norm_to_the_pth(const vec2_t v, const f32 p);                // return (v.x ^ p + v.y ^ p) 
f32    vec2_max(const vec2_t v);                                      // return max(v.x, v.y)
f32    vec2_min(const vec2_t v);                                      // return min(v.x, v.y)
f32    vec2_avg(const vec2_t v);                                      // return avarage(v.x, v.y)
f32    vec2_sum(const vec2_t v);                                      // return avarage(v.x, v.y)

// for each (iter in v) ... usage: vec2_foreach(v, it) { printf("Before: %f\n",it); it = 3.1415F; printf("After: &%f\n", it); } 
#define vec2_foreach(v, iter) \
    for (f32* __iter__ = (v).as_arr, iter = *__iter__; __iter__ < (v).as_arr + 2ULL; *__iter__ = iter, __iter__++)

// =========================================================
//                      Vector 3
// =========================================================

// Usage printf("My vector: " DVEC3_PRINT_FMT "\n", DVEC3_TO_PRINT_FMT(my_vector));
#define VEC3_PRINT_FMT "(%f, %f, %f)"
#define VEC3_TO_PRINT_FMT(v) (v).x, (v).y, (v).z

vec3_t * vec3_allocate(const f32 x, const f32 y, const f32 z, void* (*allocator)(size_t));        // allocate new vec3(x, y, z) if allocator NULL then uses malloc, caller must free
vec3_t vec3(const f32 x, const f32 y, const f32 z);
vec3_t vec3_0(void);                                                            // return vec3(0, 0, 0)
vec3_t vec3_1(void);                                                            // return vec3(1, 1, 1)
vec3_t vec3_v(const f32 val);                                                         // return vec3(val, val, val)
vec3_t vec3_e1(void);                                                           // return vec3(1, 0, 0)
vec3_t vec3_e2(void);                                                           // return vec3(0, 1, 0)
vec3_t vec3_e3(void);                                                           // return vec3(0, 0, 1)
vec3_t vec3_neg(const vec3_t v);                                                      // return -v
vec3_t vec3_clamp(const vec3_t v, const vec3_t m, const vec3_t M);                                // return vec3(fclamp(v.x, m.x, M.x), fclamp(v.y, m.y, M.y), fclamp(v.z, m.z, M.z))
vec3_t vec3_add(const vec3_t v, const vec3_t u);                                            // return vec3(v.x + u.x, v.y + u.y, v.z + u.z)
vec3_t vec3_sub(const vec3_t v, const vec3_t u);                                            // return vec3(v.x - u.x, v.y - u.y, v.z - u.z)
vec3_t vec3_mul(const vec3_t v, const vec3_t u);                                            // return vec3(v.x * u.x, v.y * u.y, v.z * u.z)
vec3_t vec3_div(const vec3_t v, const vec3_t u);                                            // return vec3(v.x / u.x, v.y / u.y, v.z / u.z)
vec3_t vec3_add_v(const vec3_t v, const f32 val);                                           // return vec3(v.x + val, v.y + val, v.z + val)
vec3_t vec3_sub_v(const vec3_t v, const f32 val);                                           // return vec3(v.x - val, v.y - val, v.z - val)
vec3_t vec3_mul_v(const vec3_t v, const f32 val);                                           // return vec3(v.x * val, v.y * val, v.z * val)
vec3_t vec3_div_v(const vec3_t v, const f32 val);                                           // return vec3(v.x / val, v.y / val, v.z / val)
vec3_t vec3_activate(const vec3_t v, f32 (*activation_func)(f32));                    // return vec3(func(v.x), func(v.y), func(v.z))
vec3_t vec3_normalize(const vec3_t v);                                                // return v / vec3_length(v)
vec3_t vec3_abs(const vec3_t v);                                                      // return vec3(abs(v.x), abs(v.y), abs(v.z))
vec3_t vec3_lerp_v(const vec3_t a, const vec3_t b, const f32 t);                                 // return a + (b - a) * t
vec3_t vec3_slerp_v(const vec3_t a, const vec3_t b, const f32 t);                                // return a * cos(vec3_angle_between(a,b) * t) + (b - a * vec3_dot(a,b)) * sin(vec3_angle_between(a,b) * t)
vec3_t vec3_lerp(const vec3_t a, const vec3_t b, const vec3_t t);                                 // return a + (b - a) * t
vec3_t vec3_slerp(const vec3_t a, const vec3_t b, const vec3_t t);                                // return a * cos(vec3_angle_between(a,b) * t) + (b - a * vec3_dot(a,b)) * sin(vec3_angle_between(a,b) * t)
vec3_t vec3_linear_comb2(const vec3_t v, const vec3_t u, const f32 a1, const f32 a2);                   // return v * a1 + u * a2
vec3_t vec3_linear_comb3(const vec3_t v, const vec3_t u, const vec3_t w, const f32 a1, const f32 a2, const f32 a3); // return v * a1 + u * a2 + w * a3
vec3_t vec3_transform(const vec3_t v, const mat3x3_t M);                                    // return M * v (matrix mult)              
vec3_t vec3_from_angle(const f32 alpha, const f32 gamma);                                   // return vec3(cos(alpha) * sin(gamma), sin(alpha) * sin(gamma), cos(gamma))
vec3_t vec3_from_angle_and_r(const f32 alpha, const f32 gamma, const f32 radius);                 // return vec3_from_angle(alpha, gamma) * radius
vec3_t vec3_cross(const vec3_t v, const vec3_t u);                                          // return v * u as cross product 
vec3_t vec3_cross_normalize(const vec3_t v, const vec3_t u);                                // return vec3_cross(v, u) / vec3_length(vec3_cross(v, u)) 
bool   vec3_is_eq(const vec3_t v, const vec3_t u);
bool   vec3_is_eq_approx(const vec3_t v, const vec3_t u, f32 epsilon);
f32    vec3_length(const vec3_t v);                                                   // return sqrt(v.x * v.x + v.y * v.y + v.z * v.z)
f32    vec3_dist(const vec3_t v, const vec3_t u);                                           // return vec3_length(v - u)
f32    vec3_dot(const vec3_t v, const vec3_t u);                                            // return v.x * u.x + v.y * u.y + v.z * u.z
f32    vec3_angle_between(const vec3_t v, const vec3_t u);                                  // return arccos(vec3_dot(v,u)/(vec3_length(v) * vec3_length(u)))
f32    vec3_dist_sqr(const vec3_t v, const vec3_t u);                                       // return vec3_dist(v,u) * vec3_dist(v,u)
f32    vec3_length_sqr(const vec3_t v);                                               // return vec3_length(v) * vec3_length(v)
f32    vec3_lp_norm(const vec3_t v, const f32 p);                                           // return pow(sum(pow(v.as_arr[i], p)), 1 / p)
f32    vec3_lp_norm_to_the_pth(const vec3_t v, const f32 p);                                // return pow(vec3_lp_norm(v, p), p)
f32    vec3_max(const vec3_t v);                                                      // return max(v.x, v.y, v.z)
f32    vec3_min(const vec3_t v);                                                      // return min(v.x, v.y, v.z)
f32    vec3_avg(const vec3_t v);                                                      // return avarage(v.x, v.y, v.z)
f32    vec3_sum(const vec3_t v);                                                      // return avarage(v.x, v.y, v.z)

// for each (iter in v) ... usage: vec3_foreach(v, it) { printf("Before: %f\n",it); it = PI; printf("After: &%f\n", it); } 
#define vec3_foreach(v, iter) \
    for (f32* __iter__ = (v).as_arr, iter = *__iter__; __iter__ < (v).as_arr + 3ULL; *__iter__ = iter, __iter__++)


// =========================================================
//                      Vector 4
// =========================================================

// Usage printf("My vector: " DVEC4_PRINT_FMT "\n", DVEC4_TO_PRINT_FMT(my_vector));
#define VEC4_PRINT_FMT "(%f, %f, %f, %f)"
#define VEC4_TO_PRINT_FMT(v) (v).x, (v).y, (v).z, (v).w

vec4_t * vec4_allocate(const f32 x, const f32 y, const f32 z, const f32 w, void* (*allocator)(size_t));
vec4_t vec4(const f32 x, const f32 y, const f32 z, const f32 w);
vec4_t vec4_0(void);
vec4_t vec4_1(void);
vec4_t vec4_v(const f32 val);
vec4_t vec4_e1(void);
vec4_t vec4_e2(void);
vec4_t vec4_e3(void);
vec4_t vec4_e4(void);
vec4_t vec4_neg(const vec4_t v);
vec4_t vec4_abs(const vec4_t v);
vec4_t vec4_normalize(const vec4_t v);
vec4_t vec4_add(const vec4_t v, const vec4_t u);
vec4_t vec4_sub(const vec4_t v, const vec4_t u);
vec4_t vec4_mul(const vec4_t v, const vec4_t u);
vec4_t vec4_div(const vec4_t v, const vec4_t u);
vec4_t vec4_add_v(const vec4_t v, const f32 val);
vec4_t vec4_sub_v(const vec4_t v, const f32 val);
vec4_t vec4_mul_v(const vec4_t v, const f32 val);
vec4_t vec4_div_v(const vec4_t v, const f32 val);
vec4_t vec4_activate(const vec4_t v, f32 (*activation_func)(f32));
vec4_t vec4_clamp(const vec4_t v, const vec4_t m, const vec4_t M);
vec4_t vec4_lerp_v(const vec4_t a, const vec4_t b, const f32 t);
vec4_t vec4_slerp_v(const vec4_t a, const vec4_t b, const f32 t);
vec4_t vec4_lerp(const vec4_t a, const vec4_t b, const vec4_t t);
vec4_t vec4_slerp(const vec4_t a, const vec4_t b, const vec4_t t);
vec4_t vec4_linear_comb2(const vec4_t v, const vec4_t u, const f32 a1, const f32 a2);
vec4_t vec4_linear_comb3(const vec4_t v, const vec4_t u, const vec4_t w, const f32 a1, const f32 a2, const f32 a3);
vec4_t vec4_linear_comb4(const vec4_t v, const vec4_t u, const vec4_t w, const vec4_t r, const f32 a1, const f32 a2, const f32 a3, const f32 a4);
vec4_t vec4_transform(const vec4_t v, const mat4x4_t m);
vec4_t vec4_from_angle(const f32 alpha, const f32 beta, const f32 gamma);
vec4_t vec4_from_angle_and_r(const f32 alpha, const f32 beta, const f32 gamma, const f32 radius);
bool   vec4_is_eq(const vec4_t v, const vec4_t u);
bool   vec4_is_eq_approx(const vec4_t v, const vec4_t u, f32 epsilon);
f32    vec4_length(const vec4_t v);
f32    vec4_dist(const vec4_t v, const vec4_t u);
f32    vec4_length_sqr(const vec4_t v);
f32    vec4_dist_sqr(const vec4_t v, const vec4_t u);
f32    vec4_dot(const vec4_t v, const vec4_t u);
f32    vec4_angle_between(const vec4_t v, const vec4_t u);
f32    vec4_lp_norm(const vec4_t v, const f32 p);
f32    vec4_lp_norm_to_the_pth(const vec4_t v, const f32 p);
f32    vec4_max(const vec4_t v);
f32    vec4_min(const vec4_t v);
f32    vec4_avg(const vec4_t v);
f32    vec4_sum(const vec4_t v);

// for each (iter in v) ... usage: vec3_foreach(v, it) { printf("Before: %f\n",it); it = PI; printf("After: &%f\n", it); } 
#define vec4_foreach(v, iter) \
    for (f32* __iter__ = (v).as_arr, iter = *__iter__; __iter__ < (v).as_arr + 4ULL; *__iter__ = iter, __iter__++)

    
    // =========================================================
    //                      Vector
    // =========================================================
    
// =========================================================
//                      Matrix 2x2
// =========================================================

#define MAT2x2_PRINT_FMT "| %f %f|\n"\
                         "| %f %f|"
#define MAT2x2_TO_PRINT_FMT(m) (m).a11, (m).a12, (m).a21, (m).a22

mat2x2_t * mat2x2_allocate(const f32 a11, const f32 a12, const f32 a21, const f32 a22, void* (*allocator)(size_t));
mat2x2_t mat2x2(const f32 a11, const f32 a12, const f32 a21, const f32 a22);
mat2x2_t mat2x2_0(void);
mat2x2_t mat2x2_1(void);
mat2x2_t mat2x2_unit(void);
mat2x2_t mat2x2_scalar(f32 val);
mat2x2_t mat2x2_v(const f32 val);
mat2x2_t mat2x2_rotation_matrix(const f32 alpha);
mat2x2_t mat2x2_rows(const vec2_t R1, const vec2_t R2);
mat2x2_t mat2x2_cols(const vec2_t C1, const vec2_t C2);
mat2x2_t mat2x2_neg(const mat2x2_t m);
mat2x2_t mat2x2_abs(const mat2x2_t m);
mat2x2_t mat2x2_transpose(const mat2x2_t m);
mat2x2_t mat2x2_adjugate(const mat2x2_t m);
mat2x2_t mat2x2_inverse(const mat2x2_t m);
mat2x2_t mat2x2_gram(const vec2_t v1, const vec2_t v2);
mat2x2_t mat2x2_add(const mat2x2_t m1, const mat2x2_t m2);
mat2x2_t mat2x2_sub(const mat2x2_t m1, const mat2x2_t m2);
mat2x2_t mat2x2_mul(const mat2x2_t m1, const mat2x2_t m2);
mat2x2_t mat2x2_div(const mat2x2_t m1, const mat2x2_t m2);
mat2x2_t mat2x2_add_v(const mat2x2_t m1, const f32 val);
mat2x2_t mat2x2_sub_v(const mat2x2_t m1, const f32 val);
mat2x2_t mat2x2_mul_v(const mat2x2_t m1, const f32 val);
mat2x2_t mat2x2_div_v(const mat2x2_t m1, const f32 val);
mat2x2_t mat2x2_mat_mul(const mat2x2_t m1, const mat2x2_t m2);
mat2x2_t mat2x2_lerp_v(const mat2x2_t a, const mat2x2_t b, const f32 t);
mat2x2_t mat2x2_lerp(const mat2x2_t a, const mat2x2_t b, const mat2x2_t t);
mat2x2_t mat2x2_linear_comb2(const mat2x2_t m1, const mat2x2_t m2, const f32 a1, const f32 a2);
mat2x2_t mat2x2_linear_comb3(const mat2x2_t m1, const mat2x2_t m2, const mat2x2_t m3, const f32 a1, const f32 a2, const f32 a3);
mat2x2_t mat2x2_linear_comb4(const mat2x2_t m1, const mat2x2_t m2, const mat2x2_t m3, const mat2x2_t m4, const f32 a1, const f32 a2, const f32 a3, const f32 a4);
vec2_t   mat2x2_col1(const mat2x2_t m);
vec2_t   mat2x2_col2(const mat2x2_t m);
vec2_t   mat2x2_row1(const mat2x2_t m);
vec2_t   mat2x2_row2(const mat2x2_t m);
vec2_t   mat2x2_diagonal(const mat2x2_t m);
bool     mat2x2_is_eq(const mat2x2_t m1, const mat2x2_t m2);
bool     mat2x2_is_eq_approx(const mat2x2_t m1, const mat2x2_t m2, const f32 epsilon);
bool     mat2x2_is_invertable(const mat2x2_t m);
bool     mat2x2_is_symmetric(const mat2x2_t m);
bool     mat2x2_is_anti_symmetric(const mat2x2_t m);
f32      mat2x2_trace(const mat2x2_t m);
f32      mat2x2_norm(const mat2x2_t m);
f32      mat2x2_max(const mat2x2_t m);
f32      mat2x2_min(const mat2x2_t m);
f32      mat2x2_avg(const mat2x2_t m);
f32      mat2x2_det(const mat2x2_t m);
f32      mat2x2_sum(const mat2x2_t m);

// for each (iter in v) ... usage: vec3_foreach(v, it) { printf("Before: %f\n",it); it = PI; printf("After: &%f\n", it); } 
#define mat2x2_foreach(m, iter) \
    for (f32* __iter__ = (m).as_arr, iter = *__iter__; __iter__ < (m).as_arr + 4ULL; *__iter__ = iter, __iter__++)

// =========================================================
//                      Matrix 3x3
// =========================================================

// =========================================================
//                      Matrix
// =========================================================

// =========================================================
//                      Matrix
// =========================================================

#endif // DMATH_DEFINTIONS_NO_LINEAR_ALGEBRA

#if !defined(DMATH_DEFINITIONS_NO_COMPLEX)

// =========================================================
//                      Complex
// =========================================================

#define COMPLEX_PRINT_FMT "%f + %fi"
#define COMPLEX_TO_PRINT_FMT(z) (z).x, (z).y

complex_t* complex_allocate(const f32 x, const f32 y, void* (*allocator)(size_t));
complex_t complex(const f32 x, const f32 y);
complex_t complex_polar(const f32 r, const f32 theta);
complex_t complex_polar_vec(const vec2_t polar);
complex_t complex_v(const f32 val);
complex_t complex_neg(const complex_t z);
complex_t complex_conjugate(const complex_t z);
complex_t complex_inverse(const complex_t z);
complex_t complex_add(const complex_t z1, const complex_t z2);
complex_t complex_sub(const complex_t z1, const complex_t z2);
complex_t complex_div(const complex_t z1, const complex_t z2);
complex_t complex_mul(const complex_t z1, const complex_t z2);
complex_t complex_pow(const complex_t z1, const complex_t z2);
complex_t complex_sin(const complex_t z);
complex_t complex_cos(const complex_t z);
complex_t complex_tan(const complex_t z);
complex_t complex_csc(const complex_t z);
complex_t complex_sec(const complex_t z);
complex_t complex_cot(const complex_t z);
complex_t complex_asin(const complex_t z);
complex_t complex_acos(const complex_t z);
complex_t complex_atan(const complex_t z);
complex_t complex_acot(const complex_t z);
complex_t complex_asec(const complex_t z);
complex_t complex_acsc(const complex_t z);
complex_t complex_sinh(const complex_t z);
complex_t complex_cosh(const complex_t z);
complex_t complex_tanh(const complex_t z);
complex_t complex_coth(const complex_t z);
complex_t complex_sech(const complex_t z);
complex_t complex_csch(const complex_t z);
complex_t complex_asinh(const complex_t z);
complex_t complex_acosh(const complex_t z);
complex_t complex_atanh(const complex_t z);
complex_t complex_acoth(const complex_t z);
complex_t complex_asech(const complex_t z);
complex_t complex_acsch(const complex_t z);
complex_t complex_ceil(const complex_t z);
complex_t complex_floor(const complex_t z);
complex_t complex_round(const complex_t z);
complex_t complex_abs(const complex_t z);
complex_t complex_log(const complex_t z);
complex_t complex_exp(const complex_t z);
complex_t complex_sqrt(const complex_t z);
vec2_t    complex_to_polar(const complex_t z);
vec2_t    complex_to_vec2(const complex_t z);
bool      complex_is_eq(const complex_t z1, const complex_t z2);
bool      complex_is_eq_approx(const complex_t z1, const complex_t z2, const f32 epsilon);
f32       complex_mag(const complex_t z);
f32       complex_mag_sqr(const complex_t z);
f32       complex_arg(const complex_t z);
f32       complex_dist(const complex_t z1, const complex_t z2);
f32       complex_dist_sqr(const complex_t z1, const complex_t z2);
f32       complex_re(const complex_t z);
f32       complex_im(const complex_t z);

#define complex_foreach(z, iter) \
    for (f32* __iter__ = (z).as_arr, iter = *__iter__; __iter__ < (z).as_arr + 4ULL; *__iter__ = iter, __iter__++)

// =========================================================
//                      Quaternion
// =========================================================

#endif // DMATH_DEFINITIONS_NO_COMPLEX

#if !defined(DMATH_DEFINITIONS_NO_GEOMETRY)

// =========================================================
//                      Rectangle
// =========================================================

rectangle_t * rectangle_allocate(const f32 x, const f32 y, const f32 w, const f32 h, const f32 r, void* (*allocator)(size_t));
rectangle_t   rectangle(const f32 x, const f32 y, const f32 w, const f32 h, const f32 r);
rectangle_t   rectangle_v(const vec2_t pos, const vec2_t scale, const f32 r);
rectangle_t   rectangle_square(const f32 x, const f32 y, const f32 size, const f32 r);
rectangle_t   rectangle_square_v(const vec2_t pos, const f32 size, const f32 r);
rectangle_t   rectangle_translate(const rectangle_t rect, const vec2_t translation);
rectangle_t   rectangle_set_center(const rectangle_t rect, const vec2_t where);
rectangle_t   rectangle_scale(const rectangle_t rect, const vec2_t scale);
rectangle_t   rectangle_set_scale(const rectangle_t rect, const vec2_t scale);
rectangle_t   rectangle_rotate(const rectangle_t rect, const f32 rotation);
rectangle_t   rectangle_set_rot(const rectangle_t rect, const f32 rotation);
vec2_t        rectangle_center(const rectangle_t rect);
vec2_t        rectangle_size(const rectangle_t rect);
f32           rectangle_rotation(const rectangle_t rect);
bool          rectangle_AA_grid(const u32 rows, const u32 cols, const vec2_t pos, const vec2_t scale, const vec2_t gap, rectangle_t * out);
bool          rectangle_is_eq(const rectangle_t rect1, const rectangle_t rect2);
bool          rectangle_is_eq_approx(const rectangle_t rect1, const rectangle_t rect2, const f32 epsilon);
bool          rectangle_are_overlaps(const rectangle_t rect1, const rectangle_t rect2);
bool          rectangle_are_overlaps_approx(const rectangle_t rect1, const rectangle_t rect2, const f32 epsilon);
bool          rectangle_is_point_inside(const rectangle_t rect, const vec2_t point);
f32           rectangle_area(const rectangle_t rect);
f32           rectangle_circumference(const rectangle_t rect);
f32           rectangle_dist(const rectangle_t rect1, const rectangle_t rect2);
f32           rectangle_dist_to_point(const rectangle_t rect, const vec2_t point);

#endif // DMATH_DEFINITIONS_NO_GEOMETRY

// =========================================================
//                      IMPLEMENTATION
// =========================================================
#if defined(DMATH_IMPLEMENTATION) || 1

#if !defined(DMATH_NO_STDLIB)
#include <stdlib.h> // malloc
#endif

#if !defined(DMATH_NO_REGULAR_TYPES)
inline f32 f32_clamp(const f32 x, const f32 min_x, const f32 max_x)
{
    f32 result = 0.0F;

    result = x < max_x ? x : max_x;
    result = result > min_x ? result : min_x;

    return result;
}

inline f32 f32_lerp(const f32 a, const f32 b, const f32 t)
{
    f32 result = 0.0F;

    result = a + (b - a) * t;

    return result;
}

inline i32 i32_clamp(const i32 x, const i32 m, const i32 M)
{
    i32 result = 0;

    result = x;
    result = result < M ? result : M;
    result = result > m ? result : m;

    return result;
}

u32 u32_pack_bytes(const u8 msb, const u8 msb2, const u8 lsb2, const u8 lsb)
{
    u32 result = 0UL;

    result |= (u32)(msb)  << 8U * 3U;
    result |= (u32)(msb2) << 8U * 2U;
    result |= (u32)(lsb2) << 8U * 1U;
    result |= (u32)(lsb)  << 8U * 0U;

    return result;
}

inline u32 u32_reverse_indian(const u32 u)
{
    u32 result = 0UL;

    const u32 x1 = (u & 0x000000FF) << 24U;
    const u32 x2 = (u & 0x0000FF00) << 8U;
    const u32 x3 = (u & 0x00FF0000) >> 8U;
    const u32 x4 = (u & 0xFF000000) >> 24U;

    result = x1 | x2 | x3 | x4;

    return result;
}

inline u8 u8_min(const u8 a, const u8 b)
{
    u8 result = 0U;

    result = a < b ? a : b;

    return result;
}

inline u8 u8_max(const u8 a, const u8 b)
{
    u8 result = 0U;

    result = a > b ? a : b;

    return result;
}

inline u8 u8_clamp(const u8 x, const u8 m, const u8 M)
{
    u8 result = 0U;

    result = x;

    result = result < M ? result : M;
    result = result > m ? result : m;

    return result;
}

inline u8 u8_bit_at(const u8 byte, const u8 where)
{
    u8 result = 0U;

    result = (byte) & (1U << where);

    return result;
}

inline u8 u8_unpack_byte_32(const u32 bytes, const u8 where)
{
    u8 result = 0U;

    const u8 mask = 0xFF;

    result = (u8)(((((u32)mask) << where) & bytes) >> where);

    return result;
}

inline u8 u8_unpack_byte_64(const u64 bytes, const u8 where)
{
    u8 result = 0U;

    const u8 mask = 0xFF;

    result = (u8)(((((u64)mask) << where) & bytes) >> where);

    return result;
}

#endif

#if !defined(DMATH_DEFINTIONS_NO_LINEAR_ALGEBRA)

inline vec2_t *vec2_allocate(const f32 x, const f32 y, void *(*allocator)(size_t))
{
    #if defined(DMATH_NO_STDLIB)
        if (!allocator) return NULL;
        void* (*A)(size_t) = allocator;
    #else
        void * (*A)(size_t) = allocator != NULL ? allocator : malloc;
    #endif
    
    void * block = A(sizeof(vec2_t));
    if (!block) return NULL;

    vec2_t * result = (vec2_t*) block;
    result->x = x;
    result->y = y;

    return result;
}

inline vec2_t vec2(const f32 x, const f32 y)
{
    vec2_t result = { 0.0F };

    result.x = x;
    result.y = y;

    return result;
}

inline vec2_t vec2_0(void)
{
    vec2_t result = { 0.0F };

    return result;
}

inline vec2_t vec2_1(void)
{
    vec2_t result = { 0.0F };

    result.x = 1.0F;
    result.y = 1.0F;

    return result;
}

inline vec2_t vec2_e1(void)
{
    vec2_t result = { 0.0F };

    result.x = 1.0F;
    result.y = 0.0F;

    return result;
}

inline vec2_t vec2_e2(void)
{
    vec2_t result = { 0.0F };

    result.x = 0.0F;
    result.y = 1.0F;

    return result;
}

inline vec2_t vec2_v(const f32 v)
{
    vec2_t result = { 0.0F };

    result.x = v;
    result.y = v;

    return result;
}

inline vec2_t vec2_neg(const vec2_t v)
{
    vec2_t result= { 0.0F };

    result.x = -v.x;
    result.y = -v.y;

    return result;
}

inline vec2_t vec2_clamp(const vec2_t v, const vec2_t m, const vec2_t M)
{
    vec2_t result = { 0.0F };

    result.x = f32_clamp(v.x, m.x, M.x);
    result.y = f32_clamp(v.y, m.y, M.y);

    return result;
}

inline vec2_t vec2_add(const vec2_t v1, const vec2_t v2)
{
    vec2_t result = { 0.0F };

    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;

    return result;
}

inline vec2_t vec2_sub(const vec2_t v1, const vec2_t v2)
{
    vec2_t result = { 0.0F };

    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;

    return result;
}

inline vec2_t vec2_mul(const vec2_t v1, const vec2_t v2)
{
    vec2_t result = { 0.0F };

    result.x = v1.x * v2.x;
    result.y = v1.y * v2.y;

    return result;
}

inline vec2_t vec2_div(const vec2_t v1, const vec2_t v2)
{
    vec2_t result = { 0.0F };

    const f32 dx = v2.x != 0.0F ? v2.x : 1.0F;
    const f32 dy = v2.y != 0.0F ? v2.y : 1.0F;

    result.x = v1.x / dx;
    result.y = v1.y / dy;

    return result;
}

inline vec2_t vec2_add_v(const vec2_t v, const f32 val)
{
    vec2_t result = { 0.0F };

    result.x = v.x + val;
    result.y = v.y + val;

    return result;
}

inline vec2_t vec2_sub_v(const vec2_t v, const f32 val)
{
    vec2_t result = { 0.0F };

    result.x = v.x - val;
    result.y = v.y - val;

    return result;
}

inline vec2_t vec2_mul_v(const vec2_t v, const f32 val)
{
    vec2_t result = { 0.0F };

    result.x = v.x * val;
    result.y = v.y * val;

    return result;
}

inline vec2_t vec2_div_v(const vec2_t v, const f32 val)
{
    vec2_t result = { 0.0F };

    const f32 d = val != 0.0F ? val : 1.0F;

    result.x = v.x / d;
    result.y = v.y / d;

    return result;
}

inline vec2_t vec2_activate(const vec2_t v, f32 (*activation_func)(f32))
{
    vec2_t result = { 0.0F };

    if (!activation_func) return result;

    result.x = activation_func(v.x);
    result.y = activation_func(v.y);

    return result;
}

inline vec2_t vec2_normalize(const vec2_t v)
{
    vec2_t result = { 0.0F };

    const f32 length_sqr = v.x * v.x + v.y * v.y;
    f32 length = sqrtf(length_sqr);
    
    length = length == 0.0F ? 1.0F : length;

    result.x = v.x / length;
    result.y = v.y / length;

    return result;
}

inline vec2_t vec2_abs(const vec2_t v)
{
    vec2_t result = { 0.0F };

    result.x = fabsf(v.x);
    result.y = fabsf(v.y);

    return result;
}

inline vec2_t vec2_lerp_v(const vec2_t a, const vec2_t b, const f32 t)
{
    vec2_t result = { 0.0F };

    result.x = a.x + (b.x - a.x) * t;
    result.y = a.y + (b.y - a.y) * t;

    return result;

}

inline vec2_t vec2_slerp_v(const vec2_t a, const vec2_t b, const f32 t)
{
    vec2_t result = { 0.0F };

    f32 dot = a.x * b.x + a.y * b.y;

    f32 alen = sqrtf(a.x * a.x + a.y * a.y);
    f32 blen = sqrtf(b.x * b.x + b.y * b.y);

    f32 div = alen * blen;
    div = div == 0.0F ? dot : div;

    f32 angle = acosf(dot / div);
    vec2_t adot = { .x = a.x * dot, .y = a.y * dot };

    result.x = a.x * cosf(angle * t) + (b.x - adot.x) * sinf(angle * t);  
    result.y = a.y * cosf(angle * t) + (b.y - adot.y) * sinf(angle * t);  

    return result;
}

inline vec2_t vec2_lerp(const vec2_t a, const vec2_t b, const vec2_t t)
{
    vec2_t result = { 0.0F };

    result.x = a.x + (b.x - a.x) * t.x;
    result.y = a.y + (b.y - a.y) * t.y;

    return result;
}

inline vec2_t vec2_slerp(const vec2_t a, const vec2_t b, const vec2_t t)
{
    vec2_t result = { 0.0F };

    const f32 dot = a.x * b.x + a.y * b.y;

    const f32 alen = sqrtf(a.x * a.x + a.y * a.y);
    const f32 blen = sqrtf(b.x * b.x + b.y * b.y);

    f32 div = alen * blen;
    div = div == 0.0F ? dot : div;

    const f32 angle = acosf(dot / div);
    const vec2_t adot = { .x = a.x * dot, .y = a.y * dot };

    result.x = a.x * cosf(angle * t.x) + (b.x - adot.x) * sinf(angle * t.x);  
    result.y = a.y * cosf(angle * t.y) + (b.y - adot.y) * sinf(angle * t.y);  

    return result;
}

inline vec2_t vec2_linear_comb(const vec2_t v, const vec2_t u, const f32 a1, const f32 a2)
{
    vec2_t result = { 0.0F };

    result.x = v.x * a1 + u.x * a2;
    result.y = v.y * a1 + u.y * a2;

    return result;
}

inline vec2_t vec2_transform(const vec2_t v, const mat2x2_t M)
{
    vec2_t result = { 0.0F };

    result.x = v.x * M.a11 + v.y * M.a12;
    result.y = v.x * M.a21 + v.y * M.a22;

    return result;
}

inline vec2_t vec2_from_angle(const f32 alpha)
{
    vec2_t result = { 0.0F };

    result.x = cosf(alpha);
    result.x = sinf(alpha);

    return result;
}

inline vec2_t vec2_from_angle_and_r(const f32 alpha, const f32 radius)
{
    vec2_t result = { 0.0F };

    result.x = cosf(alpha) * radius;
    result.x = sinf(alpha) * radius;

    return result;
}

inline bool vec2_is_eq(const vec2_t v, const vec2_t u)
{
    bool result = false;

    result = v.x == u.x && v.y == u.y;

    return result;
}

inline bool vec2_is_eq_approx(const vec2_t v, const vec2_t u, f32 epsilon)
{
    bool result = false;

    result = fabs(v.x - u.x) < epsilon && fabs(v.y - u.y) < epsilon;

    return result;
}

inline f32 vec2_length(const vec2_t v)
{
    f32 result = 0.0F;

    const f32 length_sqr = v.x * v.x + v.y * v.y;
    result = sqrtf(length_sqr);

    return result;
}

inline f32 vec2_dist(const vec2_t v, const vec2_t u)
{
    f32 result = 0.0F;

    const f32 x = v.x - u.x;
    const f32 y = v.y - u.y;
    const f32 dist_sqr = x * x + y * y;
    result = sqrtf(dist_sqr);

    return result;
}

inline f32 vec2_dot(const vec2_t v, const vec2_t u)
{
    f32 result = 0.0F;

    result = v.x * u.x + v.y * u.y;

    return result;
}

inline f32 vec2_angle_between(const vec2_t v, const vec2_t u)
{
    f32 result = 0.0F;

    const f32 dot      = v.x * u.x + v.y * u.y;
    const f32 v_length = sqrtf(v.x * v.x + v.y * v.y);
    const f32 u_length = sqrtf(u.x * u.x + u.y * u.y);
    f32 d        = v_length * u_length;

    d = d == 0.0F ? dot : d; // Arbitrary angle when one of them is zero-vector

    result = acosf(dot / d);

    return result;
}

inline f32 vec2_dist_sqr(const vec2_t v, const vec2_t u)
{
    f32 result = 0.0F;

    const f32 x = v.x - u.x;
    const f32 y = v.y - u.y;

    result = x * x + y * y;

    return result;
}

inline f32 vec2_length_sqr(const vec2_t v)
{
    f32 result = 0.0F;

    result = v.x * v.x * + v.y * v.y;

    return result;
}

inline f32 vec2_lp_norm(const vec2_t v, const f32 p)
{
    f32 result = 0.0F;

    if (p <= 0.0F) return result;

    const f32 xp = powf(v.x, p);
    const f32 yp = powf(v.y, p);

    result = powf(xp + yp, 1.0F / p);

    return result;
}

inline f32 vec2_lp_norm_to_the_pth(const vec2_t v, const f32 p)
{
    f32 result = 0.0F;

    if (p <= 0.0F) return result;

    const f32 xp = powf(v.x, p);
    const f32 yp = powf(v.y, p);

    result = xp + yp;

    return result;
}

inline f32 vec2_max(const vec2_t v)
{
    f32 result = 0.0F;

    result = fmaxf(v.x, v.y);

    return result;
}

inline f32 vec2_min(const vec2_t v)
{
    f32 result = 0.0F;

    result = fminf(v.x, v.y);

    return result;
}

inline f32 vec2_avg(const vec2_t v)
{
    f32 result = 0.0F;

    result = (v.x + v.y) * 0.5F;
    
    return result;
}

inline f32 vec2_sum(const vec2_t v)
{
    f32 result = 0.0F;

    result = v.x + v.y;

    return result;
}

inline vec3_t *vec3_allocate(const f32 x, const f32 y, const f32 z, void *(*allocator)(size_t))
{
    #if defined(DMATH_NO_STDLIB)
        if (!allocator) return NULL;
        void* (*A)(size_t) = allocator;
    #else
        void * (*A)(size_t) = allocator != NULL ? allocator : malloc;
    #endif
    void* block = A(sizeof(vec3_t));
    if (!block) return NULL;

    vec3_t* result = (vec3_t*)block;

    result->x = x;
    result->y = y;
    result->z = z;

    return result;
}

inline vec3_t vec3(const f32 x, const f32 y, const f32 z)
{
    vec3_t result = { 0.0F };

    result.x = x;
    result.y = y;
    result.z = z;
    
    return result;
}

inline vec3_t vec3_0(void)
{
    vec3_t result = { 0.0F };
    
    return result;
}

inline vec3_t vec3_1(void)
{
    vec3_t result = { 0.0F };

    result.x = 1.0F;
    result.y = 1.0F;
    result.z = 1.0F;
    
    return result;
}

inline vec3_t vec3_v(const f32 val)
{
    vec3_t result = { 0.0F };

    result.x = val;
    result.y = val;
    result.z = val;
    
    return result;
}

inline vec3_t vec3_e1(void)
{
    vec3_t result = { 0.0F };

    result.x = 1.0F;
    result.y = 0.0F;
    result.z = 0.0F;
    
    return result;
}

inline vec3_t vec3_e2(void)
{
    vec3_t result = { 0.0F };

    result.x = 0.0F;
    result.y = 1.0F;
    result.z = 0.0F;
    
    return result;
}

inline vec3_t vec3_e3(void)
{
    vec3_t result = { 0.0F };

    result.x = 0.0F;
    result.y = 0.0F;
    result.z = 1.0F;
    
    return result;
}

inline vec3_t vec3_neg(const vec3_t v)
{
    vec3_t result = { 0.0F };

    result.x = -v.x;
    result.y = -v.y;
    result.z = -v.z;
    
    return result;
}

inline vec3_t vec3_clamp(const vec3_t v, const vec3_t m, const vec3_t M)
{
    vec3_t result = { 0.0F };

    result.x = f32_clamp(v.x, m.x, M.x);
    result.y = f32_clamp(v.y, m.y, M.y);
    result.z = f32_clamp(v.z, m.z, M.z);

    return result;
}

inline vec3_t vec3_add(const vec3_t v, const vec3_t u)
{
    vec3_t result = { 0.0F };

    result.x = v.x + u.x;
    result.y = v.y + u.y;
    result.z = v.z + u.z;
    
    return result;
}

inline vec3_t vec3_sub(const vec3_t v, const vec3_t u)
{
    vec3_t result = { 0.0F };

    result.x = v.x - u.x;
    result.y = v.y - u.y;
    result.z = v.z - u.z;
    
    return result;
}

inline vec3_t vec3_mul(const vec3_t v, const vec3_t u)
{
    vec3_t result = { 0.0F };

    result.x = v.x * u.x;
    result.y = v.y * u.y;
    result.z = v.z * u.z;
    
    return result;
}

inline vec3_t vec3_div(const vec3_t v, const vec3_t u)
{
    vec3_t result = { 0.0F };

    const f32 dx = u.x != 0.0F ? u.x : 1.0F;
    const f32 dy = u.y != 0.0F ? u.y : 1.0F;
    const f32 dz = u.z != 0.0F ? u.z : 1.0F;

    result.x = v.x / dx;
    result.y = v.y / dy;
    result.z = v.z / dz;
    
    return result;
}

inline vec3_t vec3_add_v(const vec3_t v, const f32 val)
{
    vec3_t result = { 0.0F };

    result.x = v.x + val;
    result.y = v.y + val;
    result.z = v.z + val;
    
    return result;
}

inline vec3_t vec3_sub_v(const vec3_t v, const f32 val)
{
    vec3_t result = { 0.0F };

    result.x = v.x - val;
    result.y = v.y - val;
    result.z = v.z - val;
    
    return result;
}

inline vec3_t vec3_mul_v(const vec3_t v, const f32 val)
{
    vec3_t result = { 0.0F };

    result.x = v.x * val;
    result.y = v.y * val;
    result.z = v.z * val;
    
    return result;
}

inline vec3_t vec3_div_v(const vec3_t v, const f32 val)
{
    vec3_t result = { 0.0F };

    const f32 dval = val != 0.0F ? val : 1.0F;

    result.x = v.x / dval;
    result.y = v.y / dval;
    result.z = v.z / dval;
    
    return result;
}

inline vec3_t vec3_activate(const vec3_t v, f32 (*activation_func)(f32))
{
    vec3_t result = { 0.0F };

    if (!activation_func) return result;

    result.x = activation_func(v.x);
    result.y = activation_func(v.y);
    result.z = activation_func(v.z);
    
    return result;
}

inline vec3_t vec3_normalize(const vec3_t v)
{
    vec3_t result = { 0.0F };

    const f32 sqr_len = v.x * v.x + v.y * v.y + v.z * v.z;
    const f32 len = sqrtf(sqr_len);

    result.x = v.x / len;
    result.y = v.y / len;
    result.z = v.z / len;
    
    return result;
}

inline vec3_t vec3_abs(const vec3_t v)
{
    vec3_t result = { 0.0F };

    result.x = fabsf(v.x);
    result.y = fabsf(v.y);
    result.z = fabsf(v.z);
    
    return result;
}

inline vec3_t vec3_lerp_v(const vec3_t a, const vec3_t b, const f32 t)
{
    vec3_t result = { 0.0F };

    result.x = a.x + (b.x - a.x) * t;
    result.y = a.y + (b.y - a.y) * t;
    result.z = a.z + (b.z - a.z) * t;
    
    return result;
}

inline vec3_t vec3_slerp_v(const vec3_t a, const vec3_t b, const f32 t)
{
    vec3_t result = { 0.0F };

    const f32 dot = a.x * b.x + a.y * b.y + a.z * b.z;

    const f32 alen = sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
    const f32 blen = sqrtf(b.x * b.x + b.y * b.y + b.z * b.z);

    f32 div = alen * blen;
    div = div == 0.0F ? dot : div;

    const f32 angle = acosf(dot / div);
    const vec3_t adot = { .x = a.x * dot, .y = a.y * dot, .z = a.y * dot };

    result.x = a.x * cosf(angle * t) + (b.x - adot.x) * sinf(angle * t);  
    result.y = a.y * cosf(angle * t) + (b.y - adot.y) * sinf(angle * t);  
    result.z = a.z * cosf(angle * t) + (b.z - adot.z) * sinf(angle * t);  

    return result;
}

inline vec3_t vec3_lerp(const vec3_t a, const vec3_t b, const vec3_t t)
{
    vec3_t result = { 0.0F };

    result.x = a.x + (b.x - a.x) * t.x;
    result.y = a.y + (b.y - a.y) * t.y;
    result.z = a.z + (b.z - a.z) * t.z;
    
    return result;
}

inline vec3_t vec3_slerp(const vec3_t a, const vec3_t b, const vec3_t t)
{
    vec3_t result = { 0.0F };

    const f32 dot = a.x * b.x + a.y * b.y + a.z * b.z;

    const f32 alen = sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
    const f32 blen = sqrtf(b.x * b.x + b.y * b.y + b.z * b.z);

    f32 div = alen * blen;
    div = div == 0.0F ? dot : div;

    const f32 angle = acosf(dot / div);
    const vec3_t adot = { .x = a.x * dot, .y = a.y * dot, .z = a.y * dot };

    result.x = a.x * cosf(angle * t.x) + (b.x - adot.x) * sinf(angle * t.x);  
    result.y = a.y * cosf(angle * t.y) + (b.y - adot.y) * sinf(angle * t.y);  
    result.z = a.z * cosf(angle * t.z) + (b.z - adot.z) * sinf(angle * t.z);  

    return result;
}

inline vec3_t vec3_linear_comb2(const vec3_t v, const vec3_t u, const f32 a1, const f32 a2)
{
    vec3_t result = { 0.0F };

    result.x = v.x * a1 + u.x * a2;
    result.y = v.y * a1 + u.y * a2;
    result.z = v.z * a1 + u.z * a2;
    
    return result;
}

inline vec3_t vec3_linear_comb3(const vec3_t v, const vec3_t u, const vec3_t w, const f32 a1, const f32 a2, const f32 a3)
{
    vec3_t result = { 0.0F };

    result.x = v.x * a1 + u.x * a2 + w.x * a3;
    result.y = v.y * a1 + u.y * a2 + w.y * a3;
    result.z = v.z * a1 + u.z * a2 + w.z * a3;
    
    return result;
}

inline vec3_t vec3_transform(const vec3_t v, const mat3x3_t M)
{
    vec3_t result = { 0.0F };

    result.x = v.x * M.a11 + v.y * M.a12 + v.z * M.a13;
    result.y = v.x * M.a21 + v.y * M.a22 + v.z * M.a23;
    result.z = v.x * M.a31 + v.y * M.a32 + v.z * M.a33;
    
    return result;
}

inline vec3_t vec3_from_angle(const f32 alpha, const f32 gamma)
{
    vec3_t result = { 0.0F };

    const f32 sina = sinf(alpha);
    const f32 cosa = cosf(alpha);
    const f32 sing = sinf(gamma);
    const f32 cosg = cosf(gamma);

    result.x = cosa * sing;
    result.y = sina * sing;
    result.z = cosg;
    
    return result;
}

inline vec3_t vec3_from_angle_and_r(const f32 alpha, const f32 gamma, const f32 radius)
{
    vec3_t result = { 0.0F };

    const f32 sina = sinf(alpha);
    const f32 cosa = cosf(alpha);
    const f32 sing = sinf(gamma);
    const f32 cosg = cosf(gamma);

    result.x = cosa * sing * radius;
    result.y = sina * sing * radius;
    result.z = cosg * radius;
    
    return result;
}

inline vec3_t vec3_cross(const vec3_t v, const vec3_t u)
{
    vec3_t result = { 0.0F };

    result.x = v.y * u.z - v.z * u.y;
    result.y = v.z * u.x - v.x * u.z;
    result.z = v.x * u.y - v.y * u.x;
    
    return result;
}

inline vec3_t vec3_cross_normalize(const vec3_t v, const vec3_t u)
{
    vec3_t result = { 0.0F };

    result.x = v.y * u.z - v.z * u.y;
    result.y = v.z * u.x - v.x * u.z;
    result.z = v.x * u.y - v.y * u.x;

    const f32 len_sqr = result.x * result.x + result.y * result.y + result.z * result.z;
    const f32 len = sqrtf(len_sqr);

    if (len != 0.0F)
    {
        result.x /= len;
        result.y /= len;
        result.z /= len;
    }
    
    return result;
}

inline bool vec3_is_eq(const vec3_t v, const vec3_t u)
{
    bool result = false;

    result = v.x == u.x && v.y == u.y && v.z == u.z;

    return result;
}

inline bool vec3_is_eq_approx(const vec3_t v, const vec3_t u, f32 epsilon)
{
    bool result = false;

    result = fabs(v.x - u.x) < epsilon && fabs(v.y - u.y) < epsilon && fabs(v.z - u.z) < epsilon;

    return result;
}

inline f32 vec3_length(const vec3_t v)
{
    f32 result = 0.0F;

    const f32 len_sqr = v.x * v.x + v.y * v.y + v.z * v.z;
    result = sqrtf(len_sqr);

    return result;
}

inline f32 vec3_dist(const vec3_t v, const vec3_t u)
{
    f32 result = 0.0F;
    
    const f32 dx = v.x - u.x; 
    const f32 dy = v.y - u.y; 
    const f32 dz = v.z - u.z;

    const f32 dist_sqr = dx * dx + dy *  dy + dz * dz;
    result = sqrtf(dist_sqr);

    return result;
}

inline f32 vec3_dot(const vec3_t v, const vec3_t u)
{
    f32 result = 0.0F;

    result = v.x * u.x + v.y * u.y + v.z * u.z;

    return result;
}

inline f32 vec3_angle_between(const vec3_t v, const vec3_t u)
{
    f32 result = 0.0F;

    const f32 dot = v.x * u.x + v.y * u.y + v.z * u.z;
    
    const f32 vlen = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    const f32 ulen = sqrtf(u.x * u.x + u.y * u.y + u.z * u.z);

    f32 div = vlen * ulen;
    div = div == 0.0F ? dot : div;

    result = acosf(dot / div);

    return result;
}

inline f32 vec3_dist_sqr(const vec3_t v, const vec3_t u)
{
    f32 result = 0.0F;
    
    const f32 dx = v.x - u.x; 
    const f32 dy = v.y - u.y; 
    const f32 dz = v.z - u.z;

    result = dx * dx + dy *  dy + dz * dz;

    return result;
}

inline f32 vec3_length_sqr(const vec3_t v)
{
    f32 result = 0.0F;

    result = v.x * v.x + v.y * v.y + v.z * v.z;

    return result;
}

inline f32 vec3_lp_norm(const vec3_t v, const f32 p)
{
    f32 result = 0.0F;

    if (p <= 0.0F) return result;

    const f32 xp = powf(v.x, p);
    const f32 yp = powf(v.y, p);
    const f32 zp = powf(v.z, p);

    result = powf(xp + yp + zp, 1.0F / p);

    return result;
}

inline f32 vec3_lp_norm_to_the_pth(const vec3_t v, const f32 p)
{
    f32 result = 0.0F;

    if (p <= 0.0F) return result;

    const f32 xp = powf(v.x, p);
    const f32 yp = powf(v.y, p);
    const f32 zp = powf(v.z, p);

    result = xp + yp + zp;

    return result;
}

inline f32 vec3_max(const vec3_t v)
{
    f32 result = 0.0F;

    result = fmaxf(v.x, fmaxf(v.y, v.z));

    return result;
}

inline f32 vec3_min(const vec3_t v)
{
    f32 result = 0.0F;

    result = fminf(v.x, fminf(v.y, v.z));

    return result;
}

inline f32 vec3_avg(const vec3_t v)
{
    f32 result = 0.0F;

    result = (v.x + v.y + v.z) / 3.0F;

    return result;
}

inline f32 vec3_sum(const vec3_t v)
{
    f32 result = 0.0F;

    result = v.x + v.y + v.z;

    return result;
}

inline vec4_t * vec4_allocate(const f32 x, const f32 y, const f32 z, const f32 w, void* (*allocator)(size_t))
{
    #if defined(DMATH_NO_STDLIB)
        if (!allocator) return NULL;
        void* (*A)(size_t) = allocator;
    #else
        void * (*A)(size_t) = allocator != NULL ? allocator : malloc;
    #endif
    void* block = A(sizeof(vec4_t));
    if (!block) return NULL;

    vec4_t* result = (vec4_t*)block;

    result->x = x;
    result->y = y;
    result->z = z;
    result->w = w;

    return result;
}

inline vec4_t vec4(const f32 x, const f32 y, const f32 z, const f32 w)
{
    vec4_t result = { 0.0F };

    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;

    return result;
}

inline vec4_t vec4_0(void)
{
    vec4_t result = { 0.0F };

    result.x = 0.0F;
    result.y = 0.0F;
    result.z = 0.0F;
    result.w = 0.0F;

    return result;
}

inline vec4_t vec4_1(void)
{
    vec4_t result = { 0.0F };

    result.x = 1.0F;
    result.y = 1.0F;
    result.z = 1.0F;
    result.w = 1.0F;

    return result;
}

inline vec4_t vec4_v(const f32 val)
{
    vec4_t result = { 0.0F };

    result.x = val;
    result.y = val;
    result.z = val;
    result.w = val;

    return result;
}

inline vec4_t vec4_e1(void)
{
    vec4_t result = { 0.0F };

    result.x = 1.0F;
    result.y = 0.0F;
    result.z = 0.0F;
    result.w = 0.0F;

    return result;
}

inline vec4_t vec4_e2(void)
{
    vec4_t result = { 0.0F };

    result.x = 0.0F;
    result.y = 1.0F;
    result.z = 0.0F;
    result.w = 0.0F;

    return result;
}

inline vec4_t vec4_e3(void)
{
    vec4_t result = { 0.0F };

    result.x = 0.0F;
    result.y = 0.0F;
    result.z = 1.0F;
    result.w = 0.0F;

    return result;
}

inline vec4_t vec4_e4(void)
{
    vec4_t result = { 0.0F };

    result.x = 0.0F;
    result.y = 0.0F;
    result.z = 0.0F;
    result.w = 1.0F;

    return result;
}

inline vec4_t vec4_neg(const vec4_t v)
{
    vec4_t result = { 0.0F };

    result.x = -v.x;
    result.y = -v.y;
    result.z = -v.z;
    result.w = -v.w;

    return result;
}

inline vec4_t vec4_abs(const vec4_t v)
{
    vec4_t result = { 0.0F };

    result.x = fabsf(v.x);
    result.y = fabsf(v.y);
    result.z = fabsf(v.z);
    result.w = fabsf(v.w);

    return result;
}

inline vec4_t vec4_normalize(const vec4_t v)
{
    vec4_t result = { 0.0F };

    f32 len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
    f32 div = len > 0.0F ? len : 1.0F;

    result.x = v.x / div;
    result.y = v.y / div;
    result.z = v.z / div;
    result.w = v.w / div;

    return result;
}

inline vec4_t vec4_add(const vec4_t v, const vec4_t u)
{
    vec4_t result = { 0.0F };

    result.x = v.x + u.x;
    result.y = v.y + u.y;
    result.z = v.z + u.z;
    result.w = v.w + u.w;
    
    return result;
}

inline vec4_t vec4_sub(const vec4_t v, const vec4_t u)
{
    vec4_t result = { 0.0F };

    result.x = v.x - u.x;
    result.y = v.y - u.y;
    result.z = v.z - u.z;
    result.w = v.w - u.w;
    
    return result;
}

inline vec4_t vec4_mul(const vec4_t v, const vec4_t u)
{
    vec4_t result = { 0.0F };

    result.x = v.x * u.x;
    result.y = v.y * u.y;
    result.z = v.z * u.z;
    result.w = v.w * u.w;
    
    return result;
}

inline vec4_t vec4_div(const vec4_t v, const vec4_t u)
{
    vec4_t result = { 0.0F };

    const f32 dx = u.x != 0.0F ? u.x : 1.0F; 
    const f32 dy = u.y != 0.0F ? u.y : 1.0F; 
    const f32 dz = u.z != 0.0F ? u.z : 1.0F; 
    const f32 dw = u.w != 0.0F ? u.w : 1.0F; 

    result.x = v.x / dx;
    result.y = v.y / dy;
    result.z = v.z / dz;
    result.w = v.w / dw;
    
    return result;
}

inline vec4_t vec4_add_v(const vec4_t v, const f32 val)
{
    vec4_t result = { 0.0F };

    result.x = v.x + val;
    result.y = v.y + val;
    result.z = v.z + val;
    result.w = v.w + val;
    
    return result;
}

inline vec4_t vec4_sub_v(const vec4_t v, const f32 val)
{
    vec4_t result = { 0.0F };

    result.x = v.x - val;
    result.y = v.y - val;
    result.z = v.z - val;
    result.w = v.w - val;
    
    return result;
}

inline vec4_t vec4_mul_v(const vec4_t v, const f32 val)
{
    vec4_t result = { 0.0F };

    result.x = v.x * val;
    result.y = v.y * val;
    result.z = v.z * val;
    result.w = v.w * val;
    
    return result;
}

inline vec4_t vec4_div_v(const vec4_t v, const f32 val)
{
    vec4_t result = { 0.0F };

    f32 div = val != 0.0F ? val : 1.0F;

    result.x = v.x / div;
    result.y = v.y / div;
    result.z = v.z / div;
    result.w = v.w / div;
    
    return result;
}

inline vec4_t vec4_activate(const vec4_t v, f32 (*activation_func)(f32))
{
    vec4_t result = { 0.0F };

    if (!activation_func) return result;

    result.x = activation_func(v.x);
    result.y = activation_func(v.y);
    result.z = activation_func(v.z);
    result.w = activation_func(v.w);
    
    return result;
}

inline vec4_t vec4_clamp(const vec4_t v, const vec4_t m, const vec4_t M)
{
    vec4_t result = { 0.0F };

    result.x = f32_clamp(v.x, m.x, M.x);
    result.y = f32_clamp(v.y, m.y, M.y);
    result.z = f32_clamp(v.z, m.z, M.z);
    result.w = f32_clamp(v.w, m.w, M.w);
    
    return result;
}

inline vec4_t vec4_lerp_v(const vec4_t a, const vec4_t b, const f32 t)
{
    vec4_t result = { 0.0F };

    result.x = f32_lerp(a.x, b.x, t);
    result.y = f32_lerp(a.y, b.y, t);
    result.z = f32_lerp(a.z, b.z, t);
    result.w = f32_lerp(a.w, b.w, t);
    
    return result;
}

inline vec4_t vec4_slerp_v(const vec4_t a, const vec4_t b, const f32 t)
{
    vec4_t result = { 0.0F };

    const f32 dot = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;

    const f32 alen = sqrtf(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
    const f32 blen = sqrtf(b.x * b.x + b.y * b.y + b.z * b.z + b.w * b.w);

    f32 div = alen * blen;
    div = div == 0.0F ? dot : div;

    const f32 angle = acosf(dot / div);
    const vec4_t adot = { .x = a.x * dot, .y = a.y * dot, .z = a.y * dot, .w = a.w * dot };

    result.x = a.x * cosf(angle * t) + (b.x - adot.x) * sinf(angle * t);  
    result.y = a.y * cosf(angle * t) + (b.y - adot.y) * sinf(angle * t);  
    result.z = a.z * cosf(angle * t) + (b.z - adot.z) * sinf(angle * t);  
    result.w = a.w * cosf(angle * t) + (b.w - adot.w) * sinf(angle * t);  

    return result;
}

inline vec4_t vec4_lerp(const vec4_t a, const vec4_t b, const vec4_t t)
{
    vec4_t result = { 0.0F };

    result.x = f32_lerp(a.x, b.x, t.x);
    result.y = f32_lerp(a.y, b.y, t.y);
    result.z = f32_lerp(a.z, b.z, t.z);
    result.w = f32_lerp(a.w, b.w, t.w);
    
    return result;
}

inline vec4_t vec4_slerp(const vec4_t a, const vec4_t b, const vec4_t t)
{
    vec4_t result = { 0.0F };

    const f32 dot = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;

    const f32 alen = sqrtf(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
    const f32 blen = sqrtf(b.x * b.x + b.y * b.y + b.z * b.z + b.w * b.w);

    f32 div = alen * blen;
    div = div == 0.0F ? dot : div;

    const f32 angle = acosf(dot / div);
    const vec4_t adot = { .x = a.x * dot, .y = a.y * dot, .z = a.y * dot, .w = a.w * dot };

    result.x = a.x * cosf(angle * t.x) + (b.x - adot.x) * sinf(angle * t.x);  
    result.y = a.y * cosf(angle * t.y) + (b.y - adot.y) * sinf(angle * t.y);  
    result.z = a.z * cosf(angle * t.z) + (b.z - adot.z) * sinf(angle * t.z);  
    result.w = a.w * cosf(angle * t.w) + (b.w - adot.w) * sinf(angle * t.w);  

    return result;
}

inline vec4_t vec4_linear_comb2(const vec4_t v, const vec4_t u, const f32 a1, const f32 a2)
{
    vec4_t result = { 0.0F };

    result.x = v.x * a1 + u.x * a2;
    result.y = v.y * a1 + u.y * a2;
    result.z = v.z * a1 + u.z * a2;
    result.w = v.w * a1 + u.w * a2;
    
    return result;
}

inline vec4_t vec4_linear_comb3(const vec4_t v, const vec4_t u, const vec4_t w, const f32 a1, const f32 a2, const f32 a3)
{
    vec4_t result = { 0.0F };

    result.x = v.x * a1 + u.x * a2 + w.x * a3;
    result.y = v.y * a1 + u.y * a2 + w.y * a3;
    result.z = v.z * a1 + u.z * a2 + w.z * a3;
    result.w = v.w * a1 + u.w * a2 + w.w * a3;
    
    return result;
}

inline vec4_t vec4_linear_comb4(const vec4_t v, const vec4_t u, const vec4_t w, const vec4_t r, const f32 a1, const f32 a2, const f32 a3, const f32 a4)
{
    vec4_t result = { 0.0F };

    result.x = v.x * a1 + u.x * a2 + w.x * a3 + r.x * a4;
    result.y = v.y * a1 + u.y * a2 + w.y * a3 + r.y * a4;
    result.z = v.z * a1 + u.z * a2 + w.z * a3 + r.z * a4;
    result.w = v.w * a1 + u.w * a2 + w.w * a3 + r.w * a4;
    
    return result;
}

inline vec4_t vec4_transform(const vec4_t v, const mat4x4_t M)
{
    vec4_t result = { 0.0F };

    result.x = v.x * M.a11 + v.y * M.a12 + v.z * M.a13 + v.w * M.a14;
    result.y = v.x * M.a21 + v.y * M.a22 + v.z * M.a23 + v.w * M.a24;
    result.z = v.x * M.a31 + v.y * M.a32 + v.z * M.a33 + v.w * M.a34;
    result.w = v.x * M.a41 + v.y * M.a42 + v.z * M.a43 + v.w * M.a44;

    return result;
}

inline vec4_t vec4_from_angle(const f32 alpha, const f32 beta, const f32 gamma)
{
    vec4_t result = { 0.0F };

    const f32 sina = sinf(alpha);
    const f32 cosa = cosf(alpha);
    const f32 sinb = sinf(beta);
    const f32 cosb = cosf(beta);
    const f32 sing = sinf(gamma);
    const f32 cosg = cosf(gamma);

    result.x = cosa * sinb * sing;
    result.y = sina * sinb * sing;
    result.z = cosb * sing;
    result.w = cosg;

    return result;
}

inline vec4_t vec4_from_angle_and_r(const f32 alpha, const f32 beta, const f32 gamma, const f32 radius)
{
    vec4_t result = { 0.0F };

    const f32 sina = sinf(alpha);
    const f32 cosa = cosf(alpha);
    const f32 sinb = sinf(beta);
    const f32 cosb = cosf(beta);
    const f32 sing = sinf(gamma);
    const f32 cosg = cosf(gamma);

    result.x = cosa * sinb * sing * radius;
    result.y = sina * sinb * sing * radius;
    result.z = cosb * sing * radius;
    result.w = cosg * radius;

    return result;
}

inline bool vec4_is_eq(const vec4_t v, const vec4_t u)
{
    bool result = false;

    result = ((v.x == u.x) && (v.y == u.y) && (v.z == u.z) && (v.w == u.w));

    return result;
}

inline bool vec4_is_eq_approx(const vec4_t v, const vec4_t u, f32 epsilon)
{
    bool result = false;

    result = fabs(v.x - u.x) < epsilon && fabs(v.y - u.y) < epsilon && fabs(v.z - u.z) < epsilon && fabs(v.w - u.w) < epsilon;

    return result;
}

inline f32 vec4_length(const vec4_t v)
{
    f32 result = 0.0F;

    result = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);

    return result;
}

inline f32 vec4_dist(const vec4_t v, const vec4_t u)
{
    f32 result = 0.0F;

    const f32 dx = v.x - u.x;
    const f32 dy = v.y - u.y;
    const f32 dz = v.z - u.z;
    const f32 dw = v.w - u.w;

    result = sqrtf(dx * dx + dy * dy + dz * dz + dw * dw);

    return result;
}

inline f32 vec4_length_sqr(const vec4_t v)
{
    f32 result = 0.0F;

    result = v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;

    return result;
}

inline f32 vec4_dist_sqr(const vec4_t v, const vec4_t u)
{
    f32 result = 0.0F;

    const f32 dx = v.x - u.x;
    const f32 dy = v.y - u.y;
    const f32 dz = v.z - u.z;
    const f32 dw = v.w - u.w;

    result = dx * dx + dy * dy + dz * dz + dw * dw;

    return result;
}

inline f32 vec4_dot(const vec4_t v, const vec4_t u)
{
    f32 result = 0.0F;

    result = v.x * u.x + v.y * u.y + v.z * u.z + v.w * u.w; 

    return result;
}

inline f32 vec4_angle_between(const vec4_t v, const vec4_t u)
{
    f32 result = 0.0F;

    const f32 dot = v.x * u.x + v.y * u.y + v.z * u.z + v.w * u.w; 

    const f32 vlen = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
    const f32 ulen = sqrtf(u.x * u.x + u.y * u.y + u.z * u.z + u.w * u.w);

    f32 div = vlen * ulen;
    div = div == 0.0F ? dot : div;

    result = acosf(dot / div);

    return result;
}

inline f32 vec4_lp_norm(const vec4_t v, const f32 p)
{
    f32 result = 0.0F;

    if (p <= 0.0F) return result;

    const f32 xp = powf(v.x, p);
    const f32 yp = powf(v.y, p);
    const f32 zp = powf(v.z, p);
    const f32 wp = powf(v.w, p);

    result = powf(xp + yp + zp + wp, 1.0F / p);

    return result;
}

inline f32 vec4_lp_norm_to_the_pth(const vec4_t v, const f32 p)
{
    f32 result = 0.0F;

    if (p <= 0.0F) return result;

    const f32 xp = powf(v.x, p);
    const f32 yp = powf(v.y, p);
    const f32 zp = powf(v.z, p);
    const f32 wp = powf(v.w, p);

    result = xp + yp + zp + wp;

    return result;
}

inline f32 vec4_max(const vec4_t v)
{
    f32 result = 0.0F;

    result = fmaxf(v.x, fmaxf(v.y, fmaxf(v.z, v.w)));

    return result;
}

inline f32 vec4_min(const vec4_t v)
{
    f32 result = 0.0F;

    result = fminf(v.x, fminf(v.y, fminf(v.z, v.w)));

    return result;
}

inline f32 vec4_avg(const vec4_t v)
{
    f32 result = 0.0F;

    result = (v.x + v.y + v.z + v.w) * 0.25F;

    return result;
}

inline f32 vec4_sum(const vec4_t v)
{
    f32 result = 0.0F;
    
    result = v.x + v.y + v.z + v.w;

    return result;
}

inline mat2x2_t * mat2x2_allocate(const f32 a11, const f32 a12, const f32 a21, const f32 a22, void* (*allocator)(size_t))
{
    #if defined(DMATH_NO_STDLIB)
        if (!allocator) return NULL;
        void* (*A)(size_t) = allocator;
    #else
        void * (*A)(size_t) = allocator != NULL ? allocator : malloc;
    #endif
    void* block = A(sizeof(mat2x2_t));
    if (!block) return NULL;

    mat2x2_t* result = (mat2x2_t*)block;

    result->a11 = a11;
    result->a12 = a12;
    result->a21 = a21;
    result->a22 = a22;

    return result;    
}

inline mat2x2_t mat2x2(const f32 a11, const f32 a12, const f32 a21, const f32 a22)
{
    mat2x2_t result = { 0.0F };

    result.a11 = a11;
    result.a12 = a12;
    result.a21 = a21;
    result.a22 = a22;

    return result;
}

inline mat2x2_t mat2x2_0(void)
{
    mat2x2_t result = { 0.0F };

    result.a11 = 0.0F;
    result.a12 = 0.0F;
    result.a21 = 0.0F;
    result.a22 = 0.0F;

    return result;
}

inline mat2x2_t mat2x2_1(void)
{
    mat2x2_t result = { 0.0F };

    result.a11 = 1.0F;
    result.a12 = 1.0F;
    result.a21 = 1.0F;
    result.a22 = 1.0F;

    return result;
}

inline mat2x2_t mat2x2_unit(void)
{
    mat2x2_t result = { 0.0F };

    result.a11 = 1.0F;
    result.a12 = 0.0F;
    result.a21 = 0.0F;
    result.a22 = 1.0F;

    return result;
}

inline mat2x2_t mat2x2_scalar(f32 val)
{
    mat2x2_t result = { 0.0F };

    result.a11 = val;
    result.a12 = 0.0F;
    result.a21 = 0.0F;
    result.a22 = val;

    return result;
}

inline mat2x2_t mat2x2_v(const f32 val)
{
    mat2x2_t result = { 0.0F };

    result.a11 = val;
    result.a12 = val;
    result.a21 = val;
    result.a22 = val;

    return result;
}

inline mat2x2_t mat2x2_rotation_matrix(const f32 alpha)
{
    mat2x2_t result = { 0.0F };

    result.a11 = cosf(alpha);
    result.a12 = -sinf(alpha);
    result.a21 = sinf(alpha);
    result.a22 = cosf(alpha);

    return result;
}

inline mat2x2_t mat2x2_rows(const vec2_t R1, const vec2_t R2)
{
    mat2x2_t result = { 0.0F };

    result.a11 = R1.x;
    result.a12 = R1.y;
    result.a21 = R2.x;
    result.a22 = R2.y;

    return result;
}

inline mat2x2_t mat2x2_cols(const vec2_t C1, const vec2_t C2)
{
    mat2x2_t result = { 0.0F };

    result.a11 = C1.x;
    result.a12 = C2.x;
    result.a21 = C1.y;
    result.a22 = C2.y;

    return result;
}

inline mat2x2_t mat2x2_neg(const mat2x2_t m)
{
    mat2x2_t result = { 0.0F };

    result.a11 = -m.a11;
    result.a12 = -m.a12;
    result.a21 = -m.a21;
    result.a22 = -m.a22;

    return result;
}

inline mat2x2_t mat2x2_abs(const mat2x2_t m)
{
    mat2x2_t result = { 0.0F };

    result.a11 = fabsf(m.a11);
    result.a12 = fabsf(m.a12);
    result.a21 = fabsf(m.a21);
    result.a22 = fabsf(m.a22);

    return result;
}

inline mat2x2_t mat2x2_transpose(const mat2x2_t m)
{
    mat2x2_t result = { 0.0F };

    result.a11 = m.a11;
    result.a12 = m.a21;
    result.a21 = m.a12;
    result.a22 = m.a22;

    return result;    
}

inline mat2x2_t mat2x2_adjugate(const mat2x2_t m)
{
    mat2x2_t result = { 0.0F };

    result.a11 = m.a22;
    result.a12 = -m.a21;
    result.a21 = -m.a12;
    result.a22 = m.a11;

    return result;
}

inline mat2x2_t mat2x2_inverse(const mat2x2_t m)
{
    mat2x2_t result = { 0.0F };

    const f32 div = m.a11 * m.a22 - m.a12 * m.a21;

    if (div == 0.0f) return result; 

    result.a11 =  m.a22 / div;
    result.a12 = -m.a21 / div;
    result.a21 = -m.a12 / div;
    result.a22 =  m.a11 / div;

    return result;
}

inline mat2x2_t mat2x2_gram(const vec2_t v1, const vec2_t v2)
{
    mat2x2_t result = { 0.0F };

    const f32 dot = v1.x * v2.x + v1.y * v2.y;

    result.a11 = v1.x * v1.x + v1.y * v1.y;
    result.a12 = dot;
    result.a21 = dot;
    result.a22 = v2.x * v2.x + v2.y * v2.y;

    return result;
}

inline mat2x2_t mat2x2_add(const mat2x2_t m1, const mat2x2_t m2)
{
    mat2x2_t result = { 0.0F };

    result.a11 = m1.a11 + m2.a11;
    result.a12 = m1.a12 + m2.a12;
    result.a21 = m1.a21 + m2.a21;
    result.a22 = m1.a22 + m2.a22;

    return result;
}

inline mat2x2_t mat2x2_sub(const mat2x2_t m1, const mat2x2_t m2)
{
    mat2x2_t result = { 0.0F };

    result.a11 = m1.a11 - m2.a11;
    result.a12 = m1.a12 - m2.a12;
    result.a21 = m1.a21 - m2.a21;
    result.a22 = m1.a22 - m2.a22;

    return result;
}

inline mat2x2_t mat2x2_mul(const mat2x2_t m1, const mat2x2_t m2)
{
    mat2x2_t result = { 0.0F };

    result.a11 = m1.a11 * m2.a11;
    result.a12 = m1.a12 * m2.a12;
    result.a21 = m1.a21 * m2.a21;
    result.a22 = m1.a22 * m2.a22;

    return result;
}

inline mat2x2_t mat2x2_div(const mat2x2_t m1, const mat2x2_t m2)
{
    mat2x2_t result = { 0.0F };

    const f32 div11 = m2.a11 == 0.0F ? 1.0F : m2.a11;
    const f32 div12 = m2.a12 == 0.0F ? 1.0F : m2.a12;
    const f32 div21 = m2.a21 == 0.0F ? 1.0F : m2.a21;
    const f32 div22 = m2.a22 == 0.0F ? 1.0F : m2.a22;

    result.a11 = m1.a11 / div11;
    result.a12 = m1.a12 / div12;
    result.a21 = m1.a21 / div21;
    result.a22 = m1.a22 / div22;

    return result;
}

inline mat2x2_t mat2x2_add_v(const mat2x2_t m1, const f32 val)
{
    mat2x2_t result = { 0.0F };

    result.a11 = m1.a11 + val;
    result.a12 = m1.a12 + val;
    result.a21 = m1.a21 + val;
    result.a22 = m1.a22 + val;

    return result;
}

inline mat2x2_t mat2x2_sub_v(const mat2x2_t m1, const f32 val)
{
    mat2x2_t result = { 0.0F };

    result.a11 = m1.a11 - val;
    result.a12 = m1.a12 - val;
    result.a21 = m1.a21 - val;
    result.a22 = m1.a22 - val;

    return result;
}

inline mat2x2_t mat2x2_mul_v(const mat2x2_t m1, const f32 val)
{
    mat2x2_t result = { 0.0F };

    result.a11 = m1.a11 * val;
    result.a12 = m1.a12 * val;
    result.a21 = m1.a21 * val;
    result.a22 = m1.a22 * val;

    return result;
}

inline mat2x2_t mat2x2_div_v(const mat2x2_t m1, const f32 val)
{
    mat2x2_t result = { 0.0F };

    f32 dval = val == 0.0F ? 1.0F : val;

    result.a11 = m1.a11 / dval;
    result.a12 = m1.a12 / dval;
    result.a21 = m1.a21 / dval;
    result.a22 = m1.a22 / dval;

    return result;
}

inline mat2x2_t mat2x2_mat_mul(const mat2x2_t m1, const mat2x2_t m2)
{
    mat2x2_t result = { 0.0F };

    result.a11 = m1.a11 * m2.a11 + m1.a12 * m2.a21;
    result.a12 = m1.a11 * m2.a21 + m1.a12 * m2.a22;
    result.a21 = m1.a21 * m2.a11 + m1.a22 * m2.a21;
    result.a22 = m1.a21 * m2.a21 + m1.a22 * m2.a22;

    return result;
}

inline mat2x2_t mat2x2_lerp_v(const mat2x2_t a, const mat2x2_t b, const f32 t)
{
    mat2x2_t result = { 0.0F };

    result.a11 = f32_lerp(a.a11, b.a11, t);
    result.a12 = f32_lerp(a.a12, b.a12, t);
    result.a21 = f32_lerp(a.a21, b.a21, t);
    result.a22 = f32_lerp(a.a22, b.a22, t);

    return result;
}

inline mat2x2_t mat2x2_lerp(const mat2x2_t a, const mat2x2_t b, const mat2x2_t t)
{
    mat2x2_t result = { 0.0F };

    result.a11 = f32_lerp(a.a11, b.a11, t.a11);
    result.a12 = f32_lerp(a.a12, b.a12, t.a12);
    result.a21 = f32_lerp(a.a21, b.a21, t.a21);
    result.a22 = f32_lerp(a.a22, b.a22, t.a22);

    return result;
}

inline mat2x2_t mat2x2_linear_comb2(const mat2x2_t m1, const mat2x2_t m2, const f32 a1, const f32 a2)
{
    mat2x2_t result = { 0.0F };

    result.a11 = m1.a11 * a1 + m2.a11 * a2;
    result.a12 = m1.a12 * a1 + m2.a12 * a2;
    result.a21 = m1.a21 * a1 + m2.a21 * a2;
    result.a22 = m1.a22 * a1 + m2.a22 * a2;

    return result;
}

inline mat2x2_t mat2x2_linear_comb3(const mat2x2_t m1, const mat2x2_t m2, const mat2x2_t m3, const f32 a1, const f32 a2, const f32 a3)
{
    mat2x2_t result = { 0.0F };

    result.a11 = m1.a11 * a1 + m2.a11 * a2 + m3.a11 * a3;
    result.a12 = m1.a12 * a1 + m2.a12 * a2 + m3.a12 * a3;
    result.a21 = m1.a21 * a1 + m2.a21 * a2 + m3.a21 * a3;
    result.a22 = m1.a22 * a1 + m2.a22 * a2 + m3.a22 * a3;

    return result;
}

inline mat2x2_t mat2x2_linear_comb4(const mat2x2_t m1, const mat2x2_t m2, const mat2x2_t m3, const mat2x2_t m4, const f32 a1, const f32 a2, const f32 a3, const f32 a4)
{
    mat2x2_t result = { 0.0F };

    result.a11 = m1.a11 * a1 + m2.a11 * a2 + m3.a11 * a3 + m4.a11 * a4;
    result.a12 = m1.a12 * a1 + m2.a12 * a2 + m3.a12 * a3 + m4.a12 * a4;
    result.a21 = m1.a21 * a1 + m2.a21 * a2 + m3.a21 * a3 + m4.a21 * a4;
    result.a22 = m1.a22 * a1 + m2.a22 * a2 + m3.a22 * a3 + m4.a22 * a4;

    return result;
}

inline vec2_t mat2x2_col1(const mat2x2_t m)
{
    vec2_t result = { 0.0F };

    result.x = m.a11;
    result.y = m.a21;

    return result;
}

inline vec2_t mat2x2_col2(const mat2x2_t m)
{
    vec2_t result = { 0.0F };

    result.x = m.a12;
    result.y = m.a22;

    return result;
}

inline vec2_t mat2x2_row1(const mat2x2_t m)
{
    vec2_t result = { 0.0F };

    result.x = m.a11;
    result.y = m.a12;

    return result;
}

inline vec2_t mat2x2_row2(const mat2x2_t m)
{
    vec2_t result = { 0.0F };

    result.x = m.a21;
    result.y = m.a22;

    return result;
}

inline vec2_t mat2x2_diagonal(const mat2x2_t m)
{
    vec2_t result = { 0.0F };

    result.x = m.a11;
    result.y = m.a22;

    return result;
}

inline bool mat2x2_is_eq(const mat2x2_t m1, const mat2x2_t m2)
{
    bool result = false;

    result = m1.a11 == m2.a11 && m1.a12 == m2.a12 && m1.a21 == m2.a21 && m1.a22 == m2.a22;

    return result;
}

inline bool mat2x2_is_eq_approx(const mat2x2_t m1, const mat2x2_t m2, const f32 epsilon)
{
    bool result = false;

    result = fabsf(m1.a11 - m2.a11) < epsilon && 
             fabsf(m1.a12 - m2.a12) < epsilon && 
             fabsf(m1.a21 - m2.a21) < epsilon && 
             fabsf(m1.a22 - m2.a22) < epsilon;

    return result;
}

inline bool mat2x2_is_invertable(const mat2x2_t m)
{
    bool result = false;

    result = m.a11 * m.a22 != m.a12 * m.a22;

    return result;
}

inline bool mat2x2_is_symmetric(const mat2x2_t m)
{
    bool result = false;
    
    result = m.a12 == m.a22;
    
    return result;
}

inline bool mat2x2_is_anti_symmetric(const mat2x2_t m)
{
    bool result = false;
    
    result = m.a12 == -m.a22;
    
    return result;
}

inline f32 mat2x2_trace(const mat2x2_t m)
{
    f32 result = 0.0F;

    result = m.a11 + m.a22;

    return result;
}

inline f32 mat2x2_norm(const mat2x2_t m)
{
    f32 result = 0.0F;

    const f32 sum_entries_sqr = m.a11 * m.a11 + m.a12 * m.a12 + m.a21 * m.a21 + m.a22 * m.a22;
    result = sqrtf(sum_entries_sqr);

    return result;
}

inline f32 mat2x2_max(const mat2x2_t m)
{
    f32 result = 0.0F;

    result = fmaxf(m.a11, fmaxf(m.a12, fmaxf(m.a21, m.a22)));

    return result;
}

inline f32 mat2x2_min(const mat2x2_t m)
{
    f32 result = 0.0F;
    
    result = fminf(m.a11, fminf(m.a12, fminf(m.a21, m.a22)));
    
    return result;
}

inline f32 mat2x2_avg(const mat2x2_t m)
{
    f32 result = 0.0F;
    
    result = (m.a11 + m.a12 + m.a21 + m.a22) * 0.25F;
    
    return result;
}

inline f32 mat2x2_det(const mat2x2_t m)
{
    f32 result = 0.0F;
    
    result = m.a11 * m.a22 - m.a12 * m.a21;
    
    return result;
}

inline f32 mat2x2_sum(const mat2x2_t m)
{
    f32 result = 0.0F;
    
    result = m.a11 + m.a12 + m.a21 + m.a22;
    
    return result;
}

#endif

#if !defined(DMATH_DEFINITIONS_NO_COMPLEX)

inline complex_t *complex_allocate(const f32 x, const f32 y, void *(*allocator)(size_t))
{
    #if defined(DMATH_NO_STDLIB)
        if (!allocator) return NULL;
        void* (*A)(size_t) = allocator;
    #else
        void * (*A)(size_t) = allocator != NULL ? allocator : malloc;
    #endif
    
    void * block = A(sizeof(complex_t));
    if (!block) return NULL;

    complex_t * result = (complex_t*) block;
    result->x = x;
    result->y = y;

    return result;
}

inline complex_t complex(const f32 x, const f32 y)
{
    complex_t result = { 0.0F };

    result.x = x;
    result.y = y;

    return result;
}

inline complex_t complex_polar(const f32 r, const f32 theta)
{
    complex_t result = { 0.0F };

    result.x = r * sinf(theta);
    result.y = r * cosf(theta);

    return result;
}

inline complex_t complex_polar_vec(const vec2_t polar)
{
    complex_t result = { 0.0F };

    result.x = polar.x * sinf(polar.y);
    result.y = polar.x * cosf(polar.y);

    return result;
}

inline complex_t complex_v(const f32 val)
{
    complex_t result = { 0.0F };
    
    result.x = val;
    result.y = val;
    
    return result;
}

inline complex_t complex_neg(const complex_t z)
{
    complex_t result = { 0.0F };
    
    result.x = -z.x;
    result.y = -z.y;
    
    return result;
}

inline complex_t complex_conjugate(const complex_t z)
{
    complex_t result = { 0.0F };
    
    result.x = z.x;
    result.y = -z.y;
    
    return result;
}

inline complex_t complex_inverse(const complex_t z)
{
    complex_t result = { 0.0F };
    
    const f32 mag_sqr = z.x * z.x + z.y * z.y;
    if (mag_sqr == 0.0F) return result;

    result.x = z.a / mag_sqr;
    result.y = -z.b / mag_sqr;
    
    return result;
}

inline complex_t complex_add(const complex_t z1, const complex_t z2)
{
    complex_t result = { 0.0F };

    result.x = z1.x + z2.x;
    result.y = z1.y + z2.y;

    return result;
}

inline complex_t complex_sub(const complex_t z1, const complex_t z2)
{
    complex_t result = { 0.0F };
    
    result.x = z1.x - z2.x;
    result.y = z1.y - z2.y;
    
    return result;
}

inline complex_t complex_div(const complex_t z1, const complex_t z2)
{
    complex_t result = { 0.0F };
    
    const f32 mag_sqr = z2.x * z2.x + z2.y * z2.y;
    if (mag_sqr == 0.0F) return result;

    f32 a = z2.a / mag_sqr, b = -z2.b / mag_sqr;

    result.x = z1.x * a - z1.y * b;
    result.y = z1.x * b + z1.y * a;
    
    return result;
}

inline complex_t complex_mul(const complex_t z1, const complex_t z2)
{
    complex_t result = { 0.0F };

    result.x = z1.x * z2.x - z1.y * z2.y;
    result.y = z1.x * z2.y + z1.y * z2.x;

    return result;
}

inline complex_t complex_pow(const complex_t z1, const complex_t z2)
{
    (void)(z1);
    (void)(z2);
    /*
     *  TODO: Implement 
    */

    assert(0 && "Not implemented");
}

inline complex_t complex_sin(const complex_t z)
{
    complex_t result = { 0.0F };

    result.x = sinf(z.x) * coshf(z.y);
    result.y = cosf(z.x) * sinhf(z.y);

    return result;
}

inline complex_t complex_cos(const complex_t z)
{
    complex_t result = { 0.0F };
    
    result.x = cosf(z.x) * coshf(z.y);
    result.y = -sinf(z.x) * sinhf(z.y);
    
    return result;
}

inline complex_t complex_tan(const complex_t z)
{
    complex_t result = { 0.0F };
 
    f32 d = cosf(z.x) + coshf(z.y);
        d = d == 0.0F ? 1.0F : d;

    result.x = sinf(z.x) / d;
    result.y = sinhf(z.y) / d;
    
    return result;
}

inline complex_t complex_csc(const complex_t z)
{
    complex_t result = { 0.0F };

    f32 d = 0.5F * (cosh(2.0F * z.y) - cosf(2.0F * z.x));
    d = d == 0.0F ? 1.0F : d;

    result.x = sinf(z.x) * coshf(z.y) / d;
    result.y = -cosf(z.x) * sinhf(z.y) / d;

    return result;
}

inline complex_t complex_sec(const complex_t z)
{
    complex_t result = { 0.0F };

    f32 d = 0.5F * (coshf(2.0F * z.y) + 0.5F * cosf(2.0F * z.x));
    d = d == 0.0F ? 1.0F : d;

    result.x = cosf(z.x) * coshf(z.y) / d;
    result.y = sinf(z.x) * sinhf(z.y) / d;

    return result;
}

inline complex_t complex_cot(const complex_t z)
{
    complex_t result = { 0.0F };

    const f32 a = 2.0F * z.x;
    const f32 b = 2.0F * z.y;
    f32 d = cosf(a) - coshf(b);
    d = d == 0.0F ? 1.0F : d;

    result.x = -sinf(a) / d;
    result.y = sinh(b) / d;

    return result;
}

inline complex_t complex_asin(const complex_t z)
{
    complex_t result = { 0.0F };

    const f32 a = z.x;
    const f32 b = z.y;

    const complex_t s = { 
        .x = 1.0F + b * b - a * a, 
        .y = -2.0F * a * b 
    };
    const complex_t s_sqrt = complex_sqrt(s);

    const complex_t u = { 
        .x = s_sqrt.x - b,
        .y = s_sqrt.y + a
    };
    const complex_t u_log = complex_log(u);

    result.x = u_log.y;
    result.y = -u_log.x;
    
    return result;
}

inline complex_t complex_acos(const complex_t z)
{
    complex_t result = { 0.0F };

    const f32 a = z.x;
    const f32 b = z.y;

    const complex_t s = {
        .x = 1.0F + b * b - a * a,
        .y = -2.0F * a * b
    };
    const complex_t s_sqrt = complex_sqrt(s);

    const complex_t u = {
        .x = s_sqrt.x - b,
        .y = s_sqrt.y + a
    };
    const complex_t u_log = complex_log(u);

    result.x = PI / 2.0F - u_log.y;
    result.y = u_log.x;

    return result;
}

inline complex_t complex_atan(const complex_t z)
{
    complex_t result = { 0.0F };

    const f32 a = z.x;
    const f32 b = z.y;

    if (a == 0.0F) return result;

    const f32 d = a * a + (1.0F - b) * (1.0F - b);
    const f32 div = d == 0.0F ? 1.0F : d;

    const complex_t t = {
        .x = (1.0F - a * a - b * b) / div,
        .y = -2.0F * a / div
    };
    const complex_t t_log = complex_log(t);

    result.x = -0.5F * t_log.y;
    result.y = 0.5F * t_log.x;
    
    return result;
}

inline complex_t complex_acot(const complex_t z)
{
    complex_t result = { 0.0F };

    const f32 a = z.x;
    const f32 b = z.y;

    if (b == 0.0F)
    {
        result.x = atan2f(1.0F, a);
        result.y = 0.0F;

        return result;
    }

    const f32 d = a * a + b * b;

    if (d == 0.0F) return result;

    const f32 div = d == 0.0F ? 1.0F : d;

    const complex_t t = {
        .x = a / div,
        .y = a / div
    };
    
    const complex_t t_atan = complex_atan(t);
    
    result.x = t_atan.x;
    result.y = t_atan.y;

    return result;
}

inline complex_t complex_asec(const complex_t z)
{
    complex_t result = { 0.0F };

    const f32 a = z.x;
    const f32 b = z.y;

    if (a == 0.0F && b == 0.0F) return result;

    const f32 d = a * a + b * b;

    const complex_t t = {
        .x = a / d,
        .y = -b / d
    };

    result = complex_acos(t);

    return result;
}

inline complex_t complex_acsc(const complex_t z)
{
    complex_t result = { 0.0F };

    const f32 a = z.x;
    const f32 b = z.y;

    if (a == 0.0F && b == 0.0F) return result;

    const f32 d = a * a + b * b;

    const complex_t t = {
        .x = a / d,
        .y = -b / d
    };

    result = complex_asin(t);

    return result;
}

inline complex_t complex_sinh(const complex_t z)
{
    complex_t result = { 0.0F };

    const f32 a = z.x;
    const f32 b = z.y;

    result.x = sinhf(a) * cosf(b);
    result.y = coshf(a) * sinf(b);

    return result;
}

inline complex_t complex_cosh(const complex_t z)
{
    complex_t result = { 0.0F };

    const f32 a = z.x;
    const f32 b = z.y;

    result.x = coshf(a) * cosf(b);
    result.y = sinhf(a) * sinf(b);

    return result;
}

inline complex_t complex_tanh(const complex_t z)
{
    complex_t result = { 0.0F };

    const f32 a = z.x;
    const f32 b = z.y;
    const f32 d = coshf(a) + cosf(b);
    const f32 div = d == 0.0F ? 1.0F : d;

    result.x = sinhf(a) / div;
    result.y = sinf(b) / div;

    return result;
}

inline complex_t complex_coth(const complex_t z)
{
    complex_t result = { 0.0F };

    const f32 a = 2.0F * z.x;
    const f32 b = 2.0F * z.y;
    const f32 d = coshf(a) + cosf(b);
    const f32 div = d == 0.0F ? 1.0F : d;

    result.x = sinhf(a) / div;
    result.y = -sinf(b) / div;

    return result;
}

inline complex_t complex_sech(const complex_t z)
{
    complex_t result = { 0.0F };

    const f32 a = z.x;
    const f32 b = z.y;
    const f32 d = coshf(2.0F * a) + cosf(2.0F * b);
    const f32 div = d == 0.0F ? 1.0F : d;

    result.x = 2.0F * coshf(a) * cosf(b) / div;
    result.y = -2.0F * sinhf(a) * sinf(b) / div;

    return result;
}

inline complex_t complex_csch(const complex_t z)
{
    complex_t result = { 0.0F };

    const f32 a = z.x;
    const f32 b = z.y;
    const f32 d = -coshf(2.0F * a) + cosf(2.0F * b);
    const f32 div = d == 0.0F ? 1.0F : d;

    result.x = -2.0F * sinhf(a) * cosf(b) / div;
    result.y = 2.0F * coshf(a) * sinf(b) / div;

    return result;
}

inline complex_t complex_asinh(const complex_t z)
{
    complex_t result = { 0.0F };

    const f32 a = z.x;
    const f32 b = z.y;

    if (b == 0.0F)
    {
        if (a == 0.0F) return result;

        const f32 x = fabsf(a);
        const f32 r = logf(x + sqrtf(x * x + 1.0F));

        result.x = a < 0.0F ? -r : r;
        result.y = 0.0F;

        return result;
    }

    const complex_t t = {
        .x = a * a - b * b + 1.0F,
        .y = 2 * a * b
    };

    const complex_t t_sqrt = complex_sqrt(t);

    const complex_t u = {
        .x = a + t_sqrt.x,
        .y = b + t.y
    };
    const complex_t u_log = complex_log(u);

    result = u_log;

    return result;
}

inline complex_t complex_acosh(const complex_t z)
{
    complex_t result = { 0.0F };

    const f32 a = z.x;
    const f32 b = z.y;

    if (b == 0.0F)
    {
        if (a > 1.0F)
        {
            result.x = logf(a + sqrtf(a - 1.0F) * sqrtf(a + 1.0F));
            result.y = 0.0F;

            return result;
        }
        else if (a < -1.0F)
        {
            const f32 t = sqrtf(a * a - 1.0F);

            result.x = logf(t - a);
            result.y = PI;

            return result;
        }

        result.x = 0.0F;
        result.y = acosf(a);

        return result;
    }

    const complex_t t = {
        .x = a - 1.0F,
        .y = b
    };
    const complex_t t_sqrt = complex_sqrt(t);

    const complex_t u = {
        .x = a + 1.0F,
        .y = b
    };
    const complex_t u_sqrt = complex_sqrt(u);

    const complex_t w = {
        .x = a + t_sqrt.x * u_sqrt.x - t_sqrt.y * u_sqrt.y,
        .y = b + t_sqrt.x * u_sqrt.y + t_sqrt.y * u_sqrt.x
    };

    result = complex_log(w);

    return result;
}

inline complex_t complex_atanh(const complex_t z)
{
    complex_t result = { 0.0F };

    const f32 a = z.x;
    const f32 b = z.y;

    if (b == 0.0F)
    {
        if (a == 0.0F) return result;

        if (a == -1.0F || a == 1.0F) return result;

        if (-1.0F < a && a < 1.0F)
        {
            result.x = 0.5F * logf((1.0F + a) / (1.0F - a));
            result.y = 0.0F;

            return result;
        }
        else if (a > 1.0F)
        {
            const f32 alpha = (a + 1.0F) / (a - 1.0F);

            result.x = 0.5F * logf(alpha);
            result.y = -PI / 2.0F;

            return result;
        }
        else
        {
            const f32 alpha = (1.0F + a) / (1.0F - a);

            result.x = 0.5F * logf(-alpha);
            result.y = PI / 2.0F;

            return result;
        }
    }

    const f32 oma = 1.0F - a;
    const f32 opa = 1.0F + a;

    const f32 d = oma * oma + b * b;

    const f32 xr = (oma * opa - b * b) / d;
    const f32 xi = (oma + opa) * b / d;

    result.x = logf(sqrtf(xr * xr + xi * xi)) / 2.0F;
    result.y = atan2f(xi ,xr) / 2.0F;

    return result;
}

inline complex_t complex_acoth(const complex_t z)
{
    complex_t result = { 0.0F };

    const f32 a = z.x;
    const f32 b = z.y;

    if (a == 0.0F && b == 0.0F)
    {
        result.x = 0.0F;
        result.y = PI / 2.0F;

        return result;
    }

    const f32 d = a * a + b * b;

    const complex_t t = { 
        .x = a / d,
        .y = -b / d
    };

    result = complex_atanh(t);

    return result;
}

inline complex_t complex_asech(const complex_t z)
{
    complex_t result = { 0.0F };

    const f32 a = z.x;
    const f32 b = z.y;

    const f32 d = a * a + b * b;

    if (d == 0.0F) return result;

    const complex_t t = {
        .x = a / d,
        .y = -b / d 
    };

    result = complex_acosh(t);

    return result;
}

inline complex_t complex_acsch(const complex_t z)
{
    complex_t result = { 0.0F };

    const f32 a = z.x;
    const f32 b = z.y;

    if (b == 0.0F)
    {
        if (a == 0.0F) return result;

        const f32 a_inv = 1.0F / a;

        result.x = logf(a_inv + sqrtf(a_inv * a_inv + 1.0F));
        result.y = 0.0F;

        return result;
    }

    const f32 d = a * a + b * b;

    const complex_t t = {
        .x = a / d,
        .y = -b / d 
    };

    result = complex_asinh(t);

    return result;
}

inline complex_t complex_ceil(const complex_t z)
{
    complex_t result = { 0.0F };

    result.x = ceilf(z.x);
    result.y = ceilf(z.y);

    return result;
}

inline complex_t complex_floor(const complex_t z)
{
    complex_t result = { 0.0F };

    result.x = floorf(z.x);
    result.y = floorf(z.y);

    return result;
}

inline complex_t complex_round(const complex_t z)
{
    complex_t result = { 0.0F };

    result.x = roundf(z.x);
    result.y = roundf(z.y);

    return result;
}

inline complex_t complex_abs(const complex_t z)
{
    complex_t result = { 0.0F };

    result.x = fabsf(z.x);
    result.y = fabsf(z.y);

    return result;
}

inline complex_t complex_log(const complex_t z)
{
    complex_t result = { 0.0F };

    if (z.y == 0.0F && z.x > 0.0F)
    {
        result.x = logf(z.x);
        result.y = 0.0F;

        return result;
    }

    result.x = logf(sqrtf(z.x * z.x + z.y * z.y));
    result.y = atan2f(z.y, z.x);
    
    return result;
}

inline complex_t complex_exp(const complex_t z)
{
    complex_t result = { 0.0F };

    const f32 exp_re = expf(z.x);

    if (z.y == 0.0F)
    {
        result.x = exp_re;
        result.y = 0.0F;

        return result;
    }

    result.x = exp_re * cosf(z.y);
    result.y = exp_re * sinf(z.y);

    return result;
}

inline complex_t complex_sqrt(const complex_t z)
{
    complex_t result = { 0.0F };

    if (z.y == 0.0F)
    {
        if (z.x >= 0.0F)
        {
            result.x = sqrtf(z.x);
            result.y = 0.0F;
        }
        else
        {
            result.x = 0.0F;
            result.y = sqrtf(-z.x);
        }
        return result;
    }

    const f32 r = sqrtf(z.x * z.x + z.y * z.y);
    const f32 re = sqrtf(0.5F * (r + fabsf(z.x)));
    const f32 im = fabsf(z.y) / (2.0F * re);

    if (z.x >= 0.0F)
    {
        result.x = re;
        result.y = z.y < 0.0F ? -im : im;
    }
    else
    {
        result.x = im;
        result.y = z.y < 0.0F ? -re : re; 
    }
    
    return result;
}

inline vec2_t complex_to_polar(const complex_t z)
{
    vec2_t result = { 0.0F };

    result.x = sqrtf(z.x * z.x + z.y * z.y); // radius
    result.y = atan2f(z.y, z.x); // theta

    return result;
}

inline vec2_t complex_to_vec2(const complex_t z)
{
    vec2_t result = { 0.0F };

    result.x = z.x;
    result.y = z.y;

    return result;
}

inline bool complex_is_eq(const complex_t z1, const complex_t z2)
{
    bool result = false;

    result = z1.x == z2.x && z1.y == z2.y;

    return result;
}

inline bool complex_is_eq_approx(const complex_t z1, const complex_t z2, const f32 epsilon)
{    
    bool result = false;

    result = fabsf(z1.x - z2.x) < epsilon && fabsf(z1.y - z2.y) < epsilon;

    return result;
}

inline f32 complex_mag(const complex_t z)
{
    f32 result = 0.0F;

    result = sqrtf(z.x * z.x + z.y * z.y);

    return result;
}

inline f32 complex_mag_sqr(const complex_t z)
{
    f32 result = 0.0F;

    result = z.x * z.x + z.y * z.y;

    return result;
}

inline f32 complex_arg(const complex_t z)
{
    f32 result = 0.0F;

    result = atan2f(z.y, z.x);

    return result;
}

inline f32 complex_dist(const complex_t z1, const complex_t z2)
{
    f32 result = 0.0F;

    const f32 dx = z1.x - z2.x; 
    const f32 dy = z1.y - z2.y; 

    const f32 dist_sqr = dx * dx + dy * dy;

    result = sqrtf(dist_sqr); 

    return result;
}

inline f32 complex_dist_sqr(const complex_t z1, const complex_t z2)
{
    f32 result = 0.0F;

    const f32 dx = z1.x - z2.x; 
    const f32 dy = z1.y - z2.y; 

    const f32 dist_sqr = dx * dx + dy * dy;

    result = dist_sqr; 

    return result;
}

inline f32 complex_re(const complex_t z)
{
    f32 result = 0.0F;

    result = z.x;

    return result;
}

inline f32 complex_im(const complex_t z)
{
    f32 result = 0.0F;

    result = z.y;

    return result;
}

#endif // DMATH_DEFINITIONS_NO_COMPLEX

#if !defined(DMATH_DEFINITIONS_NO_GEOMETRY)

inline rectangle_t * rectangle_allocate(
    const f32 x, 
    const f32 y, 
    const f32 w, 
    const f32 h, 
    const f32 r,  
    void* (*allocator)(size_t)
)
{
    #if defined(DMATH_NO_STDLIB)
        if (!allocator) return NULL;
        void* (*A)(size_t) = allocator;
    #else
        void * (*A)(size_t) = allocator != NULL ? allocator : malloc;
    #endif
    
    void * block = A(sizeof(rectangle_t));
    if (!block) return NULL;

    rectangle_t * result = (rectangle_t*) block;
    
    result->x = x;
    result->y = y;
    result->w = w;
    result->h = h;
    result->r = r;

    return result;  
}

inline rectangle_t rectangle(const f32 x, const f32 y, const f32 w, const f32 h, const f32 r)
{
    rectangle_t result = { 0 };

    result.x = x;
    result.y = y;
    result.w = w;
    result.h = h;
    result.r = r;

    return result;
}

inline rectangle_t rectangle_v(const vec2_t pos, const vec2_t scale, const f32 r)
{
    rectangle_t result = { 0 };

    result.x = pos.x;
    result.y = pos.y;
    result.w = scale.x;
    result.h = scale.y;
    result.r = r;

    return result;
}

inline rectangle_t rectangle_square(const f32 x, const f32 y, const f32 size, const f32 r)
{
    rectangle_t result = { 0 };

    result.x = x;
    result.y = y;
    result.w = size;
    result.h = size;
    result.r = r;

    return result;
}

inline rectangle_t rectangle_square_v(const vec2_t pos, const f32 size, const f32 r)
{
    rectangle_t result = { 0 };

    result.x = pos.x;
    result.y = pos.y;
    result.w = size;
    result.h = size;
    result.r = r;

    return result;
}

inline rectangle_t rectangle_translate(const rectangle_t rect, const vec2_t translation)
{
    rectangle_t result = rect;

    result.x += translation.x;
    result.y += translation.y;

    return result;
}

inline rectangle_t rectangle_set_center(const rectangle_t rect, const vec2_t where)
{
    rectangle_t result = rect;

    result.x = where.x;
    result.y = where.y;

    return result;
}

inline rectangle_t rectangle_scale(const rectangle_t rect, const vec2_t scale)
{
    rectangle_t result = rect;

    result.w *= scale.x;
    result.h *= scale.y;

    return result;
}

inline rectangle_t rectangle_set_scale(const rectangle_t rect, const vec2_t scale)
{
    rectangle_t result = rect;

    result.w = scale.x;
    result.h = scale.y;

    return result;
}

inline rectangle_t rectangle_rotate(const rectangle_t rect, const f32 rotation)
{
    rectangle_t result = rect;

    result.r += rotation;

    return result;
}

inline rectangle_t rectangle_set_rot(const rectangle_t rect, const f32 rotation)
{
    rectangle_t result = rect;

    result.r = rotation;

    return result;
}

inline vec2_t rectangle_center(const rectangle_t rect)
{
    vec2_t result = { 0.0F };

    result.x = rect.x;
    result.y = rect.y;

    return result;
}

inline vec2_t rectangle_size(const rectangle_t rect)
{
    vec2_t result = { 0.0F };

    result.x = rect.w;
    result.y = rect.h;

    return result;
}

inline f32 rectangle_rotation(const rectangle_t rect)
{
    f32 result = 0.0F;

    result = rect.r;

    return result;
}

inline bool rectangle_AA_grid(
    const u32 rows, 
    const u32 cols, 
    const vec2_t pos, 
    const vec2_t scale, 
    const vec2_t gap, 
    rectangle_t *out
)
{
    if (!out) return false;

    if (!rows || !cols) return false;

    const vec2_t cell_scale = {
        .x = (scale.x  - (cols - 1U) * gap.x) / (cols), 
        .y = (scale.y  - (rows - 1U) * gap.y) / (rows), 
    };

    const vec2_t top_left_cell_center = {
        .x = pos.x - scale.x / 2.0F + cell_scale.x / 2.0F,
        .y = pos.y - scale.y / 2.0F + cell_scale.y / 2.0F
    };

    rectangle_t* p = out;

    for (u32 c = 0; c < cols; c++)
    {
        for (u32 r = 0; r < rows; r++)
        {
            const vec2_t cell_center = {
                .x = top_left_cell_center.x + c * (cell_scale.x + gap.x),
                .y = top_left_cell_center.y + r * (cell_scale.y + gap.y),
            };

            rectangle_t cell = rectangle_v(cell_center, cell_scale, 0.0F);
            
            *p++ = cell;
        }
    }

    return true;
}

inline bool rectangle_is_eq(const rectangle_t rect1, const rectangle_t rect2)
{
    bool result = false;

    result = rect1.x == rect2.x &&
             rect1.y == rect2.y &&
             rect1.w == rect2.w &&
             rect1.h == rect2.h &&
             rect1.r == rect2.r;

    return result;
}

inline bool rectangle_is_eq_approx(const rectangle_t rect1, const rectangle_t rect2, const f32 epsilon)
{
    bool result = false;

    result = fabsf(rect1.x - rect2.x) < epsilon &&
             fabsf(rect1.y - rect2.y) < epsilon &&
             fabsf(rect1.w - rect2.w) < epsilon &&
             fabsf(rect1.h - rect2.h) < epsilon &&
             fabsf(rect1.r - rect2.r) < epsilon;

    return result;
}

inline bool rectangle_are_overlaps(const rectangle_t rect1, const rectangle_t rect2)
{
    (void) (rect1);
    (void) (rect2);

    // TODO: Implement
    assert(0 && "Not Implemented");
    return false;
}

inline bool rectangle_are_overlaps_approx(const rectangle_t rect1, const rectangle_t rect2, const f32 epsilon)
{    
        (void) (rect1);
    (void) (rect2);
    (void) (epsilon);
    // TODO: Implement
    assert(0 && "Not Implemented");
    return false;
}

inline bool rectangle_is_point_inside(const rectangle_t rect, const vec2_t point)
{
    (void) (rect);
    (void) (point);
    // TODO: Implement
    assert(0 && "Not Implemented");
    return false;
}

inline f32 rectangle_area(const rectangle_t rect)
{
    f32 result = 0.0F;

    result = rect.w * rect.h;

    return result;
}

inline f32 rectangle_circumference(const rectangle_t rect)
{
    f32 result = 0.0F;

    result = (rect.w + rect.h) * 2.0F;

    return result;
}

inline f32 rectangle_dist(const rectangle_t rect1, const rectangle_t rect2)
{
        (void) (rect1);
    (void) (rect2);
    // TODO: Implement
    assert(0 && "Not Implemented");
    return 0.0F;
}

inline f32 rectangle_dist_to_point(const rectangle_t rect, const vec2_t point)
{    
    (void) (rect);
    (void) (point);
    // TODO: Implement
    assert(0 && "Not Implemented");
    return 0.0F;
}

#endif // DMATH_DEFINITIONS_NO_GEOMETRY

#endif