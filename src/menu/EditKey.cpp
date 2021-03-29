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

#ifndef IS_COMPONENT_H

#include "Component.h"
#include "configuration/Configuration.h" // TODO: not good to make dependancy here!



EditKey::EditKey ()
{
  key = 'A';
}

EditKey::EditKey (const std::string &text)
{
  key = 'A';
  this->text = text;
}

void EditKey::setText (const std::string &text)
{
  this->text = text;
}

void EditKey::setKey (int key)
{
  if (key >= 256)
  {
    this->special = key - 256;
    specialActive = true;
  }
  else
  {
    this->key = key;
    specialActive = false;
  }
}

int EditKey::getKey ()
{
  if (specialActive)
    return special + 256;
  else
    return key;
}

void EditKey::draw ()
{
  if (!visible) return;
  char buf [64];
  Color *col = &StandardTextColor;
  glLoadName (id);
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBegin (GL_QUADS);
  if (!highlight)
    glColor4ubv (colback.c);
  else
    glColor4ub (255, 255, 0, colback.c [3]);
  glVertex3f (0.1 * x, 0.1 * y, menuZ);
  glVertex3f (0.1 * (x + w), 0.1 * y, menuZ);
  glVertex3f (0.1 * (x + w), 0.1 * (y - h), menuZ);
  glVertex3f (0.1 * x, 0.1 * (y - h), menuZ);
  glEnd ();
  glDisable (GL_BLEND);
  font1->zoom = 0.1 * (h - 0.1);
  float yf = (y - h - 0.01) / (h - 0.1);
  float xf = (x + 0.25) / (h - 0.1);
  font1->drawText (xf, yf, menuZ, text, *col);
  if (specialActive)
    conf.key2string (special + 256, buf);
  else
    conf.key2string (key, buf);
  font1->drawText (xf + (w - 2) / h, yf, menuZ, buf, *col);
  font1->zoom = 0.1;
}

void EditKey::triggerMouse (int id, int button)
{
  if (!active) return;
  highlight = true;
}

void EditKey::eventKey (unsigned char key)
{
  char buf [256];
  if (!active) return;
  if (highlight)
  {
    conf.key2string (key, buf);
    if (buf [0] != '\0')
    {
      this->key = (int) key;
      specialActive = false;
      execFunction (key);
    }
  }
}

void EditKey::eventSpecial (int key)
{
  char buf [256];
  if (!active) return;
  if (highlight)
  {
    conf.key2string (key + 256, buf);
    if (buf [0] != '\0')
    {
      this->special = key;
      specialActive = true;
      execFunction (special + 256);
    }
  }
}

#endif
