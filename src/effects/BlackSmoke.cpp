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



BlackSmoke::BlackSmoke (Space *space)
{
  this->space = space;
  ttl = 0;
  space->addObject (this);
  draw = false;
  trafo.scaling.x = 0.1;
  drawLight = false;
//  alpha = -1;
}

void BlackSmoke::setBlackSmoke (float x, float y, float z, float myphi, float maxzoom, int len)
{
  this->myphi = myphi;
  trafo.translation.set (x, y, z);
  this->maxzoom = maxzoom;
  ttl = len;
  maxlen = len;
  draw = true;
  trafo.scaling.x = 0;
}

void BlackSmoke::move (Uint32 dt, float camphi, float camgamma)
{
  this->camphi = camphi;
  if (ttl > 0)
  {
    trafo.scaling.x = maxzoom * (maxlen - ttl) / maxlen;
    trafo.translation.y += 0.04 * dt / timestep;
    ttl -= dt;
    if (ttl <= 0)
    {
      ttl = 0;
      draw = false;
    }
  }
}

void BlackSmoke::drawGL (Vector3 &tl, float alpha2, float lum2, bool drawLight2, bool istextured2)
{
  if (ttl <= 0 && !specialeffects)
    return;

  if (draw == 2 || frustum.isSphereInFrustum (tl.x + this->trafo.translation.x, tl.y + this->trafo.translation.y, tl.z + this->trafo.translation.z, this->trafo.scaling.x))
  {
    glDepthMask (GL_FALSE);
    if (antialiasing)
      gl.enableLinearTexture (texsmoke3->textureID, texsmoke3->mipmap);
    else
      gl.disableLinearTexture (texsmoke3->textureID, texsmoke3->mipmap);
    gl.enableAlphaBlending ();
    glEnable (GL_ALPHA_TEST);
    glAlphaFunc (GL_GEQUAL, 0.02);
    gl.enableTexture (texsmoke3->textureID);
    glBegin (GL_QUADS);
    int myalpha = 255 - (maxlen - ttl) * 255 / maxlen;
    if (myalpha > 255) myalpha = 255;
    glColor4ub (0, 0, 0, myalpha);
    float myzoom = trafo.translation.x;
    float cosphi = COS(camphi);
    float sinphi = SIN(camphi);
    glTexCoord2f (0, 0);
    glVertex3f (this->trafo.translation.x - myzoom * cosphi, this->trafo.translation.y + myzoom, this->trafo.translation.z + myzoom * sinphi);
    glTexCoord2f (1, 0);
    glVertex3f (this->trafo.translation.x + myzoom * cosphi, this->trafo.translation.y + myzoom, this->trafo.translation.z - myzoom * sinphi);
    glTexCoord2f (1, 1);
    glVertex3f (this->trafo.translation.x + myzoom * cosphi, this->trafo.translation.y - myzoom, this->trafo.translation.z - myzoom * sinphi);
    glTexCoord2f (0, 1);
    glVertex3f (this->trafo.translation.x - myzoom * cosphi, this->trafo.translation.y - myzoom, this->trafo.translation.z + myzoom * sinphi);
    glEnd ();
    glDisable (GL_TEXTURE_2D);
    glDisable (GL_ALPHA_TEST);
    gl.disableAlphaBlending ();
    glDepthMask (GL_TRUE);
  }
}

#endif
