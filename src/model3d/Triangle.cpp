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



Triangle::Triangle ()
{
  v [0] = NULL;
  v [1] = NULL;
  v [2] = NULL;
}

void Triangle::calcNormal (Vector3 *n)
{
  Vector3 dummy;
  n->set (v [1]->vector);
  n->sub (v [0]->vector);
  dummy.set (v [2]->vector);
  dummy.sub (v [0]->vector);
  n->crossproduct (dummy);
}

void Triangle::setVertices (Vertex *a, Vertex *b, Vertex *c)
{
  int i;
  Vector3 dummy;
  v [0] = a;
  v [1] = b;
  v [2] = c;
  calcNormal (&dummy);
  dummy.norm ();
  if (dummy.z > 0)
    dummy.neg ();
  for (i = 0; i < 3; i ++)
  {
    v [i]->addNormal (dummy);
  }
}

#endif

