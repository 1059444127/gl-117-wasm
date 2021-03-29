/*  -*- mode:c++; c-basic-offset:2; -*-
    GL-117
    Copyright 2001, 2004 Thomas A. Drexl aka heptargon
    This file is copyright 2004 Jean-Marc Le Peuvedic

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

#ifndef IS_SHADERS_H
#define IS_SHADERS_H

#include "../config.h"

class GlLandscape;

class GlShaders {
public:
  // draw all the trees in a part of the terrain
  virtual void drawTrees(int ax, int ex, int ay, int ey, int step)=0;
  // load frame uniform parameters of the shader
  virtual void loadFrameUniformParams(int phi)=0;
  // virtual destructor
  virtual ~GlShaders() {};
};

// Init everything *after* OpenGL has been initialized.
extern GlShaders * createShaders(GlLandscape& _land);
extern void destroyShaders(GlShaders *);

#endif // IS_SHADERS_H
