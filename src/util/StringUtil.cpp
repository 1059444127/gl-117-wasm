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

#include "Util.h"

#include <stdio.h>
#include <stdarg.h>
#include <algorithm>

std::string UpperString (const std::string &str)
{
  std::string upperstr (str);
//  transform (upperstr.begin(), upperstr.end(), upperstr.begin(), toupper); // does not work on VC6
  return upperstr;
}

std::string LowerString (const std::string &str)
{
  std::string lowerstr (str);
//  transform (lowerstr.begin(), lowerstr.end(), lowerstr.begin(), tolower); // does not work on VC6
  return lowerstr;
}

std::string FormatString (const char *format, ...)
{
  const int MAXALLOCSIZE = 4096;
  std::string retString;
  int size = 32;
  char *buffer = NULL;
  while (size < MAXALLOCSIZE)
  {
    buffer = (char *) realloc (buffer, size);
    if (buffer)
    {
      va_list args;
      va_start (args, format);
#ifdef _MSC_VER
      if (_vsnprintf (buffer, size, format, args) != -1)
#else
      if (vsnprintf (buffer, size, format, args) < size)
#endif
      {
        retString = buffer;
        free (buffer);
        break;
      }
    }
    size *= 2;
  }
  return retString;
}
