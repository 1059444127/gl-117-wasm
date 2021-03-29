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



MissionConvoy2::MissionConvoy2 ()
{
  id = MISSION_CONVOY2;
  strcpy (name, "SAM CONVOY");
  strcpy (briefing, "THERE IS A CONVOY DELIVERING WEAPONS. DESTROY IT BUT CAUTION: THERE ARE MOBILE SAMS AND POSSIBLY ENEMY FIGHTERS IN THE REGION");
  autoLFBriefing ();
  alliedfighters = 2;
  maxtime = 5000 * timestep;
  selfighter [1] = Hawk2Descriptor;
  alliedpilot [0] = PILOT_SHADOW;
}

void MissionConvoy2::start ()
{
  int i;
  day = 1;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 60;
  heading = 230;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_DESERT, NULL);
  int px = 200, py = 200;
  playerInit ();
  fplayer->trafo.translation.x = px;
  fplayer->trafo.translation.z = py + 100;
  alliedInit (HawkDescriptor, alliedpilot [0]); // always match together 0<->1, 1<->2 etc.
  fighter [1]->trafo.translation.x = px + 5;
  fighter [1]->trafo.translation.z = py + 105;
  for (i = 2; i <= 3; i ++)
  {
    objectInit (new Tank (MobileSamDescriptor), 0, 200);
    fighter [i]->target = fighter [0];
    int phi = (i - 2) * 180;
    fighter [i]->trafo.translation.x = px + SIN(phi) * 5.0;
    fighter [i]->trafo.translation.z = py + COS(phi) * 5.0;
    fighter [i]->thrust = 0;
    fighter [i]->getPrototype ()->maxthrust = 0;
  }
  for (i = 4; i <= 9; i ++)
  {
    if (i == 6 || i == 7)
      objectInit (new Tank (TruckDescriptor), 0, 400);
    else
      objectInit (new Tank (Truck2Descriptor), 0, 400);
    fighter [i]->target = fighter [0];
    fighter [i]->trafo.translation.x = px + (i % 3) * 3 - 4;
    fighter [i]->trafo.translation.z = py + (i / 3) * 3 - 4;
  }
  for (i = 10; i <= 12; i ++)
  {
    objectInit (new Fighter (CrowDescriptor), 0, 400 - i * 10);
    fighter [i]->target = fighter [Math::random (2)];
//    fighter [i]->o = &model_fige;
    fighter [i]->trafo.translation.x = px - i * 10;
    fighter [i]->trafo.translation.z = py - i * 10;
  }
  invertZ (); // only invert if NO objects are mapped to flat ground
}

/*int MissionConvoy2::processtimer (Uint32 dt)
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

void MissionConvoy2::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
}

#endif
