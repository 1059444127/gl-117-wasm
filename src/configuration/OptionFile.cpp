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

#include "Configuration.h"
#include "logging/Logging.h"
#include "util/Util.h"

#include <cassert>

OptionFile::OptionFile (const std::string &filename)
{
  char token [4096];

  optionList.clear ();

  FILE *file;
  file = fopen(filename.c_str(), "rb");
  if (!file)
    return;
/*  int ret = file.open (filename.c_str ());
    printf ("File %s opened.", filename.c_str());
  DISPLAY_DEBUG(FormatString ("File %s opened.", filename.c_str ()));
  assert (ret);
  if (!ret)
    return; */

  while(fgets(token, 4096, file))
  {
    char *tagstart = token;
    while (*tagstart == ' ' || *tagstart == '\t')
      tagstart ++;
    if (*tagstart == '#')
      continue;
    char *tagend = tagstart;
    while (*tagend != '=' && *tagend != '\0' && *tagend != '\r' && *tagend != '\n' && *tagend != ' ')
      tagend ++;
    *tagend = '\0';
    std::string name = tagstart;

    char *valstart = tagend + 1;
    while (*valstart == ' ' || *valstart == '\t' || *valstart == '=')
      valstart ++;
    if (*valstart == '#')
      continue;
    char *valend = valstart;
    while (*valend != '=' && *valend != '\0' && *valend != '\r' && *valend != '\n' && *valend != ' ')
      valend ++;
    *valend = '\0';
    std::string value = valstart;

/*  file.setWhitespace (" \t\r\n");
  file.addComment ("#", "\n");
  file.setQuotes ("\"'");

  while (file.nextToken (token, 4096))
  {
    std::string name = token;
    if (name == "") break; // the last token may be empty due to comments!
        printf ("%s\n", token);
        fflush(stdout);
    if (!file.nextToken (token, 4096) || strcmp (token, "="))
    {
        printf ("=: %s\n", token);
        fflush(stdout);
//      DISPLAY_ERROR(FormatString ("Syntax error in file %s line %d. Expected \"=\", but got \"%s\"",
//                    filename.c_str (), file.getLine (), token));
      assert (false);
      return;
    }
        printf ("=: %s\n", token);
        fflush(stdout);
    if (!file.nextToken (token, 4096))
    {
        printf ("V: %s\n", token);
        fflush(stdout);
      DISPLAY_ERROR(FormatString ("Syntax error in file %s line %d. Expected a value, but got \"%s\"",
                    filename.c_str (), file.getLine (), token));
      assert (false);
      return;
    }
        printf ("V: %s\n", token);
        fflush(stdout);
    std::string value = token; */

    optionList.insert (std::pair <std::string, std::string> (name, value));
  }

//  file.close ();
  fclose (file);
}

OptionFile::~OptionFile ()
{
  optionList.clear ();
}

bool OptionFile::getString (const std::string &name, std::string &value)
{
  OptionList::iterator it = optionList.find (name);
  if (it == optionList.end ())
  {
//    DISPLAY_DEBUG(FormatString ("Option \"%s\" not found in file \"%s\".",
//                  name.c_str (), file.getFilename ()));
    return false;
  }
  value = it->second;
  return true;
}

bool OptionFile::getBoolean (const std::string &name, bool &value)
{
  std::string str;
  if (!getString (name, str)) return false;

  if (UpperString (str) == "TRUE") value = true;
  else value = false;
  return true;
}

bool OptionFile::getInteger (const std::string &name, int &value)
{
  std::string str;
  if (!getString (name, str)) return false;

  value = atoi (str.c_str ());
  return true;
}

bool OptionFile::getFloat (const std::string &name, float &value)
{
  std::string str;
  if (!getString (name, str)) return false;

  char *ptr;
  double d = strtod (str.c_str (), &ptr);
  value = static_cast<float>(d);
  return true;
}

#endif
