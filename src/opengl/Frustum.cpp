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

#ifndef IS_GLPRIMITIVES_H

#include "GlPrimitives.h"
#include "includegl.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>



Frustum frustum;

// Frustum Culling according to OpenGL Page
void Frustum::extractFrustum()
{
  float proj[16];
  float modl[16];
  float clip[16];
  float t;

  // extract transformation matrices
  glGetFloatv( GL_PROJECTION_MATRIX, proj );
  glGetFloatv( GL_MODELVIEW_MATRIX, modl );

  clip[0] = modl[0] * proj[0] + modl[1] * proj[4] + modl[2] * proj[8] + modl[3] * proj[12];
  clip[1] = modl[0] * proj[1] + modl[1] * proj[5] + modl[2] * proj[9] + modl[3] * proj[13];
  clip[2] = modl[0] * proj[2] + modl[1] * proj[6] + modl[2] * proj[10] + modl[3] * proj[14];
  clip[3] = modl[0] * proj[3] + modl[1] * proj[7] + modl[2] * proj[11] + modl[3] * proj[15];
  clip[4] = modl[4] * proj[0] + modl[5] * proj[4] + modl[6] * proj[8] + modl[7] * proj[12];
  clip[5] = modl[4] * proj[1] + modl[5] * proj[5] + modl[6] * proj[9] + modl[7] * proj[13];
  clip[6] = modl[4] * proj[2] + modl[5] * proj[6] + modl[6] * proj[10] + modl[7] * proj[14];
  clip[7] = modl[4] * proj[3] + modl[5] * proj[7] + modl[6] * proj[11] + modl[7] * proj[15];
  clip[8] = modl[8] * proj[0] + modl[9] * proj[4] + modl[10] * proj[8] + modl[11] * proj[12];
  clip[9] = modl[8] * proj[1] + modl[9] * proj[5] + modl[10] * proj[9] + modl[11] * proj[13];
  clip[10] = modl[8] * proj[2] + modl[9] * proj[6] + modl[10] * proj[10] + modl[11] * proj[14];
  clip[11] = modl[8] * proj[3] + modl[9] * proj[7] + modl[10] * proj[11] + modl[11] * proj[15];

  clip[12] = modl[12] * proj[0] + modl[13] * proj[4] + modl[14] * proj[8] + modl[15] * proj[12];
  clip[13] = modl[12] * proj[1] + modl[13] * proj[5] + modl[14] * proj[9] + modl[15] * proj[13];
  clip[14] = modl[12] * proj[2] + modl[13] * proj[6] + modl[14] * proj[10] + modl[15] * proj[14];
  clip[15] = modl[12] * proj[3] + modl[13] * proj[7] + modl[14] * proj[11] + modl[15] * proj[15];

  // extract numbers for the RIGHT plane
  frustum[0][0] = clip[3] - clip[0];
  frustum[0][1] = clip[7] - clip[4];
  frustum[0][2] = clip[11] - clip[8];
  frustum[0][3] = clip[15] - clip[12];

  // normalize the result
  t = sqrt(frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] + frustum[0][2] * frustum[0][2]);
  frustum[0][0] /= t;
  frustum[0][1] /= t;
  frustum[0][2] /= t;
  frustum[0][3] /= t;

  // extract the numbers for the LEFT plane
  frustum[1][0] = clip[3] + clip[0];
  frustum[1][1] = clip[7] + clip[4];
  frustum[1][2] = clip[11] + clip[8];
  frustum[1][3] = clip[15] + clip[12];

  // normalize the result
  t = sqrt(frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] + frustum[1][2] * frustum[1][2]);
  frustum[1][0] /= t;
  frustum[1][1] /= t;
  frustum[1][2] /= t;
  frustum[1][3] /= t;

  // extract the BOTTOM plane
  frustum[2][0] = clip[3] + clip[1];
  frustum[2][1] = clip[7] + clip[5];
  frustum[2][2] = clip[11] + clip[9];
  frustum[2][3] = clip[15] + clip[13];

  // normalize the result
  t = sqrt(frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] + frustum[2][2] * frustum[2][2]);
  frustum[2][0] /= t;
  frustum[2][1] /= t;
  frustum[2][2] /= t;
  frustum[2][3] /= t;

  // extract the TOP plane
  frustum[3][0] = clip[3] - clip[1];
  frustum[3][1] = clip[7] - clip[5];
  frustum[3][2] = clip[11] - clip[9];
  frustum[3][3] = clip[15] - clip[13];

  // normalize the result
  t = sqrt(frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] + frustum[3][2] * frustum[3][2]);
  frustum[3][0] /= t;
  frustum[3][1] /= t;
  frustum[3][2] /= t;
  frustum[3][3] /= t;

  // extract the FAR plane
  frustum[4][0] = clip[3] - clip[2];
  frustum[4][1] = clip[7] - clip[6];
  frustum[4][2] = clip[11] - clip[10];
  frustum[4][3] = clip[15] - clip[14];

  // normalize the result
  t = sqrt(frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] + frustum[4][2] * frustum[4][2]);
  frustum[4][0] /= t;
  frustum[4][1] /= t;
  frustum[4][2] /= t;
  frustum[4][3] /= t;

  // extract the NEAR plane
  frustum[5][0] = clip[3] + clip[2];
  frustum[5][1] = clip[7] + clip[6];
  frustum[5][2] = clip[11] + clip[10];
  frustum[5][3] = clip[15] + clip[14];

  // normalize the result
  t = sqrt(frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] + frustum[5][2] * frustum[5][2]);
  frustum[5][0] /= t;
  frustum[5][1] /= t;
  frustum[5][2] /= t;
  frustum[5][3] /= t;
}

bool Frustum::isPointInFrustum( float x, float y, float z )
{
  int i;
  for (i = 0; i < 6; i ++)
    if (frustum[i][0] * x + frustum[i][1] * y + frustum[i][2] * z + frustum[i][3] <= 0)
      return false;
  return true;
}

bool Frustum::isSphereInFrustum( float x, float y, float z, float radius )
{
  int i;
  for (i = 0; i < 6; i ++)
    if (frustum[i][0] * x + frustum[i][1] * y + frustum[i][2] * z + frustum[i][3] <= -radius)
      return false;
  return true;
}

bool Frustum::isCubeInFrustum( float x, float y, float z, float size )
{
  int i;
  for (i = 0; i < 6; i ++)
  {
    if (frustum[i][0] * (x - size) + frustum[i][1] * (y - size) + frustum[i][2] * (z - size) + frustum[i][3] > 0)
      continue;
    if (frustum[i][0] * (x + size) + frustum[i][1] * (y - size) + frustum[i][2] * (z - size) + frustum[i][3] > 0)
      continue;
    if (frustum[i][0] * (x - size) + frustum[i][1] * (y + size) + frustum[i][2] * (z - size) + frustum[i][3] > 0)
      continue;
    if (frustum[i][0] * (x + size) + frustum[i][1] * (y + size) + frustum[i][2] * (z - size) + frustum[i][3] > 0)
      continue;
    if (frustum[i][0] * (x - size) + frustum[i][1] * (y - size) + frustum[i][2] * (z + size) + frustum[i][3] > 0)
      continue;
    if (frustum[i][0] * (x + size) + frustum[i][1] * (y - size) + frustum[i][2] * (z + size) + frustum[i][3] > 0)
      continue;
    if (frustum[i][0] * (x - size) + frustum[i][1] * (y + size) + frustum[i][2] * (z + size) + frustum[i][3] > 0)
      continue;
    if (frustum[i][0] * (x + size) + frustum[i][1] * (y + size) + frustum[i][2] * (z + size) + frustum[i][3] > 0)
      continue;
    return false;
  }
  return true;
}

#endif

