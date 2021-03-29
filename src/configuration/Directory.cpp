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

#ifndef IS_DIRS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL.h>

#ifndef _MSC_VER // no microsoft?
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include "config.h"
#include "configuration/Directory.h"
#include "logging/Logging.h"
#include "util/Util.h"

#include <cassert>

std::string Directory::saves ("");
std::string Directory::textures ("");
std::string Directory::sounds ("");
std::string Directory::music ("");
std::string Directory::models ("");
std::string Directory::maps ("");
std::string Directory::shaders ("");
std::string Directory::units ("");

char *Directory::strtok (char *str, int len, const char *tok)
{
  char *str0 = str;
  int z = 0;
  while (*str != tok [0])
  {
    str ++;
    z ++;
    if (z >= len)
      return NULL;
  }
  *str = 0;
  return str0;
}

void Directory::append (std::string &target, const std::string &str)
{
#ifdef _MSC_VER
  if (target [target.length () - 1] != '\\')
    target.append ("\\");
  target.append (str);
  target.append ("\\");
#else
  if (target [target.length () - 1] != '/')
    target.append ("/");
  target.append (str);
  target.append ("/");
#endif
/*#ifdef _MSC_VER
  if (target [strlen (target) - 1] != '\\')
    strcat (target, "\\");
  strcat (target, str);
  strcat (target, "\\");
#else
  if (target [strlen (target) - 1] != '/')
    strcat (target, "/");
  strcat (target, str);
  strcat (target, "/");
#endif */
}

Directory::Directory ()
{
}

void Directory::init (char *arg)
{
  char *prefpath = SDL_GetPrefPath(NULL, "gl-117");
#ifdef _MSC_VER
  char path [4096];
  int bscount = 0;
  strcpy (path, arg);
  char *p;
  for (p = path; p < path + strlen (path); p ++)
    if (*p == '\\')
      bscount ++;
  if (bscount >= 2)
  {
    p = path + strlen (path);
    while (*p != '\\') p --;
    p --;
    while (*p != '\\') p --;
    if ((toupper (p [1]) == 'D' && toupper (p [2]) == 'E' && toupper (p [3]) == 'B') ||
        (toupper (p [1]) == 'R' && toupper (p [2]) == 'E' && toupper (p [3]) == 'L'))
    {
      p --;
      while (*p != '\\') p --;
    }
    *p = 0;
  }
  else
  {
    DISPLAY_FATAL("Binary file has no context to the data files");
    DISPLAY_WARN("Do not execute from console, just doubleclick");
    assert (false);
    exit (EXIT_CONTEXT);
  }
  textures = path;
  music = path;
  sounds = path;
  models = path;
  maps = path;
  shaders = path;
  units = path;
  if (prefpath != NULL)
  {
    saves = prefpath;
    SDL_free(prefpath);
  }
  else
  {
    saves = path;
  }
  append (saves, "saves");
#else
  char *home = getenv ("HOME");
  char *env = getenv ("GL117");
  char *path = getenv ("PATH");
  char myfile [4096];
  strcpy (myfile, DATADIR);
  std::cerr << "file: " << myfile << std::endl;
  bool founddir = false;
  struct stat mystat;

  if (!stat (myfile, &mystat))
  {
    if (S_ISDIR (mystat.st_mode))
    {
      founddir = true;
    }
  }
  if (!founddir)
  {

//    FILE *in;
    char cwd [4096];
    getcwd (cwd, 4096); // get current working directory
    char mypath [8092];
    strncpy (mypath, cwd, 8092);
    strncat (mypath, ":", 8092);
    strncat (mypath, path, 8092);
    char *p = mypath;
    int pathlen = strlen (mypath);

    if (arg [0] != '/')
    {
      p = strtok (p, (int) (path + pathlen - p), ":");
      while (p + strlen (p) - 1 < mypath + pathlen)
      {
        strncpy (myfile, p, 4096);
        if (myfile [strlen (myfile) - 1] != '/')
          strncat (myfile, "/", 4096);
        if (*arg == '.' && *(arg+1) == '/')
          strncat (myfile, arg + 2, 4096);
        else
          strncat (myfile, arg, 4096);
        if (!stat (myfile, &mystat))
        {
	        if (S_ISREG (mystat.st_mode))
	        {
            goto found;
          }
        }
        p = strtok (p + strlen (p) + 1, (int) (path + pathlen - p), ":");
      }
    }
    DISPLAY_FATAL("Binary file has no context to the data files.");
    exit (EXIT_CONTEXT);

  found:;
    int bscount = 0;
    for (p = myfile; p < myfile + strlen (myfile); p ++)
      if (*p == '/')
        bscount ++;
    if (bscount >= 2)
    {
      p = myfile + strlen (myfile);
      while (*p != '/') p --;
      p --;
      while (*p != '/') p --;
      if (p [1] == '.' && p [2] == '/')
      {
        p --;
        while (*p != '/') p --;
      }
      p ++;
      *p = 0;
    }
    else
    {
      strcpy (myfile, "/usr/local/share/");
      DISPLAY_INFO("Binary file has no context to the data files. Presuming data files are in /usr/local/share");
//      exit (EXIT_CONTEXT);
    }

  } // if (!founddir)

  if (prefpath != NULL)
  {
    saves = prefpath;
    SDL_free(prefpath);
    append (saves, "saves");
  }
  else if (home != NULL)
  {
    saves = home;
    append (saves, ".config/gl-117");
    if (stat (saves.c_str (), &mystat))
    {
      mkdir (saves.c_str (), S_IRWXU);
    }
  }
  else
  {
    if (env != NULL)
    {
      saves = env;
    }
    else
    {
      saves = "..";
    }
  }
  if (env != NULL)
  {
    textures = env;
    music = env;
    sounds = env;
    models = env;
    maps = env;
    shaders = path;
    units = env;
  }
  else
  {
    DISPLAY_INFO(FormatString("Found gl-117 data directory %s ", myfile));
    textures = myfile;
    music = myfile;
    sounds = myfile;
    models = myfile;
    maps = myfile;
    shaders = myfile;
    units = myfile;
  }
#endif
  append (textures, "textures");
  append (music, "music");
  append (sounds, "sounds");
  append (models, "models");
  append (maps, "maps");
  append (shaders, "shaders");
  append (units, "units");
}

/*Directory::~Directory ()
{
}*/

const std::string Directory::getSaves (const std::string &name)
{
  std::string dir = saves;
  dir.append (name);
  return dir;
}

const std::string Directory::getTextures (const std::string &name)
{
  std::string dir = textures;
  dir.append (name);
  return dir;
}

const std::string Directory::getMusic (const std::string &name)
{
  std::string dir = music;
  dir.append (name);
  return dir;
}

const std::string Directory::getSounds (const std::string &name)
{
  std::string dir = sounds;
  dir.append (name);
  return dir;
}

const std::string Directory::getModels (const std::string &name)
{
  std::string dir = models;
  dir.append (name);
  return dir;
}

const std::string Directory::getMaps (const std::string &name)
{
  std::string dir = maps;
  dir.append (name);
  return dir;
}

const std::string Directory::getShaders (const std::string &name)
{
  std::string dir = shaders;
  dir.append (name);
  return dir;
}

const std::string Directory::getUnits (const std::string &name)
{
  std::string dir = units;
  dir.append (name);
  return dir;
}

#endif
