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



MissionTutorial3::MissionTutorial3 ()
{
  id = MISSION_TUTORIAL3;
  strcpy (name, "TUTORIAL: AERODYNAMICS");
  alliedfighters = 1;
  strcpy (briefing, "THIS TUTORIAL WILL TEACH YOU THE BASICS OF AERODYNAMICS USING THE SIMULATION MODEL FOR PROVIDING MORE REALISTIC PHYSICS THAN THE SIMPLE ACTION MODEL.");
  autoLFBriefing ();
  maxtime = 2500 * timestep;
  heading = 210;
}

void MissionTutorial3::start ()
{
  day = 1;
  clouds = 2;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 50;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_SEA, NULL);
  playerInit ();
  fplayer->trafo.translation.x = 220;
  fplayer->trafo.translation.z = -30;
}

int MissionTutorial3::processtimer (Uint32 dt)
{
  timer += dt;
  fplayer->realism = true;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
    return 2;
  if (state == 1 && fighter.size () == 1)
    return 1;
  return 0;
}

void MissionTutorial3::draw ()
{
  int timeroff = 100 * timestep, timerdelay = 350 * timestep, timerlag = 120 * timestep;
  if (timer >= 0 && timer <= timeroff - 20)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
  else if (timer > timeroff && timer <= timeroff + timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "THE MENU OPTIONS/CONTROLS LETS YOU DEFINE DIFFICULTY", textcolor);
    font1->drawTextCentered (0, 6, -2.5, "AND REALISM. CHANGING THESE SETTINGS WILL ONLY HAVE", textcolor);
    font1->drawTextCentered (0, 5, -2.5, "EFFECT ON THE NEXT MISSION YOU START", textcolor);
  }
  else if (timer > timeroff + timerdelay && timer <= timeroff + 2 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "APART FROM YOUR CURRENT PHYSICS SETTINGS", textcolor);
    font1->drawTextCentered (0, 6, -2.5, "LET'S USE THE SIMULATION MODEL AND DISCUSS", textcolor);
    font1->drawTextCentered (0, 5, -2.5, "THE FOUR FORCES THRUST, DRAG, LIFT, AND WEIGHT", textcolor);
  }
  else if (timer > timeroff + 2 * timerdelay && timer <= timeroff + 3 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "THRUST IS FORWARD FORCE CREATED BY THE ENGINES.", textcolor);
    font1->drawTextCentered (0, 6, -2.5, "CHANGE THE THRUST TO GAIN OR LOSE SPEED", textcolor);
  }
  else if (timer > timeroff + 3 * timerdelay && timer <= timeroff + 4 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "DRAG IS THE RETARDING FORCE THAT LIMITS THE AIRCRAFT'S", textcolor);
    font1->drawTextCentered (0, 6, -2.5, "SPEED. IT IS MAINLY CAUSED BY THE AIRCRAFT'S STRUCTURE", textcolor);
    font1->drawTextCentered (0, 5, -2.5, "THAT PROTUDES INTO THE WIND", textcolor);
  }
  else if (timer > timeroff + 4 * timerdelay && timer <= timeroff + 5 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "LIFT IS THE UPWARD FORCE CREATED BY THE AIRFLOW THAT PASSES", textcolor);
    font1->drawTextCentered (0, 6, -2.5, "OVER THE WINGS. IN STRAIGHT UNACCELERATED FLIGHT, IT", textcolor);
    font1->drawTextCentered (0, 5, -2.5, "COMPENSATES THE WEIGHT AND YOUR FIGHTER WON'T CLIMB OR DIVE", textcolor);
  }
  else if (timer > timeroff + 5 * timerdelay && timer <= timeroff + 6 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "THE LIFT FORCE DEPENDS ON THE SPEED:", textcolor);
    font1->drawTextCentered (0, 6, -2.5, "LOW SPEED WILL CAUSE THE AIRPLANE TO DIVE,", textcolor);
    font1->drawTextCentered (0, 5, -2.5, "AT HIGH SPEED IT WILL EVEN CLIMB", textcolor);
  }
  else if (timer > timeroff + 6 * timerdelay && timer <= timeroff + 8 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "NOW TRY THIS EFFECT BY FLYING STRAIGHT AHEAD AT 0 DEGREE", textcolor);
    font1->drawTextCentered (0, 6, -2.5, "ELEVATION WHILE USING THE LOWEST/HIGHEST THRUST SETTINGS", textcolor);
  }
  else if (timer > timeroff + 8 * timerdelay && timer <= timeroff + 10 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "IF YOU FLY A ROLL, THE LIFT WILL NOT OPPOSE THE THRUST", textcolor);
    font1->drawTextCentered (0, 6, -2.5, "AND YOU WILL LOSE HEIGHT. TRY!", textcolor);
  }
  else if (timer > timeroff + 10 * timerdelay && timer <= timeroff + 12 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "IF YOU ROLL TO THE SIDE, THE LIFT WILL TEAR THE AIRCRAFT", textcolor);
    font1->drawTextCentered (0, 6, -2.5, "FROM STRAIGHT FLIGHT TO A DIAGONAL DIRECTION WHILE", textcolor);
    font1->drawTextCentered (0, 5, -2.5, "LOSING HEIGHT. TRY!", textcolor);
  }
  else if (timer > timeroff + 12 * timerdelay && timer <= timeroff + 13 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "THE OVERALL FORCE IS GAINED BY ADDING THE FOUR FORCES.", textcolor);
    font1->drawTextCentered (0, 6, -2.5, "IT IS MEASURED IN 'G' WITH 1G MEANING THE EARTH'S GRAVITY", textcolor);
  }
  else if (timer > timeroff + 13 * timerdelay && timer <= timeroff + 14 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "YOU ARE MOSTLY OPPOSED TO MORE THAN ONLY 1G. BE CAREFUL:", textcolor);
    font1->drawTextCentered (0, 6, -2.5, "9G OR MORE MAY LEAD TO A BLACKOUT,", textcolor);
    font1->drawTextCentered (0, 5, -2.5, "AT LESS THAN -3G THE BLOOD WILL SHOOT INTO YOUR HEAD", textcolor);
  }
  else if (timer > timeroff + 14 * timerdelay && timer <= timeroff + 15 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "NOW ROLL ON THE BACK AND GET RID OF", textcolor);
    font1->drawTextCentered (0, 6, -2.5, "THE TWO TRANSPORTERS AHEAD", textcolor);
  }
  if (timer >= timeroff + 14 * timerdelay - timerlag / 2 && state == 0)
  {
    state ++;
    Fighter *f;
    f = new Fighter (TransportDescriptor);
    objectInit (f, 0, 200);
    f->trafo.translation.x = fplayer->trafo.translation.x - 30;
    f->trafo.translation.z = fplayer->trafo.translation.z - 30;
    f = new Fighter (TransportDescriptor);
    objectInit (f, 0, 200);
    f->trafo.translation.x = fplayer->trafo.translation.x + 30;
    f->trafo.translation.z = fplayer->trafo.translation.z + 30;
  }
}

#endif
