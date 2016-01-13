//
//  main.cpp
//  TextToImage
//
//  Created by Nirav Sheth on 1/12/16.
//
//
#include <iostream>
#include "DrawText.cpp"

int main(int argc, const char * argv[]) {
  FontOptions *fo = new FontOptions;
  fo->fontFamily = "Arial";
  fo->bold = false;
  fo->italic = false;
  fo->size = 60;
  ColorOptions *co = new ColorOptions;
  co->r = 100;
  co->g = 100;
  co->b = 100;
  co->a = 1;
  
  ShadowOptions *so = new ShadowOptions;
  ColorOptions *sco = new ColorOptions;
  sco->r = 255;
  sco->g = 0;
  sco->b = 0;
  sco->a = .2;
  so->colorOptions = sco;
  so->offsetX = 3;
  so->offsetY = 3;
  OutputBuffer *buffer = new OutputBuffer;
  ConvertTextToImage(buffer, "Quantity", fo, co, so);
  FILE *f = fopen("out.png", "wb");
  fwrite(buffer->data, 1, buffer->length, f);
  return 0;
}
