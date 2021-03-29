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

#ifndef IS_MODEL3DFACTORY_H
#define IS_MODEL3DFACTORY_H

#include "model3d/Model3d.h" // ok

#include <map>
#include <string>


typedef std::map<std::string, Model3d *> Model3dList;

/**
* Use the factory to load models from arbitrary files.
* The factory decides according to the filename, if the model has to be loaded from disk
* or if the model is already in memory.
*
* @example
* Model3dFactory mf;
* Model3d *model = mf.getModel3d ("myfile.3ds");
*/
class Model3dFactory
{
  public:

    static Model3dList model3dList;

    static Model3d *getModel (const std::string &filename);
};

#endif

