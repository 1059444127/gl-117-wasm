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

#ifndef IS_BITMAP_H

#include "loadbitmap/Bitmap.h"

#include <cassert>



Bitmap::Bitmap ()
{
  data = 0;
  width = 0;
  height = 0;
}

/*unsigned char *getPixel (int x, int y) const
{
  assert (x >= 0 && y >= 0 && x < width && y < height);

  return &data [(y * width + x) * 4];
}

void setPixel (int x, int y, unsigned char *c)
{
  assert (x >= 0 && y >= 0 && x < width && y < height);

  int address = (y * width + x) * 4;
  data [address + 0] = c [0];
  data [address + 1] = c [1];
  data [address + 2] = c [2];
  data [address + 3] = c [3];
}*/

#endif
