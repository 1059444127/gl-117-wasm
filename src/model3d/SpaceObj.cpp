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

#ifndef IS_SPACE_H

#include "Model3d.h"
#include "opengl/GlPrimitives.h"



SpaceObj::SpaceObj ()
{
  o = 0L;
  space = NULL;
  init ();
}

SpaceObj::SpaceObj (const SpaceObj &that)
{
  valid = that.valid;
  o = that.o;
  draw = that.draw;
  lum = that.lum;
  drawLight = that.drawLight;
  explode = that.explode;
  trafo.set (that.trafo);
  space = that.space;
}

SpaceObj::SpaceObj (Model3d *o, const Transformation &trafo)
{
  this->o = o;
  this->trafo.set (trafo);
  space = NULL;
  init ();
}

SpaceObj::~SpaceObj ()
{
}

void SpaceObj::init ()
{
  explode = 0;
  draw = true;
  lum = 1.0;
  drawLight = true;
  valid = true;
}

void SpaceObj::addRefModel (const SpaceObj &ref2)
{
  ref.push_back (ref2);
}

void SpaceObj::translate (Vector3 &v)
{
  trafo.translation.set (v);
}

void SpaceObj::translate (float x, float y, float z)
{
  trafo.translation.set (x, y, z);
}

void SpaceObj::rotate (short a, short b, short c)
{
  trafo.rotation.set (a, b, c);
}

void SpaceObj::rotateOn (short a, short b, short c)
{
  trafo.rotation.add (a, b, c);
}

/*void SpaceObj::drawGL (Vector3 &tl, float alpha2, float lum2, bool drawLight2, bool isTextured2)
{
  int i;
  Vector3 tl1;
  tl1.x = tl.x + this->trafo.translation.x;
  tl1.y = tl.y + this->trafo.translation.y;
  tl1.z = tl.z + this->trafo.translation.z;
  if (draw == 0) return;
  if (draw == 2 || frustum.isSphereInFrustum (tl1.x, tl1.y, tl1.z, this->trafo.scaling.x * 1.5))
  {
    if (drawLight && drawLight2)
    {
      glEnable (GL_LIGHTING);
      o->draw (tl, this->trafo.translation, this->trafo.rotation, this->trafo.scaling.x, lum * lum2, explode);
      if (!ref.empty ())
      {
        glPushMatrix ();
        glTranslatef (tl1.x, tl1.y - 0.001 * explode * explode / timestep / timestep, tl1.z);
        glRotatef (trafo.rotation.phi + 90, 0, -1, 0);
        glRotatef (-trafo.rotation.gamma + 90, 0, 0, 1);
        glRotatef (trafo.rotation.theta + 180, 1, 0, 0);
        glScalef (trafo.scaling.x, trafo.scaling.y, trafo.scaling.z);
        if (o->refpoint)
          for (i = 0; i < ref.size (); i ++)
            if (ref [i].trafo.scaling.x > 0.001)
              ref [i].o->draw (o->refpoint [i / 3], ref [i].trafo.translation, ref [i].trafo.rotation, ref [i].trafo.scaling.x, lum * lum2, explode);
        glPopMatrix ();
      }
    }
    else
    {
      glDisable (GL_LIGHTING);
      if (isTextured2)
      {
        o->drawNoLight (tl, this->trafo.translation, this->trafo.rotation, this->trafo.scaling.x, explode);
      }
      else
      {
        if (drawLight2)
          o->drawNoTexture (tl, this->trafo.translation, this->trafo.rotation, this->trafo.scaling.x, lum * lum2, explode);
        else
          o->drawNoTexture (tl, this->trafo.translation, this->trafo.rotation, this->trafo.scaling.x, 1.0, explode);
      }
    }
  }
}*/

#endif
