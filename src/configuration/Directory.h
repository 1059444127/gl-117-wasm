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
#define IS_DIRS_H

#include <string>

/**
* This class handles the game directories to save/load configurations and to load game data.
* For Unix it requires HOME to be set and it must be compiled with #define DATADIR (as compiler option)
* to know where the game is installed (by make install).
* One object "dirs" is already predefined, please call init() once at startup.
*/
class Directory
{
  public:

    static std::string saves;    ///< path to saves, UNIX and Windows differ
    static std::string textures; ///< path to textures...
    static std::string music;
    static std::string sounds;
    static std::string models;
    static std::string maps;
    static std::string shaders;
    static std::string units;

    Directory ();
//    virtual ~Directory ();

    /// this method must be called once at startup with argv [0] of main as parameter!
    static void init (char *arg);

    /// get dir name containing the saves...
    static const std::string getSaves (const std::string &name);
    static const std::string getTextures (const std::string &name);
    static const std::string getSounds (const std::string &name);
    static const std::string getMusic (const std::string &name);
    static const std::string getModels (const std::string &name);
    static const std::string getMaps (const std::string &name);
    static const std::string getShaders (const std::string &name);
    static const std::string getUnits (const std::string &name);

  protected:

    static char *strtok (char *str, int len, const char *tok);
    static void append (std::string &target, const std::string &str); // append subdir/file to path using (back-)slashes
};

//extern Dirs dirs;

#endif
