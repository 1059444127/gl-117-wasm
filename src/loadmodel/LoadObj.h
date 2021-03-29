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

#ifndef IS_LOADOBJ_H
#define IS_LOADOBJ_H

#define int32 int

#include "model3d/Model3d.h" // ok

class TextFileObj
{
  public:
  FILE *in;
  int32 size;
  int32 filepointer;
  char *data;

  TextFileObj (const char *filename);
  ~TextFileObj ();
  float readFloat (int32 offset);
  float readFloat ();
  int readInteger (int32 offset);
  int readInteger ();
  char *readWord (int32 offset);
  char *readWord ();
  char *getChar ();
  void nextWhite ();
  void skipWhite ();
};

/**
* LoadObj handles all of the loading code.
*/
class LoadObj
{
  public:

    LoadObj ();

    bool importObj (Model3d *model, char *filename);

  private:

    void computeNormals (Model3d *model);
    void computeColors (Model3d *model);
    void normalize (Model3d *model);
    void loadObject (Model3d *model);
};

#endif
