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



MissionGround1::MissionGround1 ()
{
  id = MISSION_SADEFENSE;
  strcpy (name, "SA DEFENSE");
  strcpy (briefing, "OUR ENEMY HAS BUILT UP A BASE WITH SOME AIR DEFENCES AROUND. THE PILOTS OF OUR SQUADRON HAVE TO TAKE OUT THESE AirBeginDescriptor DEFENCES.");
  autoLFBriefing ();
  alliedfighters = 2;
  alliedpilot [0] = PILOT_PRIMETIME;
  maxtime = 3500 * timestep;
}

void MissionGround1::start ()
{
  int i;
  day = 0;
  clouds = 3;
  weather = WEATHER_THUNDERSTORM;
  camera = 0;
  sungamma = 40;
  heading = 200;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_ALPINE_NOLAKE, NULL);
  int px, py;
  l->searchPlain (1, 1, &px, &py);
  l->flatten (px, py, 3, 3);
//  px = px - MAXX / 2;
//  py = MAXX / 2 - py;
  playerInit ();
  fplayer->trafo.translation.x = px + 10;
  fplayer->trafo.translation.z = py + 80;
  alliedInit (FalconDescriptor, alliedpilot [0]);
  fighter [1]->trafo.translation.x = px + 20;
  fighter [1]->trafo.translation.z = py + 90;
  for (i = 2; i <= 4; i ++)
  {
    if (i == 2)
      objectInit (new StaticAa (SamDescriptor), 0, 220);
    else
      objectInit (new StaticAa (SacDescriptor), 0, 200);
    fighter [i]->trafo.translation.x = px - 9 + i * 3;
    fighter [i]->trafo.translation.z = py;
    fighter [i]->target = fighter [0];
  }
  l->searchPlain (2, 1, &px, &py);
  l->flatten (px, py, 3, 3);
  for (i = 5; i <= 6; i ++)
  {
    objectInit (new StaticAa (SacDescriptor), 0, 200);
    fighter [i]->trafo.translation.x = px - 17 + i * 3;
    fighter [i]->trafo.translation.z = py;
    fighter [i]->target = fighter [0];
  }
  objectInit (new StaticPassive (TentDescriptor), 0, 200);
  fighter [7]->trafo.translation.x = px + 1;
  fighter [7]->trafo.translation.z = py - 1;
  objectInit (new StaticPassive (TentDescriptor), 0, 200);
  fighter [8]->trafo.translation.x = px - 1;
  fighter [8]->trafo.translation.z = py - 1;
  fplayer->target = fighter [2];
  fighter [1]->target = fighter [2];
}

/*int MissionGround1::processtimer (Uint32 dt)
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

void MissionGround1::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
}

#endif
