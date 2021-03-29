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


StaticAa::StaticAa (const UnitDescriptor &desc)
  : AiObj (desc)
{
}

StaticAa::StaticAa (const UnitDescriptor &desc, Space *space2, Model3d *o2, float zoom2)
  : AiObj (desc, space2, o2, zoom2)
{
}

StaticAa::~StaticAa ()
{
}

// core AI method
void StaticAa::aiAction (Uint32 dt, std::vector<AiObj *> &f, std::vector<AiObj *> &m, std::vector<DynamicObj *> &c, std::vector<DynamicObj *> &flare, std::vector<DynamicObj *> &chaff, float camphi, float camgamma)
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
    recheight2 = recheight + getMinimumHeight ();
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
  if (ai)
  {
    recrot.gamma = (int) ((recheight2 - trafo.translation.y) * 10 - currot.gamma + 360);
  }

  // do a smooth roll
  if (easymodel == 1)
  {
    easyPiloting (dt);
  }

  limitRotation ();

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

  recthrust = 0; thrust = 0;
  if (aw > 5)
  {
    recrot.theta = getPrototype ()->maxrot.theta;
  }
  else if (aw < -5)
  {
    recrot.theta = -getPrototype ()->maxrot.theta;
  }
  else
  {
    recrot.theta = 0;
  }

  // thrust and manoever calculations
  if (firecannonttl <= 0)
    for (unsigned i = 0; i < f.size (); i ++)
      if (f [i]->active && party != f [i]->party)
      {
        estimateFighterHeading (f [i]);

        if (id == SacDescriptor)
          if (f [i]->trafo.translation.y > trafo.translation.y + 2)
          {
            if (fabs (aw) <= 20 && disttarget < 50) // + aggressive
              fireCannon (c, currot.phi + aw);
            firecannonttl = firerate * timestep;
          }
        if (firemissilettl <= 0)
          if (id == SamDescriptor)
            if (fabs (aw) < 25 && disttarget < 45) // + aggressive
              if (f [i]->trafo.translation.y > trafo.translation.y + 2)
              {
                ttf = 0;
                fireMissileAirFF (m, f [i]);
                firemissilettl += (20 + firerate * 10) * timestep;
              }
      }
}

#endif
