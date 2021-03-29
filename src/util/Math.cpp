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

#include "Math.h"

#include <math.h>
#include <stdlib.h>



#ifdef MULTIPLAYER
int randommaster [64] [64];
int randptr = 0;
#endif

Math::Math ()
{
#ifdef MULTIPLAYER
  unsigned i, i2;
  for (i = 0; i < 63; i ++)
    for (i2 = 0; i2 < 63; i2 ++)
      randommaster [i] [i2] = rand ();
#endif
}

// return random number
int Math::random (int n)
{
  if (n == 0) return 0;

#ifndef MULTIPLAYER
  return rand () % n;
#else
  randptr ++;
  if (randptr >= 64) randptr = 0;
  return randommaster [randptr] [randptr] % n;
#endif
}

// return random number, but prefer extremely high and low values
int Math::extremeRandom (int n)
{
  if (n == 0) return 0;

#ifndef MULTIPLAYER
  int ret;
  ret = rand () % n;
#else
  randptr ++;
  if (randptr >= 64) randptr = 0;
  ret = randommaster [randptr] [randptr] % n;
#endif

  if ((ret % 5) <= 4)
  {
    if (ret > n/2 && ret < 3*n/4) return ret + n/4;
    else if (ret < n/2 && ret > n/4) return ret - n/4;
  }
  return ret;
}

int Math::random (int n, int x, int y)
{
  if (n == 0) return 0;

#ifndef MULTIPLAYER
  int ret;
  ret = rand () % n;
#else
  ret = (randommaster [x%63] [y%63] ^ (31*x) ^ (71*y)) % n;
#endif

  return ret;
}

int Math::extremeRandom (int n, int x, int y)
{
  if (n == 0) return 0;

#ifndef MULTIPLAYER
  int ret;
  ret = rand () % n;
#else
  ret = (randommaster [x%63] [y%63] ^ (31*x) ^ (71*y)) % n;
#endif

  if ((ret % 5) <= 4)
  {
    if (ret > n/2 && ret < 3*n/4) return ret + n/4;
    else if (ret < n/2 && ret > n/4) return ret - n/4;
  }
  return ret;
}

float Math::distance (float dx, float dy)
{
  return (float) sqrt (dx * dx + dy * dy);
}

#endif
