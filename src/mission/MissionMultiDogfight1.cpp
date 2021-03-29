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



MissionMultiDogfight1::MissionMultiDogfight1 ()
{
  id = MISSION_MULTIPLAYER_DOGFIGHT;
  strcpy (name, "MULTIPLAYER");
  strcpy (briefing, "THIS IS JUST A SIMPLE DEMO MISSION");
  autoLFBriefing ();
  alliedfighters = 2;
}

void MissionMultiDogfight1::start ()
{
  int i;
  day = 1;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 25;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_ALPINE, NULL);
  for (i = 0; i <= 1; i ++)
  {
    fighter [i]->activate ();
    fighter [i]->ai = false;
    fighter [i]->party = i;
    fighter [i]->target = fighter [0];
    fighter [i]->trafo.translation.x = 50 + i * 30;
    fighter [i]->trafo.translation.z = 100;
  }
  fighter[1]->ai = true;
  if (isserver)
    fplayer = fighter [0];
  else
    fplayer = fighter [1];
  if (controls != 100)
    fplayer->easymodel = 2;
}

/*int MissionMultiDogfight1::processtimer (Uint32 dt)
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

void MissionMultiDogfight1::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 0, -1, "DOGFIGHT");
  }
}

#endif
