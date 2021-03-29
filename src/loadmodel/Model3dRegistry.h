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

#ifndef IS_MODEL3DREGISTRY_H
#define IS_MODEL3DREGISTRY_H

#include "model3d/Model3d.h" // ok

#include <map>
#include <string>


typedef std::map<std::string, Model3d *> Model3dList;

/**
* Use the registry to register arbitrary instances of Model3d with a string as identifier.
* The registry serves as global access to the models, so they need not be reloaded
* or recreated. Note that some models are not directly loaded by 3ds, but created by primitives.
*
* @example
* Model3dRegistry mr;
* Model3d *model = ... (e.g. use factory to load)
* mr.add ("mymodel", model);
*
* @example
* Model3dRegistry mr;
* Model3d *model = mr.get ("mymodel");
*/
class Model3dRegistry
{
  public:

    static Model3dList model3dList;

    Model3dRegistry ();
    virtual ~Model3dRegistry ();

    static void add (const std::string &name, Model3d *model);
    static Model3d *get (const std::string &name);
};


#endif

