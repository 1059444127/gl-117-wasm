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



MissionTest2::MissionTest2 ()
{
  id = MISSION_TEST2;
  strcpy (name, "EAGLE TEST2");
  strcpy (briefing, "NOW SHOW YOUR PILOTING SKILLS AND GET DOWN YOUR \"ENEMY\". JUST GET AT HIS BACK - NO MISSILES, NO CANNON.");
  autoLFBriefing ();
  alliedfighters = 1;
  maxtime = 3500 * timestep;
//  alliedpilot [0] = PILOT_PRIMETIME;
}

void MissionTest2::start ()
{
  int i;
  day = 1;
  clouds = 1;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 55;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_LOW_ALPINE, NULL);
  playerInit ();
  fplayer->trafo.translation.x = 0;
  fplayer->trafo.translation.z = 50;
  for (i = 0; i < missiletypes; i ++)
  {
    fplayer->missiles [i] = 0;
  }
  fplayer->ammo = 0;

  objectInit (new Fighter (HawkDescriptor), 0, 170);
  fighter [1]->target = fplayer;
  fighter [1]->trafo.translation.x = 0;
  fighter [1]->trafo.translation.z = 0;
  fighter [1]->aggressivity = 0;
  for (i = 0; i < missiletypes; i ++)
  {
    fighter [1]->missiles [i] = 0;
  }
  fighter [1]->ammo = 0;
  fighter [1]->shield = 100000;
  fighter [1]->currot.phi = 180;
  invertZ (); // only invert if NO objects are mapped to flat ground
}

int MissionTest2::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
  for (i = 0; i <= 1; i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party == 0)
        b = true;
  }
  fplayer->aw = fplayer->getAngle (fighter [1]);
  state = 0;
  float fplayerh = fplayer->getAngleH (fighter [1]);
  if (fabs (fplayer->aw) < 10 && fabs (fighter [1]->aw) <= 130)
  {
    if (fabs (fplayerh) < 10)
      state = -1;
  }
  if (fabs (fplayer->aw) < 10 && fabs (fighter [1]->aw) > 130)
  {
    if (fabs (fplayerh) < 10 && fplayer->distance (fighter [1]) < 20)
      return 1;
  }
  if (fabs (fighter [1]->aw) < 10 && fabs (fplayer->aw) > 130)
  {
    if (fabs ((float) fighter [1]->getAngleH (fplayer)) < 10 && fighter [1]->distance (fplayer) < 20)
      return 2;
  }
  if (b) return 0;
  return 1;
}

void MissionTest2::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
  else if (state == -1)
  {
    font1->drawTextCentered (0, 4, -2, "ALMOST! GET CLEARLY BEHIND HIM!", textcolor);
  }
}

#endif
