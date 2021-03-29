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



MissionConvoy::MissionConvoy ()
{
  id = MISSION_CONVOY;
  strcpy (name, "CONVOY");
  strcpy (briefing, "WE HAVE SPOTTED AN ENEMY CONVOY. THIS IS YOUR NEXT TARGET. BE CAREFUL, THERE ARE WIESEL TANKS WITH SURFACE-AIR CANNONS.");
  autoLFBriefing ();
  alliedfighters = 2;
  maxtime = 5000 * timestep;
  alliedpilot [0] = PILOT_PRIMETIME;
}

void MissionConvoy::start ()
{
  int i;
  day = 1;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 45;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_ALPINE_EROSION, NULL);
  int px, py;
  l->searchPlain (-1, -1, &px, &py);
  playerInit ();
  fplayer->trafo.translation.x = px;
  fplayer->trafo.translation.z = py + 100;
  alliedInit (HawkDescriptor, alliedpilot [0]); // always match together 0<->1, 1<->2 etc.
  fighter [1]->trafo.translation.x = px + 5;
  fighter [1]->trafo.translation.z = py + 105;
  for (i = 2; i <= 3; i ++)
  {
    objectInit (new Tank (WieselDescriptor), 0, 200);
    fighter [i]->target = fighter [0];
    int phi = (i - 2) * 180;
    fighter [i]->trafo.translation.x = px + SIN(phi) * 5.0;
    fighter [i]->trafo.translation.z = py + COS(phi) * 5.0;
    fighter [i]->currot.phi = 359 - phi;
    fighter [i]->thrust = 0;
    fighter [i]->getPrototype ()->maxthrust = 0;
  }
  for (i = 4; i <= 9; i ++)
  {
    if (i == 6 || i == 9) objectInit (new StaticPassive (PickupDescriptor), 0, 400);
    else objectInit (new StaticPassive (TruckDescriptor), 0, 400);
    fighter [i]->target = fighter [0];
    fighter [i]->trafo.translation.x = px + 7.5 - i;
    fighter [i]->trafo.translation.z = py + 7.5 - i;
  }
}

/*int MissionConvoy::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
  for (i = 0; i <= 9; i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party == 0)
        b = true;
  }
  if (b) return 0;
  return 1;
}*/

void MissionConvoy::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
}

#endif
