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



MissionTutorial1::MissionTutorial1 ()
{
  id = MISSION_TUTORIAL;
  strcpy (name, "TUTORIAL: PILOTING");
  alliedfighters = 1;
  strcpy (briefing, "WELCOME TO THE FIRST TUTORIAL! BEFORE FLYING ANY MISSION YOU SHOULD PLAY THIS TUTORIAL. LEARN TO HANDLE YOUR FIGHTER AND THE BASIC CONTROLS LIKE TARGETING OR FIRING A MISSILE.");
  autoLFBriefing ();
  maxtime = 2500 * timestep;
  heading = 210;
}

void MissionTutorial1::start ()
{
  day = 1;
  clouds = 2;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 50;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_LOW_ALPINE, NULL);
  playerInit ();
  fplayer->trafo.translation.x = 220;
  fplayer->trafo.translation.z = -30;
}

int MissionTutorial1::processtimer (Uint32 dt)
{
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
    return 2;
  if (state == 1 && fighter.size () == 1)
    return 1;
  return 0;
}

void MissionTutorial1::draw ()
{
  char buf [250], buf2 [10];
  int timeroff = 100 * timestep, timerdelay = 300 * timestep, timerlag = 20 * timestep;
  if (timer >= 0 && timer <= timeroff - 20)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
  else if (timer > timeroff && timer <= timeroff + timerdelay - timerlag)
  {
    if (controls == CONTROLS_JOYSTICK)
    {
      font1->drawTextCentered (0, 7, -2.5, "LEAVE THE JOYSTICK CENTERED.", textcolor);
      font1->drawTextCentered (0, 6, -2.5, "THE FIGHTER SHOULD FLY STRAIGHT ON.", textcolor);
      font1->drawTextCentered (0, 5, -2.5, "IF NOT, RECALIBRATE YOUR JOYSTICK", textcolor);
    }
    else if (controls == CONTROLS_MOUSE)
    {
      font1->drawTextCentered (0, 7, -2.5, "THE MENU OPTIONS/CONTROLS LETS YOU", textcolor);
      font1->drawTextCentered (0, 6, -2.5, "DETERMINE THE INPUT DEVICE.", textcolor);
      font1->drawTextCentered (0, 5, -2.5, "CURRENTLY, THIS IS THE MOUSE", textcolor);
    }
    else if (controls == CONTROLS_KEYBOARD)
    {
      font1->drawTextCentered (0, 7, -2.5, "THE MENU OPTIONS/CONTROLS LETS YOU", textcolor);
      font1->drawTextCentered (0, 6, -2.5, "DETERMINE THE INPUT DEVICE.", textcolor);
      font1->drawTextCentered (0, 5, -2.5, "CURRENTLY, THIS IS THE KEYBOARD", textcolor);
    }
  }
  else if (timer > timeroff + timerdelay && timer <= timeroff + 2 * timerdelay - timerlag)
  {
    if (controls == CONTROLS_JOYSTICK)
    {
      font1->drawTextCentered (0, 7, -2.5, "MOVING THE JOYSTICK LEFT OR RIGHT", textcolor);
      font1->drawTextCentered (0, 6, -2.5, "WILL AFFECT THE AILERON.", textcolor);
      font1->drawTextCentered (0, 5, -2.5, "THUS YOU WILL ONLY ROLL", textcolor);
    }
    else if (controls == CONTROLS_MOUSE)
    {
      if (!mouse_reverse && !mouse_relative)
      {
        font1->drawTextCentered (0, 7, -2.5, "THE MOUSE INTERFACE IS VERY EASY.", textcolor);
        font1->drawTextCentered (0, 6, -2.5, "ALWAYS POINT TO WHERE YOU WANT TO FLY.", textcolor);
        font1->drawTextCentered (0, 5, -2.5, "YOU'LL HAVE TO MOVE THE MOUSE PERMANENTLY", textcolor);
      }
      else if (mouse_reverse && !mouse_relative)
      {
        font1->drawTextCentered (0, 7, -2.5, "THE REVERTED MOUSE IS FOR REAL GAMERS ONLY.", textcolor);
        font1->drawTextCentered (0, 6, -2.5, "THE STANDARD MOUSE INTERFACE MAY BE", textcolor);
        font1->drawTextCentered (0, 5, -2.5, "EASIER TO LEARN", textcolor);
      }
      else
      {
        font1->drawTextCentered (0, 7, -2.5, "MOUSE RELATIVE IS A LOT OF WORK.", textcolor);
        font1->drawTextCentered (0, 6, -2.5, "ONLY THE RELATIVE MOUSE COORDINATES (MOVING)", textcolor);
        font1->drawTextCentered (0, 5, -2.5, "WILL HAVE AN EFFECT", textcolor);
      }
    }
    else if (controls == CONTROLS_KEYBOARD)
    {
      font1->drawTextCentered (0, 7, -2.5, "THE KEYBOARD INTERFACE IS NOT THAT EASY.", textcolor);
      font1->drawTextCentered (0, 6, -2.5, "USING THE MOUSE IS STRONLY RECOMMENDED", textcolor);
    }
  }
  else if (timer > timeroff + 2 * timerdelay && timer <= timeroff + 3 * timerdelay - timerlag)
  {
    if (controls == CONTROLS_JOYSTICK)
    {
      font1->drawTextCentered (0, 7, -2.5, "MOVING UP OR DOWN WILL AFFECT", textcolor);
      font1->drawTextCentered (0, 6, -2.5, "THE ELEVATOR.", textcolor);
      font1->drawTextCentered (0, 5, -2.5, "YOU'LL START TO FLY A LOOP", textcolor);
    }
    else if (controls == CONTROLS_MOUSE || controls == CONTROLS_KEYBOARD)
    {
      font1->drawTextCentered (0, 7, -2.5, "MOVING RIGHT OR LEFT WILL AFFECT THE AILERON,", textcolor);
      font1->drawTextCentered (0, 6, -2.5, "MOVING UP OR DOWN WILL AFFECT THE ELEVATOR.", textcolor);
      font1->drawTextCentered (0, 5, -2.5, "THUS YOU CAN FLY ROLLS AND LOOPS", textcolor);
    }
  }
  else if (timer > timeroff + 3 * timerdelay && timer <= timeroff + 4 * timerdelay - timerlag)
  {
    if (controls == CONTROLS_JOYSTICK)
    {
      font1->drawTextCentered (0, 7, -2.5, "IF YOUR JOYSTICK HAS A SO-CALLED \"RUDDER\",", textcolor);
      font1->drawTextCentered (0, 6, -2.5, "YOU MAY ALTER THE FIGHTER'S RUDDER.", textcolor);
      font1->drawTextCentered (0, 5, -2.5, "YOU'LL SLIGHTLY FLY TO THE LEFT OR RIGHT", textcolor);
    }
    else if (controls == CONTROLS_MOUSE)
    {
      font1->drawTextCentered (0, 7, -2.5, "MOVING THE MOUSE SLIGHTLY LEFT OR RIGHT", textcolor);
      font1->drawTextCentered (0, 6, -2.5, "WILL AFFECT THE RUDDER.", textcolor);
    }
    else if (controls == CONTROLS_KEYBOARD)
    {
      font1->drawTextCentered (0, 7, -2.5, "TO ALTER THE RUDDER", textcolor);
      font1->drawTextCentered (0, 6, -2.5, "USE THE KEYS PGUP AND PGDN", textcolor);
    }
  }
  else if (timer > timeroff + 4 * timerdelay && timer <= timeroff + 5 * timerdelay - timerlag)
  {
    if (controls == CONTROLS_JOYSTICK)
    {
      font1->drawTextCentered (0, 7, -2.5, "YOUR JOYSTICK HAS ALSO A THROTTLE", textcolor);
      font1->drawTextCentered (0, 6, -2.5, "TO SPEEDUP OR DECELERATE.", textcolor);
      font1->drawTextCentered (0, 5, -2.5, "THERE IS NO WAY TO ENTIRELY STOP", textcolor);
    }
    else
    {
      font1->drawTextCentered (0, 7, -2.5, "USE THE KEYS '1' THROUGH '9' TO ALTER", textcolor);
      font1->drawTextCentered (0, 6, -2.5, "YOUR THROTTLE (SPEED).", textcolor);
      font1->drawTextCentered (0, 5, -2.5, "THERE IS NO WAY TO ENTIRELY STOP", textcolor);
    }
  }
  else if (timer > timeroff + 5 * timerdelay && timer <= timeroff + 6 * timerdelay - timerlag)
  {
    if (controls == CONTROLS_JOYSTICK)
    {
      font1->drawTextCentered (0, 7, -2.5, "THE PREDEFINED JOYSTICK BUTTONS:", textcolor);
      font1->drawTextCentered (0, 6, -2.5, "FIRE CANNON, DROP CHAFF/FLARE,", textcolor);
      font1->drawTextCentered (0, 5, -2.5, "FIRE MISSILE, CHOOSE MISSILE", textcolor);
    }
    else if (controls == CONTROLS_MOUSE)
    {
      font1->drawTextCentered (0, 7, -2.5, "LEFT MOUSE BUTTON: FIRE CANNON", textcolor);
      font1->drawTextCentered (0, 6, -2.5, "RIGHT MOUSE BUTTON: FIRE MISSILE", textcolor);
      font1->drawTextCentered (0, 5, -2.5, "BUT YOU SHOULD REALLY PREFER THE KEYBOARD", textcolor);
    }
    else if (controls == CONTROLS_KEYBOARD)
    {
      font1->drawTextCentered (0, 6, -2.5, "NOW, LETS HAVE A LOOK AT SOME IMPORTANT KEYS", textcolor);
    }
  }
  else if (timer > timeroff + 6 * timerdelay && timer <= timeroff + 7 * timerdelay - timerlag)
  {
    if (controls == CONTROLS_JOYSTICK)
    {
      font1->drawTextCentered (0, 7, -2.5, "A SUMMARY OF COMMANDS CAN BE FOUND IN THE MENU.", textcolor);
      font1->drawTextCentered (0, 6, -2.5, "\"ESC\" WILL SHOW THE MENU", textcolor);
      font1->drawTextCentered (0, 5, -2.5, "AND RETURN BACK TO THE ACTION", textcolor);
    }
    else
    {
      conf.key2string (key_firecannon, buf2);
      sprintf (buf, "%s: FIRE CANNON", buf2);
      font1->drawTextCentered (0, 7, -2.5, buf, textcolor);
      conf.key2string (key_selectmissile, buf2);
      sprintf (buf, "%s: CHOOSE MISSILE", buf2);
      font1->drawTextCentered (0, 6, -2.5, buf, textcolor);
      conf.key2string (key_firemissile, buf2);
      sprintf (buf, "%s: FIRE MISSILE", buf2);
      font1->drawTextCentered (0, 5, -2.5, buf, textcolor);
    }
  }
  else if (timer > timeroff + 7 * timerdelay && timer <= timeroff + 8 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "LOOK AT THE RADAR ON THE BOTTOM OF YOUR SCREEN.", textcolor);
    font1->drawTextCentered (0, 6, -2.5, "THERE ARE ENEMIES REPRESENTED BY A YELLOW POINT (TARGETED)", textcolor);
    font1->drawTextCentered (0, 5, -2.5, "AND A RED POINT (NOT TARGETED)", textcolor);
  }
  else if (timer > timeroff + 8 * timerdelay && timer <= timeroff + 9 * timerdelay - timerlag)
  {
    if (controls == CONTROLS_JOYSTICK)
    {
      font1->drawTextCentered (0, 7, -2.5, "YOU MAY TARGET AN ENEMY USING THE COOLIE HAT,", textcolor);
      conf.key2string (key_targetnearest, buf2);
      sprintf (buf, "OR PRESSING '%s' ON THE KEYBOARD.", buf2);
      font1->drawTextCentered (0, 6, -2.5, buf, textcolor);
      font1->drawTextCentered (0, 5, -2.5, "THEN APPROACH!", textcolor);
    }
    else
    {
      font1->drawTextCentered (0, 7, -2.5, "YOU MAY TARGET AN ENEMY PRESSING", textcolor);
      conf.key2string (key_targetnearest, buf2);
      sprintf (buf, "OR PRESSING '%s' ON THE KEYBOARD.", buf2);
      font1->drawTextCentered (0, 6, -2.5, buf, textcolor);
      font1->drawTextCentered (0, 5, -2.5, "THEN APPROACH!", textcolor);
    }
  }
  else if (timer > timeroff + 9 * timerdelay && timer <= timeroff + 10 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "SHOOT THE TARGETS TO COMPLETE", textcolor);
    font1->drawTextCentered (0, 6, -2.5, "THIS TUTORIAL SUCCESSFULLY", textcolor);
  }
  if (timer >= timeroff + 7 * timerdelay - timerlag / 2 && state == 0)
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
