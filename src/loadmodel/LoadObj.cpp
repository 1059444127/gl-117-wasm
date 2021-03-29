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

#ifndef IS_LOADOBJ_H

#include "LoadObj.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>



LoadObj::LoadObj ()
{
}

void LoadObj::computeColors (Model3d *model)
{
  int i, i2;

  if (model->numObjects <= 0)
    return;

  Vector3 n;
  Vector3 light (1.0, 1.0, 1.0);
  light.norm ();
  for (i = 0; i < model->numObjects; i ++)
  {
    Object3d *object = (model->object [i]);
    for (i2 = 0; i2 < object->numVertices; i2 ++)
    {
      n.set (object->vertex [i2].normal);
      int lum = (int) (255.0 - 255.0 * acos (n.dotproduct (light)));
      object->vertex [i2].color.c [0] = lum;
      object->vertex [i2].color.c [1] = lum;
      object->vertex [i2].color.c [2] = lum;
    }
  }
}

void LoadObj::computeNormals (Model3d *model)
{
  int i, i2, i3;

  if (model->numObjects <= 0)
    return;

  Vector3 n;
  for (i = 0; i < model->numObjects; i ++)
  {
    Object3d *object = (model->object [i]);
    for (i2 = 0; i2 < object->numTriangles; i2 ++)
    {
      object->triangle [i2].calcNormal (&n);
      if (n.x == 0 && n.y == 0 && n.z == 0) n.z = 1;
      for (i3 = 0; i3 < 3; i3 ++)
      {
        object->triangle [i2].v [i3]->addNormal (n);
      }
    }
    for (i2 = 0; i2 < object->numQuads; i2 ++)
    {
      object->quad [i2].calcNormal (&n);
      for (i3 = 0; i3 < 4; i3 ++)
      {
        object->quad [i2].v [i3]->addNormal (n);
      }
    }
    for (i2 = 0; i2 < object->numTriangles; i2 ++)
    {
      for (i3 = 0; i3 < 3; i3 ++)
      {
        object->triangle [i2].v [i3]->normal.norm ();
      }
    }
    for (i2 = 0; i2 < object->numQuads; i2 ++)
    {
      for (i3 = 0; i3 < 4; i3 ++)
      {
        object->quad [i2].v [i3]->normal.norm ();
      }
    }
  }
}

void LoadObj::normalize (Model3d *model)
{
  int i, i2;
  float minx = 1E10, miny = 1E10, minz = 1E10;
  float maxx = -1E10, maxy = -1E10, maxz = -1E10;
  for (i = 0; i < model->numObjects; i ++)
  {
    Object3d *object = (model->object [i]);
    for (i2 = 0; i2 < object->numVertices; i2 ++)
    {
      Vertex *v = &object->vertex [i2];
      if (v->vector.x > maxx) maxx = v->vector.x;
      if (v->vector.y > maxy) maxy = v->vector.y;
      if (v->vector.z > maxz) maxz = v->vector.z;
      if (v->vector.x < minx) minx = v->vector.x;
      if (v->vector.y < miny) miny = v->vector.y;
      if (v->vector.z < minz) minz = v->vector.z;
    }
  }

  float tlx = (float) (maxx + minx) / 2.0F;
  float tly = (float) (maxy + miny) / 2.0F;
  float tlz = (float) (maxz + minz) / 2.0F;
  float scx = (float) (maxx - minx) * 0.5F;
  float scy = (float) (maxy - miny) * 0.5F;
  float scz = (float) (maxz - minz) * 0.5F;
  float sc = scx > scy ? scx : scy;
  sc = scz > sc ? scz : sc;

  for (i = 0; i < model->numObjects; i ++)
  {
    Object3d *object = (model->object [i]);
    for (i2 = 0; i2 < object->numVertices; i2 ++)
    {
      Vertex *v = &object->vertex [i2];
      v->vector.x -= tlx;
      v->vector.x /= sc;
      v->vector.y -= tly;
      v->vector.y /= sc;
      v->vector.z -= tlz;
      v->vector.z /= sc;
    }
  }

//  model->scale = sc;
}

bool LoadObj::importObj (Model3d *model, char *filename)
{
  TextFileObj *file;
  file = new TextFileObj (filename);
  int32 i = 0;
  int i2;
  int vertices = 0, faces = 0;

  // precalculate number of vertices and number of triangles
  while (i < file->size - 1)
  {
    // one vertex
    if (file->data [i] == 'v' && (file->data [i + 1] == ' ' || file->data [i + 1] == '\t'))
      vertices ++;
    // read number of vertices in this face
    if (file->data [i] == 'f' && (file->data [i + 1] == ' ' || file->data [i + 1] == '\t'))
    {
      file->filepointer = i + 1;
      file->skipWhite ();
      char *p;
      int z = 0;
      do
      {
        (void) file->readInteger ();
        file->nextWhite ();
        file->skipWhite ();
        p = file->getChar ();
        z ++;
      } while (*p >= '0' && *p <= '9');
      faces += (z - 2);
    }
    i ++;
  }

  // create new object
  model->object [model->numObjects] = new Object3d;
  Object3d *object = model->object [model->numObjects];
  model->numObjects ++;
  object->numVertices = vertices;
  object->numTriangles = faces;
  object->numQuads = 0;

  // create array of vertices and faces (triangles, quads)
  object->vertex = new Vertex [object->numVertices];
  object->triangle = new Triangle [object->numTriangles];
  object->quad = new Quad [object->numQuads];

  int vn = 0, tn = 0;
  i = 0;
  while (i < file->size - 1)
  {
    // read vertex
    if (file->data [i] == 'v' && (file->data [i + 1] == ' ' || file->data [i + 1] == '\t'))
    {
      object->vertex [vn].vector.x = file->readFloat (i + 2);
      object->vertex [vn].vector.y = file->readFloat ();
      object->vertex [vn].vector.z = file->readFloat ();
      i = file->filepointer - 2;
      vn ++;
    }
    // read face
    if (file->data [i] == 'f' && (file->data [i + 1] == ' ' || file->data [i + 1] == '\t'))
    {
      file->filepointer = i + 1;
      file->skipWhite ();
      char *p;
      int z = 0;
      int a [100];
      // read vertex indices
      do
      {
        a [z] = file->readInteger ();
        file->nextWhite ();
        file->skipWhite ();
        p = file->getChar ();
        z ++;
      } while (*p >= '0' && *p <= '9' && z < 100);
      // create triangles (triangulation)
      for (i2 = 2; i2 < z; i2 ++)
      {
        object->triangle [tn].v [0] = &object->vertex [a [0] - 1];
        object->triangle [tn].v [1] = &object->vertex [a [i2 - 1] - 1];
        object->triangle [tn].v [2] = &object->vertex [a [i2] - 1];
        tn ++;
      }
    }
    i ++;
  }

  delete file;

  computeNormals (model);
  computeColors (model);
  normalize (model);

  return true;
}

#endif
