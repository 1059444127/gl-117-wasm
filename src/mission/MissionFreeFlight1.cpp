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



MissionFreeFlight1::MissionFreeFlight1 ()
{
  id = MISSION_FREEFLIGHT1;
  strcpy (name, "FREE FLIGHT");
  strcpy (briefing, "THIS IS A SIMPLE TRAINING MISSION: SHOOT ALL TRANSPORTER WAVES.");
  autoLFBriefing ();
  alliedfighters = 1;
  selweapons = 2;
  maxtime = 4000 * timestep;
}

void MissionFreeFlight1::start ()
{
  int i;
  day = 1;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 50;
  heading = 200;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_LOW_ALPINE, NULL);
  playerInit ();
  fplayer->trafo.translation.x = 20;
  fplayer->trafo.translation.z = 70;
  for (i = 1; i <= 2; i ++)
  {
    Fighter *f;
    f = new Fighter (TransportDescriptor);
    objectInit (f, 0, 395);
    f->trafo.translation.x = -i * 10;
    f->trafo.translation.z = -i * 10;
  }
  state = 0;
  laststate = 0;
  texttimer = 0;
}

int MissionFreeFlight1::processtimer (Uint32 dt)
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
  for (i = 0; i < fighter.size (); i ++)
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
      Fighter *f;
      f = new Fighter (TransportDescriptor);
      objectInit (f, 0, 395);
      int phi = 120 * i;
      f->trafo.translation.x = fplayer->trafo.translation.x + 40 * COS(phi);
      f->trafo.translation.z = fplayer->trafo.translation.z + 40 * SIN(phi);
      f->trafo.translation.y = l->getHeight (f->trafo.translation.x, f->trafo.translation.z) + 25;
    }
    return 0;
  }
  else if (state == 2)
  {
    for (i = 6; i <= 9; i ++)
    {
      Fighter *f;
      f = new Fighter (TransportDescriptor);
      objectInit (f, 0, 395);
      int phi = 90 * i;
      f->trafo.translation.x = fplayer->trafo.translation.x + 40 * COS(phi);
      f->trafo.translation.z = fplayer->trafo.translation.z + 40 * SIN(phi);
      f->trafo.translation.y = l->getHeight (f->trafo.translation.x, f->trafo.translation.z) + 25;
    }
    return 0;
  }
//  fplayer->shield = 1;
  return 1;
}

void MissionFreeFlight1::draw ()
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
      font1->drawTextCentered (0, 6, -2.5, "TRANSPORTERS RESPAWNING", textcolor);
      return;
    }
    if (state == 2)
    {
      font1->drawTextCentered (0, 6, -2.5, "TRANSPORTERS RESPAWNING", textcolor);
      return;
    }
  }

  if (timer >= 0 && timer <= timeroff - timerlag)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
}

#endif
