#ifndef VECTOR_H
#define VECTOR_H

typedef float vec3f_t[3];
typedef float vec4f_t[4];

typedef struct {
    vec4f_t position;
    vec4f_t color;
} vertex_t;

typedef struct {
    vec3f_t position;
    vec4f_t color;
} stream_t;

typedef struct {
    vec3f_t screen; // .x, .y are in screen-space, .z is z-depth
    vec4f_t color;
} fragment_t;

#endif /* VECTOR_H */
