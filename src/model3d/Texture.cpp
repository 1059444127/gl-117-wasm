/*
    GL-117
    Copyright 2001-2004 Thomas A. Drexl aka heptargon

    This file is part of GL-117.

    GL-117 is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    GL-117 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GL-117; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef IS_MODEL3D_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "model3d/Model3d.h"
#include "opengl/GlPrimitives.h"
#include "loadbitmap/Bitmap.h"
#include "logging/Logging.h"



Texture::Texture ()
{
  data = NULL;
  name = "";
  mipmap = true;
  textureID = -1;
  width = 0;
  height = 0;
  texlight = 1.0F;
  texred = 1.0F;
  texgreen = 1.0F;
  texblue = 1.0F;
  alpha = false;
}

Texture::Texture (const std::string &filename, int alphaprogram, bool mipmap, bool alpha)
{
  texlight = 1.0F;
  texred = 1.0F;
  texgreen = 1.0F;
  texblue = 1.0F;

  loadFromTGA (filename, alphaprogram, mipmap, alpha);
}

Texture::~Texture ()
{
  if (data != NULL)
    delete data;
}

bool Texture::loadFromTGA (const std::string &filename, int alphaprogram, bool mipmap, bool alpha)
{
  int i, i2;
//  char buf [4096];

//#ifdef IS_LOADTGA_H
  Bitmap *bitmap = BitmapFactory::getBitmap (filename);
  width = bitmap->width;
  height = bitmap->height;
  data = bitmap->data;

  long texl = 0;
  long texr = 0;
  long texg = 0;
  long texb = 0;

  // analyse texture file
  for (i = 0; i < height; i ++)
    for (i2 = 0; i2 < width; i2 ++)
    {
      int n2 = (i * width + i2) * 4;

      texl += (int) data [n2 + 2] + data [n2 + 1] + data [n2];
      texr += (int) data [n2];
      texg += (int) data [n2 + 1];
      texb += (int) data [n2 + 2];

      if (alphaprogram == 0) // alpha=255 or 0
      {
        if (data [n2+0] + data [n2+1] + data [n2+2] < 30)
          data [n2+3] = 0;
        else
          data [n2+3] = 255;
      }
      else if (alphaprogram == 1) // alpha=maxcolor
      {
        int max = (data [n2+0] > data [n2+1] ? data [n2+0] : data [n2+1]);
        max = (max > data [n2+2] ? max : data [n2+2]);
        data [n2+3] = max;
      }
      else if (alphaprogram == 2) // alpha=red
      {
        data [n2+3] = data [n2];
      }
      else if (alphaprogram == 3) // alpha=midcolor*6 or 0
      {
        int test = (data [n2] + data [n2 + 1] + data [n2 + 2]) * 2;
        if (test > 255) test = 255;
        else if (test < 30) test = 0;
        data [n2+3] = (unsigned char) test;
      }
      else if (alphaprogram == 4) // alpha=red, color=white
      {
        data [n2+3] = data [n2+0];
        data [n2+0] = 255;
        data [n2+1] = 255;
        data [n2+2] = 255;
      }
      else if (alphaprogram == 5) // alpha=red*2, color=black
      {
        int test = data [n2+0] * 2;
        if (test > 255) test = 255;
        data [n2+3] = test;
        data [n2+0] = 0;
        data [n2+1] = 0;
        data [n2+2] = 0;
      }
      else if (alphaprogram == 6) // alpha=red, color=black
      {
        data [n2+3] = data [n2+0];
        data [n2+0] = 0;
        data [n2+1] = 0;
        data [n2+2] = 0;
      }
    }

/*#ifndef IS_LOADTGA_H
  free (buf);
#endif*/

  texlight = (float) texl / width / height / 3 / 256; // average brightness
  texred = (float) texr / width / height / 256; // average red
  texgreen = (float) texg / width / height / 256; // average green
  texblue = (float) texb / width / height / 256; // average blue
  name = filename;
  this->mipmap = mipmap;
  this->alpha = alpha;

  textureID = gl.registerTexture (filename, data, width, height, mipmap);

  return true;
}

/*void Texture::getColor (Color *color, int x, int y) const
{
  if (x < 0) x = (int) -x % width;
  if (y < 0) y = (int) -y % height;
  if (x >= width) x = (int) x % width;
  if (y >= height) y = (int) y % height;

  int offset = y * width + x;
  offset <<= 2;

  color->c [0] = data [offset];
  color->c [1] = data [offset + 1];
  color->c [2] = data [offset + 2];
  color->c [3] = data [offset + 3];
}*/

/*void Texture::shadeLinear () const
{
  gl.enableLinearTexture (textureID, mipmap);
}

void Texture::shadeConst () const
{
  gl.disableLinearTexture (textureID, mipmap);
}*/

#endif

