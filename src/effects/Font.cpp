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

/* This file includes all special effects. */

#ifndef IS_EFFECTS_H

#include "Effects.h"
#include "opengl/GlPrimitives.h"

#include <math.h>
#include <stdlib.h>



bool Font::isPixel (int x, int y)
{
  y = texture->height - y;
  int index = (y * texture->width + x) * 4;
  if (texture->data [index] > 50 && texture->data [index + 1] > 50 && texture->data [index + 2] > 50)
    return true;
  return false;
}

void Font::extractLetters (int height, char start, int num)
{
  int i;
  int mode = 0;
  bool modeon = 0;
  this->start = start;
  this->height = height;
  int x = 0, y = 0;
  int xs = 0, xe = 0;
  n = 0;

  for (;;)
  {
    mode ++;
    for (i = y + 2; i <= y + height - 2; i += 2)
    {
      if (isPixel (x, i))
      {
        mode = 0;
      }
    }
    if (mode == 0)
    {
      if (!modeon)
      {
        xs = x;
        modeon = true;
      }
    }
    if (mode >= 5)
    {
      if (modeon)
      {
        xe = x;
        modeon = false;
        letterx [n] = xs - 2;
        lettery [n] = y;
        letterw [n] = xe - xs + 1;
        n ++;
        if (n >= num) return;
      }
    }
    x ++;
    if (x >= texture->width)
    {
      if (modeon)
      {
        xe = x;
        modeon = false;
        letterx [n] = xs - 2;
        lettery [n] = y;
        letterw [n] = xe - xs + 3;
        n ++;
        if (n >= num) return;
      }
      y += height;
      x = 0;
    }
    if (y >= texture->height) return;
  }
}

Font::Font (const std::string &filename, int height, char start, int num)
{
  texture = new Texture (filename, 0, false, true);
  extractLetters (height, start, num);
  zoom = 0.1F;
  stdcol.set (255, 255, 255, 220);
  highlightcol.set (255, 255, 0, 255);
}

Font::~Font ()
{
}

void Font::drawText (float x, float y, float z, const std::string &str, const Color &c, bool centered, int highlight, const Color &highlightcol)
{
  size_t len = str.length ();
  glDisable (GL_LIGHTING);
  glDisable (GL_DEPTH_TEST);
  gl.enableTexture (texture->textureID);
  gl.enableAlphaBlending ();
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, 0.1);
  glBegin (GL_QUADS);
  glColor4ub (c.c [0], c.c [1], c.c [2], c.c [3]);
  float xz = x * zoom, yz = y * zoom;

  if (centered)
  {
    float xw = 0;
    for (size_t i = 0; i < len; i ++)
    {
      if (str [i] >= start && str [i] < start + n)
      {
        int c = (int) (str [i] - start);
        xw += zoom * letterw [c] / height;
      }
      else if (str [i] == '\t')
      {
        int xzint = (int) (xw * 10.0);
        xzint -= xzint & 3; // modulo 4
        xzint += 4;
        xw = (float) xzint * zoom;
      }
      else
      {
        xw += 0.5 * zoom;
      }
    }
    xz -= xw / 2;
  }

  for (size_t i = 0; i < len; i ++)
  {
    if (str [i] >= start && str [i] < start + n)
    {
      if (i != highlight)
        glColor4ub (c.c [0], c.c [1], c.c [2], c.c [3]);
      else
        glColor4ub (highlightcol.c [0], highlightcol.c [1], highlightcol.c [2], highlightcol.c [3]);
      int c = (int) (str [i] - start);
      float tx = (float) letterx [c] / texture->width;
      float ty = 1.0 - (float) lettery [c] / texture->height;
      float tx2 = (float) tx + (float) letterw [c] / texture->width;
      float ty2 = (float) ty - (float) height / texture->height;
      float xi = zoom * letterw [c] / height;
      float yi = zoom;
      glTexCoord2f (tx, ty2);
      glVertex3f (xz, yz, z);
      glTexCoord2f (tx, ty);
      glVertex3f (xz, yz + yi, z);
      glTexCoord2f (tx2, ty);
      glVertex3f (xz + xi, yz + yi, z);
      glTexCoord2f (tx2, ty2);
      glVertex3f (xz + xi, yz, z);
      xz += xi;
    }
    else if (str [i] == '\t')
    {
      int xzint = (int) (xz * 10.0);
      xzint -= xzint & 3; // modulo 4
      xzint += 4;
      xz = (float) xzint * zoom;
    }
    else if (str [i] == '\n')
    {
      yz -= zoom;
      xz = x * zoom;
    }
    else
    {
      xz += 0.5 * zoom;
    }
  }
  glEnd ();
  glDisable (GL_ALPHA_TEST);
  gl.disableAlphaBlending ();
  glDisable (GL_TEXTURE_2D);
}

void Font::drawText (float x, float y, float z, const std::string &str, const Color &c, int highlight, const Color &highlightcol)
{
  drawText (x, y, z, str, c, false, highlight, highlightcol);
}

void Font::drawText (float x, float y, float z, const std::string &str, const Color &c, int highlight)
{
  drawText (x, y, z, str, c, false, highlight, highlightcol);
}

void Font::drawText (float x, float y, float z, const std::string &str, int highlight, const Color &highlightcol)
{
  drawText (x, y, z, str, stdcol, false, highlight, highlightcol);
}

void Font::drawText (float x, float y, float z, const std::string &str, int highlight)
{
  drawText (x, y, z, str, stdcol, false, highlight, highlightcol);
}

void Font::drawText (float x, float y, float z, const std::string &str, const Color &c)
{
  drawText (x, y, z, str, c, false, -1, highlightcol);
}

void Font::drawText (float x, float y, float z, const std::string &str)
{
  drawText (x, y, z, str, stdcol, false, -1, highlightcol);
}

void Font::drawTextCentered (float x, float y, float z, const std::string &str, const Color &c, int highlight, const Color &highlightcol)
{
  drawText (x, y, z, str, c, true, highlight, highlightcol);
}

void Font::drawTextCentered (float x, float y, float z, const std::string &str, const Color &c, int highlight)
{
  drawText (x, y, z, str, c, true, highlight, highlightcol);
}

void Font::drawTextCentered (float x, float y, float z, const std::string &str, const Color &c)
{
  drawText (x, y, z, str, c, true, -1, highlightcol);
}

void Font::drawTextCentered (float x, float y, float z, const std::string &str, int highlight, const Color &highlightcol)
{
  drawText (x, y, z, str, stdcol, true, highlight, highlightcol);
}

void Font::drawTextCentered (float x, float y, float z, const std::string &str, int highlight)
{
  drawText (x, y, z, str, stdcol, true, highlight, highlightcol);
}

void Font::drawTextCentered (float x, float y, float z, const std::string &str)
{
  drawText (x, y, z, str, stdcol, true, -1, highlightcol);
}

void Font::drawTextRotated (float x, float y, float z, const std::string &str, const Color &color, int timer)
{
  size_t len = str.length ();
  glDisable (GL_LIGHTING);
  glDisable (GL_DEPTH_TEST);
  gl.enableTexture (texture->textureID);
  gl.enableAlphaBlending ();
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, 0.1);
  float xz = x * 0.1, yz = y * 0.1;
  for (size_t i = 0; i < len; i ++)
  {
    if (str [i] >= start && str [i] <= start + n)
    {
      int c = (int) (str [i] - start);
      float tx = (float) letterx [c] / texture->width;
      float ty = 1.0 - (float) lettery [c] / texture->height;
      float tx2 = (float) tx + (float) letterw [c] / texture->width;
      float ty2 = (float) ty - (float) height / texture->height;
      float xi = 0.1 * letterw [c] / height;
      float yi = 0.1;
      glPushMatrix ();
      glTranslatef (xz + xi / 2, yz + yi / 2, z);
      glRotatef ((float) timer / timestep + i * 20, 1, 0, 0);
      glBegin (GL_QUADS);
      glColor4ub (color.c [0], color.c [1], color.c [2], color.c [3]);
      glTexCoord2f (tx, ty2);
      glVertex3f (-xi / 2, -yi / 2, 0);
      glTexCoord2f (tx, ty);
      glVertex3f (-xi / 2, yi / 2, 0);
      glTexCoord2f (tx2, ty);
      glVertex3f (xi / 2, yi / 2, 0);
      glTexCoord2f (tx2, ty2);
      glVertex3f (xi / 2, -yi / 2, 0);
      glEnd ();
      glPopMatrix ();
      xz += xi;
    }
    else if (str [i] == '\t')
    {
      int xzint = (int) (xz * 10.0);
      xzint -= xzint & 3; // modulo 4
      xzint += 4;
      xz = (float) xzint / 10.0;
    }
    else
    {
      xz += 0.05;
    }
  }
  glDisable (GL_ALPHA_TEST);
  gl.disableAlphaBlending ();
  glDisable (GL_TEXTURE_2D);
}

void Font::drawTextScaled (float x, float y, float z, const std::string &str, const Color &color, int timer, int highlight, const Color &highlightcol)
{
  size_t i;
  size_t len = str.length ();
  glDisable (GL_LIGHTING);
  glDisable (GL_DEPTH_TEST);
  gl.enableTexture (texture->textureID);
  gl.enableAlphaBlending ();
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, 0.1);

  float xw = 0;
  for (i = 0; i < len; i ++)
  {
    if (str [i] >= start && str [i] <= start + n)
    {
      int c = (int) (str [i] - start);
      xw += 0.1 * letterw [c] / height;
    }
    else if (str [i] == '\t')
    {
      int xzint = (int) (xw * 10.0);
      xzint -= xzint & 3; // modulo 4
      xzint += 4;
      xw = (float) xzint / 10.0;
    }
    else
    {
      xw += 0.05;
    }
  }

  float xz = x * 0.1, yz = y * 0.1;
  glPushMatrix ();
  glTranslatef (xz + xw / 2, yz, z);
  xz = 0;
  glScalef (1.0 + 0.08 * SIN(abs (timer * 8 / timestep % 360)), 1, 1);
  for (i = 0; i < len; i ++)
  {
    if (str [i] >= start && str [i] <= start + n)
    {
      int c = (int) (str [i] - start);
      float tx = (float) letterx [c] / texture->width;
      float ty = 1.0 - (float) lettery [c] / texture->height;
      float tx2 = (float) tx + (float) letterw [c] / texture->width;
      float ty2 = (float) ty - (float) height / texture->height;
      float xi = 0.1 * letterw [c] / height;
      float yi = 0.1;
      if (i != highlight)
        glColor4ub (color.c [0], color.c [1], color.c [2], color.c [3]);
      else
        glColor4ub (highlightcol.c [0], highlightcol.c [1], highlightcol.c [2], highlightcol.c [3]);
      glPushMatrix ();
      glTranslatef (xz + xi / 2 - xw / 2, yi / 2, 0);

      glBegin (GL_QUADS);
      glTexCoord2f (tx, ty2);
      glVertex3f (-xi / 2, -yi / 2, 0);
      glTexCoord2f (tx, ty);
      glVertex3f (-xi / 2, yi / 2, 0);
      glTexCoord2f (tx2, ty);
      glVertex3f (xi / 2, yi / 2, 0);
      glTexCoord2f (tx2, ty2);
      glVertex3f (xi / 2, -yi / 2, 0);
      glEnd ();

      glPopMatrix ();
      xz += xi;
    }
    else if (str [i] == '\t')
    {
      int xzint = (int) (xz * 10.0);
      xzint -= xzint & 3; // modulo 4
      xzint += 4;
      xz = (float) xzint / 10.0;
    }
    else
    {
      xz += 0.05;
    }
  }
  glPopMatrix ();
  glDisable (GL_ALPHA_TEST);
  gl.disableAlphaBlending ();
  glDisable (GL_TEXTURE_2D);
}

void Font::drawTextScaled (float x, float y, float z, const std::string &str, const Color &color, int timer, int highlight)
{
  drawTextScaled (x, y, z, str, color, timer, highlight, highlightcol);
}

void Font::drawTextScaled (float x, float y, float z, const std::string &str, const Color &color, int timer)
{
  drawTextScaled (x, y, z, str, color, timer, -1, highlightcol);
}

void Font::drawTextScaled (float x, float y, float z, const std::string &str, int timer, int highlight, const Color &highlightcol)
{
  drawTextScaled (x, y, z, str, stdcol, timer, highlight, highlightcol);
}

void Font::drawTextScaled (float x, float y, float z, const std::string &str, int timer, int highlight)
{
  drawTextScaled (x, y, z, str, stdcol, timer, highlight, highlightcol);
}

void Font::drawTextScaled (float x, float y, float z, const std::string &str, int timer)
{
  drawTextScaled (x, y, z, str, stdcol, timer, -1, highlightcol);
}

#endif
