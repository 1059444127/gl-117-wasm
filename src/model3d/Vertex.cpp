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



Vertex::Vertex ()
{
  triangles = 0;
}

Vertex::Vertex (const Vertex &v)
{
  set (v);
}

void Vertex::set (const Vertex &v)
{
  vector.set (v.vector);
  normal.set (v.normal);
  color.set (v.color);
  triangles = v.triangles;
}

void Vertex::addNormal (const Vector3 &n)
{
  triangles ++;
  normal.x = (normal.x * (triangles - 1) + n.x) / (float) triangles;
  normal.y = (normal.y * (triangles - 1) + n.y) / (float) triangles;
  normal.z = (normal.z * (triangles - 1) + n.z) / (float) triangles;
}

void Vertex::addColor (const Color &color)
{
  triangles ++;
  for (int i = 0; i < 4; i ++)
  {
    this->color.c [i] = (unsigned char) (((float) this->color.c [i] * (triangles - 1)
                                         + color.c [i]) / (float) triangles);
  }
}

#endif

