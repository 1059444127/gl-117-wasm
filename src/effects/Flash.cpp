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



Flash::Flash ()
{
  phi = 0;
}

Flash::~Flash ()
{
}

void Flash::set (float x, float y, float z, int phi)
{
  int i;
  this->phi = phi;
  w [0].x = x; w [0].y = y; w [0].z = z;
  for (i = 1; i < 10; i ++)
  {
    w [i].y = y + 6.0 * i;
    w [i].x = w [i - 1].x + 0.1 * Math::random (60) - 3.0;
    w [i].z = w [i - 1].z + 0.1 * Math::random (60) - 3.0;
  }
}

void Flash::draw ()
{
  int i;
  float myzoom = 0.3;
  float cosphi = COS(phi);
  float sinphi = SIN(phi);

  glDisable (GL_TEXTURE_2D);
  glBegin (GL_QUAD_STRIP);
  glColor3ub (255, 255, 255);

  for (i = 0; i < 10; i ++)
  {
    glVertex3f (w [i].x - myzoom * cosphi, w [i].y, w [i].z + myzoom * sinphi);
    glVertex3f (w [i].x + myzoom * cosphi, w [i].y, w [i].z - myzoom * sinphi);
  }
  glEnd ();
}

void Flash::drawHQ ()
{
  int i;
  float myzoom = 0.4;
  float alphawidth = 0.7;
  float cosphi = COS(phi);
  float sinphi = SIN(phi);

  glDisable (GL_TEXTURE_2D);
  gl.enableAlphaBlending ();
  glBegin (GL_QUADS);

  for (i = 0; i < 9; i ++)
  {
    glColor4ub (255, 255, 255, 0);
    glVertex3f (w [i].x - myzoom * cosphi, w [i].y, w [i].z + myzoom * sinphi);
    glColor4ub (255, 255, 255, 255);
    glVertex3f (w [i].x - myzoom * alphawidth * cosphi, w [i].y, w [i].z + myzoom * alphawidth * sinphi);
    glColor4ub (255, 255, 255, 255);
    glVertex3f (w [i + 1].x - myzoom * alphawidth * cosphi, w [i + 1].y, w [i + 1].z + myzoom * alphawidth * sinphi);
    glColor4ub (255, 255, 255, 0);
    glVertex3f (w [i + 1].x - myzoom * cosphi, w [i + 1].y, w [i + 1].z + myzoom * sinphi);

    glColor4ub (255, 255, 255, 255);
    glVertex3f (w [i].x - myzoom * alphawidth * cosphi, w [i].y, w [i].z + myzoom * alphawidth * sinphi);
    glVertex3f (w [i].x + myzoom * alphawidth * cosphi, w [i].y, w [i].z - myzoom * alphawidth * sinphi);
    glVertex3f (w [i + 1].x + myzoom * alphawidth * cosphi, w [i + 1].y, w [i + 1].z - myzoom * alphawidth * sinphi);
    glVertex3f (w [i + 1].x - myzoom * alphawidth * cosphi, w [i + 1].y, w [i + 1].z + myzoom * alphawidth * sinphi);

    glColor4ub (255, 255, 255, 255);
    glVertex3f (w [i].x + myzoom * alphawidth * cosphi, w [i].y, w [i].z - myzoom * alphawidth * sinphi);
    glColor4ub (255, 255, 255, 0);
    glVertex3f (w [i].x + myzoom * cosphi, w [i].y, w [i].z - myzoom * sinphi);
    glColor4ub (255, 255, 255, 0);
    glVertex3f (w [i + 1].x + myzoom * cosphi, w [i + 1].y, w [i + 1].z - myzoom * sinphi);
    glColor4ub (255, 255, 255, 255);
    glVertex3f (w [i + 1].x + myzoom * alphawidth * cosphi, w [i + 1].y, w [i + 1].z - myzoom * alphawidth * sinphi);
  }
  glEnd ();
  gl.disableAlphaBlending ();
}

#endif
