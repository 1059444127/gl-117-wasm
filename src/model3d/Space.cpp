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



Space::Space ()
{
  o.clear ();
}

Space::~Space ()
{
}

void Space::addObject (SpaceObj *o)
{
  this->o.push_back (o);
  o->space = this;
}

bool Space::removeObject (SpaceObj *o2)
{
  size_t debugsize = o.size ();
  std::vector <SpaceObj *>::iterator it = o.begin ();
  for (; it != o.end (); it ++)
    if (*it == o2)
    {
      o.erase (it);
      return true;
    }
  return false;
}

void Space::removeAllObjects ()
{
  o.clear ();
}

/*void Space::translate (Vector3 &v)
{
  tl.set (v);
}*/

/*void Space::drawGL ()
{
  int i;
  frustum.extractFrustum ();

  if (drawLight)
  {
    for (i = 0; i < o.size (); i ++)
    {
      if (o [i] != NULL)
        if (o [i]->o != NULL)
          if (o [i]->draw && !o [i]->o->alpha)
            o [i]->drawGL (tl, 1.0, lum, true, true);
    }
    for (i = 0; i < o.size (); i ++)
    {
      if (o [i] != NULL)
        if (o [i]->o != NULL)
          if (o [i]->draw && o [i]->o->alpha)
            o [i]->drawGL (tl, 1.0, lum, true, true);
    }
  }
  else
  {
    for (i = 0; i < o.size (); i ++)
    {
      if (o [i] != NULL)
        if (o [i]->o != NULL)
          if (o [i]->draw && !o [i]->o->alpha)
            o [i]->drawGL (tl, 1.0, lum, false, false);
    }
    for (i = 0; i < o.size (); i ++)
    {
      if (o [i] != NULL)
        if (o [i]->o != NULL)
          if (o [i]->draw && o [i]->o->alpha)
            o [i]->drawGL (tl, 1.0, lum, false, false);
    }
    for (i = 0; i < o.size (); i ++)
    {
      if (o [i] != NULL)
        if (o [i]->o == NULL)
          if (o [i]->draw)
            o [i]->drawGL (tl, 1.0, lum, false, false);
    }
  }
}*/

#endif
