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

#include "Model3dFactory.h"
#include "Load3ds.h"
#include "configuration/Directory.h"

#include <cassert>


Model3dList Model3dFactory::model3dList = Model3dList ();

Model3d *Model3dFactory::getModel (const std::string &filename)
{
  Model3dList::iterator it = model3dList.find (filename);
  if (it == model3dList.end ())
  {
    Model3d *model = new Model3d ();
    Load3ds loader;
    loader.setTextureDir (Directory::getTextures (""));
    loader.import3ds (model, filename.c_str ());
    model3dList.insert (std::pair<std::string, Model3d *> (filename, model));
    return model;
  }
  else
  {
    return it->second;
  }
  return 0;
}

#endif
