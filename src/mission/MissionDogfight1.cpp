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



MissionDogfight1::MissionDogfight1 ()
{
  id = MISSION_DOGFIGHT;
  strcpy (name, "TUTORIAL: FIGHTER");
  strcpy (briefing, "HERE IS THE THIRD TUTORIAL: PRACTICE YOUR DOGFIGHT SKILLS.");
  autoLFBriefing ();
  alliedfighters = 1;
  maxtime = 5000 * timestep;
}

Fighter *MissionDogfight1::addFighter ()
{
  Fighter *f = new Fighter (CrowDescriptor);
  objectInit (f, 0, 395);
  return f;
}

void MissionDogfight1::start ()
{
  day = 1;
  clouds = 2;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 25;
  heading = 220;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_ALPINE, NULL);
  playerInit ();
  fplayer->trafo.translation.x = 20;
  fplayer->trafo.translation.z = 70;
  Fighter *f = addFighter ();
  f->trafo.translation.x = -10;
  f->trafo.translation.z = -10;
  state = 0;
  laststate = 0;
  texttimer = 0;
}

int MissionDogfight1::processtimer (Uint32 dt)
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
    for (i = 2; i <= 3; i ++)
    {
      Fighter *f = addFighter ();
      f->trafo.translation.x = fplayer->trafo.translation.x + 50 + 10 * i;
      f->trafo.translation.z = fplayer->trafo.translation.z + 50 + 10 * i;
      f->trafo.translation.y = l->getHeight (f->trafo.translation.x, f->trafo.translation.z) + 15;
    }
    return 0;
  }
  else if (state == 2)
  {
    for (i = 4; i <= 6; i ++)
    {
      Fighter *f = addFighter ();
      f->trafo.translation.x = fplayer->trafo.translation.x + 50 + 10 * i;
      f->trafo.translation.z = fplayer->trafo.translation.z + 50 + 10 * i;
      f->trafo.translation.y = l->getHeight (f->trafo.translation.x, f->trafo.translation.z) + 15;
    }
    return 0;
  }
  return 1;
}

void MissionDogfight1::draw ()
{
  char buf [250], buf2 [10], buf3 [10];
  int timeroff = 100 * timestep, timerdelay = 300 * timestep, timerlag = 20 * timestep;

  if (laststate != state)
  {
    texttimer = 1;
    laststate = state;
  }
  if (texttimer > 0)
  {
    if (state == 1)
    {
      font1->drawTextCentered (0, 7, -2.5, "THAT WAS EASY", textcolor);
      font1->drawTextCentered (0, 6, -2.5, "BUT CAN YOU HANDLE TWO ENEMIES?", textcolor);
      return;
    }
    if (state == 2)
    {
      font1->drawTextCentered (0, 6, -2.5, "OK, LET'S TRY THREE ENEMIES", textcolor);
      return;
    }
  }

  if (timer >= 0 && timer <= timeroff - timerlag)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
  else if (timer >= timeroff && timer <= timeroff + timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "THERE IS AN ENEMY ATTACKING", textcolor);
    conf.key2string (key_selectmissile, buf2);
    sprintf (buf, "PRESS '%s' ON THE KEYBOARD TO", buf2);
    font1->drawTextCentered (0, 6, -2.5, buf, textcolor);
    font1->drawTextCentered (0, 5, -2.5, "SELECT A MISSILE", textcolor);
  }
  else if (timer >= timeroff + timerdelay && timer <= timeroff + 2 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "CHOOSE A FRIEND-FOE (FF) MISSILE", textcolor);
    font1->drawTextCentered (0, 6, -2.5, "WHICH DETECTS ENEMIES BY RADAR.", textcolor);
    font1->drawTextCentered (0, 5, -2.5, "SHOOT AS EARLY AS POSSIBLE", textcolor);
  }
  else if (timer >= timeroff + 2 * timerdelay && timer <= timeroff + 3 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "YOU CAN ONLY FIRE MISSILES WHEN", textcolor);
    font1->drawTextCentered (0, 6, -2.5, "THE RECTANGULAR LOCK AROUND THE ENEMY", textcolor);
    font1->drawTextCentered (0, 5, -2.5, "APPEARS YELLOW", textcolor);
  }
  else if (timer >= timeroff + 3 * timerdelay && timer <= timeroff + 4 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "CHECK THE CHAFF/FLARE DISPLAYS.", textcolor);
    font1->drawTextCentered (0, 6, -2.5, "IF THEY START BLINKING, AN ENEMY HAS", textcolor);
    font1->drawTextCentered (0, 5, -2.5, "FIRED A MISSILE TO GET YOU DOWN", textcolor);
  }
  else if (timer >= timeroff + 4 * timerdelay && timer <= timeroff + 5 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "CHAFF IS A DECOY FOR RADAR SEEKING MISSILES,", textcolor);
    font1->drawTextCentered (0, 6, -2.5, "FLARES PROTECT FROM INFRARED MISSILES", textcolor);
    conf.key2string (key_dropchaff, buf2);
    conf.key2string (key_dropflare, buf3);
    sprintf (buf, "KEYS: DROP CHAFF = '%s', DROP FLARE = '%s'", buf2, buf3);
    font1->drawTextCentered (0, 5, -2.5, buf, textcolor);
  }
  else if (timer >= timeroff + 5 * timerdelay && timer <= timeroff + 6 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "THE COUNTER MEASURE ONLY HAD AN EFFECT,", textcolor);
    font1->drawTextCentered (0, 6, -2.5, "IF THE DISPLAYS STOP BLINKING", textcolor);
  }
  else if (timer >= timeroff + 6 * timerdelay && timer <= timeroff + 7 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "A LITTLE TARGETING HINT:", textcolor);
    conf.key2string (key_targetnearest, buf2);
    sprintf (buf, "'%s' WILL TARGET THE NEAREST ENEMY IN FRONT,", buf2);
    font1->drawTextCentered (0, 6, -2.5, buf, textcolor);
    conf.key2string (key_targetlocking, buf2);
    sprintf (buf, "'%s' WILL TARGET AN ENEMY WHO HAS LOCKED YOU", buf2);
    font1->drawTextCentered (0, 5, -2.5, buf, textcolor);
  }
}

#endif
