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
//#include "main.h"

int menuId = 0;
int menuZ = -2;

Color StandardTextColor (255, 255, 255, 200);
Color DeactiveTextColor (220, 220, 220, 200);
Color HiliteTextColor (255, 255, 0, 200);

Component::Component ()
{
  menuId ++;
  id = menuId;
  func = NULL;
  highlight = false;
  active = true;
  visible = true;
  parent = NULL;
  colback.set (0, 0, 255, 120);
}

Component::~Component ()
{
}

void Component::setBounds (float x, float y, float w, float h)
{
  this->x = x;
  this->y = y;
  this->w = w;
  this->h = h;
}

void Component::draw ()
{
}

void Component::setFunction (void (*func) (Component *, int button))
{
  this->func = func;
}

void Component::execFunction (int button)
{
  if (func)
    func (this, button);
}

void Component::eventMouse (int x, int y, int button)
{
}

void Component::eventKey (unsigned char key)
{
}

void Component::eventJoystick (int button)
{
}

void Component::eventSpecial (int key)
{
}

void Component::triggerMouse (int id, int button)
{
}

void Component::setActive (bool value)
{
  active = value;
}

void Component::setVisible (bool value)
{
  visible = value;
}

void Component::setHighlight (bool value)
{
  highlight = value;
}

bool Component::isActive ()
{
  return active;
}

bool Component::isVisible ()
{
  return visible;
}

bool Component::isHighlight ()
{
  return highlight;
}

bool Component::contains (int id)
{
  if (id == this->id) return true;
  return false;
}

#endif
