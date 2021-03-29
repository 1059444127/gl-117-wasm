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

#ifndef IS_AIOBJECT_H

#include "AiObj.h"
#include "logging/Logging.h"
#include "gllandscape/GlLandscape.h"
#include "game/globals.h"
#include "util/Math.h"

#include <cassert>


StaticPassive::StaticPassive (const UnitDescriptor &desc)
  : AiObj (desc)
{
}

StaticPassive::StaticPassive (const UnitDescriptor &desc, Space *space2, Model3d *o2, float zoom2)
  : AiObj (desc, space2, o2, zoom2)
{
}

StaticPassive::~StaticPassive ()
{
}

// core AI method
void StaticPassive::aiAction (Uint32 dt, std::vector<AiObj *> &f, std::vector<AiObj *> &m, std::vector<DynamicObj *> &c, std::vector<DynamicObj *> &flare, std::vector<DynamicObj *> &chaff, float camphi, float camgamma)
{
  timer += dt;

  if (!active && !draw) // not active, not drawn, then exit
  {
    return;
  }

  checkTtl (dt);

  // move object according to our physics
  move (dt, camphi, camgamma);

  return;
}

#endif
