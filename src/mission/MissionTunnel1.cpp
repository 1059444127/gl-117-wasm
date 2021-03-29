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



MissionTunnel1::MissionTunnel1 ()
{
  selfighter [0] = RedArrowDescriptor;
  selfighter [1] = Hawk2Descriptor;
  id = MISSION_TUNNEL1;
  strcpy (name, "TUNNEL");
  strcpy (briefing, "THE TERRAIN TO GET NEAR THE ENEMY'S HQ IS GUARDED BY ELITE BUZZARD FIGHTERS. BUT THERE IS A TRENCH WE CAN PROTUDE TO GET NEAR THEIR BASE WITHOUT BEING SIGHTED. FLY AHEAD TO SCOUT THE TRENCH. JUST MAKE YOUR WAY THROUGH!");
  autoLFBriefing ();
  alliedfighters = 1;
  maxtime = 5000 * timestep;
}

void MissionTunnel1::start ()
{
  int i;
  day = 0;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 60;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_CANYON_TRENCH, NULL);
  playerInit ();
  fplayer->trafo.translation.x = 256;
  fplayer->trafo.translation.z = 256;
  fplayer->currot.phi = 90;
  for (i = 1; i <= 9; i ++)
  {
    objectInit (new StaticAa (SacDescriptor), 0, 200);
    int ix = (i / 2) * 8 - 200 + 256;
    int iy = (i & 1) * 4 - 2 + 256;
    fighter [i]->trafo.translation.x = ix;
    fighter [i]->trafo.translation.z = iy;
    fighter [i]->target = fighter [0];
  }
  objectInit (new StaticAa (SamDescriptor), 0, 200);
  fighter [i]->trafo.translation.x = -200 + 256;
  fighter [i]->trafo.translation.z = 0 + 256;
  fighter [i]->target = fighter [0];
  for (i = 11; i <= 15; i ++)
  {
    objectInit (new StaticAa (SacDescriptor), 0, 300);
    int ix = (i - 11) * 10 - 100 + 256;
    int iy = (i % 2) * 3 - 3 + 256;
    fighter [i]->trafo.translation.x = ix;
    fighter [i]->trafo.translation.z = iy;
    fighter [i]->target = fighter [0];
  }
  objectInit (new StaticAa (SamDescriptor), 0, 200);
  fighter [i]->trafo.translation.x = -250 + 256;
  fighter [i]->trafo.translation.z = -2 + 256;
  fighter [i]->target = fighter [0];
  i ++;
  objectInit (new StaticAa (SamDescriptor), 0, 200);
  fighter [i]->trafo.translation.x = -250 + 256;
  fighter [i]->trafo.translation.z = 2 + 256;
  fighter [i]->target = fighter [0];
  i ++;
  objectInit (new StaticPassive (LaserBarrierDescriptor), 0, 100);
  fighter [i]->trafo.translation.x = -350 + 256;
  fighter [i]->trafo.translation.z = 0 + 256;
  for (i = 19; i < 26; i ++)
  {
    objectInit (new StaticPassive (BuzzardDescriptor), 0, i * 8);
    fighter [i]->target = fighter [0];
    fighter [i]->trafo.translation.x = 256;
    fighter [i]->trafo.translation.z = 256;
    fighter [i]->deactivate ();
  }
  fplayer->target = fighter [6];
}

int MissionTunnel1::processtimer (Uint32 dt)
{
//  bool b = false;
  int i;
  if (timer <= 0) fplayer->trafo.translation.y = l->getHeight (fplayer->trafo.translation.x, fplayer->trafo.translation.z) + 5;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
//  if (timer > 40 * timestep)
  {
    if (fplayer->trafo.translation.y - l->getHeight (fplayer->trafo.translation.x, fplayer->trafo.translation.z) > 15)
    {
      if (!fighter [24]->active && fighter [24]->shield > 0)
      {
        for (i = 19; i < 26; i ++)
        {
          fighter [i]->activate ();
          fighter [i]->trafo.translation.x = fplayer->trafo.translation.x - 80 - (i - 18) * 8;
          fighter [i]->trafo.translation.z = fplayer->trafo.translation.z - 20;
          fighter [i]->trafo.translation.y = l->getHeight (fighter [i]->trafo.translation.x, fighter [i]->trafo.translation.z) + 25;
        }
      }
    }
  }
  if (fplayer->trafo.translation.x < -450 + 256)
    return 1;
  return 0;
}

void MissionTunnel1::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
}

#endif
