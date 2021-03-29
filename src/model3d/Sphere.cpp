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



Sphere::Sphere ()
{
}

Sphere::Sphere (float radius, int segments, float dx, float dy, float dz)
{
  init (radius, segments, dx, dy, dz, 0);
}

Sphere::~Sphere ()
{
}

int Sphere::random (int n)
{
  if (n == 0) return 0;
  return rand () % n;
}

void Sphere::init (float radius, int segments, float dx, float dy, float dz, int randomized)
{
  Object3d co;
  co.vertex = new Vertex [segments * segments * 2 + 2];
  co.triangle = new Triangle [segments * 4];
  co.quad = new Quad [segments * segments * 2];

  this->radius = radius;
  this->segments = segments;
  this->dx = dx;
  this->dy = dy;
  this->dz = dz;

  int p [4];
  float step = 180.0 / segments;
  Vertex w;

  for (float i = 0; i < 180; i += step)
    for (float i2 = 0; i2 < 360; i2 += step)
    {
      int a = ((int) i) % 360, b = ((int) i2) % 360;
      float si = SIN(a), ci = COS(a);
      float si2 = SIN(b), ci2 = COS(b);
      w.vector.x = radius * si * ci2 * dx; w.vector.y = radius * si * si2 * dy; w.vector.z = radius * ci * dz;
      p [0] = co.addVertex (w);
      a = ((int) (i + step)) % 360;
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
      if (i == 0 || i >= 180 - step - 1)
      {
        if (!random (randomized))
        {
          if (i == 0) co.triangle [co.numTriangles ++].setVertices (&co.vertex [p [0]], &co.vertex [p [1]], &co.vertex [p [2]]);
          else co.triangle [co.numTriangles ++].setVertices (&co.vertex [p [0]], &co.vertex [p [1]], &co.vertex [p [3]]);
        }
      }
      else
      {
        if (!random (randomized))
          co.quad [co.numQuads ++].setVertices (&co.vertex [p [0]], &co.vertex [p [1]], &co.vertex [p [2]], &co.vertex [p [3]]);
      }
    }

  addObject (co);
  setColor (Color (128, 128, 128, 255));
  for (int i2 = 0; i2 < object [0]->numVertices / 2; i2 ++)
  {
    object [0]->vertex [i2].normal.neg ();
  }
}

void Sphere::invertNormals ()
{
  for (int i = 0; i < object [0]->numVertices; i ++)
  {
    object [0]->vertex [i].normal.neg ();
  }
}

void Sphere::setNorthPoleColor (const Color &c, float w)
{
  int i, i2;
  for (i = 0; i < 4; i ++)
    object [0]->vertex [0].color.c [i] = c.c [i];
  int num = (int) (w * segments * segments / 2);
  num /= (segments * 2);
  num *= (segments * 2);
  for (i = 1; i <= num; i ++)
  {
    float weight = 1.0F - (float) ((int) ((i - 1) / (segments * 2)) * segments * 2) / (float) num;
    for (i2 = 0; i2 < 4; i2 ++)
      object [0]->vertex [i].color.c [i2] = (short) ((1.0F - weight) * object [0]->vertex [i].color.c [i2] + weight * c.c [i2]);
  }
}

void Sphere::setSouthPoleColor (const Color &c, float w)
{
  int i, i2;
  int max = (segments - 1) * segments * 2 + 1;
  for (i = 0; i < 4; i ++)
    object [0]->vertex [max].color.c [i] = c.c [i];
  int num = (int) (w * segments * segments / 2);
  num /= (segments * 2);
  num *= (segments * 2);
  for (i = 1; i <= num; i ++)
  {
    float weight = 1.0F - (float) ((int) ((i - 1) / (segments * 2)) * segments * 2) / (float) num;
    for (i2 = 0; i2 < 4; i2 ++)
      object [0]->vertex [max - i].color.c [i] = (short) ((1.0F - weight) * object [0]->vertex [max - i].color.c [i2] + weight * c.c [i2]);
  }
}

void Sphere::setPoleColor (int phi, int theta, const Color &c, float w)
{
  int i, i2;
  for (i = 0; i < object [0]->numVertices; i ++)
  {
    int phi2 = ((i - 1) % (segments * 2)) * 360 / (segments * 2);
    int theta2 = ((i - 1) / (segments * 2) + 1) * 360 / (segments * 2);
    if (i == 0) { theta2 = 0; phi2 = 0; }
    int dphi = phi - phi2;
    if (dphi < -180) dphi += 360;
    else if (dphi > 180) dphi -= 360;
    int dtheta = theta - theta2;
    if (dtheta < -180) dtheta += 360;
    else if (dtheta > 180) dtheta -= 360;
    float alpha = sqrt ((float) (dphi*dphi+dtheta*dtheta));
    if (alpha < 180 * w)
    {
      float weight = 1.0 - alpha / 180.0 / w;
      for (i2 = 0; i2 < 4; i2 ++)
        object [0]->vertex [i].color.c [i2] = (short) ((1.0F - weight) * object [0]->vertex [i].color.c [i2] + weight * c.c [i2]);
    }
  }
}

#endif

