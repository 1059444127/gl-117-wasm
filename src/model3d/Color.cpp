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



Color::Color ()
{
  set (255, 255, 255, 255);
}

Color::Color (const Color &color)
{
  set (color);
}

Color::Color (int red, int green, int blue, int alpha)
{
  set (red, green, blue, alpha);
}

void Color::set (const Color &color)
{
  c [0] = color.c [0];
  c [1] = color.c [1];
  c [2] = color.c [2];
  c [3] = color.c [3];
}

void Color::set (int red, int green, int blue, int alpha)
{
  c [0] = (unsigned char) red;
  c [1] = (unsigned char) green;
  c [2] = (unsigned char) blue;
  c [3] = (unsigned char) alpha;
}

bool Color::isEqual (const Color &color) const
{
  return c [0] == color.c [0] && c [1] == color.c [1] &&
         c [2] == color.c [2] && c [3] == color.c [3];
}

/* Do NOT use overloaded operators, as we will lose the feeling
   which operators are slow and which are fast.
   E.g. a = b; c = d; but first is a pointer copy, second copies a huge data structure! */
/*void Color::operator = (const Color &color)
{
  set (...);
}*/

#endif

