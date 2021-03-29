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



MissionMoonBattle::MissionMoonBattle ()
{
  id = MISSION_MOONBATTLE;
  strcpy (name, "MOON BATTLE");
  strcpy (briefing, "THERE ARE HEAVY AIR FORCES ATTACKING. WE HAVE TO FACE THEM BEFORE THEY CAN REACH OUR HEADQUARTERS.");
  autoLFBriefing ();
  selfighter [0] = RedArrowDescriptor;
  selfighter [1] = Hawk2Descriptor;
  selfighter [2] = PhoenixDescriptor;
  alliedfighters = 7;
  maxtime = 6000 * timestep;
  alliedpilot [0] = PILOT_MATRIX;
  alliedpilot [1] = PILOT_SHADOW;
  alliedpilot [2] = PILOT_FIREBIRD;
  alliedpilot [3] = PILOT_HEPTARGON;
  alliedpilot [4] = PILOT_LARA;
  alliedpilot [5] = PILOT_DRDOOM;
}

void MissionMoonBattle::start ()
{
  int i;
  day = 0;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 45;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_MOON, NULL);
  playerInit ();
  fplayer->trafo.translation.x = 0;
  fplayer->trafo.translation.z = 100;
  for (i = 1; i <= 6; i ++)
  {
    alliedInit (FalconDescriptor, alliedpilot [i - 1]);
    if (i % 2)
      fighter [i]->trafo.translation.x = ((i + 1) / 2) * 5;
    else
      fighter [i]->trafo.translation.x = -((i + 1) / 2) * 5;
    fighter [i]->trafo.translation.z = 100 + i * 5;
  }
  for (i = 7; i <= 25; i ++)
  {
    if (i <= 16)
    {
      objectInit (new Fighter (CrowDescriptor), 0, 450 - i * 10);
      fighter [i]->trafo.translation.x = -i * 5;
      fighter [i]->trafo.translation.z = -i * 5;
    }
    else if (i <= 24)
    {
      objectInit (new Fighter (BlackBirdDescriptor), 0, 450 - i * 10);
      fighter [i]->trafo.translation.x = -i * 8 - 150;
      fighter [i]->trafo.translation.z = -i * 8 - 150;
    }
    else
    {
      objectInit (new Fighter (CrowDescriptor), 0, 450 - i * 10);
      fighter [i]->trafo.translation.x = -i * 8 - 350;
      fighter [i]->trafo.translation.z = -i * 8 - 350;
    }
    fighter [i]->target = fighter [Math::random (7)];
  }
  for (i = 1; i <= 6; i ++)
  {
    fighter [i]->target = fighter [7 + i];
  }

  invertZ (); // only invert if NO objects are mapped to flat ground
}

/*int MissionMoonBattle::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
  for (i = 0; i <= 24; i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party == 0)
        b = true;
  }
  if (b) return 0;
  return 1;
}*/

void MissionMoonBattle::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
}

#endif
