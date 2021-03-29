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



MissionTest1::MissionTest1 ()
{
  id = MISSION_TEST1;
  strcpy (name, "EAGLE TEST1");
  strcpy (briefing, "WELCOME TO THE EAGLE SQUADRON. BEFORE YOU CAN FLY SERIOUS MISSIONS YOU HAVE TO SHOW YOUR SKILLS. FIRST DESTROY ALL DUMMY GROUND TARGETS IN THE DESIGNED REGION USING THE CANNON ONLY.");
  autoLFBriefing ();
  alliedfighters = 1;
  maxtime = 5000 * timestep;
//  alliedpilot [0] = PILOT_PRIMETIME;
}

void MissionTest1::start ()
{
  int i;
  day = 1;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 55;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_ALPINE_EROSION, NULL);
  int px, py;
  l->searchPlain (-1, -1, &px, &py);
  playerInit ();
  fplayer->trafo.translation.x = px;
  fplayer->trafo.translation.z = py + 100;
  for (i = 0; i < missiletypes; i ++)
  {
    fplayer->missiles [i] = 0;
  }
  for (i = 1; i <= 6; i ++)
  {
    objectInit (new StaticPassive (PickupDescriptor), 0, 400);
    fighter [i]->trafo.translation.x = px + 4 - ((i - 1) / 2) * 4;
    fighter [i]->trafo.translation.z = py + 4 - ((i - 1) & 1) * 8;
  }
}

/*int MissionTest1::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
  for (i = 0; i <= 6; i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party == 0)
        b = true;
  }
  if (b) return 0;
  return 1;
}*/

void MissionTest1::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
}

#endif
