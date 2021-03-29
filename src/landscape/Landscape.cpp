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

#ifndef IS_LANDSCAPE_H

#include "Landscape.h"
#include "util/Math.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>



/****************************************************************************
  SMOOTING, CONVOLUTION, EROSION
****************************************************************************/

// fast Gaussian convolution (smoothing terrain)
// H := height matrix, K := Gaussian kernel matrix
// Let K := u conv v := (u^T)v, then H conv K = H conv (u conv v) = (H conv u) conv v
void Landscape::convolveGauss (int radius, int hmin, int hmax) // only convolve height values in [hmin...hmax]
{
  // create u and v vectors and restrict them to the interval [mincore...maxcore]
  float core_u [MAXCORE], core_v [MAXCORE];
  int mincorex = 0, mincorey = 0, maxcorex = 0, maxcorey = 0;
  if (radius == 1)
  {
    core_u [4] = 0.25; core_u [5] = 0.5; core_u [6] = 0.25;
    core_v [4] = 0.25; core_v [5] = 0.5; core_v [6] = 0.25;
    mincorex = 4; maxcorex = 6;
    mincorey = 4; maxcorey = 6;
  }
  else if (radius == 2)
  {
    core_u [3] = 0.1F; core_u [4] = 0.2F; core_u [5] = 0.4F; core_u [6] = 0.2F; core_u [7] = 0.1F;
    core_v [3] = 0.1F; core_v [4] = 0.2F; core_v [5] = 0.4F; core_v [6] = 0.2F; core_v [7] = 0.1F;
    mincorex = 3; maxcorex = 7;
    mincorey = 3; maxcorey = 7;
  }
  else if (radius == 3)
  {
    core_u [2] = 0.05F; core_u [3] = 0.1F; core_u [4] = 0.2F; core_u [5] = 0.3F; core_u [6] = 0.2F; core_u [7] = 0.1F; core_u [8] = 0.05F;
    core_v [2] = 0.05F; core_v [3] = 0.1F; core_v [4] = 0.2F; core_v [5] = 0.3F; core_v [6] = 0.2F; core_v [7] = 0.1F; core_v [8] = 0.05F;
    mincorex = 2; maxcorex = 8;
    mincorey = 2; maxcorey = 8;
  }

  // do the convolutions
  int i, x, y, mx, my;
  float sum;
  float summe [MAXX + 1];
  for (x = 0; x <= MAXX; x ++)
    summe [x] = 0;
  for (x = 0; x <= MAXX; x ++)
  {
    for (y = 0; y <= MAXX; y ++) // H conv u
    {
      summe [y] = 0;
      for (i = mincorex; i <= maxcorex; i ++)
      {
        mx = GETCOORD(x - MAXCORE / 2 + i);
        my = y;
        summe [y] += core_u [i] * h [mx] [my];
      }
    }
    for (y = 0; y <= MAXX; y ++) // H conv v
    {
      if (h [x] [y] >= hmin && h [x] [y] <= hmax) // only convolve if current height in [hmin...hmax]
      {
        sum = 0;
        for (i = mincorey; i <= maxcorey; i ++)
        {
          my = GETCOORD(y - MAXCORE / 2 + i);
          sum += core_v [i] * summe [my];
        }
        hg [x] [y] = (int) sum;
      }
      else
      {
        hg [x] [y] = h [x] [y];
      }
    }
  }

  // copy hg back to h
  memcpy (h, hg, (MAXX+1) * (MAXX+1) * sizeof (unsigned short));
}

// 'smooth' the glaciers using an erosion function
void Landscape::smoothGlacier ()
{
  int i, i2;
  for (i = 0; i <= MAXX - 1; i ++)
    for (i2 = 0; i2 <= MAXX - 1; i2 ++)
    {
      if (f [i] [i2] == GLACIER)
      {
        int min = h[i][i2];
        min = min < h[i+1][i2] ? min : h[i+1][i2];
        min = min < h[i][i2+1] ? min : h[i][i2+1];
        min = min < h[i+1][i2+1] ? min : h[i+1][i2+1];
        h[i][i2] = min;
      }
    }
}

// Gaussian convolution for a single raster point (5x5)
void Landscape::gauss (int x, int y)
{
  if (x < 5) x = 5;
  else if (x > MAXX - 5) x = MAXX - 5;
  if (y < 5) y = 5;
  else if (y > MAXX - 5) y = MAXX - 5;

  int i, i2, i3, i4;
  int hmask [5] [5];

  int g5[5][5]={{1,1,2,1,1},
                {1,3,4,3,1},
                {2,4,5,4,2},
                {1,3,4,3,1},
                {1,1,2,1,1}};
  for (i = x-2; i <= x+2; i ++)
    for (i2 = y-2; i2 <= y+2; i2 ++)
    {
      int sum = 0;
      for (i3 = 0; i3 < 5; i3 ++)
        for (i4 = 0; i4 < 5; i4 ++)
        {
          sum += g5[i3][i4] * h[i+i3-2][i2+i4-2];
        }
      sum /= 53;
      hmask [i-x+2] [i2-y+2] = sum;
    }
  for (i = x-2; i <= x+2; i ++)
    for (i2 = y-2; i2 <= y+2; i2 ++)
    {
      h[i][i2] = hmask[2][2];
      hw[i][i2] = hmask[2][2];
    }
}

// constant height for a single raster point (7x7)
void Landscape::flatten (int x, int y, int dx, int dy)
{
  int i, i2;
  if (x < dx) x = dx;
  else if (x > MAXX - dx) x = MAXX - dx;
  if (y < dy) y = dy;
  else if (y > MAXX - dy) y = MAXX - dy;

  for (i = x-dx; i <= x+dx; i ++)
    for (i2 = y-dy; i2 <= y+dy; i2 ++)
    {
      h[i][i2] = h[x][y];
      hw[i][i2] = h[i][i2];
    }
}



/****************************************************************************
  LANDSCAPE TYPE QUERIES
****************************************************************************/

bool Landscape::isType (unsigned char type, unsigned char id) const
{
  return ((type & 0xFF) == (id << 0));
}

bool Landscape::isWoods (int type) const
{
  type &= 0xFF;
  if (type >= CONIFEROUSWOODS0 && type <= BUSHES3)
    return true;
  return false;
}

bool Landscape::isWater (int type) const
{
  type &= 0xFF;
  if (type >= WATER && type <= XDEEPWATER)
    return true;
  return false;
}

bool Landscape::isGlacier (int type) const
{
  type &= 0xFF;
  if (type == GLACIER)
    return true;
  return false;
}

bool Landscape::isGround (int x, int y) const
{
  if (x < 0 || x > MAXX || y < 0 || y > MAXX) return false;
  int type = f [y] [x];
  if (!isWater (type))
    return true;
  return false;
}



/****************************************************************************
  LANDSCAPE CREATION
****************************************************************************/

void Landscape::init ()
{
  int i, i2;
  for (i = 0; i <= MAXX; i ++)
    for (i2 = 0; i2 <= MAXX; i2 ++)
    {
      h [i] [i2] = 0; f [i] [i2] = 0;
    }
}

// cubic spline interpolation (obsolete -> Gaussian convolution)
/*  void interpolate (int n)
{
  double s, r, q, a, b, c, d, e, f;
  int i, i2, i3, i4;
  long htest, p1, p2, p3, p4;
  i3 = n;
  i = 0;
  while (i <= maxx)
  {
    i2 = 0;
    while (i2 < maxx - (1 << i3))
    {
      a = i2 + 1;
      b = i2 + (1 << i3) + 1;
      c = i2 + (1 << (i3 + 1)) + 1;
      d = h [(int) a - 1] [i];
      e = h [(int) b - 1] [i];
      f = h [(int) c - 1] [i];
      double aq = a * a, bq = b * b, cq = c * c, mba = 1/b - 1/a, mca = 1/c - 1/a;
      s = ((f/cq-d/aq)/mca-(e/bq-d/aq)/mba)/((1/cq-1/aq)/mca-(1/bq-1/aq)/mba);
      r = ((e/bq-d/aq)/mba)-(1/bq-1/aq)/mba*s;
      q = d/aq - r/a - s/aq;
      for (i4 = (int) a + 1; i4 <= (int) c - 1; i4 ++)
      {
        if (i4 != b)
        {
          htest = (long) (i4 * i4 * q + i4 * r + s);
          if (htest < 0) htest = 0;
          if (htest > 65535) htest = 65535;
          if (h [i4 - 1] [i] == 0)
          {
            h [i4 - 1] [i] = h [i4 - 1] [i] + (int) (htest / 2);
            if (i4 - 1 < (1 << i3) || i4 - 1 > maxx - (1 << i3))
              h [i4 - 1] [i] = h [i4 - 1] [i] * 2;
          }
          else
          {
            htest = htest / 2 + (htest - h [i4 - 1] [i] * 2) * (long) ((i4 - a) / (b - a) - 0.5);
            if (htest < 0) htest = 0;
            if (htest > 65535) htest = 65535;
            h [i4 - 1] [i] = h [i4 - 1] [i] + (int) htest;
          }
        }
      }
      i2 = i2 + (1 << i3);
    }
    i = i + (1 << i3);
  }
  i2 = 0;
  while (i2 <= maxx)
  {
    i = 0;
    while (i < maxx - (1 << i3))
    {
      a = i + 1;
      b = i + (1 << i3) + 1;
      c = i + (1 << (i3 + 1)) + 1;
      d = h [i2] [(int) a - 1];
      e = h [i2] [(int) b - 1];
      f = h [i2] [(int) c - 1];
      double aq = a * a, bq = b * b, cq = c * c, mba = 1/b - 1/a, mca = 1/c - 1/a;
      s = ((f/cq-d/aq)/mca-(e/bq-d/aq)/mba)/((1/cq-1/aq)/mca-(1/bq-1/aq)/mba);
      r = ((e/bq-d/aq)/mba)-(1/bq-1/aq)/mba*s;
      q = d/aq - r/a - s/aq;
      for (i4 = (int) a + 1; i4 <= (int) c - 1; i4 ++)
      {
        if (i4 != b)
        {
          htest = (long) (i4 * i4 * q + i4 * r + s);
          if (htest < 0) htest = 0;
          if (htest > 65535) htest = 65535;
          if (h [i2] [i4 - 1] == 0)
          {
            h [i2] [i4 - 1] = h [i2] [i4 - 1] + (int) (htest / 2);
            if (i4 - 1 < (1 << i3) || i4 - 1 > maxx - (1 << i3))
              h [i2] [i4 - 1] = h [i2] [i4 - 1] * 2;
          }
          else
          {
            htest = htest / 2 + (htest - h [i2] [i4 - 1] * 2) * (long) ((i4 - a) / (b - a) - 0.5);
            if (htest < 0) htest = 0;
            if (htest > 65535) htest = 65535;
            h [i2] [i4 - 1] = h [i2] [i4 - 1] + (int) htest;
          }
        }
      }
      i = i + (1 << i3);
    }
    i2 ++;
  }
}*/

void Landscape::genSurface (int elevationpc, int *heightmap)
{
  int i, i2, x, y;
  int htest, h1, h2, step;
  int n = LOG2MAXX;
  type = LAND_ALPINE;
  elevation = elevationpc * 512;
  init ();
  step = MAXX / 4;
  if (heightmap == NULL)
  {
    for (i = 0; i < 4; i ++)
      for (i2 = 0; i2 < 4; i2 ++)
        if (h [i * step] [i2 * step] == 0)
          h [i * step] [i2 * step] = 127 * 256 - 64 * elevation / 1024 + Math::extremeRandom (64 * elevation / 512);
    for (i = 0; i < 5; i ++)
    {
      h [MAXX] [i * step] = h [0] [i * step];
      h [i * step] [MAXX] = h [i * step] [0];
    }
  }
  else
  {
    step /= 2; n --;
    for (i = 0; i < 8; i ++)
      for (i2 = 0; i2 < 8; i2 ++)
        h [i * step] [i2 * step] = heightmap [i * 8 + i2];
    for (i = 0; i < 9; i ++)
    {
      h [MAXX] [i * step] = h [0] [i * step];
      h [i * step] [MAXX] = h [i * step] [0];
    }
  }
  for (i = 2; i < n; i ++)
  {
    step = step / 2; y = 0;
    do
    {
      x = step;
      do
      {
        h1 = h [x - step] [y]; h2 = h [x + step] [y];
        htest = ((h1 + h2) >> 1) - (128 >> i) * elevation / 128 + Math::random ((128 >> i) * elevation / 64, x, y);
        if (htest < 0) htest = 0;
        if (htest > 65535) htest = 65535;
        if (h [x] [y] == 0) h [x] [y] = htest;
        x = x + 2 * step;
      }
      while (x != MAXX + step);
      y = y + 2 * step;
    }
    while (y < MAXX);
    for (x = step; x <= MAXX - step; x += 2 * step)
      h [x] [MAXX] = h [x] [0];
    x = 0;
    do
    {
      y = step;
      do
      {
        h1 = h [x] [y - step]; h2 = h [x] [y + step];
        htest = ((h1 + h2) >> 1) - (128 >> i) * elevation / 128 + Math::random ((128 >> i) * elevation / 64, x, y);
        if (htest < 0) htest = 0;
        if (htest > 65535) htest = 65535;
        if (h [x] [y] == 0) h [x] [y] = htest;
        y = y + 2 * step;
      }
      while (y <= MAXX);
      x = x + step;
    }
    while (x < MAXX);
    for (y = step; y <= MAXX - step; y += 2 * step)
      h [MAXX] [y] = h [0] [y];
    elevation = (int) (elevation / 1.1);
  }
  convolveGauss (2, 0, 35000);
  convolveGauss (1, 35001, 65535);
  highestPoint = 0;
  lowestPoint = 65535;
  for (i = 0; i < MAXX; i ++)
    for (i2 = 0; i2 < MAXX; i2 ++)
    {
      if (h [i] [i2] > highestPoint) highestPoint = h [i] [i2];
      if (h [i] [i2] < lowestPoint) lowestPoint = h [i] [i2];
    }
}

void Landscape::genErosionSurface (int elevationpc, int *heightmap)
{
  int i, i2, x, y;
  int htest, h1, h2, step;
  int n = LOG2MAXX;
  type = LAND_ALPINE;
  elevation = elevationpc * 512;
  init ();
  step = MAXX / 4;
  if (heightmap == NULL)
  {
    for (i = 0; i < 4; i ++)
      for (i2 = 0; i2 < 4; i2 ++)
        if (h [i * step] [i2 * step] == 0)
          h [i * step] [i2 * step] = 127 * 256 - 64 * elevation / 1024 + Math::extremeRandom (64 * elevation / 512);
    for (i = 0; i < 5; i ++)
    {
      h [MAXX] [i * step] = h [0] [i * step];
      h [i * step] [MAXX] = h [i * step] [0];
    }
  }
  else
  {
    step /= 2; n --;
    for (i = 0; i < 8; i ++)
      for (i2 = 0; i2 < 8; i2 ++)
        h [i * step] [i2 * step] = heightmap [i * 8 + i2];
    for (i = 0; i < 9; i ++)
    {
      h [MAXX] [i * step] = h [0] [i * step];
      h [i * step] [MAXX] = h [i * step] [0];
    }
  }
  for (i = 2; i < n; i ++)
  {
    step = step / 2; y = 0;
    do
    {
      x = step;
      do
      {
        h1 = h [x - step] [y]; h2 = h [x + step] [y];
        htest = ((h1 + h2) >> 1) - (128 >> i) * elevation / 128 + Math::random ((128 >> i) * elevation / 64, x, y);
        if (htest < 0) htest = 0;
        if (htest > 65535) htest = 65535;
        if (h [x] [y] == 0) h [x] [y] = htest;
        x = x + 2 * step;
      }
      while (x != MAXX + step);
      y = y + 2 * step;
    }
    while (y < MAXX);
    for (x = step; x <= MAXX - step; x += 2 * step)
      h [x] [MAXX] = h [x] [0];
    x = 0;
    do
    {
      y = step;
      do
      {
        h1 = h [x] [y - step]; h2 = h [x] [y + step];
        htest = ((h1 + h2) >> 1) - (128 >> i) * elevation / 128 + Math::random ((128 >> i) * elevation / 64, x, y);
        if (htest < 0) htest = 0;
        if (htest > 65535) htest = 65535;
        if (h [x] [y] == 0) h [x] [y] = htest;
        y = y + 2 * step;
      }
      while (y <= MAXX);
      x = x + step;
    }
    while (x < MAXX);
    for (y = step; y <= MAXX - step; y += 2 * step)
      h [MAXX] [y] = h [0] [y];
    elevation = (int) (elevation / 1.1);
  }
  highestPoint = 0;
  lowestPoint = 65535;
  for (i = 0; i < MAXX; i ++)
    for (i2 = 0; i2 < MAXX; i2 ++)
    {
      if (h [i] [i2] > highestPoint) highestPoint = h [i] [i2];
      if (h [i] [i2] < lowestPoint) lowestPoint = h [i] [i2];
    }
  int erosion = (highestPoint - lowestPoint) / 5 + lowestPoint;
  for (i = 0; i < MAXX; i ++)
    for (i2 = 0; i2 < MAXX; i2 ++)
      if (h [i] [i2] < erosion)
        h [i] [i2] = erosion - 30 + Math::random (60, i, i2);
  convolveGauss (2, 0, 35000);
  convolveGauss (1, 35001, 65535);
}

void Landscape::genArcticSurface (int elevationpc, int *heightmap)
{
  int i, i2, x, y;
  int htest, h1, h2, step;
  int n = LOG2MAXX;
  type = LAND_ARCTIC;
  elevation = elevationpc * 512;
  init ();
  step = MAXX / 4;
  if (heightmap == NULL)
  {
    for (i = 0; i < 4; i ++)
      for (i2 = 0; i2 < 4; i2 ++)
        if (h [i * step] [i2 * step] == 0)
          h [i * step] [i2 * step] = 127 * 256 - 64 * elevation / 1024 + Math::extremeRandom (64 * elevation / 512);
    for (i = 0; i < 5; i ++)
    {
      h [MAXX] [i * step] = h [0] [i * step];
      h [i * step] [MAXX] = h [i * step] [0];
    }
  }
  else
  {
    step /= 2; n --;
    for (i = 0; i < 8; i ++)
      for (i2 = 0; i2 < 8; i2 ++)
        h [i * step] [i2 * step] = heightmap [i * 8 + i2];
    for (i = 0; i < 9; i ++)
    {
      h [MAXX] [i * step] = h [0] [i * step];
      h [i * step] [MAXX] = h [i * step] [0];
    }
  }
  for (i = 2; i < n; i ++)
  {
    step = step / 2; y = 0;
    do
    {
      x = step;
      do
      {
        h1 = h [x - step] [y]; h2 = h [x + step] [y];
        htest = ((h1 + h2) >> 1) - (128 >> i) * elevation / 128 + Math::random ((128 >> i) * elevation / 64, x, y);
        if (htest < 0) htest = 0;
        if (htest > 65535) htest = 65535;
        if (h [x] [y] == 0) h [x] [y] = htest;
        x = x + 2 * step;
      }
      while (x != MAXX + step);
      y = y + 2 * step;
    }
    while (y < MAXX);
    for (x = step; x <= MAXX - step; x += 2 * step)
      h [x] [MAXX] = h [x] [0];
    x = 0;
    do
    {
      y = step;
      do
      {
        h1 = h [x] [y - step]; h2 = h [x] [y + step];
        htest = ((h1 + h2) >> 1) - (128 >> i) * elevation / 128 + Math::random ((128 >> i) * elevation / 64, x, y);
        if (htest < 0) htest = 0;
        if (htest > 65535) htest = 65535;
        if (h [x] [y] == 0) h [x] [y] = htest;
        y = y + 2 * step;
      }
      while (y <= MAXX);
      x = x + step;
    }
    while (x < MAXX);
    for (y = step; y <= MAXX - step; y += 2 * step)
      h [MAXX] [y] = h [0] [y];
    elevation = (int) (elevation / 1.1);
  }
  convolveGauss (2, 0, 35000);
  highestPoint = 0;
  lowestPoint = 65535;
  for (i = 0; i < MAXX; i ++)
    for (i2 = 0; i2 < MAXX; i2 ++)
    {
      if (h [i] [i2] > highestPoint) highestPoint = h [i] [i2];
      if (h [i] [i2] < lowestPoint) lowestPoint = h [i] [i2];
      f [i] [i2] = GLACIER;
      if (abs (h [i] [i2] - h [i + 1] [i2]) > 300) f [i] [i2] = ROCKS;
      if (abs (h [i] [i2] - h [i] [i2 + 1]) > 300) f [i] [i2] = ROCKS;
      if (h [i] [i2] < 32836 - elevationpc * 3)
      {
        int dh = 32836 - elevationpc * 3 - h [i] [i2];
        if (dh < 2000)
          f [i] [i2] = SHALLOWWATER;
        else
          f [i] [i2] = DEEPWATER;
        hw [i] [i2] = 32836 - elevationpc * 3;
      }
    }
}

void Landscape::genMoonSurface (int height)
{
  int i, i2, i3, x, y;
  int radius, rad, depth, depth2;
  double pi = 3.14;
  int n = LOG2MAXX;

  type = LAND_MOON;
  for (i = 0; i <= MAXX; i ++)
    for (i2 = 0; i2 <= MAXX; i2 ++)
      h [i] [i2] = 30000 + Math::random (300, i, i2);

  int maxholes = 1;
  for (i = 0; i < maxholes; i ++)
  {
    x = Math::random (MAXX + 1);
    y = Math::random (MAXX + 1);
    radius = Math::random (50) + 200;
    depth = radius * height / 2;
    for (i2 = x - radius; i2 < x+radius; i2++)
      for (i3 = y - radius; i3 < y+radius; i3++)
      {
         rad = (int) sqrt ((float) ((x-i2)*(x-i2)+(y-i3)*(y-i3)));
         if (rad < radius)
         {
           if (rad < 0.67 * radius)
           {
             depth2 = (int) (cos ((float)rad / (2*radius/3) * pi/2) * depth);
           }
           else
           {
             depth2 = (int) (0.3 * sin ((float)(radius - rad) / (radius / 3) * pi) * depth * (-1));
           }
           h[GETCOORD(i2)][GETCOORD(i3)] -= depth2;
         }
      }
  }

  maxholes = Math::random (4) + 8 ;
  for (i = 0; i < maxholes; i++)
  {
    x = Math::random (MAXX + 1);
    y = Math::random (MAXX + 1);
    radius = Math::random (50) + 50;
    depth = radius * height / 2;
    for (i2 = x - radius; i2 < x+radius; i2++)
      for (i3 = y - radius; i3 < y+radius; i3++)
      {
         rad = (int) sqrt ((float) ((x-i2)*(x-i2)+(y-i3)*(y-i3)));
         if (rad < radius)
         {
           if (rad < 0.67 * radius)
           {
             depth2 = (int) (cos ((float)rad / (2*radius/3) * pi/2) * depth);
           }
           else
           {
             depth2 = (int) (0.3 * sin ((float)(radius - rad) / (radius / 3) * pi) * depth * (-1));
           }
           h[GETCOORD(i2)][GETCOORD(i3)] -= depth2;
         }
      }

  }

  maxholes = Math::random (10) + 20 ;
  for (i = 0; i < maxholes; i++)
  {
    x = Math::random (MAXX + 1);
    y = Math::random (MAXX + 1);
    radius = Math::random (20) + 20;
    depth = radius * height / 2;
    for (i2 = x - radius; i2 < x+radius; i2++)
      for (i3 = y - radius; i3 < y+radius; i3++)
      {
         rad = (int) sqrt ((float) ((x-i2)*(x-i2)+(y-i3)*(y-i3)));
         if (rad < radius)
         {
           if (rad < 0.67 * radius)
           {
             depth2 = (int) (cos ((float)rad / (2*radius/3) * pi/2) * depth);
           }
           else
           {
             depth2 = (int) (0.6 * sin ((float)(radius - rad) / (radius / 3) * pi) * depth * (-1));
           }
           h[GETCOORD(i2)][GETCOORD(i3)] -= depth2;
         }
      }
  }

  maxholes = Math::random (50) + 400;
  for (i = 0; i < maxholes; i++)
  {
    x = Math::random (MAXX + 1);
    y = Math::random (MAXX + 1);
    radius = Math::random (10) + 5;
    depth = radius * height / 3;
    for (i2 = x - radius; i2 < x+radius; i2++)
      for (i3 = y - radius; i3 < y+radius; i3++)
      {
         rad = (int) sqrt ((float) ((x-i2)*(x-i2)+(y-i3)*(y-i3)));
         if (rad < radius)
         {
           if (rad < 0.67 * radius) depth2 = (int) (cos ((float)rad / (2*radius/3) * pi/2) * depth);
           else depth2 = (int) (sin ((float)(radius - rad) / (radius / 3) * pi) * depth * (-1));
           h[GETCOORD(i2)][GETCOORD(i3)] -= depth2;
         }
      }

  }
  convolveGauss (2, 0, 65535);
}

void Landscape::genCanyonSurface (int elevationpc)
{
  int i, i2, x, y;
  int htest, h1, h2, step;
  int n = LOG2MAXX;
  type = LAND_CANYON;
  elevation = elevationpc * 512;
  init ();
  step = MAXX / 16;
  int minh = 0;
  int maxh = 127 * 256 + 64 * elevation / 1024;
    for (i = 0; i < 16; i ++)
      for (i2 = 0; i2 < 16; i2 ++)
        if (h [i * step] [i2 * step] == 0)
          if (Math::random (2, i, i2))
            h [i * step] [i2 * step] = 127 * 256 - 64 * elevation / 1024;
          else
            h [i * step] [i2 * step] = 127 * 256 + 64 * elevation / 1024;
    for (i = 0; i < 17; i ++)
    {
      h [MAXX] [i * step] = h [0] [i * step];
      h [i * step] [MAXX] = h [i * step] [0];
    }
  for (i = 4; i < n; i ++)
  {
    step = step / 2; y = 0;
    do
    {
      x = step;
      do
      {
        h1 = h [x - step] [y]; h2 = h [x + step] [y];
        htest = ((h1 + h2) >> 1) - (128 >> i) * elevation / 128 + Math::random ((128 >> i) * elevation / 64, x, y);
        if (htest < minh) htest = minh;
        if (htest > maxh) htest = maxh;
        if (h [x] [y] == 0) h [x] [y] = htest;
        x = x + 2 * step;
      }
      while (x != MAXX + step);
      y = y + 2 * step;
    }
    while (y < MAXX);
    for (x = step; x <= MAXX - step; x += 2 * step)
      h [x] [MAXX] = h [x] [0];
    x = 0;
    do
    {
      y = step;
      do
      {
        h1 = h [x] [y - step]; h2 = h [x] [y + step];
        htest = ((h1 + h2) >> 1) - (128 >> i) * elevation / 128 + Math::random ((128 >> i) * elevation / 64, x, y);
        if (htest < minh) htest = minh;
        if (htest > maxh) htest = maxh;
        if (h [x] [y] == 0) h [x] [y] = htest;
        y = y + 2 * step;
      }
      while (y <= MAXX);
      x = x + step;
    }
    while (x < MAXX);
    for (y = step; y <= MAXX - step; y += 2 * step)
      h [MAXX] [y] = h [0] [y];
    elevation = (int) (elevation / 1.1);
  }
  for (i = 0; i <= MAXX; i ++)
    for (i2 = 0; i2 <= MAXX; i2 ++)
      if (h [i] [i2] > 32836 - elevationpc * 30)
        h [i] [i2] -= h [i] [i2] % 2600;
  for (i = 0; i <= MAXX; i ++)
    for (i2 = 0; i2 <= MAXX; i2 ++)
      f [i] [i2] = REDSTONE;
  convolveGauss (3, 0, 35000);
  convolveGauss (2, 35001, 65535);
  for (i = 0; i < MAXX; i ++)
    for (i2 = 0; i2 < MAXX; i2 ++)
    {
      if (abs (h [i] [i2] - h [i + 1] [i2]) < 200 && abs (h [i] [i2] - h [i] [i2 + 1]) < 200)
      {
        f [i] [i2] = REDSAND;
        if (h [i] [i2] > 30000 && !Math::random (80) && elevationpc > 20)
          f [i] [i2] = REDTREE0;
      }
      if (elevationpc > 20)
      {
        if (h [i] [i2] < 32836 - elevationpc * 40)
        {
          int dh = 32836 - elevationpc * 40 - h [i] [i2];
          if (dh < 1500)
            f [i] [i2] = SHALLOWWATER;
          else
            f [i] [i2] = DEEPWATER;
          hw [i] [i2] = 32836 - elevationpc * 40;
        }
      }
    }

  highestPoint = 0;
  lowestPoint = 65535;
  for (i = 0; i < MAXX; i ++)
    for (i2 = 0; i2 < MAXX; i2 ++)
    {
      if (h [i] [i2] > highestPoint) highestPoint = h [i] [i2];
      if (h [i] [i2] < lowestPoint) lowestPoint = h [i] [i2];
    }
}

void Landscape::genDesertSurface (int elevationpc)
{
  int i, i2, x, y;
  int htest, h1, h2, step;
  int n = LOG2MAXX;
  type = LAND_DESERT;
  elevation = elevationpc * 512;
  init ();
  step = MAXX / 16;
    for (i = 0; i < 16; i ++)
      for (i2 = 0; i2 < 16; i2 ++)
        if (h [i * step] [i2 * step] == 0)
          h [i * step] [i2 * step] = 127 * 256 - 64 * elevation / 1024 + Math::extremeRandom (64 * elevation / 512);
    for (i = 0; i < 17; i ++)
    {
      h [MAXX] [i * step] = h [0] [i * step];
      h [i * step] [MAXX] = h [i * step] [0];
    }
  for (i = 4; i < n; i ++)
  {
    step = step / 2; y = 0;
    do
    {
      x = step;
      do
      {
        h1 = h [x - step] [y]; h2 = h [x + step] [y];
        htest = ((h1 + h2) >> 1) - (128 >> i) * elevation / 128 + Math::random ((128 >> i) * elevation / 64, x, y);
        if (htest < 0) htest = 0;
        if (htest > 65535) htest = 65535;
        if (h [x] [y] == 0) h [x] [y] = htest;
        x = x + 2 * step;
      }
      while (x != MAXX + step);
      y = y + 2 * step;
    }
    while (y < MAXX);
    for (x = step; x <= MAXX - step; x += 2 * step)
      h [x] [MAXX] = h [x] [0];
    x = 0;
    do
    {
      y = step;
      do
      {
        h1 = h [x] [y - step]; h2 = h [x] [y + step];
        htest = ((h1 + h2) >> 1) - (128 >> i) * elevation / 128 + Math::random ((128 >> i) * elevation / 64, x, y);
        if (htest < 0) htest = 0;
        if (htest > 65535) htest = 65535;
        if (h [x] [y] == 0) h [x] [y] = htest;
        y = y + 2 * step;
      }
      while (y <= MAXX);
      x = x + step;
    }
    while (x < MAXX);
    for (y = step; y <= MAXX - step; y += 2 * step)
      h [MAXX] [y] = h [0] [y];
    elevation = (int) (elevation / 1.5);
  }
  convolveGauss (2, 0, 65535);
  int i3;
  for (i = 0; i <= MAXX; i ++)
    for (i2 = 0; i2 <= MAXX; i2 ++)
    {
      const int range = 11;
      int ha [range];
      for (i3 = 0; i3 < range; i3 ++)
        ha [i3] = h [i] [GETCOORD(i2 + i3 - range / 2)];
      int hmin = 65000, hmax = 0;
      for (i3 = 0; i3 < range; i3 ++)
      {
        if (ha [i3] < hmin) hmin = ha [i3];
        if (ha [i3] > hmax) hmax = ha [i3];
      }
      if (ha [range / 2 + 1] < ha [range / 2 - 1])
      {
        hg [i] [i2] = hmin;
      }
      else
      {
        hg [i] [i2] = hmax;
      }
    }
  convolveGauss (2, 0, 65535);
  for (i = 0; i <= MAXX; i ++)
    for (i2 = 0; i2 <= MAXX; i2 ++)
      h [i] [i2] = hg [i] [i2];
  for (i = 0; i <= MAXX; i ++)
    for (i2 = 0; i2 <= MAXX; i2 ++)
      f [i] [i2] = DESERTSAND;
  for (i = 0; i < MAXX; i += 4)
    for (i2 = 0; i2 < MAXX; i2 += 4)
    {
      if (!Math::random (50))
        f [i] [i2] = CACTUS0;
    }
  highestPoint = 0;
  lowestPoint = 65535;
  for (i = 0; i < MAXX; i ++)
    for (i2 = 0; i2 < MAXX; i2 ++)
    {
      if (h [i] [i2] > highestPoint) highestPoint = h [i] [i2];
      if (h [i] [i2] < lowestPoint) lowestPoint = h [i] [i2];
    }
}

void Landscape::genTrench (int width, int height)
{
  int i, i2;
  int midleft = MAXX / 2 - width / 2, midright = MAXX / 2 + width / 2;
  int left = 0, right = 0;
  for (i = 0; i <= MAXX; i ++)
  {
    if (i != 148 && i != 52)
    {
    for (i2 = midleft + left; i2 < midright + right; i2 ++)
    {
      int height1 = height;
      if (i2 - midleft - left < 4)
      {
        height1 = height1 * (i2 - midleft - left) / 4;
        f [i] [i2] = REDSTONE;
      }
      if (i2 > midright + right - 4)
      {
        height1 = height1 * (midright + right - i2) / 4;
        f [i] [i2] = REDSTONE;
      }
      h [i] [i2] -= height1;
    }
    if (i & 1)
    {
      left += Math::random (5) - 2;
      right += Math::random (5) - 2;
      if (left < -5) left = -5;
      if (left > 5) left = 5;
      if (right < -5) right = -5;
      if (right > 5) right = 5;
    }
    }
    else
    {
      h [i] [MAXX / 2] -= height;
    }
  }
  lowestPoint -= height;
}



/****************************************************************************
  CREATE ROCKS
****************************************************************************/

void Landscape::genRocks (int diffmin, int percent)
{
  int i, i2, i3, i4, i5, d = 150;
  long hmin, hmax;
  long htest;
  double r, rd;
  rockBorder = 65535;
  hmax = highestPoint; hmin = lowestPoint;
  if (hmax - hmin >= (long) diffmin * 256) // Generation
    for (i = 0; i < MAXX; i ++)
      for (i2 = 0; i2 < MAXX; i2 ++)
      {
        r = (hmax - hmin) / 100 * percent;
        rd = h [i] [i2];
        rd = rd - h [i + 1] [i2] / 3 - h [i] [i2 + 1] / 3 - h [i + 1] [i2 + 1] / 3;
        rd = fabs (rd);
        int minrock = 0;
        if (percent > 50) minrock = (percent - 50) * 8;
        if (h [i] [i2] > hmax - r)
        {
          if (rd + minrock >= 800 - 800 * (h [i] [i2] - r) / (hmax - r))
          {
            f [i] [i2] = ROCKS + Math::random (2);
            htest = h [i] [i2] - d + Math::random (d * 2);
            if (htest > 65535) htest = 65535;
            h [i] [i2] = (unsigned short) htest;
            if (h [i] [i2] < rockBorder) rockBorder = h [i] [i2];
          }
          else if (rd + minrock >= 500 - 500 * (h [i] [i2] - r) / (hmax - r))
          {
            f [i] [i2] = GRAVEL;
          }
        }
        if (abs (h [i] [i2] - h [i + 1] [i2]) >= 600 || abs (h [i] [i2] - h [i] [i2 + 1]) >= 600)
        {
          f [i] [i2] = ROCKS + Math::random (2);
          htest = h [i] [i2] - d + Math::random (d * 2);
          if (htest > 65535) htest = 65535;
          h [i] [i2] = (unsigned short) htest;
        }
        if (h [i] [i2] > hmax - r / 2)
          if (rd < 200 * (h [i] [i2] - r / 2) / (hmax - r / 2))
            f [i] [i2] = GLACIER;
      }
  for (i = 1; i < MAXX; i ++) // Lücken schließen
    for (i2 = 1; i2 < MAXX; i2 ++)
    {
      if (f [i] [i2] != ROCKS)
        if (f [i - 1] [i2] == ROCKS && f [i + 1] [i2] == ROCKS ||
            f [i] [i2 - 1] == ROCKS && f [i] [i2 + 1] == ROCKS)
          f [i] [i2] = ROCKS;
      if (f [i] [i2] != ROCKS)
        if (f [i - 1] [i2] == GLACIER && f [i + 1] [i2] == GLACIER ||
            f [i] [i2 - 1] == GLACIER && f [i] [i2 + 1] == GLACIER)
          f [i] [i2] = GLACIER;
    }
  for (i = 1; i < MAXX; i ++) // Felsengebiet uneben
    for (i2 = 1; i2 < MAXX; i2 ++)
      if (f [i] [i2] == ROCKS)
      {
        i5 = 0;
        for (i3 = -1; i3 <= 1; i3 ++)
          for (i4 = -1; i4 <= 1; i4 ++)
            if (f [i + i3] [i2 + i4] == ROCKS)
              i5 ++;
        htest = (long) h [i] [i2] + (long) i5 * 50;
        if (htest > 65535) htest = 65535;
        h [i] [i2] = (unsigned short) htest;
      }
  smoothGlacier ();
}



/****************************************************************************
  CREATE LAKES
****************************************************************************/

int Landscape::calcLake (int ys, int xs, unsigned short level, int num, int maxextent)
{
  int i, i2, n, w, timeout = 0;
  bool hits;
  unsigned char sn = (unsigned char) num;
  int extent = 0;
//  h [xs] [ys] = level;
  w = 5;
  if (xs <= 0) xs = 1;
  if (ys <= 0) ys = 1;
  if (xs >= MAXX) xs = MAXX - 1;
  if (ys >= MAXX) ys = MAXX - 1;
  ftry [xs] [ys] = sn;
  do
  {
    timeout ++;
    i2 = 0; hits = false;
    while (i2 < MAXX * 2)
    {
      i2 ++;
      n = i2;
      if (n > MAXX) n = n - MAXX * 2 + 1;
      for (i = xs; i < MAXX; i ++)
      {
        if (ys - n >= 0 && ys - n < MAXX)
          if (h [i] [ys - n] < level && ftry [i] [ys - n] != sn)
            if ((ftry [i - 1] [ys - n] == sn) || (ftry [i] [ys - n + 1] == sn) ||
                (ftry [i + 1] [ys - n + 1] == sn) || (ftry [i - 1] [ys - n + 1] == sn))
            {
              ftry [i] [ys - n] = sn; extent ++; hits = true;
            }
        if (ys + n > 0 && ys + n <= MAXX)
          if (h [i] [ys + n] < level && ftry [i] [ys + n] != sn)
            if ((ftry [i - 1] [ys + n] == sn) || (ftry [i] [ys + n - 1] == sn) ||
                (ftry [i + 1] [ys + n - 1] == sn) || (ftry [i - 1] [ys + n - 1] == sn))
            {
              ftry [i] [ys + n] = sn; extent ++; hits = true;
            }
      }
      for (i = xs; i > 0; i --)
      {
        if (ys - n >= 0 && ys - n < MAXX)
          if (h [i] [ys - n] < level && ftry [i] [ys - n] != sn)
            if ((ftry [i + 1] [ys - n] == sn) || (ftry [i] [ys - n + 1] == sn) ||
                (ftry [i + 1] [ys - n + 1] == sn) || (ftry [i - 1] [ys - n + 1] == sn))
            {
              ftry [i] [ys - n] = sn; extent ++; hits = true;
            }
        if (ys + n > 0 && ys + n <= MAXX)
          if (h [i] [ys + n] < level && ftry [i] [ys + n] != sn)
            if ((ftry [i + 1] [ys + n] == sn) || (ftry [i] [ys + n - 1] == sn) ||
                (ftry [i + 1] [ys + n - 1] == sn) || (ftry [i - 1] [ys + n - 1] == sn))
            {
              ftry [i] [ys + n] = sn; extent ++; hits = true;
            }
      }
      for (i = ys; i < MAXX; i ++)
      {
        if (xs - n >= 0 && xs - n < MAXX)
          if (h [xs - n] [i] < level && ftry [xs - n] [i] != sn)
            if ((ftry [xs - n] [i - 1] == sn) || (ftry [xs - n + 1] [i] == sn) ||
                (ftry [xs - n + 1] [i + 1] == sn) || (ftry [xs - n + 1] [i - 1] == sn))
            {
              ftry [xs - n] [i] = sn; extent ++; hits = true;
            }
        if (xs + n > 0 && xs + n <= MAXX)
          if (h [xs + n] [i] < level && ftry [xs + n] [i] != sn)
            if ((ftry [xs + n] [i - 1] == sn) || (ftry [xs + n - 1] [i] == sn) ||
                (ftry [xs + n - 1] [i + 1] == sn) || (ftry [xs + n - 1] [i - 1] == sn))
            {
              ftry [xs + n] [i] = sn; extent ++; hits = true;
            }
      }
      for (i = ys; i > 0; i --)
      {
        if (xs - n >= 0 && xs - n < MAXX)
          if (h [xs - n] [i] < level && ftry [xs - n] [i] != sn)
            if ((ftry [xs - n] [i + 1] == sn) || (ftry [xs - n + 1] [i] == sn) ||
                (ftry [xs - n + 1] [i + 1] == sn) || (ftry [xs - n + 1] [i - 1] == sn))
            {
              ftry [xs - n] [i] = sn; extent ++; hits = true;
            }
        if (xs + n > 0 && xs + n <= MAXX)
          if (h [xs + n] [i] < level && ftry [xs + n] [i] != sn)
            if ((ftry [xs + n] [i - 1] == sn) || (ftry [xs + n - 1] [i] == sn) ||
                (ftry [xs + n - 1] [i + 1] == sn) || (ftry [xs + n - 1] [i - 1] == sn))
            {
              ftry [xs + n] [i] = sn; extent ++; hits = true;
            }
      }
    }
    if (extent > maxextent)
      return -1;
  }
  while (hits && timeout < 10);
  if (timeout >= 10)
    return -1;
  return extent;
}

void Landscape::genLake (int depthpc)
{
  for (int j = 0; j < 5; j ++)
  {
    int a1, i, i2;
    unsigned short level, zmin;
    int xs = 0, ys = 0;
    unsigned short depth = (highestPoint - lowestPoint) * depthpc / 100;
    zmin = 65535;
    int radius = MAXX * 2 * depthpc / 100;
    a1 = MAXX / 8;
    for (i = 0; i < 20; i ++)
    {
      xs = Math::random (MAXX - 2 * a1) + a1;
      ys = Math::random (MAXX - 2 * a1) + a1;
      if (hw [ys] [xs] == 0) break;
    }
    if (i == 20) return;
    int minx = xs - radius, miny = ys - radius;
    int maxx = xs + radius, maxy = ys + radius;
    if (minx < 1) minx = 1;
    if (maxx > MAXX - 1) maxx = MAXX - 1;
    if (miny < 1) miny = 1;
    if (maxy > MAXX - 1) maxy = MAXX - 1;
    xs = -1; ys = -1;
    for (i = miny; i < maxy; i += 2)
      for (i2 = minx; i2 < maxx; i2 += 2)
        if (h [i] [i2] < zmin && hw [i] [i2] == 0)
        {
          zmin = h [i] [i2];
          xs = i2; ys = i;
        }
    int extent = 0;
    if (xs >= 0)
    {
      level = zmin + depth;
      for (i = 0; i <= MAXX; i ++)
        for (i2 = 0; i2 <= MAXX; i2 ++)
        {
          ftry [i] [i2] = GRASS;
        }
      extent = calcLake (xs, ys, level, TRYLAKE, radius * radius / 2);
      if (extent <= 0) break;
      for (i = 0; i <= MAXX; i ++)
      {
        if (ftry [i] [0] != ftry [i] [MAXX])
        {
          if (ftry [i] [0] == TRYLAKE)
            extent += calcLake (MAXX, i, level, TRYLAKE, radius * radius / 2);
          else
            extent += calcLake (0, i, level, TRYLAKE, radius * radius / 2);
          if (extent <= 0) break;
        }
      }
      if (extent > 0)
      for (i = 0; i <= MAXX; i ++)
      {
        if (ftry [0] [i] != ftry [MAXX] [i])
        {
          if (ftry [0] [i] == TRYLAKE)
            extent += calcLake (i, MAXX, level, TRYLAKE, radius * radius / 2);
          else
            extent += calcLake (i, 0, level, TRYLAKE, radius * radius / 2);
          if (extent <= 0) break;
        }
      }
      int waterlevel1 = 300;
      int waterlevel2 = 600;
      int waterlevel3 = 1400;
      int waterlevel4 = 2000;
      if (type == LAND_CANYON)
      {
        waterlevel1 = 400;
        waterlevel2 = 800;
        waterlevel3 = 1500;
        waterlevel4 = 2000;
      }
      for (i = 0; i <= MAXX; i ++)
        for (i2 = 0; i2 <= MAXX; i2 ++)
          if (ftry [i] [i2] == TRYLAKE)
          {
            ftry [i] [i2] = GRASS;
            if (level - h [i] [i2] < waterlevel1)
              f [i] [i2] = XSHALLOWWATER;
            else if (level - h [i] [i2] < waterlevel2)
              f [i] [i2] = SHALLOWWATER;
            else if (level - h [i] [i2] < waterlevel3)
              f [i] [i2] = WATER;
            else if (level - h [i] [i2] < waterlevel4)
              f [i] [i2] = DEEPWATER;
            else
              f [i] [i2] = XDEEPWATER;
            hw [i] [i2] = level; // + Math::random (w);
          }

      int barrage = ROCKS;
      for (i = 0; i <= MAXX; i ++)
      {
        if (hw [i] [0] != hw [i] [MAXX]) // error: water and landscape at different heights => make a barrage
        {
          if (hw [i] [0] == 0)
          {
            h [i] [MAXX - 2] = hw [i] [MAXX] + 100;
            h [i] [MAXX - 1] = hw [i] [MAXX] + 100;
            h [i] [MAXX] = h [i] [MAXX - 1];
            h [i] [0] = h [i] [MAXX];
            for (i2 = 1; i2 < 5; i2 ++)
              if (h [i] [i2] < h [i] [0] - i2 * 1000)
              {
                h [i] [i2] = h [i] [0] - i2 * 1000;
                f [i] [i2] = barrage;
                f [i] [i2 + 1] = barrage;
              }
            f [i] [MAXX - 2] = barrage;
            f [i] [MAXX - 1] = barrage;
            f [i] [MAXX] = barrage;
            f [i] [0] = barrage;
            f [i] [1] = barrage;
          }
          else if (hw [i] [MAXX] == 0)
          {
            h [i] [2] = hw [i] [0] + 100;
            h [i] [1] = hw [i] [0] + 100;
            h [i] [0] = h [i] [1];
            h [i] [MAXX] = h [i] [0];
            for (i2 = 1; i2 < 5; i2 ++)
              if (h [i] [MAXX - i2] < h [i] [MAXX] - i2 * 1000)
              {
                h [i] [MAXX - i2] = h [i] [MAXX] - i2 * 1000;
                f [i] [MAXX - i2] = barrage;
                f [i] [MAXX - i2 - 1] = barrage;
              }
            f [i] [2] = barrage;
            f [i] [1] = barrage;
            f [i] [0] = barrage;
            f [i] [MAXX] = barrage;
            f [i] [MAXX - 1] = barrage;
          }
        }
        if (hw [0] [i] != hw [MAXX] [i]) // error: water and landscape at different heights => make a barrage
        {
          if (hw [0] [i] == 0)
          {
            h [MAXX - 2] [i] = hw [MAXX] [i] + 100;
            h [MAXX - 1] [i] = hw [MAXX] [i] + 100;
            h [MAXX] [i] = h [MAXX - 1] [i];
            h [0] [i] = h [MAXX] [i];
            for (i2 = 1; i2 < 5; i2 ++)
              if (h [i2] [i] < h [0] [i] - i2 * 1000)
              {
                h [i2] [i] = h [0] [i] - i2 * 1000;
                f [i2] [i] = barrage;
                f [i2 + 1] [i] = barrage;
              }
            f [MAXX - 2] [i] = barrage;
            f [MAXX - 1] [i] = barrage;
            f [MAXX] [i] = barrage;
            f [0] [i] = barrage;
            f [1] [i] = barrage;
          }
          else if (hw [MAXX] [i] == 0)
          {
            h [2] [i] = hw [0] [i] + 100;
            h [1] [i] = hw [0] [i] + 100;
            h [0] [i] = h [1] [i];
            h [MAXX] [i] = h [0] [i];
            for (i2 = 1; i2 < 5; i2 ++)
              if (h [MAXX - i2] [i] < h [MAXX] [i] - i2 * 1000)
              {
                h [MAXX - i2] [i] = h [MAXX] [i] - i2 * 1000;
                f [MAXX - i2] [i] = barrage;
                f [MAXX - i2 - 1] [i] = barrage;
              }
            f [2] [i] = barrage;
            f [1] [i] = barrage;
            f [0] [i] = barrage;
            f [MAXX] [i] = barrage;
            f [MAXX - 1] [i] = barrage;
          }
        }
      }
      return;
    }
  }
}



/****************************************************************************
  CREATE WOODS
****************************************************************************/

void Landscape::calcWoods (int dy)
{
  int i, i2;
  int var = 2000;
  for (i = 0; i < MAXX; i ++)
    for (i2 = 0; i2 < MAXX; i2 ++)
    {
      if (fabs ((float) (h [i] [i2] - h [i + 1] [i2])) > dy)
        if (f [i] [i2] == 0)
        {
          int ra = Math::random (3);
          if (h [i] [i2] >= rockBorder + 3000 + Math::random (var)) f [i] [i2] = (DWARFPINES1 + ra);
          else if (h [i] [i2] >= 15000 + Math::random (var) && h [i] [i2] < rockBorder - 10000 + Math::random (var)) f [i] [i2] = (MIXEDWOODS1 + ra);
          else if (h [i] [i2] < 15000 + var) f [i] [i2] = (CONIFEROUSWOODS1 + ra);
          else f [i] [i2] = (DECIDUOUSWOODS1 + ra);
        }
    }
  for (i = 1; i < MAXX; i ++)
    for (i2 = 1; i2 < MAXX; i2 ++)
      if (f [i] [i2] == 0)
        if (isWoods (f [i - 1] [i2]) && isWoods (f [i + 1] [i2]) ||
            isWoods (f [i] [i2 - 1]) && isWoods (f [i] [i2 + 1]))
        {
          int ra = Math::random (3);
          if (h [i] [i2] >= rockBorder + 3000 + Math::random (var)) f [i] [i2] = (DWARFPINES1 + ra);
          else if (h [i] [i2] >= 15000 + Math::random (var) && h [i] [i2] < rockBorder - 10000 + Math::random (var)) f [i] [i2] = (BUSHES1 + ra);
          else if (h [i] [i2] < 15000 + var) f [i] [i2] = (BUSHES1 + ra);
          else f [i] [i2] = (BUSHES1 + ra);
        }
  for (i = 4; i < MAXX - 4; i ++)
    for (i2 = 4; i2 < MAXX - 4; i2 ++)
      if (isWoods (f [i - 4] [i2]) && isWoods (f [i + 4] [i2]) &&
          isWoods (f [i] [i2 - 4]) && isWoods (f [i] [i2 + 4]) &&
      	  isWoods (f [i] [i2]))
      {
        f [i] [i2] --;
      }
  for (i = 1; i < MAXX; i ++)
    for (i2 = 1; i2 < MAXX; i2 ++)
      if (f [i] [i2] == f [i + 1] [i2] && f [i] [i2] == f [i - 1] [i2] &&
          f [i] [i2] == f [i] [i2 + 1] && f [i] [i2] == f [i] [i2 - 1])
	    {
        if ((f [i] [i2] & 4) == 1) f [i] [i2] ++;
        else if ((f [i] [i2] & 4) == 2) f [i] [i2] --;
      }
  for (i = 1; i < MAXX; i += 3)
    for (i2 = 1; i2 < MAXX; i2 += 3)
      if (isWater (f [i] [i2]))
      {
        if (f [i + 1] [i2] == GRASS) f [i + 1] [i2] = BUSHES1 + Math::random (3);
        if (f [i - 1] [i2] == GRASS) f [i - 1] [i2] = BUSHES1 + Math::random (3);
        if (f [i] [i2 + 1] == GRASS) f [i] [i2 + 1] = BUSHES1 + Math::random (3);
        if (f [i] [i2 - 1] == GRASS) f [i] [i2 - 1] = BUSHES1 + Math::random (3);
      }
}



/****************************************************************************
  CREATE RIVERS
****************************************************************************/

/*bool Landscape::riverCheck (int x, int y, int *fl, int z, int z2)
{
  if (x <= 0 || x >= MAXX || y <= 0 || y >= MAXX)
    return false;
  if (isType (f [x] [y], GRASS) || isWoods (f [x] [y]))
  {
    int bad = 0;
    for (int i = z; i <= z2; i ++)
      if (abs (fl [i * 2] - x) + abs (fl [i * 2 + 1] - y) <= 1)
        bad ++;
    if (bad <= 2) return true;
  }
  return false;
}

void Landscape::genRiver ()
{
  bool ok, ok2;
  int lang;
  int i, i2, i3;
  int fl1 = 0, fl2 = 0, fl3 = 0, fl4 = 0, z, z2, xf, yf, xf2, yf2, h1, h2;
  int fl [400] [2];
  int abbruch;
  abbruch = 0;
  lang = 30;
  do
  {
    abbruch ++;
    do
    {
      xf = Math::random (MAXX - 19) + 10; yf = Math::random (MAXX - 19) + 10;
    }
    while (!isGround (yf, xf));
    fl [200] [0] = xf; fl [200] [1] = yf;
    xf2 = xf; yf2 = yf;
    z = 0; z2 = 0;
    do
    {
      h1 = 65535;
      ok = true;
      for (i = -1; i <= 1; i ++)
        for (i2 = -1; i2 <= 1; i2 ++)
          if (xf + i >= 0 && xf + i <= MAXX &&
              yf + i2 >= 0 && yf + i2 <= MAXX)
            if (riverCheck (xf + i, yf + i2, &fl [0] [0], 200 - z, 200 + z2))
              if (h [xf + i] [yf + i2] < h [xf] [yf])
                if (h [xf] [yf] - h [xf + i] [yf + i2] < h1)
                {
                  h1 = h [xf] [yf] - h [xf + i] [yf + i2];
                  fl1 = xf + i; fl2 = yf + i2;
                  ok = false;
                }
      h2 = 65535;
      ok2 = true;
      for (i = -1; i <= 1; i ++)
        for (i2 = -1; i2 <= 1; i2 ++)
          if (xf2 + i >= 0 && xf2 + i <= MAXX &&
              yf2 + i2 >= 0 && yf2 + i2 <= MAXX)
            if (riverCheck (xf2 + i, yf2 + i2, &fl [0] [0], 200 - z, 200 + z2))
              if (h [xf2 + i] [yf2 + i2] > h [xf2] [yf2])
                if (h [xf2 + i] [yf2 + i2] - h [xf2] [yf2] < h2)
                {
                  h2 = h [xf2 + i] [yf2 + i2] - h [xf2] [yf2];
                  fl3 = xf2 + i; fl4 = yf2 + i2;
                  ok2 = false;
                }
      if (!ok)
      {
        z ++;
        fl [200 - z] [0] = fl1; fl [200 - z] [1] = fl2;
        xf = fl1; yf = fl2;
      }
      if (!ok2)
      {
        z2 ++;
        fl [200 + z2] [0] = fl3; fl [200 + z2] [1] = fl4;
        xf2 = fl3; yf2 = fl4;
      }
    }
    while ((!ok || !ok2) && (z < 200) && (z2 < 200)); //ok and ok2 or (z >= 200) or (z2 >= 200);
  }
  while (z + z2 <= lang && abbruch <= 10); //until (z + z2 > lang) or (abbruch > 2000);
  if (abbruch <= 10)
  {
    if (z > 200) z = 200;
    if (z2 > 200) z2 = 200;
    for (i = 200 - z; i <= 200 + z2; i ++)
    {
      xf = fl [i] [0]; yf = fl [i] [1];
      f [xf] [yf] = SHALLOWWATER;
      if (isWater (f [xf] [yf]))
      {
        int hf = h [xf] [yf];
        for (i2 = -1; i2 <= 1; i2 ++)
          for (i3 = -1; i3 <= 1; i3 ++)
            if (xf + i2 >= 0 && xf + i2 <= MAXX && yf + i3 >= 0 && yf + i3 < MAXX)
              if (hf - h [xf + i2] [yf + i3] > 0
                  && !isWater (f [xf + i2] [yf + i3]))
              {
                hf = h [xf + i2] [yf + i3];
              }
        hf -= 200;
        if (hf < 0) hf = 0;
        if (h [xf] [yf] - hf < 350)
        {
          f [xf] [yf] = SHALLOWWATER;
        }
        hw [xf] [yf] = h [xf] [yf];
        h [xf] [yf] = hf;
      }
    }
  }
}
*/


/****************************************************************************
  SEARCH PLANE TERRAIN STRUCTURE
****************************************************************************/

// divide map into 4x4 divisions and search plain in (divx,divy); (-1,-1) performs a global plain search
void Landscape::searchPlain (int divx, int divy, int *x, int *y)
{
  int i, i2;
  const int DIVX = MAXX / 4;

  if (divx == -1 || divy == -1)
  {
    int val [MAXX / 8] [MAXX / 8];
    for (i = 8; i <= MAXX - 8; i += 8)
      for (i2 = 8; i2 <= MAXX - 8; i2 += 8)
      {
        if ((type != LAND_CANYON && f [i] [i2] == GRASS && f [i-4] [i2] == GRASS && f [i+4] [i2] == GRASS && f [i] [i2-4] == GRASS && f [i] [i2+4] == GRASS) ||
            (type == LAND_CANYON && f [i] [i2] == REDSAND && f [i-4] [i2] == REDSAND && f [i+4] [i2] == REDSAND && f [i] [i2-4] == REDSAND && f [i] [i2+4] == REDSAND))
        {
          if (type != 2)
            val [i >> 3] [i2 >> 3] = h [i] [i2] / 4;
          else
            val [i >> 3] [i2 >> 3] = 32000 - h [i] [i2] / 4;
          val [i >> 3] [i2 >> 3] += abs (h [i] [i2] - h [i-4] [i2]);
          val [i >> 3] [i2 >> 3] += abs (h [i] [i2] - h [i+4] [i2]);
          val [i >> 3] [i2 >> 3] += abs (h [i] [i2] - h [i] [i2-4]);
          val [i >> 3] [i2 >> 3] += abs (h [i] [i2] - h [i] [i2+4]);
        }
        else
        {
          val [i >> 3] [i2 >> 3] = 100000;
        }
      }
    int min = 100000;
    for (i = 1; i < MAXX / 8; i ++)
      for (i2 = 1; i2 < MAXX / 8; i2 ++)
      {
        if (val [i] [i2] < min)
        {
          min = val [i] [i2];
          *x = 8 * i;
          *y = 8 * i2;
        }
      }
  }
  else if (divx < 4 && divy < 4)
  {
    int val [DIVX / 8] [DIVX / 8];
    for (i = 8 + divx * DIVX; i <= (divx + 1) * DIVX - 8; i += 8)
      for (i2 = 8 + divy * DIVX; i2 <= (divy + 1) * DIVX - 8; i2 += 8)
      {
        int ix = (i - 8 - divx * DIVX) >> 3;
        int iy = (i2 - 8 - divy * DIVX) >> 3;
        if (f [i] [i2] == GRASS && f [i-4] [i2] == GRASS && f [i+4] [i2] == GRASS && f [i] [i2-4] == GRASS && f [i] [i2+4] == GRASS)
        {
          val [ix] [iy] = abs (h [i] [i2] - h [i-4] [i2]);
          val [ix] [iy] += abs (h [i] [i2] - h [i+4] [i2]);
          val [ix] [iy] += abs (h [i] [i2] - h [i] [i2-4]);
          val [ix] [iy] += abs (h [i] [i2] - h [i] [i2+4]);
          val [ix] [iy] += h [i] [i2] / 250;
        }
        else
        {
          val [ix] [iy] = 100000;
        }
      }
    int min = 100000;
    for (i = 0; i < DIVX / 8 - 1; i ++)
      for (i2 = 0; i2 < DIVX / 8 - 1; i2 ++)
      {
        if (val [i] [i2] < min)
        {
          min = val [i] [i2];
          *x = 8 + 8 * i + divx * DIVX;
          *y = 8 + 8 * i2 + divy * DIVX;
        }
      }
  }
}



/****************************************************************************
  CONSTRUCTOR
****************************************************************************/

Landscape::Landscape ()
{
//  n = LOG2MAXX; //LOG2SPLINE;
  elevation = HEIGHT * 256;

  memset (hw, 0, sizeof (unsigned short) * (MAXX + 1) * (MAXX + 1));
  memset (h, 0, sizeof (unsigned short) * (MAXX + 1) * (MAXX + 1));
  memset (f, 0, sizeof (unsigned char) * (MAXX + 1) * (MAXX + 1));
}

Landscape::~Landscape ()
{
}

#endif
