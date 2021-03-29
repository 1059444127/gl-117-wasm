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



MissionDeathmatch2::MissionDeathmatch2 ()
{
  id = MISSION_DEATHMATCH2;
  strcpy (name, "TEAM DEATHMATCH");
  strcpy (briefing, "THIS IS A SHORT TEAM DEATHMATCH - 8 OPPONENTS IN 4 TEAMS AND 12 KILLS TO WIN. THE DIFFICULTY STILL DETERMINES THE OPPONENTS STRENGTH.");
  autoLFBriefing ();
  alliedfighters = 1;
  selweapons = 1;
  selfighters = 1;
  maxtime = 10000 * timestep;
}

void MissionDeathmatch2::start ()
{
  int i;
  day = 1;
  clouds = 2;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 45;
  heading = 220;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_ALPINE, NULL);
  playerInit ();
  fplayer->trafo.translation.x = 0;
  fplayer->trafo.translation.z = 50;
  for (i = 1; i <= 7; i ++)
  {
    if (i <= 1)
    {
      objectInit (new Fighter (FalconDescriptor), i / 2 + 1, 200);
    }
    else if (i <= 3)
    {
      objectInit (new Fighter (CrowDescriptor), i / 2 + 1, 200);
    }
    else if (i <= 5)
    {
      objectInit (new Fighter (BuzzardDescriptor), i / 2 + 1, 200);
    }
    else
    {
      objectInit (new Fighter (SwallowDescriptor), i / 2 + 1, 200);
    }
    fighter [i]->trafo.translation.x = 50 * SIN(i * 360 / 8);
    fighter [i]->trafo.translation.z = 50 * COS(i * 360 / 8);
  }
  for (i = 1; i <= 7; i ++)
  {
    fighter [i]->target = fighter [(i + 4) % 8];
  }
  state = 0;
  laststate = 0;
  texttimer = 0;
}

int MissionDeathmatch2::processtimer (Uint32 dt)
{
//  bool b = false;
  int i;
  if (texttimer >= 200 * timestep) texttimer = 0;
  if (texttimer > 0) texttimer += dt;
  timer += dt;
  for (i = 0; i <= 3; i ++)
  {
    if (fighter [i * 2]->stat.fighterkills + fighter [i * 2 + 1]->stat.fighterkills >= 12)
    {
//      fplayer->shield = 1;
      if (i == 0) return 1;
      else return 2;
    }
  }
  for (i = 0; i < fighter.size (); i ++)
  {
    if (!fighter [i]->active && fighter [i]->explode >= 35 * timestep)
    {
      reincarnateFighter (i);
    }
  }
  return 0;
}

void MissionDeathmatch2::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
}

#endif
