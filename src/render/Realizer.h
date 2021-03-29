/*
    GL-117
    Copyright 2001-2005 Thomas A. Drexl aka heptargon

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

#ifndef IS_REALIZER_H
#define IS_REALIZER_H

#include <string>

#include "model3d/Model3d.h"

class Model3dRealizer
{
  public:

    static const bool showCollision = false;

    void draw (const Model3d &model, const Transformation &trafo,
               float lum, int explode)
    {
      Vector3 tl2;
      Vector3 tl(trafo.translation);
      Rotation rot(trafo.rotation);
      float zoom = trafo.scaling.x;

      VertexArray va1 (VERTEXARRAY_V3N3C4T2);
      VertexArray va2 (VERTEXARRAY_V3N3C4);
      VertexArray va3 (VERTEXARRAY_V3N3C4);
      va2._glBegin (GL_TRIANGLES);
      va3._glBegin (GL_QUADS);
      VertexArray *va = &va1;

      if (model.nolight) // if model wants to be rendered without light, call draw2
      {
        glDisable (GL_LIGHTING);
        drawNoLight (model, trafo, explode);
        glEnable (GL_LIGHTING);
        return;
      }

      int i, j;
      Object3d *cm;
      float la [4] = { 0.2, 0.2, 0.2, 1.0};
      if (lum > 1)
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

      for (i = 0; i < model.numObjects; i ++)
      {
        if (model.numObjects <= 0) break;
        cm = model.object [i];
        if (cm->hasTexture)
        {
          if (antialiasing)
            gl.enableLinearTexture (cm->material->texture->textureID, cm->material->texture->mipmap);
          else
            gl.disableLinearTexture (cm->material->texture->textureID, cm->material->texture->mipmap);
        }
      }

      zoom *= model.scale;
      glPushMatrix ();
      glTranslatef (tl.x + tl2.x, tl.y + tl2.y - 0.002 * explode * explode / timestep / timestep, tl.z + tl2.z);
      float explodefac = (float) explode / 10 / timestep;

      if (showCollision)
      {
        glPushMatrix ();
        glScalef (model.cube.x, model.cube.y, model.cube.z);
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

      if (model.shading == 1)
        glShadeModel (GL_FLAT);
      else
        glShadeModel (GL_SMOOTH);

      if (model.alpha)
      { glEnable (GL_BLEND); glEnable (GL_ALPHA_TEST); glAlphaFunc (GL_GEQUAL, 0.2); }

      for (i = 0; i < model.numObjects; i ++)
      {
        if (model.numObjects <= 0) break;
        cm = model.object [i];
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

        va->_glBegin (GL_TRIANGLES);
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
            va->_glNormal3f (v->normal.x, v->normal.y, v->normal.z);
//            va2.glNormal3f (v->normal.x, v->normal.y, v->normal.z);
            if (cm->hasTexture)
            {
              if (cm->vertex)
              {
                va->_glTexCoord2f (v->tex.x, v->tex.y);
                va->_glColor4f (1, 1, 1, 1);
//                va2.glColor4f (1, 1, 1, 0.5);
              }
            }
            else
            {
              if (model.numMaterials && cm->material)
              {
                unsigned char *color = cm->material->color.c;
                int rotcol = 0;
                if (color [0] > 190 && color [1] > 190 && color [2] < 20)
                {
    //              model.rotateColor (30); // TODO: rotate
                  va->_glColor4ub (color [0] + rotcol, color [1] + rotcol, color [2] + rotcol * 3, 255);
//                  va2.glColor4ub (color [0] + rotcol, color [1] + rotcol, color [2] + rotcol * 3, 255);
                }
                else
                {
                  va->_glColor4ub (color [0], color [1], color [2], color [3]);
//                  va2.glColor4ub (color [0], color [1], color [2], 128);
                }
              }
            }
      //    glColor3ub (255, 255, 0);
            va->_glVertex3f (v->vector.x + shift.x, v->vector.y + shift.y, v->vector.z + shift.z);
//            va2.glVertex3f (v->vector.x + shift.x, v->vector.y + shift.y, v->vector.z + shift.z);
          }
        }
        va->_glEnd ();

        va->_glBegin (GL_QUADS);
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
            va->_glNormal3f (v->normal.x, v->normal.y, v->normal.z);
//            va3.glNormal3f (v->normal.x, v->normal.y, v->normal.z);
            if (cm->hasTexture)
            {
              if (cm->vertex)
              {
                va->_glTexCoord2f (v->tex.x, v->tex.y);
                va->_glColor4f (1, 1, 1, 1);
//                va3.glColor4f (1, 1, 1, 0.5);
              }
            }
            else
            {
              if (model.numMaterials && cm->material && cm->material->texture->textureID >= 0)
              {
                unsigned char *pColor = model.material [cm->material->texture->textureID]->color.c;
                va->_glColor4ub (pColor [0], pColor [1], pColor [2], pColor [3]);
//                va3.glColor4ub (pColor [0], pColor [1], pColor [2], 128);
              }
            }
            va->_glVertex3f (v->vector.x + shift.x, v->vector.y + shift.y, v->vector.z + shift.z);
//            va3.glVertex3f (v->vector.x + shift.x, v->vector.y + shift.y, v->vector.z + shift.z);
          }
        }
        va->_glEnd ();
      }

/*      float light_specular [4] = {1.0, 1.0, 1.0, 1.0};
      glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular);
      float light_ambient [3] = {0.0, 0.0, 0.0};
      glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
      float light_diffuse [3] = {0.0, 0.0, 0.0};
      glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
      glLighti (GL_LIGHT0, GL_SPOT_EXPONENT, 100);
      glLighti (GL_LIGHT0, GL_SPOT_CUTOFF, 20);
//      glEnable (GL_BLEND);
      va2.glEnd ();
      va3.glEnd ();
//      glDisable (GL_BLEND); */

      // DEBUG: show normal vectors
/*      for (i = 0; i < model.numObjects; i ++)
      {
        if (model.numObjects <= 0) break;
        cm = model.object [i];
        glBegin (GL_LINES);
        for (j = 0; j < cm->numTriangles; j++)
        {
          Vertex *v = cm->triangle [j].v [0];
          for (int whichVertex = 0; whichVertex < 3; whichVertex ++)
          {
            v = cm->triangle [j].v [whichVertex];
            glColor3ub (255, 255, 0);
            glVertex3f (v->vector.x, v->vector.y, v->vector.z);
            glVertex3f (v->vector.x + v->normal.x, v->vector.y + v->normal.y, v->vector.z + v->normal.z);
          }
        }
        glEnd ();

        glBegin (GL_LINES);
        for (j = 0; j < cm->numQuads; j++)
        {
          Vertex *v = cm->quad [j].v [0];
          for (int whichVertex = 0; whichVertex < 4; whichVertex++)
          {
            v = cm->quad [j].v [whichVertex];
            glVertex3f (v->vector.x, v->vector.y, v->vector.z);
            glVertex3f (v->vector.x + v->normal.x, v->vector.y + v->normal.y, v->vector.z + v->normal.z);
          }
        }
        glEnd ();
      } */

      if (model.alpha)
      { glDisable (GL_BLEND); glDisable (GL_ALPHA_TEST); }

      glPopMatrix ();
    }

    void drawNoLight (const Model3d &model, const Transformation &trafo,
                      int explode)
    {
      int i, j;
      Object3d *cm;
      Vector3 tl2;
      Vector3 tl(trafo.translation);
      Rotation rot(trafo.rotation);
      float zoom = trafo.scaling.x;
      int shading = 2;

      VertexArray va1 (VERTEXARRAY_V3C4T2);
      VertexArray *va = &va1;
      int alpha = 1;
//      int nolight = 0;

      for (i = 0; i < model.numObjects; i ++)
      {
        if (model.numObjects <= 0) break;
        cm = model.object [i];
        if (cm->hasTexture)
        {
          if (antialiasing)
            gl.enableLinearTexture (cm->material->texture->textureID, cm->material->texture->mipmap);
          else
            gl.disableLinearTexture (cm->material->texture->textureID, cm->material->texture->mipmap);
        }
      }

//      zoom *= scale;
      glPushMatrix ();
      glTranslatef (tl.x + tl2.x, tl.y + tl2.y - 0.002 * explode * explode / timestep / timestep, tl.z + tl2.z);
      float explodefac = (float) explode / 10 / timestep;

      if (showCollision)
      {
        glPushMatrix ();
        glScalef (model.cube.x, model.cube.y, model.cube.z);
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

/*      bool listgen = false;
      if (list2 == -1 && explode <= 0 && displaylist)
      {
        listgen = true;
        list2 = gl.createList ();
      }
      if (listgen || explode > 0 || !displaylist)
      { */

        if (shading == 1)
          glShadeModel (GL_FLAT);
        else
          glShadeModel (GL_SMOOTH);

        if (model.alpha)
        { glEnable (GL_BLEND); glEnable (GL_ALPHA_TEST); glAlphaFunc (GL_GEQUAL, 0.01); }

        for (i = 0; i < model.numObjects; i++)
        {
          if (model.numObjects <= 0) break;
          cm = model.object [i];
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

          va->_glBegin (GL_TRIANGLES);
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
              va->_glNormal3f (v->normal.x, v->normal.y, v->normal.z);
              if (cm->hasTexture)
              {
                if (cm->vertex)
                {
                  va->_glTexCoord2f (v->tex.x, v->tex.y);
                  va->_glColor4f (1, 1, 1, 1);
                }
              }
              else
              {
                unsigned char *pColor = v->color.c;
                va->_glColor4ub (pColor [0], pColor [1], pColor [2], pColor [3]);
              }
              va->_glVertex3f (v->vector.x + shift.x, v->vector.y + shift.y, v->vector.z + shift.z);
            }
          }
          va->_glEnd ();

          va->_glBegin (GL_QUADS);
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
              va->_glNormal3f (v->normal.x, v->normal.y, v->normal.z);
              if (cm->hasTexture)
              {
                if (cm->vertex)
                {
                  va->_glTexCoord2f (v->tex.x, v->tex.y);
                  va->_glColor4f (1, 1, 1, 1);
                }
              }
              else
              {
                unsigned char *pColor = v->color.c;
                va->_glColor4ub (pColor [0], pColor [1], pColor [2], pColor [3]);
              }
              va->_glVertex3f (v->vector.x + shift.x, v->vector.y + shift.y, v->vector.z + shift.z);
            }
          }
          va->_glEnd ();
        }

        if (alpha)
        { glDisable (GL_BLEND); glDisable (GL_ALPHA_TEST); }

/*        if (listgen) glEndList ();
      }
      else glCallList (list2); */

      glPopMatrix ();
    }

    void drawNoTexture (const Model3d &model, const Transformation &trafo,
                        float lum, int explode)
    {
      int i, j;
      Object3d *cm;
      Vector3 tl2;
      Vector3 tl(trafo.translation);
      Rotation rot(trafo.rotation);
      float zoom = trafo.scaling.x;

      VertexArray va1 (VERTEXARRAY_V3N3C4);
      VertexArray *va = &va1;
      int alpha = 1;
//      int nolight = 0;

//      zoom *= scale;
      glPushMatrix ();
      glTranslatef (tl.x + tl2.x, tl.y + tl2.y - 0.002 * explode * explode / timestep / timestep, tl.z + tl2.z);
      float explodefac = (float) explode / 10 / timestep;

      if (showCollision)
      {
        glPushMatrix ();
        glScalef (model.cube.x, model.cube.y, model.cube.z);
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

      for (i = 0; i < model.numObjects; i++)
      {
        if (model.numObjects <= 0) break;
        cm = model.object [i];
        glDisable (GL_TEXTURE_2D);
        glColor3ub (255, 255, 255);

        Vector3 shift;

        va->_glBegin (GL_TRIANGLES);
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
            va->_glColor3f (red, green, blue);
            va->_glVertex3f (v->vector.x + shift.x, v->vector.y + shift.y, v->vector.z + shift.z);
          }
        }
        va->_glEnd ();

        va->_glBegin (GL_QUADS);
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
            va->_glColor3f (red, green, blue);
            va->_glVertex3f (v->vector.x + shift.x, v->vector.y + shift.y, v->vector.z + shift.z);
          }
        }
        va->_glEnd ();

      }

      if (alpha)
      { glDisable (GL_BLEND); glDisable (GL_ALPHA_TEST); }

      glPopMatrix ();
    }

};

/*#include "effects/Effects.h"
#include "configuration/Configuration.h"

class StarsRealizer
{
  public:

    void draw (unsigned numStars, std::vector<Star *> &star)
    {
      for (unsigned i = 0; i < numStars; i ++)
      {
        glPointSize (LINEWIDTH(1.0F));
        glPushMatrix (); // PUSH -> 2
        glRotatef (star [i]->phi, 0.0, 1.0, 0.0);
        glRotatef (star [i]->gamma, 1.0, 0.0, 0.0);
        glTranslatef (0, 0, -10);

        glPointSize (star [i]->size * 1.5); // TODO: should depend on screen resolution!
      //    gl.enableAntiAliasing ();
        glBegin (GL_POINTS);
        glColor3ub (255, 255, 255);
        glVertex3f (0, 0, 0);
        glEnd ();
      //    gl.disableAntiAliasing ();

        glPopMatrix (); // POP -> 1
      }
    }
};*/



#endif // IS_REALIZER_H
