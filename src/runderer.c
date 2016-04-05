#include "runderer.h"
#include "math.h"
#include "matrix.h"
#include <stdint.h>
#include <stdlib.h>
#include <float.h>

// Convert r, g, b (in bytes) to a 16-bit 5R6G5B color
#define rgb_to_565(r, g, b) (uint16_t)((((r) & 0x1F) << 11) | (((g) & 0x3F) << 5) | ((b) & 0x1F))

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

    for (int i = 0; i < fb->width * fb->height; ++i) {
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

    return 0;
}

void runderer_draw_triangle_array(runderer_t *self, const vertex_t *vertices,
                                  uint count) {
  for (uint i = 0; i < count; i++) {
    stream_t verts[3];
    for (uint j = 0; j < 3; j++) {
      verts[j] =
          self->vertex_shader(vertices[i * 3 + j], self->model_matrix,
                              self->view_matrix, self->projection_matrix);
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
                                mat4f_t projection_matrix) {
  vec4f_t world_pos;
  mat_mult_vec4f(model_matrix, vertex.position, world_pos);
  vec4f_t view_pos;
  mat_mult_vec4f(view_matrix, world_pos, view_pos);
  vec4f_t screen_pos;
  mat_mult_vec4f(projection_matrix, view_pos, screen_pos);
  vec3f_t result_pos = {screen_pos[0], screen_pos[1], screen_pos[2]};

  stream_t result = {
      .color = {vertex.color[0], vertex.color[1], vertex.color[2],
                vertex.color[3]},
      .position = {result_pos[0], result_pos[1], result_pos[2]},
  };

  return result;
}

void runderer_fragment_shader_flat(const fragment_t *frag_buf,
                                   uint frags_to_process,
                                   framebuffer_t *frame) {
  for (uint i = 0; i < frags_to_process; ++i) {
    fragment_t const frag = frag_buf[i];
    uint const x = (uint)(frag.screen[0]);
    uint const y = (uint)(frag.screen[1]);
    uint16_t const color_shaded = rgb_to_565((unsigned int)(0xFF * frag.color[0]),
                                             (unsigned int)(0xFF * frag.color[1]),
                                             (unsigned int)(0xFF * frag.color[2]));
    frame->buf16[y * frame->width + x] = color_shaded;
  }
}
