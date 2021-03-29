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

#include "LoadObj.h"
#include "logging/Logging.h"
#include "util/Util.h"

#include <stdio.h>
#include <cassert>



TextFileObj::TextFileObj (const char *filename)
{
  in = fopen (filename, "rb");
  if (in == NULL)
  {
    assert (false);
    DISPLAY_FATAL(FormatString ("Cannot open file %s", filename));
    exit (EXIT_LOADFILE);
    return;
  }
  fseek (in, 0, SEEK_END);
  size = ftell (in);
  fseek (in, 0, SEEK_SET);
  data = new char [size];
  int32 z = 0;
  while (!feof (in))
  {
    fread (&data [z], 4096, sizeof (char), in);
    z += 4096;
  }
  fclose (in);
  filepointer = 0;
}

TextFileObj::~TextFileObj ()
{
  delete data;
}

float TextFileObj::readFloat (int32 offset)
{
  int32 i = offset;
  while (data [i] == ' ' || data [i] == '\t' || data [i] == 0) i ++;
  offset = i;
  while ((data [i] >= '0' && data [i] <= '9') || data [i] == '.' || data [i] == '-') i ++;
  data [i] = 0;
  filepointer = i + 1;
  char *p1;
  char **p2 = &p1;
  return (float) strtod (&data [offset], p2);
}

float TextFileObj::readFloat ()
{
  return readFloat (filepointer);
}

int TextFileObj::readInteger (int32 offset)
{
  int32 i = offset;
  while (data [i] == ' ' || data [i] == '\t' || data [i] == 0) i ++;
  offset = i;
  while ((data [i] >= '0' && data [i] <= '9') || data [i] == '-') i ++;
  data [i] = 0;
  filepointer = i + 1;
//  char *p1;
//  char **p2 = &p1;
  return (int) atoi (&data [offset]);
}
int TextFileObj::readInteger ()
{
  return readInteger (filepointer);
}

char *TextFileObj::readWord (int32 offset)
{
  int32 i = offset;
  while (data [i] == ' ' || data [i] == '\t' || data [i] == 0) i ++;
  offset = i;
  while (data [i] > ' ' && data [i] <= 'z') i ++;
  data [i] = 0;
  filepointer = i + 1;
  return &data [offset];
}

char *TextFileObj::readWord ()
{
  return readWord (filepointer);
}

char *TextFileObj::getChar ()
{
  return &data [filepointer];
}

void TextFileObj::nextWhite ()
{
  int32 i = filepointer;
  while (data [i] != ' ' && data [i] != '\t' && data [i] != '\n' && i < size) i ++;
  filepointer = i;
}

void TextFileObj::skipWhite ()
{
  int32 i = filepointer;
  while (data [i] == ' ' || data [i] == '\t') i ++;
  filepointer = i;
}

#endif
