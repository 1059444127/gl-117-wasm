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

#include "Model3d.h"



SpherePart::SpherePart ()
{
}

SpherePart::SpherePart (float radius, int segments, float phi)
{
  init (radius, segments, phi);
}

SpherePart::~SpherePart ()
{
}

void SpherePart::init (float radius, int segments, float phi)
{
  Object3d co;
  co.vertex = new Vertex [segments * 4 + 1];
  co.triangle = new Triangle [segments];
  co.quad = new Quad [segments * 3];
  this->radius = radius;
  this->segments = segments;
  float dx = 1, dy = 1, dz = 1;
  int p [4];
  float step = 360.0 / segments;
  float step2 = phi / 4;
  Vertex w;

  for (float i = 0; i < phi; i += step2)
    for (float i2 = 0; i2 < 360; i2 += step)
    {
      int a = ((int) i) % 360, b = ((int) i2) % 360;
      float si = SIN(a), ci = COS(a);
      float si2 = SIN(b), ci2 = COS(b);
      w.vector.x = radius * si * ci2 * dx; w.vector.y = radius * si * si2 * dy; w.vector.z = radius * ci * dz;
      p [0] = co.addVertex (w);
      a = ((int) (i + step2)) % 360;
      si = SIN(a); ci = COS(a);
      si2 = SIN(b); ci2 = COS(b);
      w.vector.x = radius * si * ci2 * dx; w.vector.y = radius * si * si2 * dy; w.vector.z = radius * ci * dz;
      if (a < 179 || i2 == 0) p [1] = co.addVertex (w);
      b = ((int) (i2 + step)) % 360;
      si = SIN(a); ci = COS(a);
      si2 = SIN(b); ci2 = COS(b);
      w.vector.x = radius * si * ci2 * dx; w.vector.y = radius * si * si2 * dy; w.vector.z = radius * ci * dz;
      if (a < 179) p [2] = co.addVertex (w);
      a = ((int) i) % 360;
      si = SIN(a); ci = COS(a);
      si2 = SIN(b); ci2 = COS(b);
      w.vector.x = radius * si * ci2 * dx; w.vector.y = radius * si * si2 * dy; w.vector.z = radius * ci * dz;
      p [3] = co.addVertex (w);
      if (i == 0 || i >= 180 - step2 - 0.2)
      {
        if (i == 0) co.triangle [co.numTriangles ++].setVertices (&co.vertex [p [0]], &co.vertex [p [1]], &co.vertex [p [2]]);
        else co.triangle [co.numTriangles ++].setVertices (&co.vertex [p [0]], &co.vertex [p [1]], &co.vertex [p [3]]);
      }
      else
      {
        co.quad [co.numQuads ++].setVertices (&co.vertex [p [0]], &co.vertex [p [1]], &co.vertex [p [2]], &co.vertex [p [3]]);
      }
    }

  addObject (co);
  setColor (Color (128, 128, 128, 255));
}

void SpherePart::setNorthPoleColor (const Color &c, float w)
{
  int i, i2;
  for (i = 0; i < 4; i ++)
    object [0]->vertex [0].color.c [i] = c.c [i];
  int num = (int) (w * segments * 4 / 2);
  num /= (segments * 2);
  num *= (segments * 2);
  for (i = 1; i <= num; i ++)
  {
    float weight = 1.0F - (float) ((int) ((i - 1) / (segments * 2)) * 4 * 2) / (float) num;
    for (i2 = 0; i2 < 4; i2 ++)
      object [0]->vertex [i].color.c [i2] = (short) ((1.0F - weight) * object [0]->vertex [i].color.c [i2] + weight * c.c [i2]);
  }
}

void SpherePart::setSouthPoleColor (const Color &c, float w)
{
}

void SpherePart::setPoleColor (int phi, int theta, const Color &c, float w)
{
}

#endif

