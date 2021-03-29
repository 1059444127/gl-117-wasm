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



MissionTank1::MissionTank1 ()
{
  id = MISSION_TANK1;
  strcpy (name, "TANKS");
  strcpy (briefing, "THE ENEMY IS GATHERING LOTS OF TANKS. FLY AN ASSAUT AND DESTROY THEM.");
  autoLFBriefing ();
  alliedfighters = 2;
  maxtime = 4500 * timestep;
  selfighter [1] = Hawk2Descriptor;
  alliedpilot [0] = PILOT_SHADOW;
}

void MissionTank1::start ()
{
  int i;
  day = 0;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 40;
  heading = 280;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_DESERT, NULL);
  playerInit ();
  fplayer->trafo.translation.x = 0;
  fplayer->trafo.translation.z = 100;
  alliedInit (Hawk2Descriptor, alliedpilot [0]);
  fighter [1]->trafo.translation.x = 5;
  fighter [1]->trafo.translation.z = 105;
  for (i = 2; i <= 7; i ++)
  {
    objectInit (new Tank (PantherDescriptor), 0, 400 - i * 25);
    fighter [i]->target = fighter [Math::random (2)];
    fighter [i]->trafo.translation.x = -i * 4;
    fighter [i]->trafo.translation.z = -i * 4;
    fighter [i]->getPrototype ()->maxthrust = 0;
    fighter [i]->thrust = 0;
  }
  for (i = 8; i <= 10; i ++)
  {
    objectInit (new Tank (WieselDescriptor), 0, 80);
    fighter [i]->target = fighter [Math::random (2)];
    fighter [i]->trafo.translation.x = i * 3;
    fighter [i]->trafo.translation.z = i * 3;
  }
  for (i = 11; i <= 13; i ++)
  {
    objectInit (new StaticPassive (ContainerDescriptor), 0, 0);
    fighter [i]->trafo.translation.x = i * 3;
    fighter [i]->trafo.translation.z = 60;
  }
  invertZ (); // only invert if NO objects are mapped to flat ground
}

/*int MissionTank1::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
  for (i = 0; i <= 15; i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party == 0)
        b = true;
  }
  if (b) return 0;
  return 1;
}*/

void MissionTank1::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
}

#endif
