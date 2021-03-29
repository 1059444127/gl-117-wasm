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



MissionScout::MissionScout ()
{
  id = MISSION_SCOUT;
  strcpy (name, "SCOUTS");
  strcpy (briefing, "ENEMY SCOUTS ARE APPROACHING THIS REGION. BE CARFUL, THEY ARE FLYING BUZZARDS AND SHOULD BE CONSIDERED EQUAL TO OUR FALCONS.");
  autoLFBriefing ();
  alliedfighters = 2;
  maxtime = 3000 * timestep;
  alliedpilot [0] = PILOT_PRIMETIME;
}

void MissionScout::start ()
{
  int i;
  day = 1;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 22;
  heading = 250;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_ALPINE, NULL);
  playerInit ();
  fplayer->trafo.translation.x = 0;
  fplayer->trafo.translation.z = 100;
  alliedInit (FalconDescriptor, alliedpilot [0]);
  fighter [1]->trafo.translation.x = 5;
  fighter [1]->trafo.translation.z = 105;
  for (i = 2; i <= 4; i ++)
  {
    objectInit (new Fighter (BuzzardDescriptor), 0, 170);
    fighter [i]->target = fighter [Math::random (2)];
    fighter [i]->trafo.translation.x = -i * 10;
    fighter [i]->trafo.translation.z = -i * 10;
  }
  invertZ (); // only invert if NO objects are mapped to flat ground
}

/*int MissionScout::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
  for (i = 0; i <= 4; i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party == 0)
        b = true;
  }
  if (b) return 0;
  return 1;
}*/

void MissionScout::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
}

#endif
