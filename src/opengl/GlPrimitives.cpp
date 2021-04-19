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

#ifndef IS_GLPRIMITIVES_H

#include "game/Game.h"
#include "GlPrimitives.h"
#include "includegl.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


/****************************************************************************
OpenGL
****************************************************************************/

GlPrimitives gl;

GlPrimitives::GlPrimitives ()
{
  numLists = 0; // display lists start at 1 (whereas textures start at 0)
  setFogColor (127, 127, 127);
  fogLuminance = 1.0;
}

GlPrimitives::~GlPrimitives ()
{
}

void GlPrimitives::clearBuffers () const
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GlPrimitives::swapBuffers () const
{
  SDL_GL_SwapWindow (sdlwindow);
}

void GlPrimitives::rotate (float x, float y, float z)
{
  glRotatef (z, 0.0f, 0.0f, 1.0f);
  glRotatef (y, 0.0f, 1.0f, 0.0f);
  glRotatef (x, 1.0f, 0.0f, 0.0f);
}

void GlPrimitives::callList (int list) const
{
  glCallList (list);
}

int GlPrimitives::createList ()
{
  numLists ++;
  glNewList (numLists, GL_COMPILE_AND_EXECUTE);
  return numLists;
}

void GlPrimitives::enableLinearTexture (int texnum, bool mipmap) const
{
  glBindTexture (GL_TEXTURE_2D, texnum);
  if (!mipmap)
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  else
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void GlPrimitives::disableLinearTexture (int texnum, bool mipmap) const
{
  glBindTexture (GL_TEXTURE_2D, texnum);
  if (!mipmap)
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  else
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

int GlPrimitives::registerTexture (const std::string &name, const unsigned char *data,
                         int width, int height, bool mipmap)
{
  int i;
  int numtextures = texList.size ();
  for (i = 0; i < numtextures; i ++)
    if (!texList [i].compare (name))
      return i;
  texList.push_back (std::string (name));

  glBindTexture (GL_TEXTURE_2D, numtextures);

//  if (!antialiasing) disableLinearTexture (numTextures, mipmap);
//  else enableLinearTexture (numTextures, mipmap);

// if (!mipmap)
    glTexImage2D (GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//  else
//    gluBuild2DMipmaps (GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

  return numtextures;
}

void GlPrimitives::enableAntiAliasing () const
{
  glEnable (GL_LINE_SMOOTH);
  glEnable (GL_POINT_SMOOTH);
  glEnable (GL_POLYGON_SMOOTH);
  glHint (GL_LINE_SMOOTH_HINT, GL_FASTEST);
  glHint (GL_POINT_SMOOTH_HINT, GL_FASTEST);
  glHint (GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
}

void GlPrimitives::disableAntiAliasing () const
{
  glDisable (GL_LINE_SMOOTH);
  glDisable (GL_POINT_SMOOTH);
  glDisable (GL_POLYGON_SMOOTH);
}

void GlPrimitives::enableAlphaBlending () const
{
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GlPrimitives::disableAlphaBlending () const
{
  glDisable (GL_BLEND);
}

void GlPrimitives::enableAlphaTesting (float intensity) const
{
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, intensity);
}

void GlPrimitives::disableAlphaTesting () const
{
  glDisable (GL_ALPHA_TEST);
}

void GlPrimitives::enableTexture (int num) const
{
  glBindTexture (GL_TEXTURE_2D, num);
  glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); /* GL_CLAMP or GL_REPEAT? */
  glEnable (GL_TEXTURE_2D);
}

void GlPrimitives::enableLighting () const
{
  glEnable (GL_LIGHTING);
}

void GlPrimitives::disableLighting () const
{
  glDisable (GL_LIGHTING);
}

void GlPrimitives::enableZBuffer () const
{
  glEnable (GL_DEPTH_TEST);
  glDepthFunc (GL_LEQUAL);
}

void GlPrimitives::disableZBuffer () const
{
  glDisable (GL_DEPTH_TEST);
}

void GlPrimitives::shadeFlat () const
{
  glShadeModel (GL_FLAT);
}

void GlPrimitives::shadeSmooth () const
{
  glShadeModel (GL_SMOOTH);
}

void GlPrimitives::enableFog (float view, bool fast) const
{
  float fcol [3];
  fcol [0] = fogColor [0] * fogLuminance / 256.0;
  fcol [1] = fogColor [1] * fogLuminance / 256.0;
  fcol [2] = fogColor [2] * fogLuminance / 256.0;
  glEnable (GL_FOG);
  glFogfv (GL_FOG_COLOR, fcol);
  glFogf (GL_FOG_DENSITY, 0.1);
  glFogi (GL_FOG_MODE, GL_LINEAR);
// possible alternatives:
//  glFogf (GL_FOG_DENSITY, 0.017 * 100 / view);
//  glFogi (GL_FOG_MODE, GL_EXP2);
  if (fast)
    glHint (GL_FOG_HINT, GL_FASTEST);
  else
    glHint (GL_FOG_HINT, GL_NICEST);
  glFogf (GL_FOG_START, 1.0);
  glFogf (GL_FOG_END, view);
}

void GlPrimitives::setFogColor (int r, int g, int b)
{
  fogColor [0] = (unsigned char) r;
  fogColor [1] = (unsigned char) g;
  fogColor [2] = (unsigned char) b;
}

void GlPrimitives::setFogLuminance (float lum)
{
  fogLuminance = lum;
}

#endif

