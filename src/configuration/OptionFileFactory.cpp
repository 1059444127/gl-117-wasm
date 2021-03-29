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

#include <cassert>

OptionFileList OptionFileFactory::optionFileList;

OptionFile *OptionFileFactory::get (const std::string &filename)
{
  OptionFileList::iterator it = optionFileList.find (filename);
  if (it == optionFileList.end ())
  {
    OptionFile *optionfile = new OptionFile (filename + ".txt");
    optionFileList.insert (std::pair<std::string, OptionFile *> (filename, optionfile));
    return optionfile;
  }
  else
  {
    return it->second;
  }
  return 0;
}

#endif
