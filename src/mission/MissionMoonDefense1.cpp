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



MissionMoonDefense1::MissionMoonDefense1 ()
{
  selfighter [0] = RedArrowDescriptor;
  selfighter [1] = Hawk2Descriptor;
  selfighter [2] = PhoenixDescriptor;
  alliedpilot [0] = PILOT_LARA;
  id = MISSION_MOON1;
  strcpy (name, "TURRETS");
  strcpy (briefing, "THE DESTINY OF OUR FUTURE IS SOMEWHERE ON THE MOON. OUR NEW BOMBER GL-117 WITH ITS SUPERIOR SHIELDS SHOULD BE EXTREMELY HELPFUL. FIRSTLY, WE HAVE TO TAKE OUT THE TURRETS.");
  autoLFBriefing ();
  alliedfighters = 2;
  maxtime = 5000 * timestep;
}

void MissionMoonDefense1::start ()
{
  int i;
  day = 0;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 50;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_MOON, NULL);
//  l->genTrench (12, 5000);
  playerInit ();
  fplayer->trafo.translation.x = 100;
  fplayer->trafo.translation.z = 100;
  fplayer->currot.phi = 45;
  alliedInit (PhoenixDescriptor, alliedpilot [0]);
  fighter [1]->trafo.translation.x = 105;
  fighter [1]->trafo.translation.z = 105;
  fighter [1]->currot.phi = 45;
  for (i = 2; i <= 10; i ++)
  {
    objectInit (new StaticAa (SacDescriptor), 0, 200);
    int ix = (i - 2) % 3;
    int iy = (i - 2) / 3;
    fighter [i]->trafo.translation.x = ix * 10;
    fighter [i]->trafo.translation.z = iy * 10;
    fighter [i]->target = fighter [Math::random (2)];
  }
  for (i = 11; i <= 20; i ++)
  {
    objectInit (new AiObj (MineDescriptor), 0, 220);
    int ix = (i - 11) % 3;
    int iy = (i - 11) / 3;
    fighter [i]->trafo.translation.x = ix * 20 + 40;
    fighter [i]->trafo.translation.z = iy * 20 + 40;
    fighter [i]->target = fighter [Math::random (2)];
  }
  fplayer->target = fighter [6];
  fighter [1]->target = fighter [6];

  invertZ (); // only invert if NO objects are mapped to flat ground
}

/*int MissionMoonDefense1::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
  for (i = 0; i <= 10; i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party == 0)
        b = true;
  }
  if (b) return 0;
  return 1;
}*/

void MissionMoonDefense1::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
}

#endif
