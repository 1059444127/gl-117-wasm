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
#include "logging/Logging.h"
#include "gllandscape/GlLandscape.h"
#include "game/globals.h"
#include "util/Math.h"

#include <cassert>


Missile::Missile (const UnitDescriptor &desc)
  : AiObj (desc)
{
}

Missile::Missile (const UnitDescriptor &desc, Space *space2, Model3d *o2, float zoom2)
  : AiObj (desc, space2, o2, zoom2)
{
}

Missile::~Missile ()
{
}

// core AI method
void Missile::aiAction (Uint32 dt, std::vector<AiObj *> &f, std::vector<AiObj *> &m, std::vector<DynamicObj *> &c, std::vector<DynamicObj *> &flare, std::vector<DynamicObj *> &chaff, float camphi, float camgamma)
{
  timer += dt;

  if (!active && !draw) // not active, not drawn, then exit
  {
    return;
  }

  checkTtl (dt);

  // move object according to our physics
  move (dt, camphi, camgamma);

  // set smoke
  setSmoke (dt);

  if (!active) // not active, then exit
  {
    return;
  }
  if (explode > 0 || sink > 0) // exploding or sinking, then exit
  {
    thrust = 0;
    return;
  }

  // do expensive calculations only once
  float myheight = l->getExactHeight (trafo.translation.x, trafo.translation.z);
  float targetheight = trafo.translation.y;
  if (target != NULL)
    targetheight = l->getExactHeight (target->trafo.translation.x, target->trafo.translation.z);
  if (target != NULL)
    disttarget = distance (target); // distance to target
  else
    disttarget = 1;

  // get a new target if necessary
  if (id >= MissileBeginDescriptor && id <= MissileEndDescriptor)
  {
    if (target == NULL)
      ttl = 0;
    else if (!target->active)
      ttl = 0;
  }

  selectTarget (f);

  if (!ai) return;

  // which height???
  float recheight2; // this is the height, the object wants to achieve
  int lsdist = 15;
  float flyx = trafo.translation.x + force.x * lsdist, flyz = trafo.translation.z + force.z * lsdist;
  if (manoeverheight > 0)
  {
    // precalculated height
    recheight2 = l->getExactHeight (flyx, flyz) + recheight;
  }
  else
  {
     // missiles and non intelligent objects will not change their height due to the surface
    if (target != NULL)
    {
      recheight2 = target->trafo.translation.y - 8 * target->thrust * SIN(target->currot.gamma);
    }
    else
    {
      recheight2 = recheight + getMinimumHeight ();
    }
  }

  decreaseManoeverCounter (dt);

  // calculate the recommended height, recheight2 depends on it
  if (manoeverheight <= 0)
  {
    if (target != NULL) // no fighter, has target (missile, mine)
    {
      recheight = target->trafo.translation.y - targetheight;
    }
  }

  // get recommended elevation to target
  if (ttl <= 0 && id != MineDescriptor)
  { recheight = -100; recheight2 = -100; recrot.gamma = 90; }
  else if (ai)
  {
    if (target != NULL) // is MISSILE
    {
      float dgamma = 0;
      if (disttarget <= -0.00001 || disttarget >= 0.00001) // no division by zero
        dgamma = atan ((target->trafo.translation.y - trafo.translation.y) / disttarget) * 180 / PI - (currot.gamma - 180);
      recrot.gamma = currot.gamma + dgamma;
    }
    else
    {
      recrot.gamma = (int) ((recheight2 - trafo.translation.y) * 10 - currot.gamma + 360);
    }
  }

  // do a smooth roll
  if (easymodel == 1)
  {
    easyPiloting (dt);
  }

  limitRotation ();

  if (target == NULL)
  {
    ttl = 0;
    return;
  }
  else if (target->active == false)
  {
    ttl = 0;
    return;
  }

  if (target == NULL) return;

  if (!ai || target == NULL) // no AI (player) or no target found, then exit
  {
    return;
  }

  float dx2, dz2;
  float dx = target->trafo.translation.x - trafo.translation.x, dz = target->trafo.translation.z - trafo.translation.z; // current distances

  estimateTargetPosition (&dx2, &dz2);

  estimateHeading (dx2, dz2);

  // heading calculations
  int firerate = getFireRate ();

  if (fabs (aw) < 50 && disttarget > 50) // target in front and minimum distance, then no roll
    recrot.theta = 0;
  else // otherwise chase target
  {
    if (aw < -90 || aw > 90) recrot.theta = 0;
    else if (aw > 0)
    {
      recrot.theta = aw > 90 ? 90 : aw;
    }
    else
    {
      recrot.theta = aw < -90 ? -90 : aw;
    }
  }
}

#endif
