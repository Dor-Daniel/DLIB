#pragma once 

#include "dmath_definitions.h"
#include <math.h>

#ifndef PI
#define PI 3.1415927F
#endif

#ifndef TWO_PI
#define TWO_PI 6.2831855F
#endif

f32 fclamp(f32 x, f32 min_x, f32 max_x);
f32 flerp(f32 a, f32 b, f32 t);

#if !defined(DMATH_DEFINTIONS_NO_LINEAR_ALGEBRA)

vec2_t vec2(f32 x, f32 y); 
vec2_t vec2_0(void);                                            // return vec2(0, 0)
vec2_t vec2_1(void);                                            // return vec2(1, 1)
vec2_t vec2_e1(void);                                           // return vec2(1, 0)
vec2_t vec2_e2(void);                                           // return vec2(0, 1)
vec2_t vec2_v(f32 v);                                           // return vec2(val, val)
vec2_t vec2_neg(vec2_t v);                                      // return vec2(-v.x, -v.y)
vec2_t vec2_clamp(vec2_t v, vec2_t m, vec2_t M);                // return vec2(fclamp(v.x, m.x, M.x), fclamp(v.x, m.x, M.x))
vec2_t vec2_add(vec2_t v1, vec2_t v2);                          // return vec2(v1.x + v2.x, v1.y+ v2.y)
vec2_t vec2_sub(vec2_t v1, vec2_t v2);                          // return vec2(v1.x- v2.x, v1.y - v2.y)
vec2_t vec2_mul(vec2_t v1, vec2_t v2);                          // return vec2(v1.x*v2.x, v1.y*v2.y)
vec2_t vec2_div(vec2_t v1, vec2_t v2);                          // return vec2(v1.x/v2.x, v1.y/v2.y)
vec2_t vec2_add_v(vec2_t v, f32 val);                           // return vec2(v.x + val, v.y + val)
vec2_t vec2_sub_v(vec2_t v, f32 val);                           // return vec2(v.x - val, v.y - val)
vec2_t vec2_mul_v(vec2_t v, f32 val);                           // return vec2(v.x * val, v.y * val)
vec2_t vec2_div_v(vec2_t v, f32 val);                           // return vec2(v.x / val, v.y / val)
vec2_t vec2_activate(vec2_t v, f32 (*activation_func)(f32));    // return vec2(f(v.x), f(v.y))
vec2_t vec2_normalize(vec2_t v);                                // return v / Length(v)
vec2_t vec2_abs(vec2_t v);                                      // return vec2(abs(v.x), abs(v.y))
vec2_t vec2_lerp(vec2_t a, vec2_t b, vec2_t t);                 // return a + (b - a) * t
vec2_t vec2_slerp(vec2_t a, vec2_t b, vec2_t t);                // return a * cos(vec2_angle_between(a,b) * t) + (b - a * vec2_dot(a,b)) * sin(vec2_angle_between(a,b) * t)
vec2_t vec2_linear_comb(vec2_t v, vec2_t u, f32 a1, f32 a2);    // return a1 * v1 + a2 * v2
vec2_t vec2_transform(vec2_t v, mat2x2_t M);                    // return M * v as matrix mult
vec2_t vec2_from_angle(f32 alpha);                              // return vec2(cos(alpha),sin(alpha))
vec2_t vec2_from_angle_and_r(f32 alpha, f32 radius);            // return vec2(r * cos(alpha), r * sin(alpha))
f32    vec2_length(vec2_t v);                                   // return sqrt(v.x * v.x + v.y * v.y)
f32    vec2_dist(vec2_t v, vec2_t u);                           // return length(v-u) 
f32    vec2_dot(vec2_t v, vec2_t u);                            // return v.x * u.x + v.y * u.y
f32    vec2_angle_between(vec2_t v, vec2_t u);                  // return arccos(vec2_dot(v,u)/(length(v)*length(u)))
f32    vec2_dist_sqr(vec2_t v, vec2_t u);                       // return dist(u,v) * dist(u,v)
f32    vec2_length_sqr(vec2_t v);                               // return length(v) * length(v)
f32    vec2_lp_norm(vec2_t v, f32 p);                           // return (v.x ^ p + v.y ^ p) ^ (1/p)
f32    vec2_lp_norm_to_the_pth(vec2_t v, f32 p);                // return (v.x ^ p + v.y ^ p) 
f32    vec2_max(vec2_t v);                                      // return max(v.x, v.y)
f32    vec2_min(vec2_t v);                                      // return min(v.x, v.y)
f32    vec2_avg(vec2_t v);                                      // return avarage(v.x, v.y)

// for each (iter in v) ... usage: vec2_foreach(v, it) { printf("Before: %f\n",it); it = 3.1415F; printf("After: &%f\n", it); } 
#define vec2_foreach(v, iter) \
    for (f32* __iter__ = v.as_arr, iter = *__iter__; __iter__ < v.as_arr + 2ULL; *__iter__ = iter, __iter__++)

#endif

vec3_t vec3(f32 x, f32 y, f32 z);
vec3_t vec3_0(void);                                                            // return vec3(0, 0, 0)
vec3_t vec3_1(void);                                                            // return vec3(1, 1, 1)
vec3_t vec3_v(f32 val);                                                         // return vec3(val, val, val)
vec3_t vec3_e1(void);                                                           // return vec3(1, 0, 0)
vec3_t vec3_e2(void);                                                           // return vec3(0, 1, 0)
vec3_t vec3_e3(void);                                                           // return vec3(0, 0, 1)
vec3_t vec3_neg(vec3_t v);                                                      // return -v
vec3_t vec3_clamp(vec3_t v, vec3_t m, vec3_t M);                                // return vec3(fclamp(v.x, m.x, M.x), fclamp(v.y, m.y, M.y), fclamp(v.z, m.z, M.z))
vec3_t vec3_add(vec3_t v, vec3_t u);                                            // return vec3(v.x + u.x, v.y + u.y, v.z + u.z)
vec3_t vec3_sub(vec3_t v, vec3_t u);                                            // return vec3(v.x - u.x, v.y - u.y, v.z - u.z)
vec3_t vec3_mul(vec3_t v, vec3_t u);                                            // return vec3(v.x * u.x, v.y * u.y, v.z * u.z)
vec3_t vec3_div(vec3_t v, vec3_t u);                                            // return vec3(v.x / u.x, v.y / u.y, v.z / u.z)
vec3_t vec3_add_v(vec3_t v, f32 val);                                           // return vec3(v.x + val, v.y + val, v.z + val)
vec3_t vec3_sub_v(vec3_t v, f32 val);                                           // return vec3(v.x - val, v.y - val, v.z - val)
vec3_t vec3_mul_v(vec3_t v, f32 val);                                           // return vec3(v.x * val, v.y * val, v.z * val)
vec3_t vec3_div_v(vec3_t v, f32 val);                                           // return vec3(v.x / val, v.y / val, v.z / val)
vec3_t vec3_activate(vec3_t v, f32 (*activation_func)(f32));                    // return vec3(func(v.x), func(v.y), func(v.z))
vec3_t vec3_normalize(vec3_t v);                                                // return v / vec3_length(v)
vec3_t vec3_abs(vec3_t v);                                                      // return vec3(abs(v.x), abs(v.y), abs(v.z))
vec3_t vec3_lerp(vec3_t a, vec3_t b, vec3_t t);                                 // return a + (b - a) * t
vec3_t vec3_slerp(vec3_t a, vec3_t b, vec3_t t);                                // return a * cos(vec3_angle_between(a,b) * t) + (b - a * vec3_dot(a,b)) * sin(vec3_angle_between(a,b) * t)
vec3_t vec3_linear_comb2(vec3_t v, vec3_t u, f32 a1, f32 a2);                   // return v * a1 + u * a2
vec3_t vec3_linear_comb3(vec3_t v, vec3_t u, vec3_t w, f32 a1, f32 a2, f32 a3); // return v * a1 + u * a2 + w * a3
vec3_t vec3_transform(vec3_t v, mat3x3_t M);                                    // return M * v (matrix mult)              
vec3_t vec3_from_angle(f32 alpha, f32 gamma);                                   // return vec3(cos(alpha) * sin(gamma), sin(alpha) * sin(gamma), cos(gamma))
vec3_t vec3_from_angle_and_r(f32 alpha, f32 gamma, f32 radius);                 // return vec3_from_angle(alpha, gamma) * radius
vec3_t vec3_cross(vec3_t v, vec3_t u);                                          // return v * u as cross product 
vec3_t vec3_cross_normalize(vec3_t v, vec3_t u);                                // return vec3_cross(v, u) / vec3_length(vec3_cross(v, u)) 
f32    vec3_length(vec3_t v);                                                   // return sqrt(v.x * v.x + v.y * v.y + v.z * v.z)
f32    vec3_dist(vec3_t v, vec3_t u);                                           // return vec3_length(v - u)
f32    vec3_dot(vec3_t v, vec3_t u);                                            // return v.x * u.x + v.y * u.y + v.z * u.z
f32    vec3_angle_between(vec3_t v, vec3_t u);                                  // return arccos(vec3_dot(v,u)/(vec3_length(v) * vec3_length(u)))
f32    vec3_dist_sqr(vec3_t v, vec3_t u);                                       // return vec3_dist(v,u) * vec3_dist(v,u)
f32    vec3_length_sqr(vec3_t v);                                               // return vec3_length(v) * vec3_length(v)
f32    vec3_lp_norm(vec3_t v, f32 p);                                           // return pow(sum(pow(v.as_arr[i], p)), 1 / p)
f32    vec3_lp_norm_to_the_pth(vec3_t v, f32 p);                                // return pow(vec3_lp_norm(v, p), p)
f32    vec3_max(vec3_t v);                                                      // return max(v.x, v.y, v.z)
f32    vec3_min(vec3_t v);                                                      // return min(v.x, v.y, v.z)
f32    vec3_avg(vec3_t v);                                                      // return avarage(v.x, v.y, v.z)

// for each (iter in v) ... usage: vec3_foreach(v, it) { printf("Before: %f\n",it); it = PI; printf("After: &%f\n", it); } 
#define vec3_foreach(v, iter) \
    for (f32* __iter__ = v.as_arr, iter = *__iter__; __iter__ < v.as_arr + 3ULL; *__iter__ = iter, __iter__++)

#if defined(DMATH_IMPLEMENTATION) || 1

f32 fclamp(f32 x, f32 min_x, f32 max_x)
{
    f32 result = 0.0F;

    result = x < max_x ? x : max_x;
    result = result > min_x ? result : min_x;

    return result;
}

inline f32 flerp(f32 a, f32 b, f32 t)
{
    f32 result = 0.0F;

    result = a + (b - a) * t;

    return result;
}

#if !defined(DMATH_DEFINTIONS_NO_LINEAR_ALGEBRA)

inline vec2_t vec2(f32 x, f32 y)
{
    vec2_t result = { 0 };

    result.x = x;
    result.y = y;

    return result;
}

inline vec2_t vec2_0(void)
{
    vec2_t result = { 0 };

    return result;
}

inline vec2_t vec2_1(void)
{
    vec2_t result = { 0 };

    result.x = 1.0F;
    result.y = 1.0F;

    return result;
}

inline vec2_t vec2_e1(void)
{
    vec2_t result = { 0 };

    result.x = 1.0F;
    result.y = 0.0F;

    return result;
}

inline vec2_t vec2_e2(void)
{
    vec2_t result = { 0 };

    result.x = 0.0F;
    result.y = 1.0F;

    return result;
}

inline vec2_t vec2_v(f32 v)
{
    vec2_t result = { 0 };

    result.x = v;
    result.y = v;

    return result;
}

inline vec2_t vec2_neg(vec2_t v)
{
    vec2_t result= { 0 };

    result.x = -v.x;
    result.y = -v.y;

    return result;
}

inline vec2_t vec2_clamp(vec2_t v, vec2_t m, vec2_t M)
{
    vec2_t result = { 0 };

    result.x = fclamp(v.x, m.x, M.x);
    result.y = fclamp(v.y, m.y, M.y);

    return result;
}

inline vec2_t vec2_add(vec2_t v1, vec2_t v2)
{
    vec2_t result = { 0 };

    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;

    return result;
}

inline vec2_t vec2_sub(vec2_t v1, vec2_t v2)
{
    vec2_t result = { 0 };

    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;

    return result;
}

inline vec2_t vec2_mul(vec2_t v1, vec2_t v2)
{
    vec2_t result = { 0 };

    result.x = v1.x * v2.x;
    result.y = v1.y * v2.y;

    return result;
}

inline vec2_t vec2_div(vec2_t v1, vec2_t v2)
{
    vec2_t result = { 0 };

    f32 dx = v2.x != 0.0F ? v2.x : 1.0F;
    f32 dy = v2.y != 0.0F ? v2.y : 1.0F;

    result.x = v1.x / dx;
    result.y = v1.y / dy;

    return result;
}

inline vec2_t vec2_add_v(vec2_t v, f32 val)
{
    vec2_t result = { 0 };

    result.x = v.x + val;
    result.y = v.y + val;

    return result;
}

inline vec2_t vec2_sub_v(vec2_t v, f32 val)
{
    vec2_t result = { 0 };

    result.x = v.x - val;
    result.y = v.y - val;

    return result;
}

inline vec2_t vec2_mul_v(vec2_t v, f32 val)
{
    vec2_t result = { 0 };

    result.x = v.x * val;
    result.y = v.y * val;

    return result;
}

inline vec2_t vec2_div_v(vec2_t v, f32 val)
{
    vec2_t result = { 0 };

    f32 d = val != 0.0F ? val : 1.0F;

    result.x = v.x / d;
    result.y = v.y / d;

    return result;
}

inline vec2_t vec2_activate(vec2_t v, f32 (*activation_func)(f32))
{
    vec2_t result = { 0 };

    if (!activation_func) return result;

    result.x = activation_func(v.x);
    result.y = activation_func(v.y);

    return result;
}

inline vec2_t vec2_normalize(vec2_t v)
{
    vec2_t result = { 0 };

    f32 length_sqr = v.x * v.x + v.y * v.y;
    f32 length = sqrtf(length_sqr);
    
    length = length == 0.0F ? 1.0F : length;

    result.x = v.x / length;
    result.y = v.y / length;

    return result;
}

inline vec2_t vec2_abs(vec2_t v)
{
    vec2_t result = { 0 };

    result.x = fabsf(v.x);
    result.y = fabsf(v.y);

    return result;
}

inline vec2_t vec2_lerp(vec2_t a, vec2_t b, vec2_t t)
{
    vec2_t result = { 0 };

    result.x = a.x + (b.x - a.x) * t.x;
    result.y = a.y + (b.y - a.y) * t.y;

    return result;
}

inline vec2_t vec2_slerp(vec2_t a, vec2_t b, vec2_t t)
{
    vec2_t result = { 0 };

    f32 dot = a.x * b.x + a.y * b.y;

    f32 alen = sqrtf(a.x * a.x + a.y * a.y);
    f32 blen = sqrtf(b.x * b.x + b.y * b.y);

    f32 div = alen * blen;
    div = div == 0.0F ? dot : div;

    f32 angle = acosf(dot / div);
    vec2_t adot = { .x = a.x * dot, .y = a.y * dot };

    result.x = a.x * cosf(angle * t.x) + (b.x - adot.x) * sinf(angle * t.x);  
    result.y = a.y * cosf(angle * t.y) + (b.y - adot.y) * sinf(angle * t.y);  

    return result;
}

inline vec2_t vec2_linear_comb(vec2_t v, vec2_t u, f32 a1, f32 a2)
{
    vec2_t result = { 0 };

    result.x = v.x * a1 + u.x * a2;
    result.y = v.y * a1 + u.y * a2;

    return result;
}

inline vec2_t vec2_transform(vec2_t v, mat2x2_t M)
{
    vec2_t result = { 0 };

    result.x = v.x * M.a11 + v.y * M.a12;
    result.y = v.x * M.a21 + v.y * M.a22;

    return result;
}

inline vec2_t vec2_from_angle(f32 alpha)
{
    vec2_t result = { 0 };

    result.x = cosf(alpha);
    result.x = sinf(alpha);

    return result;
}

inline vec2_t vec2_from_angle_and_r(f32 alpha, f32 radius)
{
    vec2_t result = { 0 };

    result.x = cosf(alpha) * radius;
    result.x = sinf(alpha) * radius;

    return result;
}

inline f32 vec2_length(vec2_t v)
{
    f32 result = 0;

    f32 length_sqr = v.x * v.x + v.y * v.y;
    result = sqrtf(length_sqr);

    return result;
}

inline f32 vec2_dist(vec2_t v, vec2_t u)
{
    f32 result = 0;

    f32 x = v.x - u.x;
    f32 y = v.y - u.y;
    f32 dist_sqr = x * x + y * y;
    result = sqrtf(dist_sqr);

    return result;
}

inline f32 vec2_dot(vec2_t v, vec2_t u)
{
    f32 result = 0.0F;

    result = v.x * u.x + v.y * u.y;

    return result;
}

inline f32 vec2_angle_between(vec2_t v, vec2_t u)
{
    f32 result = 0.0F;

    f32 dot      = v.x * u.x + v.y * u.y;
    f32 v_length = sqrtf(v.x * v.x + v.y * v.y);
    f32 u_length = sqrtf(u.x * u.x + u.y * u.y);
    f32 d        = v_length * u_length;

    d = d == 0.0F ? dot : d; // Arbitrary angle when one of them is zero-vector

    result = acosf(dot / d);

    return result;
}

inline f32 vec2_dist_sqr(vec2_t v, vec2_t u)
{
    f32 result = 0.0F;

    f32 x = v.x - u.x;
    f32 y = v.y - u.y;

    result = x * x + y * y;

    return result;
}

inline f32 vec2_length_sqr(vec2_t v)
{
    f32 result = 0.0F;

    result = v.x * v.x * + v.y * v.y;

    return result;
}

inline f32 vec2_lp_norm(vec2_t v, f32 p)
{
    f32 result = 0.0F;

    if (p <= 0.0F) return result;

    f32 xp = powf(v.x, p);
    f32 yp = powf(v.y, p);

    result = powf(xp + yp, 1.0F / p);

    return result;
}

inline f32 vec2_lp_norm_to_the_pth(vec2_t v, f32 p)
{
    f32 result = 0.0F;

    if (p <= 0.0F) return result;

    f32 xp = powf(v.x, p);
    f32 yp = powf(v.y, p);

    result = xp + yp;

    return result;
}

inline f32 vec2_max(vec2_t v)
{
    f32 result = 0.0F;

    result = fmaxf(v.x, v.y);

    return result;
}

inline f32 vec2_min(vec2_t v)
{
    f32 result = 0.0F;

    result = fminf(v.x, v.y);

    return result;
}

inline f32 vec2_avg(vec2_t v)
{
    f32 result = 0.0F;

    result = (v.x + v.y) * 0.5F;
    
    return result;
}

inline vec3_t vec3(f32 x, f32 y, f32 z)
{
    vec3_t result = { 0 };

    result.x = x;
    result.y = y;
    result.z = z;
    
    return result;
}

inline vec3_t vec3_0(void)
{
    vec3_t result = { 0 };
    
    return result;
}

inline vec3_t vec3_1(void)
{
    vec3_t result = { 0 };

    result.x = 1.0F;
    result.y = 1.0F;
    result.z = 1.0F;
    
    return result;
}

inline vec3_t vec3_v(f32 val)
{
    vec3_t result = { 0 };

    result.x = val;
    result.y = val;
    result.z = val;
    
    return result;
}

inline vec3_t vec3_e1(void)
{
    vec3_t result = { 0 };

    result.x = 1.0F;
    result.y = 0.0F;
    result.z = 0.0F;
    
    return result;
}

inline vec3_t vec3_e2(void)
{
    vec3_t result = { 0 };

    result.x = 0.0F;
    result.y = 1.0F;
    result.z = 0.0F;
    
    return result;
}

inline vec3_t vec3_e3(void)
{
    vec3_t result = { 0 };

    result.x = 0.0F;
    result.y = 0.0F;
    result.z = 1.0F;
    
    return result;
}

inline vec3_t vec3_neg(vec3_t v)
{
    vec3_t result = { 0 };

    result.x = -v.x;
    result.y = -v.y;
    result.z = -v.z;
    
    return result;
}

inline vec3_t vec3_clamp(vec3_t v, vec3_t m, vec3_t M)
{
    vec3_t result = { 0 };

    result.x = fclamp(v.x, m.x, M.x);
    result.y = fclamp(v.y, m.y, M.y);
    result.z = fclamp(v.z, m.z, M.z);

    return result;
}

inline vec3_t vec3_add(vec3_t v, vec3_t u)
{
    vec3_t result = { 0 };

    result.x = v.x + u.x;
    result.y = v.y + u.y;
    result.z = v.z + u.z;
    
    return result;
}

inline vec3_t vec3_sub(vec3_t v, vec3_t u)
{
    vec3_t result = { 0 };

    result.x = v.x - u.x;
    result.y = v.y - u.y;
    result.z = v.z - u.z;
    
    return result;
}

inline vec3_t vec3_mul(vec3_t v, vec3_t u)
{
    vec3_t result = { 0 };

    result.x = v.x * u.x;
    result.y = v.y * u.y;
    result.z = v.z * u.z;
    
    return result;
}

inline vec3_t vec3_div(vec3_t v, vec3_t u)
{
    vec3_t result = { 0 };

    f32 dx = u.x != 0.0F ? u.x : 1.0F;
    f32 dy = u.y != 0.0F ? u.y : 1.0F;
    f32 dz = u.z != 0.0F ? u.z : 1.0F;

    result.x = v.x / dx;
    result.y = v.y / dy;
    result.z = v.z / dz;
    
    return result;
}

inline vec3_t vec3_add_v(vec3_t v, f32 val)
{
    vec3_t result = { 0 };

    result.x = v.x + val;
    result.y = v.y + val;
    result.z = v.z + val;
    
    return result;
}

inline vec3_t vec3_sub_v(vec3_t v, f32 val)
{
    vec3_t result = { 0 };

    result.x = v.x - val;
    result.y = v.y - val;
    result.z = v.z - val;
    
    return result;
}

inline vec3_t vec3_mul_v(vec3_t v, f32 val)
{
    vec3_t result = { 0 };

    result.x = v.x * val;
    result.y = v.y * val;
    result.z = v.z * val;
    
    return result;
}

inline vec3_t vec3_div_v(vec3_t v, f32 val)
{
    vec3_t result = { 0 };

    f32 dval = val != 0.0F ? val : 1.0F;

    result.x = v.x / dval;
    result.y = v.y / dval;
    result.z = v.z / dval;
    
    return result;
}

inline vec3_t vec3_activate(vec3_t v, f32 (*activation_func)(f32))
{
    vec3_t result = { 0 };

    if (!activation_func) return result;

    result.x = activation_func(v.x);
    result.y = activation_func(v.y);
    result.z = activation_func(v.z);
    
    return result;
}

inline vec3_t vec3_normalize(vec3_t v)
{
    vec3_t result = { 0 };

    f32 sqr_len = v.x * v.x + v.y * v.y + v.z * v.z;
    f32 len = sqrtf(sqr_len);

    result.x = v.x / len;
    result.y = v.y / len;
    result.z = v.z / len;
    
    return result;
}

inline vec3_t vec3_abs(vec3_t v)
{
    vec3_t result = { 0 };

    result.x = fabsf(v.x);
    result.y = fabsf(v.y);
    result.z = fabsf(v.z);
    
    return result;
}

inline vec3_t vec3_lerp(vec3_t a, vec3_t b, vec3_t t)
{
    vec3_t result = { 0 };

    result.x = a.x + (b.x - a.x) * t.x;
    result.y = a.y + (b.y - a.y) * t.y;
    result.z = a.z + (b.z - a.z) * t.z;
    
    return result;
}

inline vec3_t vec3_slerp(vec3_t a, vec3_t b, vec3_t t)
{
    vec3_t result = { 0 };

    f32 dot = a.x * b.x + a.y * b.y + a.z * b.z;

    f32 alen = sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
    f32 blen = sqrtf(b.x * b.x + b.y * b.y + b.z * b.z);

    f32 div = alen * blen;
    div = div == 0.0F ? dot : div;

    f32 angle = acosf(dot / div);
    vec3_t adot = { .x = a.x * dot, .y = a.y * dot, .y = a.y * dot };

    result.x = a.x * cosf(angle * t.x) + (b.x - adot.x) * sinf(angle * t.x);  
    result.y = a.y * cosf(angle * t.y) + (b.y - adot.y) * sinf(angle * t.y);  
    result.z = a.z * cosf(angle * t.z) + (b.z - adot.z) * sinf(angle * t.z);  

    return result;
}

inline vec3_t vec3_linear_comb2(vec3_t v, vec3_t u, f32 a1, f32 a2)
{
    vec3_t result = { 0 };

    result.x = v.x * a1 + u.x * a2;
    result.y = v.y * a1 + u.y * a2;
    result.z = v.z * a1 + u.z * a2;
    
    return result;
}

inline vec3_t vec3_linear_comb3(vec3_t v, vec3_t u, vec3_t w, f32 a1, f32 a2, f32 a3)
{
    vec3_t result = { 0 };

    result.x = v.x * a1 + u.x * a2 + w.x * a3;
    result.y = v.y * a1 + u.y * a2 + w.y * a3;
    result.z = v.z * a1 + u.z * a2 + w.z * a3;
    
    return result;
}

inline vec3_t vec3_transform(vec3_t v, mat3x3_t M)
{
    vec3_t result = { 0 };

    result.x = v.x * M.a11 + v.y * M.a12 + v.z * M.a13;
    result.y = v.x * M.a21 + v.y * M.a22 + v.z * M.a23;
    result.z = v.x * M.a31 + v.y * M.a32 + v.z * M.a33;
    
    return result;
}

inline vec3_t vec3_from_angle(f32 alpha, f32 gamma)
{
    vec3_t result = { 0 };

    f32 sina = sinf(alpha);
    f32 cosa = cosf(alpha);
    f32 sing = sinf(gamma);
    f32 cosg = cosf(gamma);

    result.x = cosa * sing;
    result.y = sina * sing;
    result.z = cosg;
    
    return result;
}

inline vec3_t vec3_from_angle_and_r(f32 alpha, f32 gamma, f32 radius)
{
    vec3_t result = { 0 };

    f32 sina = sinf(alpha);
    f32 cosa = cosf(alpha);
    f32 sing = sinf(gamma);
    f32 cosg = cosf(gamma);

    result.x = cosa * sing * radius;
    result.y = sina * sing * radius;
    result.z = cosg * radius;
    
    return result;
}

inline vec3_t vec3_cross(vec3_t v, vec3_t u)
{
    vec3_t result = { 0 };

    result.x = v.y * u.z - v.z * u.y;
    result.y = v.z * u.x - v.x * u.z;
    result.z = v.x * u.y - v.y * u.x;
    
    return result;
}

inline vec3_t vec3_cross_normalize(vec3_t v, vec3_t u)
{
    vec3_t result = { 0 };

    result.x = v.y * u.z - v.z * u.y;
    result.y = v.z * u.x - v.x * u.z;
    result.z = v.x * u.y - v.y * u.x;

    f32 len_sqr = result.x * result.x + result.y * result.y + result.z * result.z;
    f32 len = sqrtf(len_sqr);

    if (len != 0.0F)
    {
        result.x /= len;
        result.y /= len;
        result.z /= len;
    }
    
    return result;
}

inline f32 vec3_length(vec3_t v)
{
    f32 result = 0.0F;

    f32 len_sqr = v.x * v.x + v.y * v.y + v.z * v.z;
    result = sqrtf(len_sqr);

    return result;
}

inline f32 vec3_dist(vec3_t v, vec3_t u)
{
    f32 result = 0.0F;
    
    f32 dx = v.x - u.x; 
    f32 dy = v.y - u.y; 
    f32 dz = v.z - u.z;

    f32 dist_sqr = dx * dx + dy *  dy + dz * dz;
    result = sqrtf(dist_sqr);

    return result;
}

inline f32 vec3_dot(vec3_t v, vec3_t u)
{
    f32 result = 0.0F;

    result = v.x * u.x + v.y * u.y + v.z * u.z;

    return result;
}

inline f32 vec3_angle_between(vec3_t v, vec3_t u)
{
    f32 result = 0.0F;

    f32 dot = v.x * u.x + v.y * u.y + v.z * u.z;
    
    f32 vlen = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    f32 ulen = sqrtf(u.x * u.x + u.y * u.y + u.z * u.z);

    f32 div = vlen * ulen;
    div = div == 0.0F ? dot : div;

    result = acosf(dot / div);

    return result;
}

inline f32 vec3_dist_sqr(vec3_t v, vec3_t u)
{
    f32 result = 0.0F;
    
    f32 dx = v.x - u.x; 
    f32 dy = v.y - u.y; 
    f32 dz = v.z - u.z;

    result = dx * dx + dy *  dy + dz * dz;

    return result;
}

inline f32 vec3_length_sqr(vec3_t v)
{
    f32 result = 0.0F;

    result = v.x * v.x + v.y * v.y + v.z * v.z;

    return result;
}

inline f32 vec3_lp_norm(vec3_t v, f32 p)
{
    f32 result = 0.0F;

    if (p <= 0.0F) return result;

    f32 xp = powf(v.x, p);
    f32 yp = powf(v.y, p);
    f32 zp = powf(v.z, p);

    result = powf(xp + yp + zp, 1.0F / p);

    return result;
}

inline f32 vec3_lp_norm_to_the_pth(vec3_t v, f32 p)
{
    f32 result = 0.0F;

    if (p <= 0.0F) return result;

    f32 xp = powf(v.x, p);
    f32 yp = powf(v.y, p);
    f32 zp = powf(v.z, p);

    result = xp + yp + zp;

    return result;
}

inline f32 vec3_max(vec3_t v)
{
    f32 result = 0.0F;

    result = fmaxf(v.x, fmaxf(v.y, v.z));

    return result;
}

inline f32 vec3_min(vec3_t v)
{
    f32 result = 0.0F;

    result = fminf(v.x, fminf(v.y, v.z));

    return result;
}

inline f32 vec3_avg(vec3_t v)
{
    f32 result = 0.0F;

    result = (v.x + v.y + v.z) / 3.0F;

    return result;
}

#endif


#endif