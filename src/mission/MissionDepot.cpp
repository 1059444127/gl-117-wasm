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

#ifndef IS_MISSION_H

#include "Mission.h"
#include "game/globals.h"
#include "util/Math.h"
#include "configuration/Configuration.h"
#include "gllandscape/GlLandscape.h"

#include <stdio.h>
#include <string.h>



MissionDepot::MissionDepot ()
{
  id = MISSION_DEPOT;
  strcpy (name, "DEPOTS");
  strcpy (briefing, "THERE IS STILL AN OUTPOST WITH SOME DEPOTS. DESTROY ALL THE DEPOTS. WE ARE NOT EXPECTING ANY AIR RESISTANCE.");
  autoLFBriefing ();
  alliedfighters = 2;
  alliedpilot [0] = PILOT_PRIMETIME;
  maxtime = 5500 * timestep;
}

void MissionDepot::start ()
{
  int i, i2;
  day = 0;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 40;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_ALPINE_EROSION, NULL);
  int px, py;
  l->searchPlain (-1, -1, &px, &py);
  l->flatten (px, py, 6, 6);
  for (i = px - 5; i <= px + 5; i ++)
    for (i2 = py - 6; i2 <= py + 6; i2 ++)
    {
      l->f [i] [i2] = GREYSAND;
      int sum = (int) l->r [i] [i2] + l->g [i] [i2] + l->b [i] [i2];
      sum /= 3;
      l->r [i] [i2] = sum + 15;
      l->g [i] [i2] = sum + 15;
      l->b [i] [i2] = sum - 15;
    }
  playerInit ();
  fplayer->trafo.translation.x = px + 10;
  fplayer->trafo.translation.z = py + 130;
  for (i = 1; i <= 1; i ++)
  {
    alliedInit (StormDescriptor, alliedpilot [i - 1]);
    fighter [i]->trafo.translation.x = px + 10 + i * 5;
    fighter [i]->trafo.translation.z = py + 130 + i * 5;
    fighter [i]->bomber = true;
  }
  int n = 2;
  objectInit (new StaticPassive (DepotDescriptor), 0, 400);
  fighter [n]->trafo.translation.x = px - 4;
  fighter [n]->trafo.translation.z = py;
  n ++;
  objectInit (new StaticPassive (DepotDescriptor), 0, 400);
  fighter [n]->trafo.translation.x = px + 2;
  fighter [n]->trafo.translation.z = py + 5;
  n ++;
  objectInit (new StaticPassive (HallDescriptor), 0, 400);
  fighter [n]->trafo.translation.x = px;
  fighter [n]->trafo.translation.z = py - 1;
  n ++;
  objectInit (new StaticPassive (HallDescriptor), 0, 400);
  fighter [n]->trafo.translation.x = px + 2;
  fighter [n]->trafo.translation.z = py - 1;
  n ++;
  objectInit (new StaticPassive (BigTentDescriptor), 0, 400);
  fighter [n]->trafo.translation.x = px + 1.5;
  fighter [n]->trafo.translation.z = py - 4.5;
  n ++;
  objectInit (new StaticPassive (BigTentDescriptor), 0, 400);
  fighter [n]->trafo.translation.x = px - 3;
  fighter [n]->trafo.translation.z = py + 4;
  n ++;
  objectInit (new StaticAa (SacDescriptor), 0, 200);
  fighter [n]->trafo.translation.x = px - 6;
  fighter [n]->trafo.translation.z = py + 6;
  fighter [n]->target = fighter [0];
  fighter [n]->currot.phi = 90;
  fighter [n]->getPrototype ()->maxrot.theta = 0;
  n ++;
  objectInit (new StaticAa (SacDescriptor), 0, 200);
  fighter [n]->trafo.translation.x = px - 6;
  fighter [n]->trafo.translation.z = py - 6;
  fighter [n]->target = fighter [0];
  fighter [n]->currot.phi = 0;
  fighter [n]->getPrototype ()->maxrot.theta = 0;
  n ++;
  objectInit (new StaticAa (SacDescriptor), 0, 200);
  fighter [n]->trafo.translation.x = px + 6;
  fighter [n]->trafo.translation.z = py - 6;
  fighter [n]->target = fighter [0];
  fighter [n]->currot.phi = 270;
  fighter [n]->getPrototype ()->maxrot.theta = 0;
  n ++;
  objectInit (new StaticAa (SacDescriptor), 0, 200);
  fighter [n]->trafo.translation.x = px + 6;
  fighter [n]->trafo.translation.z = py + 6;
  fighter [n]->target = fighter [0];
  fighter [n]->currot.phi = 180;
  fighter [n]->getPrototype ()->maxrot.theta = 0;
  n ++;
  objectInit (new StaticAa (SamDescriptor), 0, 200);
  fighter [n]->trafo.translation.x = px - 6;
  fighter [n]->trafo.translation.z = py + 35;
  fighter [n]->target = fighter [0];
  fighter [n]->currot.phi = 180;
  fighter [n]->getPrototype ()->maxrot.theta = 0;
  n ++;
  objectInit (new StaticAa (SamDescriptor), 0, 200);
  fighter [n]->trafo.translation.x = px + 6;
  fighter [n]->trafo.translation.z = py + 35;
  fighter [n]->target = fighter [0];
  fighter [n]->currot.phi = 180;
  fighter [n]->getPrototype ()->maxrot.theta = 0;
  n ++;
  objectInit (new StaticAa (SamDescriptor), 0, 200);
  fighter [n]->trafo.translation.x = px + 35;
  fighter [n]->trafo.translation.z = py;
  fighter [n]->target = fighter [0];
  fighter [n]->currot.phi = 180;
  fighter [n]->getPrototype ()->maxrot.theta = 0;
  n ++;
  objectInit (new StaticAa (SamDescriptor), 0, 200);
  fighter [n]->trafo.translation.x = px - 35;
  fighter [n]->trafo.translation.z = py;
  fighter [n]->target = fighter [0];
  fighter [n]->currot.phi = 180;
  fighter [n]->getPrototype ()->maxrot.theta = 0;
  n ++;
  fplayer->target = fighter [4];
  fighter [1]->target = fighter [2];
}

/*int MissionDepot::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
  for (i = 0; i <= 3; i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party == 0)
        b = true;
  }
  if (b) return 0;
  return 1;
}*/

void MissionDepot::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
}

#endif
