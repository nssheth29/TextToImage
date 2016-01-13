//
//  DrawText.cpp
//  TextToImage
//
//  Created by Nirav Sheth on 1/12/16.
//
//
#include <cairo.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>

struct FontOptions {
  bool bold;
  bool italic;
  const char *fontFamily;
  int size;
};

struct ColorOptions {
  int r;
  int g;
  int b;
  double a;
};

struct ShadowOptions {
  int offsetX;
  int offsetY;
  ColorOptions *colorOptions;
};

struct OutputBuffer {
  char *data;
  int length = 0;
};

static cairo_status_t writePngToBuffer(void *closure, const unsigned char *data, unsigned int length) {
  OutputBuffer *myBuffer = (OutputBuffer *)closure;
  if (myBuffer->length == 0)
    myBuffer->data = (char *)malloc(length);
  else
    myBuffer->data = (char *)realloc(myBuffer->data, myBuffer->length + length);
  if (myBuffer->data == nullptr)
    return CAIRO_STATUS_NO_MEMORY;
  memcpy(myBuffer->data + myBuffer->length, data, length);
  myBuffer->length = myBuffer->length + length;
  return CAIRO_STATUS_SUCCESS;
}

void ConvertTextToImage(OutputBuffer *buffer, const char *text, FontOptions *fontOptions, ColorOptions *colorOptions, ShadowOptions *shadowOptions) {
  /* Variable declarations */
  cairo_surface_t *surface;
  cairo_t *cr;
  
  double x, y, x2, y2;
  
  cairo_font_extents_t fe;
  cairo_text_extents_t te;
  
  double initialW = 1000;
  double initialH = 300;
  double fontSize = fontOptions->size;
  
  surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, initialW, initialH);
  cr = cairo_create (surface);
  cairo_set_font_size (cr, fontSize);
  
  cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
  cairo_select_font_face (cr, fontOptions->fontFamily,
                          fontOptions->italic ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL,
                          fontOptions->bold ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL);
  cairo_font_extents (cr, &fe);
  cairo_text_extents (cr, text, &te);
  
  double newWidth = te.width + 2 + (shadowOptions == nullptr ? 0 : shadowOptions->offsetX);
  double newHeight = te.height + 2 + (shadowOptions == nullptr ? 0 : shadowOptions->offsetY);
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
  
  surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, newWidth, newHeight);
  cr = cairo_create (surface);
  cairo_set_font_size (cr, fontSize);
  cairo_select_font_face (cr, fontOptions->fontFamily,
                          fontOptions->italic ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL,
                          fontOptions->bold ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL);

  x = 0;
  y = fe.ascent - fe.descent + 3;

  if (shadowOptions != nullptr) {
    x2 = x + shadowOptions->offsetX;
    y2 = y + shadowOptions->offsetY;
    cairo_move_to (cr, x2, y2);
    cairo_set_source_rgba (cr, shadowOptions->colorOptions->r, shadowOptions->colorOptions->g, shadowOptions->colorOptions->b, shadowOptions->colorOptions->a);
    cairo_show_text (cr, text);
  }
  
  cairo_move_to (cr, x, y);
  cairo_set_source_rgba (cr, colorOptions->r, colorOptions->g, colorOptions->b, colorOptions->a);
  cairo_show_text (cr, text);
  
  cairo_surface_write_to_png_stream(surface, writePngToBuffer, buffer);
//  cairo_surface_write_to_png (surface, "textextents.png");
  cairo_destroy (cr);
  cairo_surface_destroy (surface);
  

}