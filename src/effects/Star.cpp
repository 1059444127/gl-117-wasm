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



Star::Star (int phi, int gamma, float size)
{
  this->phi = phi;
  this->gamma = gamma;
  this->size = size;
}

void Star::draw ()
{
  glPointSize (size * 1.5); // TODO: should depend on screen resolution!
//    gl.enableAntiAliasing ();
  glBegin (GL_POINTS);
  glColor3ub (255, 255, 255);
  glVertex3f (0, 0, 0);
  glEnd ();
//    gl.disableAntiAliasing ();
}

#endif
