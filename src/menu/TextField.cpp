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



TextField::TextField ()
{
}

TextField::TextField (const std::string &text, float fontheight)
{
  this->text = text;
  fonth = fontheight;
  colback.set (140, 140, 140, 180);
}

void TextField::setText (const std::string &text)
{
  this->text = text;
}

void TextField::draw ()
{
  if (!visible) return;
  Color *col = &StandardTextColor;
  glLoadName (id);
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBegin (GL_QUADS);
  glColor4ubv (colback.c);
  glVertex3f (0.1 * x, 0.1 * y, menuZ);
  glVertex3f (0.1 * (x + w), 0.1 * y, menuZ);
  glVertex3f (0.1 * (x + w), 0.1 * (y - h), menuZ);
  glVertex3f (0.1 * x, 0.1 * (y - h), menuZ);
  glEnd ();
  glDisable (GL_BLEND);
  font1->zoom = 0.1 * (fonth - 0.1);
  float yf = (y - fonth - 0.1) / (fonth - 0.1);
  float xf = (x + 0.25) / (fonth - 0.1);
  font1->drawText (xf, yf, menuZ, text, *col);
  font1->zoom = 0.1;
}

void TextField::triggerMouse (int id, int button)
{
}

#endif
