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
#define IS_UNITPROTOTYPE_H

#include "configuration/Configuration.h" // ok
#include "model3d/Model3d.h" // ok
#include "util/Util.h"
#include "logging/Logging.h"
#include "configuration/Directory.h"

#include <map>
#include <cassert>



class UnitDescriptor
{
  public:
    int id;
    std::string name;
    std::string displayedName;

    UnitDescriptor ();
    UnitDescriptor (int id);
    UnitDescriptor (int id, const std::string &name, const std::string &displayedName);
    bool operator == (const UnitDescriptor &desc) const;
    bool operator < (const UnitDescriptor &desc) const;
    bool operator <= (const UnitDescriptor &desc) const;
    bool operator > (const UnitDescriptor &desc) const;
    bool operator >= (const UnitDescriptor &desc) const;
    bool operator != (const UnitDescriptor &desc) const;
    int operator + (const UnitDescriptor &desc) const;
    int operator + (int value) const;
    int operator - (const UnitDescriptor &desc) const;
    int operator - (int value) const;
};

typedef std::map<int, UnitDescriptor> UnitDescriptorList;

class UnitDescriptorRegistry
{
  public:
    static UnitDescriptorList unitDescriptorList;

    UnitDescriptorRegistry ();
    virtual ~UnitDescriptorRegistry ();
    static void add (const UnitDescriptor &desc);
    static const UnitDescriptor &get (int id);
};



const int missiletypes = 8;
const int missileracks = 4;

class UnitPrototype
{
  public:
    UnitPrototype (const UnitDescriptor &desc)
    {
    }

    virtual ~UnitPrototype ()
    {
    }
};

class DynamicObjPrototype : public UnitPrototype
{
  public:

    float impact;    ///< this value will be subtracted from the other objects shield when colliding
    float manoeverability; ///< how fast a fighter can alter its direction
    float nimbility; ///< how fast a fighter responds to alterations of recXXX (recommended XXX)
    float maxthrust; ///< maximum throttle value
    float maxshield; ///< maximum shield
    Rotation maxrot; ///< only used for easymodel calculations, no use of getPrototype ()->maxrot.phi
    float maxzoom;
    int maxttl;
    Vector3 cube;

    DynamicObjPrototype (const UnitDescriptor &desc);
    virtual ~DynamicObjPrototype ();
};

class AiObjPrototype : public DynamicObjPrototype
{
  public:

    bool dualshot;      ///< one or two cannons?
    int maxammo;
    int weight;

    AiObjPrototype (const UnitDescriptor &desc);
    virtual ~AiObjPrototype ();
};

class FighterPrototype : public AiObjPrototype
{
  public:

    int maxchaffs;
    int maxflares;
    int racks;
    int rackload [missileracks];

    FighterPrototype (const UnitDescriptor &desc);
    virtual ~FighterPrototype ();
};

#endif
