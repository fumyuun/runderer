#include "runderer.h"
#include "math.h"
#include "matrix.h"
#include <stdint.h>
#include <stdlib.h>
#include <float.h>

#define RUNDERER_FRAGBUF_N 1024*1024

/**
 * Bind the runderer instance to a framebuffer driver
 * \param run a pointer to the runderer instance to bind
 * \param fb a pointer to the framebuffer instance to bind
 * \return 0 if succesful, nonzero otherwise
 */
int runderer_bind(runderer_t *run, framebuffer_t *fb) {
    if (run == NULL || fb == NULL) {
        return 1;
    }

    run->framebuffer = fb;
    run->zbuffer = malloc(fb->width * fb->height * sizeof(ZBUF_TYPE));
    if (run->zbuffer == NULL) {
        return 2;
    }

    for (uint i = 0; i < fb->width * fb->height; ++i) {
        run->zbuffer[i] = FLT_MIN;
    }

    run->fragbuf = malloc(RUNDERER_FRAGBUF_N * sizeof(fragment_t));
    if (run->fragbuf == NULL) {
        return 3;
    }

    // For now, the light is just coming from the z-axis
    run->eye_direction[0] = 0.0f;
    run->eye_direction[1] = 0.0f;
    run->eye_direction[2] = -1.0f;

    run->active_matrix = &run->model_matrix;

    return 0;
}

void runderer_draw_triangle_array(runderer_t *self, const vertex_t *vertices,
                                  uint count) {
  for (uint i = 0; i < count; i++) {
    stream_t verts[3];
    for (uint j = 0; j < 3; j++) {
      verts[j] =
          self->vertex_shader(vertices[i * 3 + j], self->model_matrix,
                              self->view_matrix, self->projection_matrix, self->viewport_matrix);
    }
    fragment_t *begin = &self->fragbuf[0];
    fragment_t *end = begin + RUNDERER_FRAGBUF_N;
    self->triangle_rasterizer(self, verts[0], verts[1], verts[2], begin, &end);
    uint const frags_drawn = (uint)(end - begin);
    self->fragment_shader(self->fragbuf, frags_drawn, self->framebuffer);
  }
}

stream_t runderer_vertex_shader(vertex_t vertex, mat4f_t model_matrix,
                                mat4f_t view_matrix,
                                mat4f_t projection_matrix,
                                mat4f_t viewport_matrix) {
  vec4f_t world_pos;
  mat_mult_vec4f(model_matrix, vertex.position, world_pos);
  vec4f_t view_pos;
  mat_mult_vec4f(view_matrix, world_pos, view_pos);
  vec4f_t projected_pos;
  mat_mult_vec4f(projection_matrix, view_pos, projected_pos);
  vec4f_t screen_pos;
  mat_mult_vec4f(viewport_matrix, projected_pos, screen_pos);
  vec3f_t result_pos = {screen_pos[0], screen_pos[1], screen_pos[2]};

  stream_t result = {
      .color = {vertex.color[0], vertex.color[1], vertex.color[2],
                vertex.color[3]},
      .position = {result_pos[0], result_pos[1], result_pos[2]},
  };

  return result;
}

static uint16_t rgb3f_to_rgb565(vec3f_t const color){
    uint8_t const r = (uint8_t)(0x1F * color[0]);
    uint8_t const g = (uint8_t)(0x3F * color[1]);
    uint8_t const b = (uint8_t)(0x1F * color[2]);
    uint16_t result = (uint16_t)(((r & 0x1F) << 11) | ((g & 0x3F) << 5) | (b & 0x1F));
    return result;
}

void runderer_fragment_shader_flat(const fragment_t *frag_buf,
                                   uint frags_to_process,
                                   framebuffer_t *frame) {
  for (uint i = 0; i < frags_to_process; ++i) {
    fragment_t const frag = frag_buf[i];
    uint const x = (uint)(frag.screen[0]);
    uint const y = (uint)(frag.screen[1]);
    frame->buf16[y * frame->width + x] = rgb3f_to_rgb565(frag.color);
  }
}

void perspective(float field_of_view, float aspect_ratio, float n, float f, mat4f_t mat) {
    float const halftan = tanf(.5f * field_of_view);
    float const h = halftan;
    float const a = aspect_ratio;

    mat[0] = 1 / (a * h);
    mat[1] = 0;
    mat[2] = 0;
    mat[3] = 0;

    mat[4] = 0;
    mat[5] = 1 / h;
    mat[6] = 0;
    mat[7] = 0;

    mat[8] = 0;
    mat[9] = 0;
    mat[10] = (n + f) / (n - f);
    mat[11] = (2 * n * f) / (n - f);

    mat[12] = 0;
    mat[13] = 0;
    mat[14] = -1;
    mat[15] = 0;
}

void orthographic(float left, float right, float bottom, float top, float near, float far, mat4f_t mat) {
    float const tx = -(right + left) / (right - left);
    float const ty = -(top + bottom) / (top - bottom);
    float const tz = -(far + near) / (far - near);

    mat[0] = 2 / (right - left);
    mat[1] = 0;
    mat[2] = 0;
    mat[3] = tx;

    mat[4] = 0;
    mat[5] = 2 / (top - bottom);
    mat[6] = 0;
    mat[7] = ty;

    mat[8] = 0;
    mat[9] = 0;
    mat[10] = 2 / (near - far);
    mat[11] = tz;

    mat[12] = 0;
    mat[13] = 0;
    mat[14] = 0;
    mat[15] = 1;
}
