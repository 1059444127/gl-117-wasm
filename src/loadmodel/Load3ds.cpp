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

#ifndef IS_LOAD3DS_H

#include "Load3ds.h"
#include "logging/Logging.h"
#include "util/Util.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <cassert>



FILE *debugstream3ds = stdout;

/****************************************************************************
3DS LOADER
****************************************************************************/

// Contructor -> initialization
Load3ds::Load3ds ()
{
  currentChunk = new Chunk3ds;
  tempChunk = new Chunk3ds;
}

void Load3ds::setTextureDir (const std::string &texDir)
{
  textureDir = texDir;
}

// Load the whole 3DS file using the private functions below
bool Load3ds::import3ds (Model3d *model, const char *filename)
{
  char message [4096] = {0};

  file = new BinaryFile3ds (filename);

  // Read the first chuck
  readChunk(currentChunk);
  // Make sure this is a 3DS file
  if (currentChunk->ID != PRIMARY)
  {
    assert (false);
    DISPLAY_FATAL(FormatString ("Unable to load PRIMARY chuck from file: %s", filename));
    exit (EXIT_LOADFILE);
  }
  // Load objects recursively
  processNextChunk (model, currentChunk);
  // Merge equal vertices
  compile (model);
  // Calculate vertex normals
  computeNormals (model);
  // Load textures
  loadTextures (model);
  // Set vertex colors using textures
  computeColors (model);
  // Set vertices to [(-1,-1,-1);(1,1,1)]
  normalize (model);

  model->calcMissiles ();
  // Clean up
  cleanUp();

  return true;
}

void Load3ds::cleanUp ()
{
  delete file;
//  fclose (filepointer);            // Close the current file pointer
/* careful: memleak !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
  //delete currentChunk;            // Free the current chunk
  //delete tempChunk;              // Free our temporary chunk
}

void Load3ds::processNextChunk (Model3d *model, Chunk3ds *previousChunk)
{
  char version [10];
  char buf [4096];
  Object3d newObject;
  Material newTexture;

  currentChunk = new Chunk3ds;

  while (previousChunk->bytesRead < previousChunk->length)
  {
    readChunk(currentChunk);
    if (currentChunk->length > file->getSize ())
    {
      if (debug3ds)
      { fprintf (debugstream3ds, "ERROR=>cancel "); fflush (debugstream3ds); }
      return;
    }
    if (currentChunk->ID == 0 && currentChunk->length == 0)
      exit (1000);
    if (debug3ds)
    { fprintf (debugstream3ds, "%X: ", currentChunk->ID); fflush (debugstream3ds); }

    switch (currentChunk->ID)
    {
      case VERSION:
        currentChunk->bytesRead += file->readString (version, 10, currentChunk->length - currentChunk->bytesRead);
        if (version [0] > 0x03)
        {
          DISPLAY_WARN("This 3DS file is over version 3 so it may load incorrectly");
        }
        if (debug3ds) { fprintf (debugstream3ds, "Version %d\n", version [0]); fflush (debugstream3ds); }
        break;

      case OBJECTINFO:
        readChunk (tempChunk);
        if (tempChunk->ID == 15678)
        {
          tempChunk->bytesRead += file->readString (version, 10, tempChunk->length - tempChunk->bytesRead);
          currentChunk->bytesRead += tempChunk->bytesRead;
        }
        processNextChunk (model, currentChunk);
        break;

      case MATERIAL:
        model->addMaterial (newTexture);
        processNextMaterialChunk (model, currentChunk);
        break;

      case OBJECT:
        model->addObject (newObject);
        newObject.clear();
        currentChunk->bytesRead += getString (buf);
        model->object [model->numObjects - 1]->name = buf;
        processNextObjectChunk (model, (model->object [model->numObjects - 1]), currentChunk);
        break;

      case MAINSCALE:
        float mainscale;
        currentChunk->bytesRead += file->readFloat (&mainscale);
        model->scale = mainscale;
        currentChunk->bytesRead += file->skip (currentChunk->length - currentChunk->bytesRead);
        break;

      case EDITKEYFRAME:
        currentChunk->bytesRead += file->skip (currentChunk->length - currentChunk->bytesRead);
        break;

      default:
        currentChunk->bytesRead += file->skip (currentChunk->length - currentChunk->bytesRead);
        break;
    }

    previousChunk->bytesRead += currentChunk->bytesRead;
  }
  delete currentChunk;
  currentChunk = previousChunk;
}

void Load3ds::processNextObjectChunk (Model3d *model, Object3d *object, Chunk3ds *previousChunk)
{
  currentChunk = new Chunk3ds;

  while (previousChunk->bytesRead < previousChunk->length)
  {
    readChunk(currentChunk);
    if (debug3ds)
    { fprintf (debugstream3ds, "OBJ %X: ", currentChunk->ID); fflush (debugstream3ds); }

    switch (currentChunk->ID)
    {
      case OBJECT_MESH:
        processNextObjectChunk (model, object, currentChunk);
        break;

      case OBJECT_VERTICES:
        readVertices (object, currentChunk);
        break;

      case OBJECT_FACES: // triangles
        readVertexIndices (object, currentChunk);
        break;

      case OBJECT_MATERIAL:
        readObjectMaterial (model, object, currentChunk);
        break;

      case OBJECT_UV:
        readUVCoordinates (object, currentChunk);
        break;

      case OBJECT_MESHMATRIX:
        readMeshMatrix (object, currentChunk);
        break;

      default:
        currentChunk->bytesRead += file->skip (currentChunk->length - currentChunk->bytesRead);
        break;
    }

    previousChunk->bytesRead += currentChunk->bytesRead;
  }

  delete currentChunk;
  currentChunk = previousChunk;
}

void Load3ds::processNextMaterialChunk (Model3d *model, Chunk3ds *previousChunk)
{
  char buf [256];
  currentChunk = new Chunk3ds;

  while (previousChunk->bytesRead < previousChunk->length)
  {
    readChunk(currentChunk);
    if (debug3ds)
    { fprintf (debugstream3ds, "MAT %X: ", currentChunk->ID); fflush (debugstream3ds); }

    switch (currentChunk->ID)
    {
      case MAT_NAME:
        currentChunk->bytesRead += file->readString (buf, 255, currentChunk->length - currentChunk->bytesRead);
        model->material [model->numMaterials - 1]->name = buf;
        break;

      case MAT_DIFFUSE:
        readColorChunk (model->material [model->numMaterials - 1], currentChunk);
        break;

      case MAT_MAP:
        processNextMaterialChunk (model, currentChunk);
        break;

      case MAT_MAPFILE:
        currentChunk->bytesRead += file->readString (buf, 255, currentChunk->length - currentChunk->bytesRead);
        {
          char *str = buf;
          while (*str)
          {
            if (*str >= 'A' && *str <= 'Z')
              *str = tolower (*str);
            str++;
          }
        }
        model->material [model->numMaterials - 1]->filename = buf;
        break;

      case MAT_USCALE:
        readUScale (model->material [model->numMaterials - 1], currentChunk);
        break;

      case MAT_VSCALE:
        readVScale (model->material [model->numMaterials - 1], currentChunk);
        break;

      case MAT_UOFFSET:
        readUOffset (model->material [model->numMaterials - 1], currentChunk);
        break;

      case MAT_VOFFSET:
        readVOffset (model->material [model->numMaterials - 1], currentChunk);
        break;

      case MAT_ROTATION:
        readUVRotation (model->material [model->numMaterials - 1], currentChunk);
        break;

      default:
        currentChunk->bytesRead += file->skip (currentChunk->length - currentChunk->bytesRead);
        break;
    }

    previousChunk->bytesRead += currentChunk->bytesRead;
  }

  delete currentChunk;
  currentChunk = previousChunk;
}

void Load3ds::readChunk (Chunk3ds *pChunk)
{
  pChunk->bytesRead = file->readUInt16 (&pChunk->ID);
  pChunk->bytesRead += file->readUInt32 (&pChunk->length);
}

int Load3ds::getString (char *buffer)
{
  return file->readString (buffer);
}

void Load3ds::readColorChunk (Material *material, Chunk3ds *pChunk)
{
  readChunk (tempChunk);
  tempChunk->bytesRead += file->readString ((char *) material->color.c, 4, tempChunk->length - tempChunk->bytesRead);
  pChunk->bytesRead += tempChunk->bytesRead;
}

void Load3ds::readVertexIndices (Object3d *object, Chunk3ds *previousChunk)
{
  Uint16 index = 0;
  previousChunk->bytesRead += file->readUInt16 ((Uint16 *) &object->numTriangles);

  object->triangle = new Triangle [object->numTriangles];
  memset (object->triangle, 0, sizeof (Triangle) * object->numTriangles);

  for (int i = 0; i < object->numTriangles; i ++)
  {
    for (int j = 0; j < 4; j++)
    {
      previousChunk->bytesRead += file->readUInt16 (&index);
      if (j < 3)
      {
        object->triangle [i].v [j] = &object->vertex [index];
      }
    }
  }
}

void Load3ds::readMeshMatrix (Object3d *object, Chunk3ds *previousChunk)
{
  float matrix [12];
  previousChunk->bytesRead += file->readFloat ((float *) matrix, (previousChunk->length - previousChunk->bytesRead) / 4);
  // Where to put theses coords???
}

void Load3ds::readUScale (Material *material, Chunk3ds *previousChunk)
{
  previousChunk->bytesRead += file->readFloat (&material->uscale, 1);
  previousChunk->bytesRead += file->skip (currentChunk->length - currentChunk->bytesRead);
}

void Load3ds::readVScale (Material *material, Chunk3ds *previousChunk)
{
  previousChunk->bytesRead += file->readFloat (&material->vscale, 1);
  previousChunk->bytesRead += file->skip (currentChunk->length - currentChunk->bytesRead);
}

void Load3ds::readUOffset (Material *material, Chunk3ds *previousChunk)
{
  previousChunk->bytesRead += file->readFloat (&material->uoffset, 1);
  previousChunk->bytesRead += file->skip (currentChunk->length - currentChunk->bytesRead);
}

void Load3ds::readVOffset (Material *material, Chunk3ds *previousChunk)
{
  previousChunk->bytesRead += file->readFloat (&material->voffset, 1);
  previousChunk->bytesRead += file->skip (currentChunk->length - currentChunk->bytesRead);
}

void Load3ds::readUVRotation (Material *material, Chunk3ds *previousChunk)
{
  previousChunk->bytesRead += file->readFloat (&material->wrot, 1);
  previousChunk->bytesRead += file->skip (currentChunk->length - currentChunk->bytesRead);
}

void Load3ds::readUVCoordinates (Object3d *object, Chunk3ds *previousChunk)
{
  previousChunk->bytesRead += file->readUInt16 ((Uint16 *) &object->numTexVertex);

  Vector2 *p = new Vector2 [object->numTexVertex];

  previousChunk->bytesRead += file->readFloat ((float *) p, (previousChunk->length - previousChunk->bytesRead) / 4);

  for (int i = 0; i < object->numTexVertex; i ++)
    object->vertex [i].tex.set (p [i]);
  delete []p;
}

void Load3ds::readVertices (Object3d *object, Chunk3ds *previousChunk)
{
  int i;
  previousChunk->bytesRead += file->readUInt16 ((Uint16 *) &object->numVertices);

  object->vertex = new Vertex [object->numVertices];
  memset (object->vertex, 0, sizeof (Vertex) * object->numVertices);

  Vector3 *p = new Vector3 [object->numVertices];

  previousChunk->bytesRead += file->readFloat ((float *) p, (previousChunk->length - previousChunk->bytesRead) / 4);

  for (i = 0; i < object->numVertices; i ++)
  {
    object->vertex [i].vector.set (p [i]);
  }

  // Flip the y values with the z values as 3DMAX changed them and negate z
  for (i = 0; i < object->numVertices; i ++)
  {
    float fTempY = object->vertex [i].vector.y;
    object->vertex [i].vector.y = object->vertex [i].vector.z;
    object->vertex [i].vector.z = -fTempY;
  }
  delete []p;
}

void Load3ds::readObjectMaterial (Model3d *model, Object3d *object, Chunk3ds *previousChunk)
{
  char materialName [255] = {0};
  previousChunk->bytesRead += getString (materialName);

  for (int i = 0; i < model->numMaterials; i ++)
  {
    if (strcmp (materialName, model->material [i]->name.c_str ()) == 0)
    {
      object->material = model->material [i];
      if (model->material [i]->filename.length ())
        if ((model->material [i]->filename [0] >= 'A' && model->material [i]->filename [0] <= 'Z') ||
            (model->material [i]->filename [0] >= 'a' && model->material [i]->filename [0] <= 'z'))
        {
          object->hasTexture = true;
        }
      break;
    }
    else
    {
      object->material = NULL;
    }
  }

  currentChunk->bytesRead += file->skip (currentChunk->length - currentChunk->bytesRead);
}

void Load3ds::compile (Model3d *model)
{
  // Merge numerically equal vertices
  // This is necessary to get a smooth shaded object
  int i;
  for (i = 0; i < model->numObjects; i ++)
  {
    Object3d *co = model->object [i];
    for (int i2 = 1; i2 < co->numVertices; i2 ++)
    {
      for (int i3 = 0; i3 < i2; i3 ++)
      {
        if (co->vertex [i2].vector.isEqual (co->vertex [i3].vector) &&   // same coordinates
            co->vertex [i2].tex.isEqual (co->vertex [i3].tex))           // same texture coordinates
        {
          for (int i4 = 0; i4 < co->numTriangles; i4 ++)
          {
            if (co->triangle [i4].v [0] == &co->vertex [i2]) co->triangle [i4].v [0] = &co->vertex [i3];
            if (co->triangle [i4].v [1] == &co->vertex [i2]) co->triangle [i4].v [1] = &co->vertex [i3];
            if (co->triangle [i4].v [2] == &co->vertex [i2]) co->triangle [i4].v [2] = &co->vertex [i3];
          }
        }
      }
    }
  }

  // Scale texture coordinated by uscale, vscale
  for (i = 0; i < model->numObjects; i ++)
  {
    Object3d *co = model->object [i];
    if (co->material)
    {
      float uscale = co->material->uscale;
      float vscale = co->material->vscale;
      float uoffset = co->material->uoffset;
      float voffset = co->material->voffset;
      for (int i2 = 0; i2 < co->numVertices; i2 ++)
      {
        float ax = co->vertex [i2].tex.x - 0.5;
        float ay = co->vertex [i2].tex.y - 0.5;
        float phi = -co->material->wrot;
        co->vertex [i2].tex.x = ax * COS(phi) - ay * SIN(phi) + 0.5;
        co->vertex [i2].tex.y = ax * SIN(phi) + ay * COS(phi) + 0.5;
        co->vertex [i2].tex.x -= uoffset;
        co->vertex [i2].tex.y += voffset;
        co->vertex [i2].tex.x = (co->vertex [i2].tex.x - 0.5) * uscale + 0.5;
        co->vertex [i2].tex.y = (co->vertex [i2].tex.y - 0.5) * vscale + 0.5;
      }
    }
  }
}

void Load3ds::computeNormals (Model3d *model)
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
      for (i3 = 0; i3 < 3; i3 ++)
      {
        object->triangle [i2].v [i3]->addNormal (n);
      }
    }
    for (i2 = 0; i2 < object->numTriangles; i2 ++)
    {
      for (i3 = 0; i3 < 3; i3 ++)
      {
        object->triangle [i2].v [i3]->normal.norm ();
      }
    }
  }

}

void Load3ds::loadTextures (Model3d *model)
{
  int i, i2;
  char str [4096];
  for (i = 0; i < model->numObjects; i ++)
  {
    if (model->object [i]->hasTexture)
    {
      strcpy (str, textureDir.c_str ());
      strcat (str, model->object [i]->material->filename.c_str ());
      for (i2 = (int) strlen (str) - 1; i2 >= 0; i2 --)
      {
        if (i2 > 2 && str [i2] == '.')
        {
          str [i2+1] = 't';
          str [i2+2] = 'g';
          str [i2+3] = 'a';
          str [i2+4] = 0;
          break;
        }
      }

      model->object [i]->material->texture = new Texture (str, -1, true, false);
//      if (model->object [i]->material->texture == NULL)
//        model->object [i]->hasTexture = false;
    }
  }
}

void Load3ds::computeColors (Model3d *model)
{
  int i, i2;

  if (model->numObjects <= 0)
    return;

  Color c;
  for (i = 0; i < model->numObjects; i ++)
  {
    Object3d *object = (model->object [i]);
    for (i2 = 0; i2 < object->numVertices; i2 ++)
    {
      if (object->hasTexture)
      {
/*        Vertex *v = &object->vertex [i2];
        Texture *tex = object->material->texture;
        tex->getColor (&c, (int) (v->tex.x * tex->width), (int) (v->tex.y * tex->height));
        int val;
        if (c.c [0] < 200 || c.c [1] < 200)
        {
          val = c.c [0]; val = val * 2 / 3; c.c [0] = val;
          val = c.c [1]; val = val * 2 / 3; c.c [1] = val;
          val = c.c [2]; val = val * 2 / 3; c.c [2] = val;
        }
        v->color = c; */
      }
      else
      {
        if (object->material)
        {
          object->vertex [i2].color.c [0] = object->material->color.c [0];
          object->vertex [i2].color.c [1] = object->material->color.c [1];
          object->vertex [i2].color.c [2] = object->material->color.c [2];
        }
        else
        {
          object->vertex [i2].color.c [0] = 200;
          object->vertex [i2].color.c [1] = 200;
          object->vertex [i2].color.c [2] = 200;
        }
      }
    }
  }

}

void Load3ds::normalize (Model3d *model)
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

  float tlx = (maxx + minx) / 2.0;
  float tly = (maxy + miny) / 2.0;
  float tlz = (maxz + minz) / 2.0;
  model->vscale.x = (maxx - minx) * 0.5;
  model->vscale.y = (maxy - miny) * 0.5;
  model->vscale.z = (maxz - minz) * 0.5;
  float sc = model->vscale.x > model->vscale.y ? model->vscale.x : model->vscale.y;
  sc = model->vscale.z > sc ? model->vscale.z : sc;

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
}

#endif
