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



Object3d::Object3d ()
  : name("")
{
  clear();
}

int Object3d::addVertex (const Vertex &w)
{
  int i;
  for (i = 0; i < numVertices; i ++)
    if (w.vector.isEqual (vertex [i].vector, 1e-3F) && w.color.isEqual (vertex [i].color))
      break;
  if (i == numVertices)
    vertex [numVertices ++].set (w);
//  return numVertices - 1;
  return i;
}

void Object3d::setColor (const Color &col)
{
  int i;
  for (i = 0; i < numVertices; i ++)
    memcpy (vertex [i].color.c, (const void *) &col, 4 * sizeof (unsigned char));
}

void Object3d::clear(void)
{
  // This is weird design. This is even a design BUG. After using the
  // copy constructor to make a shallow copy, ownership of the data
  // pointed to by material/vertex/... is definitively transferred to
  // the copy by a call to this method, which clears the pointers
  // without freeing the memory.
  numVertices = 0;
  numTriangles = 0;
  numQuads = 0;
  numTexVertex = 0;
  material = 0;
  hasTexture = false;
  vertex = 0;
  triangle = 0;
  quad = 0;
}

#endif

