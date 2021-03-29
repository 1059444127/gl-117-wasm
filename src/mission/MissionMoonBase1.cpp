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



MissionMoonBase1::MissionMoonBase1 ()
{
  selfighter [0] = RedArrowDescriptor;
  selfighter [1] = Hawk2Descriptor;
  selfighter [2] = PhoenixDescriptor;
  id = MISSION_MOON3;
  strcpy (name, "SNEAKING");
  strcpy (briefing, "TIME TO ATTACK. THE BASE IS HEAVILY GUARDED BY BLACKBIRDS. USE AN AIR FIELD TO GET NEAR THE BASE AND DESTROY IT.");
  autoLFBriefing ();
  alliedfighters = 1;
  maxtime = 3500 * timestep;
}

void MissionMoonBase1::start ()
{
  int i;
  day = 0;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 50;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_FLAT_MOON, NULL);
  playerInit ();
  fplayer->trafo.translation.x = -20;
  fplayer->trafo.translation.z = -40;
  fplayer->currot.phi = 200;
  for (i = 1; i < 21; i ++)
  {
    objectInit (new AiObj (AsteroidDescriptor), 0, 150);
    fighter [i]->trafo.translation.x = COS((i * 140) % 360) * 4.5;
    fighter [i]->trafo.translation.z = i * 0.6;
    fighter [i]->trafo.translation.y = l->getHeight (fighter [i]->trafo.translation.x, fighter [i]->trafo.translation.z) + SIN((i * 160) % 360) * 4.0 + 25.0;
    fighter [i]->target = fighter [0];
    fighter [i]->currot.phi = 200;
  }
  for (i = 21; i < 29; i ++)
  {
    objectInit (new Fighter (BlackBirdDescriptor), 0, 200);
    fighter [i]->trafo.translation.x = (i - 20) * 4;
    fighter [i]->trafo.translation.z = 210;
    fighter [i]->target = fighter [0];
    fighter [i]->currot.phi = 50;
  }
  objectInit (new StaticPassive (MoonBaseDescriptor), 0, 100);
  fighter [29]->trafo.translation.x = 50 + difficulty * 15;
  fighter [29]->trafo.translation.z = 180 + difficulty * 30;
  fplayer->target = fighter [29];
}

int MissionMoonBase1::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
  for (i = 29; i <= 29; i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party == 0)
        b = true;
  }
  if (b) return 0;
  return 1;
}

void MissionMoonBase1::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
}

#endif
