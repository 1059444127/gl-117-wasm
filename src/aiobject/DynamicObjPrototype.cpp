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

/* This file includes all AI objects instancing models. */

#ifndef IS_UNITPROTOTYPE_H

#include "UnitPrototype.h"
#include "logging/Logging.h"



DynamicObjPrototype::DynamicObjPrototype (const UnitDescriptor &desc)
  : UnitPrototype (desc)
{
  OptionFile *file = OptionFileFactory::get (Directory::getUnits (desc.name.c_str ()));
  if (!file->getFloat ("impact", impact))
    impact = 5;
  if (!file->getFloat ("manoeverability", manoeverability))
    manoeverability = 0.5;
  if (!file->getFloat ("nimbility", nimbility))
    nimbility = 0.0;
  if (!file->getFloat ("thrust", maxthrust))
    maxthrust = 0.0;
  if (!file->getFloat ("shield", maxshield))
    maxshield = 100.0;
  if (!file->getFloat ("rot.gamma", maxrot.gamma))
    maxrot.gamma = 70.0;
  if (!file->getFloat ("rot.theta", maxrot.theta))
    maxrot.theta = 90.0;
  maxrot.phi = 0; // no restriction for the heading
  if (!file->getFloat ("zoom", maxzoom))
    maxzoom = 0.35;
  if (!file->getInteger ("ttl", maxttl))
    maxttl = -1;

  float xyzcube;
  if (!file->getFloat ("cube", xyzcube))
  {
    if (!file->getFloat ("cubex", cube.x))
      cube.x = 1.0;
    if (!file->getFloat ("cubey", cube.y))
      cube.y = 1.0;
    if (!file->getFloat ("cubez", cube.z))
      cube.z = 1.0;
  }
  else
  {
    cube.set (xyzcube, xyzcube, xyzcube);
  }
}

DynamicObjPrototype::~DynamicObjPrototype ()
{
}

#endif
