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



MissionTutorial2::MissionTutorial2 ()
{
  id = MISSION_TUTORIAL2;
  strcpy (name, "TUTORIAL: BOMBER");
  strcpy (briefing, "THE SECOND TUTORIAL SHOWS YOU HOW TO USE THE WEAPONS OF A BOMBER TO BLOW UP HEAVILY ARMORED TARGETS.");
  autoLFBriefing ();
  alliedfighters = 1;
  maxtime = 5000 * timestep;
  selfighter [0] = StormDescriptor;
  selfighters = 1;
  wantfighter = 0;
  selweapons = 1;
  selweapon [0] = DfmDescriptor;
  wantweapon = 0;
//  alliedpilot [0] = PILOT_PRIMETIME;
}

void MissionTutorial2::start ()
{
  int i;
  day = 1;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 50;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_ALPINE_EROSION, NULL);
  int px, py;
  l->searchPlain (-1, -1, &px, &py);
  playerInit ();
  fplayer->trafo.translation.x = px;
  fplayer->trafo.translation.z = py + 150;
  for (i = 1; i <= 2; i ++)
  {
    objectInit (new Tank (PantherDescriptor), 0, 400);
    fighter [i]->trafo.translation.x = px + 6 - i * 4;
    fighter [i]->trafo.translation.z = py + 6 - i * 4;
    fighter [i]->getPrototype ()->maxthrust = 0;
  }
}

/*int MissionTutorial2::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
  for (i = 0; i <= 2; i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party == 0)
        b = true;
  }
  if (b) return 0;
  return 1;
}*/

void MissionTutorial2::draw ()
{
  char buf [250], buf2 [10];
  int timeroff = 100 * timestep, timerdelay = 300 * timestep, timerlag = 20 * timestep;
  if (timer >= 0 && timer <= timeroff - 20)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
  else if (timer > timeroff && timer <= timeroff + timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "FIRSTLY, HAVE A LOOK AT YOUR MISSILES:", textcolor);
    conf.key2string (key_selectmissile, buf2);
    sprintf (buf, "PRESS '%s' TO SWITCH BETWEEN", buf2);
    font1->drawTextCentered (0, 6, -2.5, buf, textcolor);
    font1->drawTextCentered (0, 5, -2.5, "AGM (AIR-GROUND-MISSILE) AND DF (DUMB FIRE)", textcolor);
  }
  else if (timer > timerdelay + timeroff && timer <= timeroff + 2 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "LET'S USE THE AGM: AS SOON AS THE TARGET", textcolor);
    font1->drawTextCentered (0, 6, -2.5, "IS LOCKED (YELLOW), SHOOT THE AGM AT ANY", textcolor);
    font1->drawTextCentered (0, 5, -2.5, "DIRECTION AND WATCH WHERE IT GOES.", textcolor);
  }
  else if (timer > 2 * timerdelay + timeroff && timer <= timeroff + 3 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "THEN TRY A DF MISSILE BUT CAREFUL:", textcolor);
    font1->drawTextCentered (0, 6, -2.5, "IT IS QUITE 'DUMB' AND WILL FLY STRAIGHT AHEAD.", textcolor);
  }
  else if (timer > 3 * timerdelay + timeroff && timer <= timeroff + 4 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 6, -2.5, "HOWEVER, DF MISSILES WILL CAUSE MUCH MORE DAMAGE", textcolor);
  }
  else if (timer > 4 * timerdelay + timeroff && timer <= timeroff + 5 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 6, -2.5, "NOW, BLOW UP THE TWO TANKS", textcolor);
  }
}

#endif
