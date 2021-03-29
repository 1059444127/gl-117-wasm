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
#include "opengl/GlPrimitives.h"



Model3d::Model3d ()
{
  numObjects = 0;
  numMaterials = 0;
  shading = 0;
/*  displaylist = true;
  list1 = -1;
  list2 = -1;
  list3 = -1;*/
  scale = 1.0F;
//  name = "";
  nolight = false;
  alpha = false;
/*  light_ambient [0] = 0.3; light_ambient [1] = 0.3; light_ambient [2] = 0.3; light_ambient [3] = 1;
  light_diffuse [0] = 0.9; light_diffuse [1] = 0.9; light_diffuse [2] = 0.9; light_diffuse [3] = 1;
  light_ambient2 [0] = 0.2; light_ambient2 [1] = 0.2; light_ambient2 [2] = 0.2; light_ambient2 [3] = 1;
  light_diffuse2 [0] = 0.1; light_diffuse2 [1] = 0.1; light_diffuse2 [2] = 0.1; light_diffuse [3] = 1;*/
  numRefpoints = 0;
  refpoint = NULL;
//  va = new VertexArray (VERTEXARRAY_V3N3C4T2);
}

/*void Model3d::setName (const std::string &name)
{
  this->name = name;
} */

void Model3d::addMaterial (const Material &material)
{
/*  if (numMaterials >= MaxMaterialsPerModel) exit (100);
  this->material [numMaterials] = new Material;
  if (this->material [numMaterials] == NULL) exit (100);
  memcpy (this->material [numMaterials], &material, sizeof (Material));*/
  numMaterials ++;
  this->material.push_back (new Material (material));
}

void Model3d::addObject (const Object3d &object)
{
/*  if (numObjects >= MaxObjectsPerModel) exit (101);
  this->object [numObjects] = new Object3d;
  if (this->object [numObjects] == NULL) exit (101);
  memcpy (this->object [numObjects], &object, sizeof (Object3d));*/
  numObjects ++;
  this->object.push_back (new Object3d (object));
  rotcol = 0;
}

void Model3d::addRefPoint (const Vector3 &tl)
{
  int i, i2;
  if (refpoint == NULL)
  {
    refpoint = new Vector3 [10];
  }
  for (i = 0; i < numRefpoints; i ++)
  {
    if (tl.z < refpoint [i].z)
    {
      for (i2 = numRefpoints; i2 > i; i2 --)
      {
        refpoint [i2].set (refpoint [i2 - 1]);
      }
      refpoint [i].set (tl);
      goto fertigref1;
    }
  }
  refpoint [numRefpoints].set (tl);
fertigref1:;
  numRefpoints ++;
}

Model3d::~Model3d ()
{
  int i;
  for (i = 0; i < numMaterials; i ++)
    delete material [i];
  for (i = 0; i < numObjects; i ++)
    delete object [i];
  if (refpoint)
  {
    delete refpoint;
  }
}

void Model3d::setColor (const Color &col)
{
  int i;
  for (i = 0; i < numObjects; i++)
  {
    object [i]->setColor (col);
  }
}

void Model3d::calcMissiles ()
{
  int i;
  Model3d *model = this;
  Vector3 tlmissile (0, 0.3, 0.3);
  for (i = 0; i < model->numObjects; i ++)
  {
    if (model->object [i]->numVertices == 4)
    {
      Object3d *o = model->object [i];
      float sumx = 0, sumz = 0;
      float maxy = 2;
      int i2;
      for (i2 = 0; i2 < o->numVertices; i2 ++)
      {
        sumx += o->vertex [i2].vector.x;
        if (o->vertex [i2].vector.y < maxy)
          maxy = o->vertex [i2].vector.y;
        sumz += o->vertex [i2].vector.z;
      }
      tlmissile.x = sumx / 4.0F;
      tlmissile.y = maxy;
      tlmissile.z = sumz / 4.0F;

      tlmissile.y = maxy;
      model->addRefPoint (tlmissile);
    }
  }
}

void Model3d::drawVertexNormals (const Object3d &cm, float zoom)
{
  glColor3ub (255, 0, 0);
  glBegin (GL_LINES);
  for (int j = 0; j < cm.numVertices; j++)
  if (cm.vertex [j].triangles > 0)
  {
    glVertex3f (cm.vertex [j].vector.x*zoom, cm.vertex [j].vector.y*zoom, cm.vertex [j].vector.z*zoom);
    glVertex3f ((cm.vertex [j].vector.x + cm.vertex [j].normal.x / 5)*zoom, (cm.vertex [j].vector.y + cm.vertex [j].normal.y / 5)*zoom, (cm.vertex [j].vector.z + cm.vertex [j].normal.z / 5)*zoom);
  }
  glEnd ();
}

int Model3d::rotateColor (int n)
{
  if (n == 0) return 0;
  rotcol ++;
  if (rotcol > n) rotcol = 0;
  return rotcol;
}

void Model3d::scaleTexture (float fx, float fy)
{
  int i;
  for (i = 0; i < numObjects; i ++)
  {
    Object3d *o = object [i];
    int i2;
    for (i2 = 0; i2 < o->numVertices; i2 ++)
    {
      o->vertex [i2].tex.x *= fx;
      o->vertex [i2].tex.y *= fy;
    }
  }
}

/*void Model3d::draw (const Vector3 &tl, const Vector3 &tl2, const Rotation &rot,
                    float zoom, float lum, int explode)
{
  if (nolight) // if model wants to be rendered without light, call draw2
  {
    glDisable (GL_LIGHTING);
    drawNoLight (tl, tl2, rot, zoom, explode);
    glEnable (GL_LIGHTING);
    return;
  }

  int i, j;
  Object3d *cm;
  float la [4] = { 0.2, 0.2, 0.2, 1.0};
  if (lum >= 1)
  {
    float addl = lum;
    if (addl >= 5) addl = 5;
    la [0] = 0.2 * addl;
    la [1] = 0.2 * addl;
    la [2] = 0.2 * addl;
  }
  glLightfv (GL_LIGHT0, GL_AMBIENT, la);

  float ld [4] = { 0.7, 0.7, 0.7, 1.0};
  if (lum != 1)
  {
    ld [0] *= lum;
    ld [1] *= lum;
    ld [2] *= lum;
    if (ld [0] > 1.0) ld [0] = 1.0;
    if (ld [1] > 1.0) ld [1] = 1.0;
    if (ld [2] > 1.0) ld [2] = 1.0;
  }
  glLightfv (GL_LIGHT0, GL_DIFFUSE, ld);

  for (i = 0; i < numObjects; i ++)
  {
    if (numObjects <= 0) break;
    cm = object [i];
    if (cm->hasTexture)
    {
      if (antialiasing)
        cm->material->texture->shadeLinear ();
      else
        cm->material->texture->shadeConst ();
    }
  }

  zoom *= scale;
  glPushMatrix ();
  glTranslatef (tl.x + tl2.x, tl.y + tl2.y - 0.002 * explode * explode / timestep / timestep, tl.z + tl2.z);
  float explodefac = (float) explode / 10 / timestep;

  if (showCollision)
  {
    glPushMatrix ();
    glScalef (cube.x, cube.y, cube.z);
    glColor3ub (255, 0, 0);
    glBegin (GL_LINE_STRIP);
    glVertex3f (1, 1, 1);
    glVertex3f (1, 1, -1);
    glVertex3f (1, -1, -1);
    glVertex3f (1, -1, 1);
    glVertex3f (1, 1, 1);
    glEnd ();
    glBegin (GL_LINE_STRIP);
    glVertex3f (-1, 1, 1);
    glVertex3f (-1, 1, -1);
    glVertex3f (-1, -1, -1);
    glVertex3f (-1, -1, 1);
    glVertex3f (-1, 1, 1);
    glEnd ();
    glBegin (GL_LINES);
    glVertex3f (1, 1, 1);
    glVertex3f (-1, 1, 1);
    glVertex3f (1, -1, -1);
    glVertex3f (-1, -1, -1);
    glVertex3f (1, -1, 1);
    glVertex3f (-1, -1, 1);
    glVertex3f (1, 1, -1);
    glVertex3f (-1, 1, -1);
    glEnd ();
    glPopMatrix ();
  }

  glRotatef (rot.phi+90, 0, -1, 0);
  glRotatef (-rot.gamma+90, 0, 0, 1);
  glRotatef (rot.theta+180, 1, 0, 0);
  glScalef (zoom, zoom, zoom);

  if (shading == 1)
    glShadeModel (GL_FLAT);
  else
    glShadeModel (GL_SMOOTH);

  if (alpha)
  { glEnable (GL_BLEND); glEnable (GL_ALPHA_TEST); glAlphaFunc (GL_GEQUAL, 0.2); }

  for (i = 0; i < numObjects; i ++)
  {
    if (numObjects <= 0) break;
    cm = object [i];
    if (cm->hasTexture)
    {
      glEnable (GL_TEXTURE_2D);
      glColor4f (1, 1, 1, 1);
      glBindTexture (GL_TEXTURE_2D, cm->material->texture->textureID);
    }
    else
    {
      glDisable (GL_TEXTURE_2D);
      glColor4f (1, 1, 1, 1);
    }

    if (cm->material != NULL)
    {
      if (cm->material->color.c [0] > 190 && cm->material->color.c [1] > 190 && cm->material->color.c [2] < 20)
        glDisable (GL_LIGHTING);
      else
        glEnable (GL_LIGHTING);
    }

    Vector3 shift;

    va->glBegin (GL_TRIANGLES);
    for (j = 0; j < cm->numTriangles; j++)
    {
      Vertex *v = cm->triangle [j].v [0];
      if (explode > 0)
      {
        shift.x = v->normal.x * explodefac;
        shift.y = v->normal.y * explodefac;
        shift.z = v->normal.z * explodefac;
      }
      for (int whichVertex = 0; whichVertex < 3; whichVertex ++)
      {
        v = cm->triangle [j].v [whichVertex];
        va->glNormal3f (v->normal.x, v->normal.y, v->normal.z);
        if (cm->hasTexture)
        {
          if (cm->vertex)
          {
            va->glTexCoord2f (v->tex.x, v->tex.y);
            va->glColor4f (1, 1, 1, 1);
          }
        }
        else
        {
          if (numMaterials && cm->material)
          {
            unsigned char *color = cm->material->color.c;
            if (color [0] > 190 && color [1] > 190 && color [2] < 20)
            {
              rotateColor (30);
              va->glColor4ub (color [0] + rotcol, color [1] + rotcol, color [2] + rotcol * 3, 255);
            }
            else
            {
              va->glColor4ub (color [0], color [1], color [2], color [3]);
            }
          }
        }
  //    glColor3ub (255, 255, 0);
        va->glVertex3f (v->vector.x + shift.x, v->vector.y + shift.y, v->vector.z + shift.z);
      }
    }
    va->glEnd ();

    va->glBegin (GL_QUADS);
    for (j = 0; j < cm->numQuads; j++)
    {
      Vertex *v = cm->quad [j].v [0];
      if (explode > 0)
      {
        shift.x = v->normal.x * explodefac;
        shift.y = v->normal.y * explodefac;
        shift.z = v->normal.z * explodefac;
      }
      for (int whichVertex = 0; whichVertex < 4; whichVertex++)
      {
        v = cm->quad [j].v [whichVertex];
        va->glNormal3f (v->normal.x, v->normal.y, v->normal.z);
        if (cm->hasTexture)
        {
          if (cm->vertex)
          {
            va->glTexCoord2f (v->tex.x, v->tex.y);
            va->glColor4f (1, 1, 1, 1);
          }
        }
        else
        {
          if (numMaterials && cm->material && cm->material->texture->textureID >= 0)
          {
            unsigned char *pColor = material [cm->material->texture->textureID]->color.c;
            va->glColor4ub (pColor [0], pColor [1], pColor [2], pColor [3]);
          }
        }
        va->glVertex3f (v->vector.x + shift.x, v->vector.y + shift.y, v->vector.z + shift.z);
      }
    }
    va->glEnd ();
  }

  if (alpha)
  { glDisable (GL_BLEND); glDisable (GL_ALPHA_TEST); }

  glPopMatrix ();
}

void Model3d::drawNoLight (const Vector3 &tl, const Vector3 &tl2, const Rotation &rot,
                          float zoom, int explode)
{
  int i, j;
  Object3d *cm;

  for (i = 0; i < numObjects; i ++)
  {
    if (numObjects <= 0) break;
    cm = object [i];
    if (cm->hasTexture)
    {
      if (antialiasing)
        cm->material->texture->shadeLinear ();
      else
        cm->material->texture->shadeConst ();
    }
  }

  zoom *= scale;
  glPushMatrix ();
  glTranslatef (tl.x + tl2.x, tl.y + tl2.y - 0.002 * explode * explode / timestep / timestep, tl.z + tl2.z);
  float explodefac = (float) explode / 10 / timestep;

  if (showCollision)
  {
    glPushMatrix ();
    glScalef (cube.x, cube.y, cube.z);
    glColor3ub (255, 0, 0);
    glBegin (GL_LINE_STRIP);
    glVertex3f (1, 1, 1);
    glVertex3f (1, 1, -1);
    glVertex3f (1, -1, -1);
    glVertex3f (1, -1, 1);
    glVertex3f (1, 1, 1);
    glEnd ();
    glBegin (GL_LINE_STRIP);
    glVertex3f (-1, 1, 1);
    glVertex3f (-1, 1, -1);
    glVertex3f (-1, -1, -1);
    glVertex3f (-1, -1, 1);
    glVertex3f (-1, 1, 1);
    glEnd ();
    glBegin (GL_LINES);
    glVertex3f (1, 1, 1);
    glVertex3f (-1, 1, 1);
    glVertex3f (1, -1, -1);
    glVertex3f (-1, -1, -1);
    glVertex3f (1, -1, 1);
    glVertex3f (-1, -1, 1);
    glVertex3f (1, 1, -1);
    glVertex3f (-1, 1, -1);
    glEnd ();
    glPopMatrix ();
  }

  glRotatef (rot.phi+90, 0, -1, 0);
  glRotatef (-rot.gamma+90, 0, 0, 1);
  glRotatef (rot.theta+180, 1, 0, 0);
  glScalef (zoom, zoom, zoom);

  bool listgen = false;
  if (list2 == -1 && explode <= 0 && displaylist)
  {
    listgen = true;
    list2 = gl.createList ();
  }
  if (listgen || explode > 0 || !displaylist)
  {

    if (shading == 1)
      glShadeModel (GL_FLAT);
    else
      glShadeModel (GL_SMOOTH);

    if (alpha)
    { glEnable (GL_BLEND); glEnable (GL_ALPHA_TEST); glAlphaFunc (GL_GEQUAL, 0.01); }

    for (i = 0; i < numObjects; i++)
    {
      if (numObjects <= 0) break;
      cm = object [i];
      if (cm->hasTexture)
      {
        glEnable (GL_TEXTURE_2D);
        glColor4ub (255, 255, 255, 255);
        glBindTexture (GL_TEXTURE_2D, cm->material->texture->textureID);
      }
      else
      {
        glDisable (GL_TEXTURE_2D);
        glColor4ub (255, 255, 255, 255);
      }

      Vector3 shift;

      va->glBegin (GL_TRIANGLES);
      for (j = 0; j < cm->numTriangles; j++)
      {
        Vertex *v = cm->triangle [j].v [0];
        if (explode > 0)
        {
          shift.x = v->normal.x * explodefac;
          shift.y = v->normal.y * explodefac;
          shift.z = v->normal.z * explodefac;
        }
        for (int whichVertex = 0; whichVertex < 3; whichVertex ++)
        {
          v = cm->triangle [j].v [whichVertex];
          va->glNormal3f (v->normal.x, v->normal.y, v->normal.z);
          if (cm->hasTexture)
          {
            if (cm->vertex)
            {
              va->glTexCoord2f (v->tex.x, v->tex.y);
              va->glColor4f (1, 1, 1, 1);
            }
          }
          else
          {
            unsigned char *pColor = v->color.c;
            va->glColor4ub (pColor [0], pColor [1], pColor [2], pColor [3]);
          }
          va->glVertex3f (v->vector.x + shift.x, v->vector.y + shift.y, v->vector.z + shift.z);
        }
      }
      va->glEnd ();

      va->glBegin (GL_QUADS);
      for (j = 0; j < cm->numQuads; j++)
      {
        Vertex *v = cm->quad [j].v [0];
        if (explode > 0)
        {
          shift.x = v->normal.x * explodefac;
          shift.y = v->normal.y * explodefac;
          shift.z = v->normal.z * explodefac;
        }
        for (int whichVertex = 0; whichVertex < 4; whichVertex ++)
        {
          v = cm->quad [j].v [whichVertex];
          va->glNormal3f (v->normal.x, v->normal.y, v->normal.z);
          if (cm->hasTexture)
          {
            if (cm->vertex)
            {
              va->glTexCoord2f (v->tex.x, v->tex.y);
              va->glColor4f (1, 1, 1, 1);
            }
          }
          else
          {
            unsigned char *pColor = v->color.c;
            va->glColor4ub (pColor [0], pColor [1], pColor [2], pColor [3]);
          }
          va->glVertex3f (v->vector.x + shift.x, v->vector.y + shift.y, v->vector.z + shift.z);
        }
      }
      va->glEnd ();
    }

    if (alpha)
    { glDisable (GL_BLEND); glDisable (GL_ALPHA_TEST); }

    if (listgen) glEndList ();
  }
  else glCallList (list2);

  glPopMatrix ();
}

void Model3d::drawNoTexture (const Vector3 &tl, const Vector3 &tl2, const Rotation &rot,
                            float zoom, float lum, int explode)
{
  int i, j;
  Object3d *cm;
//  float mx=0, my=0, mz=0, ix=0, iy=0, iz=0;
  zoom *= scale;
  glPushMatrix ();
  glTranslatef (tl.x + tl2.x, tl.y + tl2.y - 0.002 * explode * explode / timestep / timestep, tl.z + tl2.z);
  float explodefac = (float) explode / 10 / timestep;

  if (showCollision)
  {
    glPushMatrix ();
    glScalef (cube.x, cube.y, cube.z);
    glColor3ub (255, 0, 0);
    glBegin (GL_LINE_STRIP);
    glVertex3f (1, 1, 1);
    glVertex3f (1, 1, -1);
    glVertex3f (1, -1, -1);
    glVertex3f (1, -1, 1);
    glVertex3f (1, 1, 1);
    glEnd ();
    glBegin (GL_LINE_STRIP);
    glVertex3f (-1, 1, 1);
    glVertex3f (-1, 1, -1);
    glVertex3f (-1, -1, -1);
    glVertex3f (-1, -1, 1);
    glVertex3f (-1, 1, 1);
    glEnd ();
    glBegin (GL_LINES);
    glVertex3f (1, 1, 1);
    glVertex3f (-1, 1, 1);
    glVertex3f (1, -1, -1);
    glVertex3f (-1, -1, -1);
    glVertex3f (1, -1, 1);
    glVertex3f (-1, -1, 1);
    glVertex3f (1, 1, -1);
    glVertex3f (-1, 1, -1);
    glEnd ();
    glPopMatrix ();
  }

  glRotatef (rot.phi+90, 0, -1, 0);
  glRotatef (-rot.gamma+90, 0, 0, 1);
  glRotatef (rot.theta+180, 1, 0, 0);
  glScalef (zoom, zoom, zoom);

  if (alpha)
  { glEnable (GL_BLEND); glEnable (GL_ALPHA_TEST); glAlphaFunc (GL_GEQUAL, 0.2); }

  for (i = 0; i < numObjects; i++)
  {
    if (numObjects <= 0) break;
    cm = object [i];
    glDisable (GL_TEXTURE_2D);
    glColor3ub (255, 255, 255);

    Vector3 shift;

    va->glBegin (GL_TRIANGLES);
    for (j = 0; j < cm->numTriangles; j++)
    {
      Vertex *v = cm->triangle [j].v [0];
      if (explode > 0)
      {
        shift.x = v->normal.x * explodefac;
        shift.y = v->normal.y * explodefac;
        shift.z = v->normal.z * explodefac;
      }
      for (int whichVertex = 0; whichVertex < 3; whichVertex ++)
      {
        v = cm->triangle [j].v [whichVertex];
  //      glNormal3f (v->normal.x, v->normal.y, v->normal.z);
        unsigned char *pColor = v->color.c;
        float red = lum * pColor [0] / 256;
        float green = lum * pColor [1] / 256;
        float blue = lum * pColor [2] / 256;
        if (red >= 1.0) red = 1.0;
        if (green >= 1.0) green = 1.0;
        if (blue >= 1.0) blue = 1.0;
        va->glColor3f (red, green, blue);
        va->glVertex3f (v->vector.x + shift.x, v->vector.y + shift.y, v->vector.z + shift.z);
      }
    }
    va->glEnd ();

    va->glBegin (GL_QUADS);
    for (j = 0; j < cm->numQuads; j++)
    {
      Vertex *v = cm->quad [j].v [0];
      if (explode > 0)
      {
        shift.x = v->normal.x * explodefac;
        shift.y = v->normal.y * explodefac;
        shift.z = v->normal.z * explodefac;
      }
      for (int whichVertex = 0; whichVertex < 4; whichVertex ++)
      {
        v = cm->quad [j].v [whichVertex];
  //      glNormal3f (v->normal.x, v->normal.y, v->normal.z);
        unsigned char *pColor = v->color.c;
        float red = lum * pColor [0] / 256;
        float green = lum * pColor [1] / 256;
        float blue = lum * pColor [2] / 256;
        if (red >= 1.0) red = 1.0;
        if (green >= 1.0) green = 1.0;
        if (blue >= 1.0) blue = 1.0;
        va->glColor3f (red, green, blue);
        va->glVertex3f (v->vector.x + shift.x, v->vector.y + shift.y, v->vector.z + shift.z);
      }
    }
    va->glEnd ();

  }

  if (alpha)
  { glDisable (GL_BLEND); glDisable (GL_ALPHA_TEST); }

  glPopMatrix ();
}*/

/*void Model3d::draw3 (const Vector3 &tl, const Vector3 &tl2, const Rotation &rot,
					float zoom, int explode)
{
  int i, j;
  Object3d *cm;
//  float mx=0, my=0, mz=0, ix=0, iy=0, iz=0;
  zoom *= scale;
  glPushMatrix ();
  glTranslatef (tl.x + tl2.x, tl.y + tl2.y - 0.002 * explode * explode / timestep / timestep, tl.z + tl2.z);
  float explodefac = (float) explode / 10 / timestep;

  if (showcollision)
  {
    glPushMatrix ();
    glScalef (cube.x, cube.y, cube.z);
    glColor3ub (255, 0, 0);
    glBegin (GL_LINE_STRIP);
    glVertex3f (1, 1, 1);
    glVertex3f (1, 1, -1);
    glVertex3f (1, -1, -1);
    glVertex3f (1, -1, 1);
    glVertex3f (1, 1, 1);
    glEnd ();
    glBegin (GL_LINE_STRIP);
    glVertex3f (-1, 1, 1);
    glVertex3f (-1, 1, -1);
    glVertex3f (-1, -1, -1);
    glVertex3f (-1, -1, 1);
    glVertex3f (-1, 1, 1);
    glEnd ();
    glBegin (GL_LINES);
    glVertex3f (1, 1, 1);
    glVertex3f (-1, 1, 1);
    glVertex3f (1, -1, -1);
    glVertex3f (-1, -1, -1);
    glVertex3f (1, -1, 1);
    glVertex3f (-1, -1, 1);
    glVertex3f (1, 1, -1);
    glVertex3f (-1, 1, -1);
    glEnd ();
    glPopMatrix ();
  }

  glRotatef (rot.c+90, 0, -1, 0);
  glRotatef (-rot.a+90, 0, 0, 1);
  glRotatef (rot.b+180, 1, 0, 0);
  glScalef (zoom, zoom, zoom);

  bool listgen = false;
  if (list3 == -1 && explode <= 0 && displaylist)
  {
    listgen = true;
    list3 = gl->createList ();
  }
  if (listgen || explode > 0 || !displaylist)
  {

    if (shading == 1)
      glShadeModel (GL_FLAT);
    else
      glShadeModel (GL_SMOOTH);

    if (alpha)
    { glEnable (GL_BLEND); glEnable (GL_ALPHA_TEST); glAlphaFunc (GL_GEQUAL, 0.2); }

    for (i = 0; i < numObjects; i++)
    {
      if (numObjects <= 0) break;
      cm = object [i];
      glDisable (GL_TEXTURE_2D);
      glColor3ub (255, 255, 255);

      Vector3 shift;

      va->glBegin (GL_TRIANGLES);
      for (j = 0; j < cm->numTriangles; j++)
      {
        Vertex *v = cm->triangle [j].v [0];
        if (explode > 0)
        {
          shift.x = v->normal.x * explodefac;
          shift.y = v->normal.y * explodefac;
          shift.z = v->normal.z * explodefac;
        }
        for (int whichVertex = 0; whichVertex < 3; whichVertex ++)
        {
          v = cm->triangle [j].v [whichVertex];
          va->glNormal3f (v->normal.x, v->normal.y, v->normal.z);
          if (cm->hasTexture && false)
          {
            if (cm->vertex)
            {
              va->glTexCoord2f (v->tex.x, v->tex.y);
              va->glColor4f (1, 1, 1, 1);
            }
          }
          else
          {
            unsigned char *pColor = v->color.c;
            va->glColor3ub (pColor[0], pColor[1], pColor[2]);
          }
          va->glVertex3f (v->vector.x + shift.x, v->vector.y + shift.y, v->vector.z + shift.z);
        }
      }
      va->glEnd ();

      va->glBegin (GL_QUADS);
      for (j = 0; j < cm->numQuads; j++)
      {
        Vertex *v = cm->quad [j].v [0];
        if (explode > 0)
        {
          shift.x = v->normal.x * explodefac;
          shift.y = v->normal.y * explodefac;
          shift.z = v->normal.z * explodefac;
        }
        for (int whichVertex = 0; whichVertex < 4; whichVertex ++)
        {
          v = cm->quad [j].v [whichVertex];
          va->glNormal3f (v->normal.x, v->normal.y, v->normal.z);
          if (cm->hasTexture && false)
          {
            if (cm->vertex)
            {
              va->glTexCoord2f (v->tex.x, v->tex.y);
              va->glColor4f (1, 1, 1, 1);
            }
          }
          else
          {
            unsigned char *pColor = v->color.c;
            va->glColor3ub (pColor[0], pColor[1], pColor[2]);
          }
          va->glVertex3f (v->vector.x + shift.x, v->vector.y + shift.y, v->vector.z + shift.z);
        }
      }
      va->glEnd ();
    }

    if (alpha)
    { glDisable (GL_BLEND); glDisable (GL_ALPHA_TEST); }

    if (listgen) glEndList ();
  }
  else glCallList (list3);

  glPopMatrix ();
}*/

#endif

