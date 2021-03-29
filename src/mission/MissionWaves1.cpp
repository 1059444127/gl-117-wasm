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
#include "loadmodel/Model3dRegistry.h"

#include <stdio.h>
#include <string.h>



MissionWaves1::MissionWaves1 ()
{
  id = MISSION_WAVES1;
  strcpy (name, "WAVES");
  strcpy (briefing, "THERE ARE TWO ENEMIES. IF YOU MANAGE TO TAKE THEM OUT, YOU PROCEED TO THE NEXT WAVE CONTAINING THREE OPPONENTS AND SO ON.");
  autoLFBriefing ();
  alliedfighters = 1;
  maxtime = 10000 * timestep;
}

void MissionWaves1::start ()
{
  int i;
  day = 1;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 50;
  heading = 220;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_ALPINE, NULL);
  playerInit ();
  fplayer->trafo.translation.x = 20;
  fplayer->trafo.translation.z = 70;
  for (i = 1; i <= 9; i ++)
  {
    if (i <= 2)
    {
      objectInit (new Fighter (CrowDescriptor), 0, 395);
    }
    else if (i <= 5)
    {
      objectInit (new Fighter (SwallowDescriptor), 0, 395);
      fighter [i]->deactivate ();
    }
    else if (i <= 9)
    {
      objectInit (new Fighter (BuzzardDescriptor), 0, 395);
      fighter [i]->deactivate ();
    }
    fighter [i]->party = 0;
    fighter [i]->target = fighter [0];
    fighter [i]->trafo.translation.x = -i * 10;
    fighter [i]->trafo.translation.z = -i * 10;
  }
  state = 0;
  laststate = 0;
  texttimer = 0;
}

int MissionWaves1::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  if (texttimer >= 200 * timestep) texttimer = 0;
  if (texttimer > 0) texttimer += dt;
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
  state ++;
  if (state == 1)
  {
    for (i = 3; i <= 5; i ++)
    {
      fighter [i]->activate ();
      fighter [i]->trafo.translation.x = fplayer->trafo.translation.x + 50 + 10 * i;
      fighter [i]->trafo.translation.z = fplayer->trafo.translation.z + 50 + 10 * i;
      fighter [i]->trafo.translation.y = l->getHeight (fighter [i]->trafo.translation.x, fighter [i]->trafo.translation.z) + 15;
    }
    playerInit (false);
    return 0;
  }
  else if (state == 2)
  {
    for (i = 6; i <= 9; i ++)
    {
      fighter [i]->activate ();
      fighter [i]->trafo.translation.x = fplayer->trafo.translation.x + 50 + 10 * i;
      fighter [i]->trafo.translation.z = fplayer->trafo.translation.z + 50 + 10 * i;
      fighter [i]->trafo.translation.y = l->getHeight (fighter [i]->trafo.translation.x, fighter [i]->trafo.translation.z) + 15;
    }
    playerInit (false);
    return 0;
  }
//  fplayer->shield = 1;
  return 1;
}

void MissionWaves1::draw ()
{
  int timeroff = 100 * timestep, /*timerdelay = 300 * timestep,*/ timerlag = 20 * timestep;

  if (laststate != state)
  {
    texttimer = 1;
    laststate = state;
  }
  if (texttimer > 0)
  {
    if (state == 1)
    {
      font1->drawTextCentered (0, 6, -2.5, "NEXT WAVE - YOU ARE FULLY REPAIRED", textcolor);
      return;
    }
    if (state == 2)
    {
      font1->drawTextCentered (0, 6, -2.5, "NEXT WAVE - YOU ARE FULLY REPAIRED", textcolor);
      return;
    }
  }

  if (timer >= 0 && timer <= timeroff - timerlag)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
}

#endif
