#ifndef RUNDERER_H
#define RUNDERER_H

/**
 * \file runderer.h
 * The main runderer instance. We have to bind it to a framebuffer instance.
 * Some 3D direct rendering functions are included for testing purposes: these
 * shapes are drawn directly on the screen without any perspective.
 */

#include "framebuffer.h"
#include "vector.h"
#include "matrix.h"

#define ZBUF_TYPE float

struct runderer;

typedef void (*draw_triangle_array_func_t)
(struct runderer* self, vertex_t const* vertices, unsigned int count);

typedef void (*draw_quad_array_func_t)
(struct runderer* self, vertex_t const* vertices, unsigned int count);

typedef stream_t (*vertex_shader_func_t)
(vertex_t vertex, mat4f_t model_matrix, mat4f_t view_matrix, mat4f_t projection_matrix, mat4f_t viewport_matrix);

typedef void (*triangle_rasterizer_func_t)
(struct runderer* self, stream_t p1, stream_t p2, stream_t p3, fragment_t* frag_buf_begin, fragment_t** frag_buf_end);

typedef void (*line_rasterizer_func_t)
(stream_t p1, stream_t p2, fragment_t* frag_buf_begin, fragment_t* frag_buf_end);

typedef void (*fragment_shader_func_t)
(fragment_t const * frag_buf, unsigned int frags_to_process, framebuffer_t* frame, ZBUF_TYPE* zbuffer);

typedef struct runderer {
	framebuffer_t* framebuffer;
	ZBUF_TYPE *zbuffer;

	vec4f_t eye_direction;
	mat4f_t model_matrix;
	mat4f_t view_matrix;
	mat4f_t projection_matrix;
	mat4f_t viewport_matrix;

	fragment_t *fragbuf;

    draw_triangle_array_func_t draw_triangle_array;
    draw_quad_array_func_t draw_quad_array;
	vertex_shader_func_t vertex_shader;
	triangle_rasterizer_func_t triangle_rasterizer;
	line_rasterizer_func_t line_rasterizer;
	fragment_shader_func_t fragment_shader;

	// TODO: move these outside the runderer state
    mat4f_t *active_matrix;
    int active_mode;
} runderer_t;

/**
 * Bind the runderer instance to a framebuffer driver
 * \param run a pointer to the runderer instance to bind
 * \param fb a pointer to the framebuffer instance to bind
 * \return 0 if succesful, nonzero otherwise
 */
int runderer_bind(runderer_t *run, framebuffer_t *fb);

/**
 * Unbind the runderer instance, freeing it's buffers
 * \param run a pointer to the runderer instance to unbind
 */
void runderer_unbind(runderer_t *run);

stream_t runderer_vertex_shader(vertex_t vertex, mat4f_t model_matrix, mat4f_t view_matrix, mat4f_t projection_matrix, mat4f_t viewport_matrix);
void runderer_draw_triangle_array(runderer_t* self, vertex_t const* vertices, unsigned int count);
void runderer_draw_quad_array(runderer_t* self, vertex_t const* vertices, unsigned int count);
void runderer_fragment_shader_flat(fragment_t const * frag_buf, unsigned int frags_to_process, framebuffer_t* frame, ZBUF_TYPE* zbuffer);

void perspective(float field_of_view, float aspect_ratio, float n, float f,
                 mat4f_t mat);

void orthographic(float left,
                  float right,
                  float bottom,
                  float top,
                  float near, // (positive value, near plane will be at -n)
                  float far, // (positive value, far plane will be at -f)
                  mat4f_t mat);

#endif
