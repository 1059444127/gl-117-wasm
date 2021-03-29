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

#ifndef IS_LOAD3DS_H

#include "Load3ds.h"
#include "logging/Logging.h"
#include "util/Util.h"

#include <stdio.h>
#include <cassert>


BinaryFile3ds::BinaryFile3ds (const char *filename)
{
  in = fopen (filename, "rb");
  if (in == NULL)
  {
    DISPLAY_FATAL(FormatString ("Cannot open file %s", filename));
    assert (false);
    exit (EXIT_LOADFILE);
    return;
  }
  fseek (in, 0, SEEK_END);
  size = ftell (in);
  fseek (in, 0, SEEK_SET);
  data = new unsigned char [size];
  memset (data, 0, size);
  Uint32 z = 0;
  while (z < size)
  {
    size_t tmp;
    if (size - z >= 4096)
      tmp = fread (&data [z], 1, 4096, in);
    else
      tmp = fread (&data [z], 1, size - z, in);
    z += 4096;
  }
  fclose (in);
  filepointer = 0;
}

BinaryFile3ds::~BinaryFile3ds ()
{
  delete[] data;
}

int BinaryFile3ds::readFloat (float *f)
{
  assert (filepointer < size);
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  ret [0] = data [filepointer + 3];
  ret [1] = data [filepointer + 2];
  ret [2] = data [filepointer + 1];
  ret [3] = data [filepointer];
  ret [4] = 0;
  *f = *((float *) ret);
#else
  *f = *((float *) &data [filepointer]);
#endif
  filepointer += 4;
  return 4;
}

int BinaryFile3ds::readFloat (float *f, const int n)
{
  assert (filepointer < size);
  int i;
  for (i = 0; i < n; i ++)
  {
    readFloat (&f [i]);
  }
  return n * 4;
}

int BinaryFile3ds::readUInt32 (Uint32 *i)
{
  assert (filepointer < size);
#ifdef WORDS_BIGENDIAN
  ret [0] = data [filepointer + 3];
  ret [1] = data [filepointer + 2];
  ret [2] = data [filepointer + 1];
  ret [3] = data [filepointer];
  ret [4] = 0;
  *i = *((Uint32 *) ret);
#else
  *i = *((Uint32 *) &data [filepointer]);
#endif
  filepointer += 4;
  return 4;
}

int BinaryFile3ds::readUInt16 (Uint16 *i)
{
  assert (filepointer < size);
#ifdef WORDS_BIGENDIAN
  ret [0] = data [filepointer + 1];
  ret [1] = data [filepointer + 0];
  ret [2] = 0;
  *i = *((Uint16 *) ret);
#else
  *i = *((Uint16 *) &data [filepointer]);
#endif
  filepointer += 2;
  return 2;
}

int BinaryFile3ds::readString (char *ptr, const int ptrmax, const int len)
{
  assert (filepointer < size);
  int n = len;
  if (n <= 0) return 0;
  if (filepointer + n > size)
    n = size - filepointer;
  if (n > ptrmax) exit (200);
  memcpy (ptr, &data [filepointer], n);
  filepointer += n;
  return n;
}

int BinaryFile3ds::readString (char *ptr, const int len)
{
  assert (filepointer < size);
  int n = len;
  if (n <= 0) return 0;
  if (filepointer + n > size)
    n = size - filepointer;
  memcpy (ptr, &data [filepointer], n);
  filepointer += n;
  return n;
}

int BinaryFile3ds::readString (char *ptr)
{
  assert (filepointer < size);
  int i = 0;
  while (data [filepointer] != 0 && filepointer < size)
  {
    ptr [i] = data [filepointer];
    i ++;
    filepointer ++;
  }
  ptr [i] = 0;
  filepointer ++;
  i ++;
  return i;
}

int BinaryFile3ds::skip (const int len)
{
  assert (filepointer < size);
  int n = len;
  if (filepointer + n > size)
    n = size - filepointer;
  filepointer += n;
  return n;
}

Uint32 BinaryFile3ds::getSize ()
{
  return size;
}

#endif
