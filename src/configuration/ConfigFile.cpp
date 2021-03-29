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

#ifndef IS_CONFIGURATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "configuration/Configuration.h"
#include "logging/Logging.h"
#include "util/Util.h"



ConfigFile::ConfigFile () {}

ConfigFile::ConfigFile (const char *fname)
{
  char buf2 [4096];
  bool commentmode = false;
  FILE *in;
  length = 0;
  in = fopen (fname, "rb");
  if (in != NULL)
  {
    length = fread (buf, 1, 32000, in);
    fclose (in);
  }
  else
  {
    DISPLAY_WARN(FormatString ("Could not load %s", fname));
    buf [0] = 0;
  }
  for (int i = 0; i < length; i ++)
  {
    if (buf [i] == '#') commentmode = true;
    if (buf [i] == '\n') commentmode = false;
    if (commentmode) buf [i] = ' ';
    else buf [i] = tolower (buf [i]);
  }
}

const char *ConfigFile::skipwhite (const char *str)
{
  while (*str == ' ' || *str == '\t')
    str ++;
  return str;
}

const char *ConfigFile::skipnum (const char *str)
{
  while (*str >= '0' && *str <= '9')
    str ++;
  return str;
}

const char *ConfigFile::skipalphanum (const char *str)
{
  while (*str >= '0' && *str <= 'z')
    str ++;
  return str;
}

char *ConfigFile::getString (char *dest, const char *str)
{
  const char *strf, *stre;
  char cmpstr [256];
  int i;
  if (strlen (str) >= 256) { return NULL; }
  strcpy (cmpstr, str);
  if (buf [0] == 0) { return NULL; }
  for (i = 0; i < (int) strlen (cmpstr); i ++)
  {
    cmpstr [i] = tolower (cmpstr [i]);
  }
  strf = strstr (buf, cmpstr);
  if (strf == NULL) { return NULL; }
  strf += strlen (cmpstr);
  strf = skipwhite (strf);
  if (*strf == 0) { return NULL; }
  strf ++;
  if (*strf == 0) { return NULL; }
  strf = skipwhite (strf);
  if (*strf == 0) { return NULL; }
  stre = skipalphanum (strf);
  if (stre - strf >= 256) { return NULL; }
  if (*strf == 0) { return NULL; }
  for (i = 0; i < (int) (stre - strf); i ++)
    dest [i] = *(strf+i);
  dest [i] = 0;
  return dest;
}

int ConfigFile::getValue (const char *str)
{
  char res [256];
  getString (res, str);
  if (res == NULL) return -1;
  return atoi (res);
}

int ConfigFile::openOutput (const char *fname)
{
  out = fopen (fname, "wb");
  if (out == NULL) return 0;
  return 1;
}

int ConfigFile::write (const char *str1, int n)
{
  char str [256];
  if (strlen (str1) + 8 > 256) return 0;
  sprintf (str, "%s = %d\n", str1, n);
  fwrite (str, 1, strlen (str), out);
  return 1;
}

int ConfigFile::write (const char *str1, char c)
{
  char str [256];
  if (c <= 32 || c >= 97)
  {
    write (str1, (int) c);
    return 1;
  }
  if (strlen (str1) + 8 > 256) return 0;
  sprintf (str, "%s = %c\n", str1, c);
  fwrite (str, 1, strlen (str), out);
  return 1;
}

void ConfigFile::writeText (const char *str)
{
  fwrite (str, 1, strlen (str), out);
  fwrite ("\n", 1, 1, out);
}

void ConfigFile::close ()
{
  fclose (out);
}

#endif
