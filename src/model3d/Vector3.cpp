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



Vector3::Vector3 ()
{
  set (0, 0, 0);
}

Vector3::Vector3 (float xyz)
{
  set (xyz, xyz, xyz);
}

Vector3::Vector3 (float x, float y, float z)
{
  set (x, y, z);
}

Vector3::Vector3 (const Vector3 &v)
{
  set (v);
}

void Vector3::set (const Vector3 &v)
{
  x = v.x;
  y = v.y;
  z = v.z;
}

void Vector3::set (float x, float y, float z)
{
  this->x = x;
  this->y = y;
  this->z = z;
}

void Vector3::neg ()
{
  x = -x; y = -y; z = -z;
}

void Vector3::add (const Vector3 &v)
{
  x += v.x; y += v.y; z += v.z;
}

void Vector3::sub (const Vector3 &v)
{
  x -= v.x; y -= v.y; z -= v.z;
}

void Vector3::mul (float fac)
{
  x *= fac; y *= fac; z *= fac;
}

void Vector3::crossproduct (const Vector3 &v)
{
  float nx = y * v.z - z * v.y;
  float ny = z * v.x - x * v.z;
  float nz = x * v.y - y * v.x;
  x = nx;
  y = ny;
  z = nz;
}

float Vector3::dotproduct (const Vector3 &v) const
{
  return x * v.x + y * v.y + z * v.z;
}

float Vector3::length () const
{
  return (float) sqrt (x * x + y * y + z * z);
}

void Vector3::norm ()
{
  float d = sqrt (x * x + y * y + z * z);
  if (d == 0)
    d = 1E-10;
  x /= d;
  y /= d;
  z /= d;
}

bool Vector3::isEqual (const Vector3 &v) const
{
  return x == v.x && y == v.y && z == v.z;
}

bool Vector3::isEqual (const Vector3 &v, float tolerance) const
{
  return x >= v.x - tolerance && x <= v.x + tolerance &&
         y >= v.y - tolerance && y <= v.y + tolerance &&
         z >= v.z - tolerance && z <= v.z + tolerance;
}

#endif

