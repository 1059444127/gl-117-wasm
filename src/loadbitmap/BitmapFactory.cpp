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

#include "Bitmap.h"

#include <cassert>

BitmapList BitmapFactory::bitmapList = BitmapList ();

Bitmap *BitmapFactory::getBitmap (const std::string &name)
{
  BitmapList::iterator it = bitmapList.find (name);
  if (it == bitmapList.end ())
  {
    Bitmap *bitmap = new Bitmap ();
    bitmap->data = LoadTga::load (name.c_str (), &bitmap->width, &bitmap->height);
    bitmapList.insert (std::pair<std::string, Bitmap *> (name, bitmap));
    return bitmap;
  }
  else
  {
    assert(it->second->data);
    return it->second;
  }
  return 0;
}

#endif
