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

#include <math.h>
#include <stdlib.h>



Explosion::Explosion (Space *space, Model3d *sphere)
{
  this->space = space;
  o = sphere;
  ttl = 0;
  space->addObject (this);
  draw = false;
  trafo.scaling.x = 0.1;
  drawLight = false;
}

void Explosion::setExplosion (float x, float y, float z, float vx, float vy, float vz, float maxzoom, int len)
{
  trafo.translation.set (x, y, z);
  this->maxzoom = maxzoom;
  ttl = len;
  maxlen = len;
  draw = true;
  v.set (vx, vy, vz);
}

void Explosion::move (Uint32 dt, float camphi, float camgamma)
{
  if (ttl > 0)
  {
    float timefac = (float) dt / (float) timestep;
    trafo.scaling.x = SIN(ttl * 180 / maxlen) * maxzoom;
    ttl -= dt;
    trafo.translation.y += 0.01 * timefac;
    if (ttl <= 0)
    {
      ttl = 0;
      draw = false;
    }
    float brakepower = pow ((float) 0.98, (float) timefac);
    v.mul (brakepower);
    trafo.translation.x += v.x * timefac;
    trafo.translation.y += v.y * timefac;
    trafo.translation.z += v.z * timefac;
  }
}

#endif
