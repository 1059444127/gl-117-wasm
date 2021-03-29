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



Material::Material ()
{
  uscale = 1;
  vscale = 1;
  utile = 1;
  vtile = 1;
  uoffset = 0;
  voffset = 0;
  wrot = 0;
  texture = 0;
  filename = "";
  name = "";
}

Material::Material (const Material &material)
{
  uscale = material.uscale;
  vscale = material.vscale;
  utile = material.utile;
  vtile = material.vtile;
  uoffset = material.uoffset;
  voffset = material.voffset;
  wrot = material.wrot;
  texture = material.texture; // careful: pointer copy!
  filename = material.filename;
  name = material.name;
}

#endif

