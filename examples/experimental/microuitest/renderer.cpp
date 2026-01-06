#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "src/components/framework/ui/egl/egl.h"
#include "src/components/framework/ui/egl/display.h"
#include "src/components/framework/ui/egl/context.h"
#include "src/components/framework/ui/egl/window.h"
#include "src/components/framework/ui/opengles/opengles.h"
#include "src/components/framework/ui/opengles/constants.h"
#include "src/components/framework/ui/opengles/glwindow.h"
#include "renderer.h"
#include "atlas.inl"
//#include <GL/gl.h>
#define BUFFER_SIZE 16384

static float   tex_buf[BUFFER_SIZE *  8];
static float  vert_buf[BUFFER_SIZE *  8];
static unsigned char color_buf[BUFFER_SIZE * 16];
static unsigned int  index_buf[BUFFER_SIZE *  6];

static int width  = 800;
static int height = 600;
static int buf_idx;

static SDL_Window *window;
qor::components::OpenGLESFeature* opengl;

void r_init(void) {
  /* init SDL window */
  window = SDL_CreateWindow(
    NULL, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    width, height, SDL_WINDOW_OPENGL);
  SDL_GL_CreateContext(window);

  /* init gl */
  opengl->Enable(GL_BLEND);
  opengl->BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  opengl->Disable(GL_CULL_FACE);
  opengl->Disable(GL_DEPTH_TEST);
  opengl->Enable(GL_SCISSOR_TEST);
  opengl->Enable(GL_TEXTURE_2D);
  opengl->EnableClientState(GL_VERTEX_ARRAY);
  opengl->EnableClientState(GL_TEXTURE_COORD_ARRAY);
  opengl->glEnableClientState(GL_COLOR_ARRAY);

  /* init texture */
  unsigned int id;
  opengl->GenTextures(1, &id);
  opengl->BindTexture(GL_TEXTURE_2D, id);
  opengl->TexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, ATLAS_WIDTH, ATLAS_HEIGHT, 0,
    GL_ALPHA, GL_UNSIGNED_BYTE, atlas_texture);
  opengl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  opengl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  assert(opengl->GetError() == 0);
}


static void flush(void) {
  if (buf_idx == 0) { return; }

  opengl->Viewport(0, 0, width, height);
  opengl->MatrixMode(GL_PROJECTION);
  opengl->PushMatrix();
  opengl->LoadIdentity();
  opengl->Ortho(0.0f, width, height, 0.0f, -1.0f, +1.0f);
  opengl->MatrixMode(GL_MODELVIEW);
  opengl->PushMatrix();
  opengl->LoadIdentity();

  opengl->TexCoordPointer(2, GL_FLOAT, 0, tex_buf);
  opengl->VertexPointer(2, GL_FLOAT, 0, vert_buf);
  opengl->ColorPointer(4, GL_UNSIGNED_BYTE, 0, color_buf);
  opengl->DrawElements(GL_TRIANGLES, buf_idx * 6, GL_UNSIGNED_INT, index_buf);

  opengl->MatrixMode(GL_MODELVIEW);
  opengl->PopMatrix();
  opengl->MatrixMode(GL_PROJECTION);
  opengl->PopMatrix();

  buf_idx = 0;
}


static void push_quad(mu_Rect dst, mu_Rect src, mu_Color color) {
  if (buf_idx == BUFFER_SIZE) { flush(); }

  int texvert_idx = buf_idx *  8;
  int   color_idx = buf_idx * 16;
  int element_idx = buf_idx *  4;
  int   index_idx = buf_idx *  6;
  buf_idx++;

  /* update texture buffer */
  float x = src.x / (float) ATLAS_WIDTH;
  float y = src.y / (float) ATLAS_HEIGHT;
  float w = src.w / (float) ATLAS_WIDTH;
  float h = src.h / (float) ATLAS_HEIGHT;
  tex_buf[texvert_idx + 0] = x;
  tex_buf[texvert_idx + 1] = y;
  tex_buf[texvert_idx + 2] = x + w;
  tex_buf[texvert_idx + 3] = y;
  tex_buf[texvert_idx + 4] = x;
  tex_buf[texvert_idx + 5] = y + h;
  tex_buf[texvert_idx + 6] = x + w;
  tex_buf[texvert_idx + 7] = y + h;

  /* update vertex buffer */
  vert_buf[texvert_idx + 0] = dst.x;
  vert_buf[texvert_idx + 1] = dst.y;
  vert_buf[texvert_idx + 2] = dst.x + dst.w;
  vert_buf[texvert_idx + 3] = dst.y;
  vert_buf[texvert_idx + 4] = dst.x;
  vert_buf[texvert_idx + 5] = dst.y + dst.h;
  vert_buf[texvert_idx + 6] = dst.x + dst.w;
  vert_buf[texvert_idx + 7] = dst.y + dst.h;

  /* update color buffer */
  memcpy(color_buf + color_idx +  0, &color, 4);
  memcpy(color_buf + color_idx +  4, &color, 4);
  memcpy(color_buf + color_idx +  8, &color, 4);
  memcpy(color_buf + color_idx + 12, &color, 4);

  /* update index buffer */
  index_buf[index_idx + 0] = element_idx + 0;
  index_buf[index_idx + 1] = element_idx + 1;
  index_buf[index_idx + 2] = element_idx + 2;
  index_buf[index_idx + 3] = element_idx + 2;
  index_buf[index_idx + 4] = element_idx + 3;
  index_buf[index_idx + 5] = element_idx + 1;
}


void r_draw_rect(mu_Rect rect, mu_Color color) {
  push_quad(rect, atlas[ATLAS_WHITE], color);
}


void r_draw_text(const char *text, mu_Vec2 pos, mu_Color color) {
  mu_Rect dst = { pos.x, pos.y, 0, 0 };
  for (const char *p = text; *p; p++) {
    if ((*p & 0xc0) == 0x80) { continue; }
    int chr = mu_min((unsigned char) *p, 127);
    mu_Rect src = atlas[ATLAS_FONT + chr];
    dst.w = src.w;
    dst.h = src.h;
    push_quad(dst, src, color);
    dst.x += dst.w;
  }
}


void r_draw_icon(int id, mu_Rect rect, mu_Color color) {
  mu_Rect src = atlas[id];
  int x = rect.x + (rect.w - src.w) / 2;
  int y = rect.y + (rect.h - src.h) / 2;
  push_quad(mu_rect(x, y, src.w, src.h), src, color);
}


int r_get_text_width(const char *text, int len) {
  int res = 0;
  for (const char *p = text; *p && len--; p++) {
    if ((*p & 0xc0) == 0x80) { continue; }
    int chr = mu_min((unsigned char) *p, 127);
    res += atlas[ATLAS_FONT + chr].w;
  }
  return res;
}


int r_get_text_height(void) {
  return 18;
}


void r_set_clip_rect(mu_Rect rect) {
  flush();
  opengl->Scissor(rect.x, height - (rect.y + rect.h), rect.w, rect.h);
}


void r_clear(mu_Color clr) {
  flush();
  opengl->learColor(clr.r / 255., clr.g / 255., clr.b / 255., clr.a / 255.);
  opengl->Clear(GL_COLOR_BUFFER_BIT);
}


void r_present(void) {
  flush();
  SDL_GL_SwapWindow(window);
}
