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

#ifndef IS_AIOBJECT_H

#include "AiObj.h"

#include <cassert>



UnitDescriptor::UnitDescriptor ()
{
  this->id = -1;
  this->name = "";
  this->displayedName = "";
}

UnitDescriptor::UnitDescriptor (int id)
{
  this->id = id;
  this->name = "";
  this->displayedName = "";
}

UnitDescriptor::UnitDescriptor (int id, const std::string &name, const std::string &displayedName)
{
  this->id = id;
  this->name = name;
  this->displayedName = displayedName;
}

bool UnitDescriptor::operator == (const UnitDescriptor &desc) const
{
  return id == desc.id;
}

bool UnitDescriptor::operator < (const UnitDescriptor &desc) const
{
  return id < desc.id;
}

bool UnitDescriptor::operator <= (const UnitDescriptor &desc) const
{
  return id <= desc.id;
}

bool UnitDescriptor::operator > (const UnitDescriptor &desc) const
{
  return id > desc.id;
}

bool UnitDescriptor::operator >= (const UnitDescriptor &desc) const
{
  return id >= desc.id;
}

bool UnitDescriptor::operator != (const UnitDescriptor &desc) const
{
  return id != desc.id;
}

int UnitDescriptor::operator + (const UnitDescriptor &desc) const
{
  return id + desc.id;
}

int UnitDescriptor::operator + (int value) const
{
  return id + value;
}

int UnitDescriptor::operator - (const UnitDescriptor &desc) const
{
  return id - desc.id;
}

int UnitDescriptor::operator - (int value) const
{
  return id - value;
}

#endif
