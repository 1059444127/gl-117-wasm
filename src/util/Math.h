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

#ifndef IS_MATHTAB_H
#define IS_MATHTAB_H

#include <math.h>

#define MULTIPLAYER 1
#undef MULTIPLAYER

// exact sine, cosine functions
#define PI     (atan(1.0)*4.0)
#define SIN(x) sin(x*PI/180.0)
#define COS(x) cos(x*PI/180.0)

/**
* This class includes a collection of functions and precalculated tables.
*/
class Math
{
  public:

    Math ();

    /// random integer number [0;n-1]
    static int random (int n);
    /// random integer number [0;n-1]
    static int random (int n, int x, int y);
    /// random integer number [0;n-1], prefer extremely low/high values
    static int extremeRandom (int n);
    /// random integer number [0;n-1], prefer extremely low/high values
    static int extremeRandom (int n, int x, int y);
    /// distance (Euklidean norm, 2D vector)
    static float distance (float dx, float dy);

  protected:

#ifdef MULTIPLAYER
    static int randommaster [64] [64];
    static int randptr = 0;
#endif
};

#endif
