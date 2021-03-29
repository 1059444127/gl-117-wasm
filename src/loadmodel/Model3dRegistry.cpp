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

#include "Model3dRegistry.h"
#include "configuration/Directory.h"

#include <cassert>


Model3dList Model3dRegistry::model3dList = Model3dList ();

Model3dRegistry::Model3dRegistry ()
{
}

Model3dRegistry::~Model3dRegistry ()
{
  for (Model3dList::iterator it = model3dList.begin (); it != model3dList.end (); ++ it)
  {
    delete it->second;
    it->second = NULL;
  }
  model3dList.clear ();
}

void Model3dRegistry::add (const std::string &name, Model3d *model)
{
  assert (model);
  model3dList.insert (std::pair<std::string, Model3d *> (name, model));
}

Model3d *Model3dRegistry::get (const std::string &name)
{
  Model3dList::iterator it = model3dList.find (name);
  if (it == model3dList.end ())
  {
    assert (false);
    return NULL;
  }
  else
  {
    return it->second;
  }
}


#endif
