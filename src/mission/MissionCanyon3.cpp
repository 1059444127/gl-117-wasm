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



MissionCanyon3::MissionCanyon3 ()
{
  selfighter [0] = RedArrowDescriptor;
  selfighter [1] = Hawk2Descriptor;
  alliedpilot [0] = PILOT_MATRIX;
  alliedpilot [1] = PILOT_PRIMETIME;
  alliedpilot [2] = PILOT_FIREBIRD;
  alliedpilot [3] = PILOT_LARA;
  id = MISSION_CANYON3;
  strcpy (name, "MAIN BASE");
  strcpy (briefing, "THERE IS ONE MAIN BASE IN THE CANYON, CONNECTED TO A BASE ON THE MOON. WE MUST TAKE OUT THIS BASE FIRST!");
  autoLFBriefing ();
  alliedfighters = 4;
  maxtime = 3000 * timestep;
}

void MissionCanyon3::start ()
{
  int i, px, py;
  day = 0;
  clouds = 3;
  weather = WEATHER_THUNDERSTORM;
  camera = 0;
  sungamma = 45;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_CANYON, NULL);
  l->searchPlain (-1, -1, &px, &py);
  playerInit ();
  fplayer->trafo.translation.x = px + 100;
  fplayer->trafo.translation.z = py + 100;
  fplayer->currot.phi = 45;
  for (i = 1; i <= 4; i ++)
  {
    alliedInit (RedArrowDescriptor, alliedpilot [i - 1]);
    if (i % 2)
      fighter [i]->trafo.translation.x = px + 100 + ((i + 1) / 2) * 5;
    else
      fighter [i]->trafo.translation.x = px + 100 - ((i + 1) / 2) * 5;
    fighter [i]->trafo.translation.z = py + 100 + i * 5;
    fighter [i]->currot.phi = 45;
  }
  objectInit (new StaticPassive (ComplexDescriptor), 0, 0);
  fighter [5]->trafo.translation.x = px;
  fighter [5]->trafo.translation.z = py;
  fighter [5]->getPrototype ()->maxthrust = 0;
  fighter [5]->thrust = 0;
  for (i = 6; i <= 9; i ++)
  {
    objectInit (new StaticPassive (HallDescriptor), 0, 100);
    fighter [i]->getPrototype ()->maxthrust = 0;
    fighter [i]->thrust = 0;
  }
  fighter [6]->trafo.translation.x = px + 3;
  fighter [6]->trafo.translation.z = py + 3;
  fighter [7]->trafo.translation.x = px - 3;
  fighter [7]->trafo.translation.z = py + 3;
  fighter [8]->trafo.translation.x = px - 3;
  fighter [8]->trafo.translation.z = py - 3;
  fighter [9]->trafo.translation.x = px + 3;
  fighter [9]->trafo.translation.z = py - 3;
  for (i = 10; i <= 19; i ++)
  {
    if (i <= 15)
    {
      objectInit (new Fighter (CrowDescriptor), 0, i * 20);
    }
    else
    {
      objectInit (new Fighter (BuzzardDescriptor), 0, i * 15);
    }
    fighter [i]->target = fighter [Math::random (5)];
    fighter [i]->currot.phi = 180;
    fighter [i]->trafo.translation.x = px - i * 3;
    fighter [i]->trafo.translation.z = py - i * 3;
  }
  fplayer->target = fighter [5];
  for (i = 1; i <= 4; i ++)
    fighter [i]->target = fighter [5 + i];
}

/*int MissionCanyon3::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
  for (i = 0; i <= 19; i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party == 0)
        b = true;
  }
  if (b) return 0;
  return 1;
}*/

void MissionCanyon3::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
}

#endif
