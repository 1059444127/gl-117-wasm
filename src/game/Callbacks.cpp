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

#ifndef IS_CALLBACKS_H

#include "Callbacks.h"
#include "globals.h"
#include "Game.h"
#include "mission/Mission.h"
#include "pilot/Pilot.h"

/*#include "menu/Component.h"
#include "mission/MapLoader.h"
#include "configuration/Directory.h"
#include "opengl/GlPrimitives.h"
#include "landscape/Landscape.h"
#include "net/net.h"
#include "util/Math.h"
#include "cockpit/Cockpit.h"
#include "configuration/Configuration.h"
#include "mission/Mission.h"
#include "gllandscape/GlLandscape.h"
#include "logging/Logging.h"
#include "game/Game.h"
#include "game/Events.h"
#include "game/Callbacks.h"*/

void callbackReturn (Component *comp, int key)
{
  switch_game ();
}

void callbackStats (Component *comp, int key)
{
  statestats.key (27, 0, 0);
}

void callbackQuitNow (Component *comp, int key)
{
  game_quit ();
}

void callbackSwitchMainMenu (Component *comp, int key)
{
  switch_menu ();
}

void callbackQuit (Component *comp, int key)
{
  switch_quit ();
}

void callbackCredits (Component *comp, int key)
{
  switch_credits ();
}

void callbackPilots (Component *comp, int key)
{
  switch_fame ();
}

int currentaxis = 0;

void callbackJoystickAxis (Component *comp, int key)
{
  int i;
  int buttonnum = -1;

  // get clicked button's number
  for (i = 0; i < 12; i ++)
  {
    if (comp == controlsmenu [2]->components [i])
    {
      buttonnum = i;
    }
  }
  if (buttonnum == -1) return;

  // read text from label of button
  Label *la = (Label *) controlsmenu [2]->components [buttonnum + 1];

  int joynum = 0;
  int joyaxis = 0;
  if (la->text [1] == '/') joynum = -1;
  else
  {
    joynum = la->text [0] - 'A';
    joyaxis = la->text [2] - '0';
  }

  if (key == MOUSE_BUTTON_LEFT)
  {
    if (joynum == -1)
    {
      joynum = 0;
      joyaxis = 0;
    }
    else if (joynum >= joysticks) joynum = -1;
    else
    {
      joyaxis ++;
      if (joyaxis >= sdljoystickaxes [joynum])
      {
        joyaxis = 0;
        joynum ++;
        if (joynum >= joysticks) joynum = -1;
      }
    }
  }
  else
  {
    if (joynum == -1)
    {
      joynum = joysticks - 1;
      joyaxis = sdljoystickaxes [joynum] - 1;
    }
    else if (joynum >= joysticks) joynum = -1;
    else
    {
      joyaxis --;
      if (joyaxis < 0)
      {
        joynum --;
        if (joynum < 0)
        {
          joynum = -1;
          joyaxis = 0;
        }
        else
          joyaxis = sdljoystickaxes [joynum] - 1;
      }
    }
  }

  int *joysetting = NULL;
  if (buttonnum == 0) joysetting = &joystick_aileron;
  else if (buttonnum == 2) joysetting = &joystick_elevator;
  else if (buttonnum == 4) joysetting = &joystick_throttle;
  else if (buttonnum == 6) joysetting = &joystick_rudder;
  else if (buttonnum == 8) joysetting = &joystick_view_x;
  else if (buttonnum == 10) joysetting = &joystick_view_y;

  *joysetting = joynum * 1000 + joyaxis;
  char latext [4];
  if (joynum >= 0)
  {
    latext [0] = 'A' + joynum;
    latext [1] = 32;
    latext [2] = '0' + joyaxis;
    latext [3] = 0;
    la->setText (latext);
  }
  else
  {
    la->setText ("N/A");
  }
}

void callbackSwitchStartMission (Component *comp, int key)
{
  pleaseWait ();
  if (!game_levelInit ())
  {
    switch_menu ();
    return;
  }
  switch_game ();
  missionactive = true;
}

void callbackFighterPrev (Component *comp, int key)
{
  int maxfighter = 6;
  Pilot *p = pilots->pilot [pilots->aktpilot];
  if (p->mission_state [MISSION_DEPOT] == 1) maxfighter ++;
  if (p->mission_state [MISSION_SHIP1] == 1) maxfighter ++;
  if (p->mission_state [MISSION_CANYON3] == 1) maxfighter ++;
  if (p->mission_state [MISSION_MOON1] == 1) maxfighter ++;
  if (key == MOUSE_BUTTON_LEFT)
  {
    aktfighter --;
    if (aktfighter < 0) aktfighter = maxfighter - 1;
  }
}

void callbackFighterNext (Component *comp, int key)
{
  int maxfighter = 6;
  Pilot *p = pilots->pilot [pilots->aktpilot];
  if (p->mission_state [MISSION_DEPOT] == 1) maxfighter ++;
  if (p->mission_state [MISSION_SHIP1] == 1) maxfighter ++;
  if (p->mission_state [MISSION_CANYON3] == 1) maxfighter ++;
  if (p->mission_state [MISSION_MOON1] == 1) maxfighter ++;
  if (key == MOUSE_BUTTON_LEFT)
  {
    aktfighter ++;
    if (aktfighter >= maxfighter) aktfighter = 0;
  }
}

void callbackFighter (Component *comp, int key)
{
  switch_fighter ();
}

void setpilotstext ()
{
  char buf [64];
  int i;
  sprintf (buf, "     %s %s", pilots->pilot [pilots->aktpilot]->getShortRank (MISSION_CAMPAIGN1, MISSION_CAMPAIGN2 - 1).c_str (), pilots->pilot [pilots->aktpilot]->name.c_str ());
  ((Label *) submenu [0]->components [1])->setText (buf);
  for (i = 0; i < pilots->aktpilots; i ++)
  {
    sprintf (buf, "     %s %s", pilots->pilot [i]->getShortRank (MISSION_CAMPAIGN1, MISSION_CAMPAIGN2 - 1).c_str (), pilots->pilot [i]->name.c_str ());
    ((Label *) submenu [0]->components [i + 2])->setText (buf);
  }
  for (; i < 5; i ++)
  {
    ((Label *) submenu [0]->components [i + 2])->setText ("N/A");
  }
}

void callbackPilotsAdd (Component *comp, int key)
{
  char buf [64];
  if (key == MOUSE_BUTTON_LEFT)
  {
    strcpy (buf, ((EditField *) submenu [0]->components [9])->text.c_str ());
    if (strlen (buf) > 0 && pilots->aktpilots < 5)
    {
      pilots->add (buf);
      ((EditField *) submenu [0]->components [9])->setText ("");
      setpilotstext ();
    }
  }
}

void callbackPilotsDelete (Component *comp, int key)
{
  if (key == MOUSE_BUTTON_RIGHT)
  {
    pilots->rm ();
    setpilotstext ();
  }
}

void callbackPilotsList (Component *comp, int key)
{
  int i;
  for (i = 0; i < 5; i ++)
  {
    if (comp == submenu [0]->components [i + 2])
    {
      if (i < pilots->aktpilots)
      {
        pilots->aktpilot = i;
        setpilotstext ();
      }
    }
  }
}

void callbackJoystick (Component *comp, int key)
{
  int i;
  for (i = 12; i < 23; i ++)
    if (comp == controlsmenu [0]->components [i])
    {
      int z = 12;
      if (i == z ++) joystick_firecannon = key;
      else if (i == z ++) joystick_firemissile = key;
      else if (i == z ++) joystick_dropflare = key;
      else if (i == z ++) joystick_dropchaff = key;
      else if (i == z ++) joystick_selectmissile = key;
      else if (i == z ++) joystick_targetnearest = key;
      else if (i == z ++) joystick_targetlocking = key;
      else if (i == z ++) joystick_targetnext = key;
      else if (i == z ++) joystick_targetprevious = key;
      else if (i == z ++) joystick_thrustup = key;
      else if (i == z ++) joystick_thrustdown = key;
    }
}

void callbackMouseDefaults (Component *comp, int key)
{
  char buf [256];
  mouse_sensitivity = 100;
  mouse_reverse = false;
  mouse_relative = false;
  mouse_autorudder = 30;

  sprintf (buf, "%d%%", mouse_autorudder);
  ((Label *) controlsmenu [1]->components [7])->setText (buf);
  if (mouse_relative) sprintf (buf, "ON");
  else sprintf (buf, "OFF");
  ((Label *) controlsmenu [1]->components [5])->setText (buf);
  if (mouse_reverse) sprintf (buf, "ON");
  else sprintf (buf, "OFF");
  ((Label *) controlsmenu [1]->components [3])->setText (buf);
  sprintf (buf, "%d%%", mouse_sensitivity);
  ((Label *) controlsmenu [1]->components [1])->setText (buf);
}

void callbackMouseAutorudder (Component *comp, int key)
{
  char buf [256];
  if (key == MOUSE_BUTTON_LEFT)
  {
    mouse_autorudder += 10;
    if (mouse_autorudder > 100) mouse_autorudder = 0;
  }
  else
  {
    mouse_autorudder -= 10;
    if (mouse_autorudder < 0) mouse_autorudder = 100;
  }
  sprintf (buf, "%d%%", mouse_autorudder);
  ((Label *) controlsmenu [1]->components [7])->setText (buf);
}

void callbackMouseRelative (Component *comp, int key)
{
  char buf [256];
  mouse_relative = !mouse_relative;
  if (mouse_relative) sprintf (buf, "ON");
  else sprintf (buf, "OFF");
  ((Label *) controlsmenu [1]->components [5])->setText (buf);
}

void callbackMouseReverse (Component *comp, int key)
{
  char buf [256];
  mouse_reverse = !mouse_reverse;
  if (mouse_reverse) sprintf (buf, "ON");
  else sprintf (buf, "OFF");
  ((Label *) controlsmenu [1]->components [3])->setText (buf);
}

void callbackMouseSensitivity (Component *comp, int key)
{
  char buf [256];
  if (key == MOUSE_BUTTON_LEFT)
  {
    mouse_sensitivity += 10;
    if (mouse_sensitivity > 200) mouse_sensitivity = 70;
  }
  else
  {
    mouse_sensitivity -= 10;
    if (mouse_sensitivity < 70) mouse_sensitivity = 200;
  }
  sprintf (buf, "%d%%", mouse_sensitivity);
  ((Label *) controlsmenu [1]->components [1])->setText (buf);
}

void callbackDefaultsJoystick (Component *comp, int key)
{
  joystick_aileron = 0;
  joystick_elevator = 1;
  joystick_throttle = 2;
  joystick_rudder = 3;
  joystick_view_x = 4;
  joystick_view_y = 5;

  int z = 1;
  ((Label *) controlsmenu [2]->components [z])->setText ("A 0");
  z += 2;
  ((Label *) controlsmenu [2]->components [z])->setText ("A 1");
  z += 2;
  ((Label *) controlsmenu [2]->components [z])->setText ("A 2");
  z += 2;
  ((Label *) controlsmenu [2]->components [z])->setText ("A 3");
  z += 2;
  ((Label *) controlsmenu [2]->components [z])->setText ("A 4");
  z += 2;
  ((Label *) controlsmenu [2]->components [z])->setText ("A 5");

  joystick_firecannon = 0;
  joystick_firemissile = 2;
  joystick_dropflare = 3;
  joystick_dropchaff = 3;
  joystick_selectmissile = 1;
  joystick_targetnearest = 101;
  joystick_targetlocking = 103;
  joystick_targetnext = 100;
  joystick_targetprevious = 102;
  joystick_thrustup = 4;
  joystick_thrustdown = 5;

  z = 12;
  ((EditJoystick *) controlsmenu [2]->components [z ++])->setButton (joystick_firecannon);
  ((EditJoystick *) controlsmenu [2]->components [z ++])->setButton (joystick_firemissile);
  ((EditJoystick *) controlsmenu [2]->components [z ++])->setButton (joystick_dropflare);
  ((EditJoystick *) controlsmenu [2]->components [z ++])->setButton (joystick_dropchaff);
  ((EditJoystick *) controlsmenu [2]->components [z ++])->setButton (joystick_selectmissile);
  ((EditJoystick *) controlsmenu [2]->components [z ++])->setButton (joystick_targetnearest);
  ((EditJoystick *) controlsmenu [2]->components [z ++])->setButton (joystick_targetlocking);
  ((EditJoystick *) controlsmenu [2]->components [z ++])->setButton (joystick_targetnext);
  ((EditJoystick *) controlsmenu [2]->components [z ++])->setButton (joystick_targetprevious);
  ((EditJoystick *) controlsmenu [2]->components [z ++])->setButton (joystick_thrustup);
  ((EditJoystick *) controlsmenu [2]->components [z ++])->setButton (joystick_thrustdown);
}

void callbackDefaults (Component *comp, int key)
{
  key_firecannon = 32;
  key_firemissile = 13;
  key_dropflare = 'F';
  key_dropchaff = 'C';
  key_selectmissile = 'M';
  key_targetnearest = 'E';
  key_targetlocking = 'L';
  key_targetnext = 'N';
  key_targetprevious = 'P';
  key_thrustup = 'S';
  key_thrustdown = 'X';

  int z = 5;
  ((EditKey *) controlsmenu [0]->components [z ++])->setKey (key_firecannon);
  ((EditKey *) controlsmenu [0]->components [z ++])->setKey (key_firemissile);
  ((EditKey *) controlsmenu [0]->components [z ++])->setKey (key_dropflare);
  ((EditKey *) controlsmenu [0]->components [z ++])->setKey (key_dropchaff);
  ((EditKey *) controlsmenu [0]->components [z ++])->setKey (key_selectmissile);
  ((EditKey *) controlsmenu [0]->components [z ++])->setKey (key_targetnearest);
  ((EditKey *) controlsmenu [0]->components [z ++])->setKey (key_targetlocking);
  ((EditKey *) controlsmenu [0]->components [z ++])->setKey (key_targetnext);
  ((EditKey *) controlsmenu [0]->components [z ++])->setKey (key_targetprevious);
  ((EditKey *) controlsmenu [0]->components [z ++])->setKey (key_thrustup);
  ((EditKey *) controlsmenu [0]->components [z ++])->setKey (key_thrustdown);
}

void callbackKeyboard (Component *comp, int key)
{
  int i;
  for (i = 5; i < 16; i ++)
    if (comp == controlsmenu [0]->components [i])
    {
      int z = 5;
      if (i == z ++) key_firecannon = key;
      else if (i == z ++) key_firemissile = key;
      else if (i == z ++) key_dropflare = key;
      else if (i == z ++) key_dropchaff = key;
      else if (i == z ++) key_selectmissile = key;
      else if (i == z ++) key_targetnearest = key;
      else if (i == z ++) key_targetlocking = key;
      else if (i == z ++) key_targetnext = key;
      else if (i == z ++) key_targetprevious = key;
      else if (i == z ++) key_thrustup = key;
      else if (i == z ++) key_thrustdown = key;
    }
}

void textControls (char *buf)
{
  if (controls == CONTROLS_KEYBOARD) sprintf (buf, "%s", "KEYBOARD");
  else if (controls == CONTROLS_MOUSE) sprintf (buf, "%s", "MOUSE");
  else if (controls == CONTROLS_JOYSTICK) sprintf (buf, "%s", "JOYSTICK");
}

void callbackControls (Component *comp, int key)
{
  char buf [256];
  if (key == MOUSE_BUTTON_LEFT)
  {
    fplayer->rolleffect = 0;
    fplayer->ruddereffect = 0;
    fplayer->elevatoreffect = 0;
    keyb_elev = 0;
    keyb_roll = 0;
    keyb_rudder = 0;
    controls ++;
    if (controls > 2) controls = 0;
    if (controls == CONTROLS_JOYSTICK && !joysticks) controls = CONTROLS_KEYBOARD;
  }
  textControls (buf);
  ((Label *) optmenu [2]->components [5])->setText (buf);
  allmenus.components [11]->setVisible (false);
  allmenus.components [12]->setVisible (false);
  allmenus.components [13]->setVisible (false);
  if (controls == CONTROLS_KEYBOARD) allmenus.components [11]->setVisible (true);
  else if (controls == CONTROLS_JOYSTICK) allmenus.components [13]->setVisible (true);
  else allmenus.components [12]->setVisible (true);
}

void callbackPhysics (Component *comp, int key)
{
  char buf [256];
  physics = !physics;
  if (!physics) sprintf (buf, "%s", "ACTION");
  else sprintf (buf, "%s", "SIMULATION");
  ((Label *) optmenu [2]->components [3])->setText (buf);
}

void callbackDifficulty (Component *comp, int key)
{
  char buf [256];
  if (key == MOUSE_BUTTON_LEFT)
  {
    difficulty ++;
    if (difficulty > 2) difficulty = 0;
  }
  else
  {
    difficulty --;
    if (difficulty < 0) difficulty = 2;
  }
  if (difficulty == 0) sprintf (buf, "%s", "EASY");
  else if (difficulty == 1) sprintf (buf, "%s", "NORMAL");
  else if (difficulty == 2) sprintf (buf, "%s", "HARD");
  ((Label *) optmenu [2]->components [1])->setText (buf);
}

void callbackSound (Component *comp, int key)
{
  char buf [256];
  if (sound->audio)
  {
    if (key == MOUSE_BUTTON_LEFT)
    {
      sound->volumesound += 10;
      if (sound->volumesound > 100)
        sound->volumesound = 0;
      volumesound = sound->volumesound;
      sound->setVolume ();
      setPlaneVolume ();
      sound->play (SOUND_CLICK1, false);
      statemenu.reshape ();
    }
    else
    {
      sound->volumesound -= 10;
      if (sound->volumesound < 0)
        sound->volumesound = 100;
      volumesound = sound->volumesound;
      sound->setVolume ();
      setPlaneVolume ();
      sound->play (SOUND_CLICK1, false);
      statemenu.reshape ();
    }
    sprintf (buf, "%d%%", volumesound);
    ((Label *) optmenu [1]->components [1])->setText (buf);
  }
}

void callbackMusic (Component *comp, int key)
{
  char buf [256];
  if (sound->audio)
  {
    if (key == MOUSE_BUTTON_LEFT)
    {
      sound->volumemusic += 10;
      if (sound->volumemusic > 100)
      {
        sound->volumemusic = 0;
        sound->haltMusic ();
      }
      volumemusic = sound->volumemusic;
      sound->setVolumeMusic ();
      statemenu.reshape ();
    }
    else
    {
      sound->volumemusic -= 10;
      if (sound->volumemusic < 0)
        sound->volumemusic = 100;
      volumemusic = sound->volumemusic;
      sound->setVolumeMusic ();
      statemenu.reshape ();
    }
    if (sound->volumemusic != 0 && !sound->musicplaying)
      playRandomMusic ();
    sprintf (buf, "%d%%", volumemusic);
    ((Label *) optmenu [1]->components [3])->setText (buf);
  }
}

void callbackBrightness (Component *comp, int key)
{
  char buf [256];
  if (key == MOUSE_BUTTON_LEFT)
  {
    brightness += 10;
    if (brightness > 50) brightness = -50;
  }
  else
  {
    brightness -= 10;
    if (brightness < -50) brightness = 50;
  }
  sprintf (buf, "%d%%", brightness);
  ((Label *) optmenu [0]->components [13])->setText (buf);
}

void callbackResolution (Component *comp, int key)
{
  int display = SDL_GetWindowDisplayIndex(sdlwindow);
  int numres = SDL_GetNumDisplayModes(display);
  SDL_DisplayMode modes[numres];
  for (int i = 0; i < numres; i++)
  {
    if (SDL_GetDisplayMode(display, i, &modes[i]) != 0)
    {
      memset(&modes[i], 0, sizeof(modes[i]));
    }
  }
  int found = 0;
  char buf [256];

  if (key == MOUSE_BUTTON_LEFT)
  {
    for (int i = 0; i < numres; i ++)
      if (wantwidth == modes [i].w && wantheight == modes [i].h)
      {
        found = i + 1;
      }
  }
  else
  {
    for (int i = 0; i < numres; i ++)
      if (wantwidth == modes [i].w && wantheight == modes [i].h)
      {
        found = i - 1;
      }
  }

  if (found < 0) found = numres - 1;
  else if (found >= numres) found = 0;

  wantwidth = modes [found].w;
  wantheight = modes [found].h;

  sprintf (buf, "%d*%d", wantwidth, wantheight);
  ((Label *) optmenu [0]->components [16])->setText (buf);
}

void callbackFullscreen (Component *comp, int key)
{
  if (wantfullscreen) wantfullscreen = 0;
  else wantfullscreen = 1;

  if (wantfullscreen) ((Label *) optmenu [0]->components [18])->setText ("YES");
  else ((Label *) optmenu [0]->components [18])->setText ("NO");
}

void callbackDynamicLighting (Component *comp, int key)
{
  char buf [256];
  dynamiclighting = (dynamiclighting == 0 ? 1 : 0);
  if (dynamiclighting) strcpy (buf, "ON");
  else strcpy (buf, "OFF");
  ((Label *) optmenu [0]->components [11])->setText (buf);
}

void callbackSpecialEffects (Component *comp, int key)
{
  char buf [256];
  specialeffects = (specialeffects == 0 ? 1 : 0);
  if (specialeffects) strcpy (buf, "ON");
  else strcpy (buf, "OFF");
  ((Label *) optmenu [0]->components [9])->setText (buf);
}

void callbackAntialiasing (Component *comp, int key)
{
  char buf [256];
  antialiasing = (antialiasing == 0 ? 1 : 0);
  if (antialiasing) strcpy (buf, "ON");
  else strcpy (buf, "OFF");
  ((Label *) optmenu [0]->components [7])->setText (buf);
  events.setAntialiasing ();
}

void callbackDithering (Component *comp, int key)
{
  char buf [256];
  dithering = (dithering == 0 ? 1 : 0);
  if (dithering) strcpy (buf, "ON");
  else strcpy (buf, "OFF");
  ((Label *) optmenu [0]->components [5])->setText (buf);
}

void callbackView (Component *comp, int key)
{
  char buf [256];
  if (key == MOUSE_BUTTON_LEFT)
  {
    view += 10;
    if (view > VIEW_MAX) view = VIEW_MIN;
    statemenu.reshape ();
  }
  else
  {
    view -= 10;
    if (view < VIEW_MIN) view = VIEW_MAX;
    statemenu.reshape ();
  }
  sprintf (buf, "%d", (int) view);
  ((Label *) optmenu [0]->components [3])->setText (buf); // this is also set by autoconfig in menu_timer
}

void callbackQuality (Component *comp, int key)
{
  char buf [256];
  if (key == MOUSE_BUTTON_LEFT)
  {
    quality ++;
    if (quality > 5) quality = 0;
  }
  else
  {
    quality --;
    if (quality < 0) quality = 5;
  }
  sprintf (buf, "%d", quality);
  ((Label *) optmenu [0]->components [1])->setText (buf); // this is also set by autoconfig in menu_timer
}

void callbackTraining (Component *comp, int key)
{
  allmenus.setVisible (false);

  switch_mission (getTrainingIdFromValue (comp->id - trainingstartid));
}

void callbackCampaign (Component *comp, int key)
{
  allmenus.setVisible (false);
  switch_mission (getCampaignIdFromValue (comp->id - campaignstartid));
}

void callbackMainMenu (Component *comp, int key)
{
  int i;
  for (i = 0; i < 6; i ++)
    if (comp == mainbutton [i])
    {
      allmenus.setVisible (false);
      allmenus.visible = true;
      allmenus.components [0]->setVisible (true);
      allmenus.components [i + 1]->setVisible (true);
      currentsubmenu = allmenus.components [i + 1];
      if (missionactive)
        mainbutton [6]->setVisible (true);
      else
        mainbutton [6]->setVisible (false);
      if (i == 3)
      {
        if (currentoptmenu) currentoptmenu->setVisible (true);
        if (currentoptmenu == allmenus.components [10])
        {
          if (controls == CONTROLS_KEYBOARD) allmenus.components [11]->setVisible (true);
          else if (controls == CONTROLS_JOYSTICK) allmenus.components [13]->setVisible (true);
          else allmenus.components [12]->setVisible (true);
        }
      }
      return;
    }
  for (i = 0; i < 3; i ++)
    if (comp == optbutton [i])
    {
      allmenus.components [8]->setVisible (false);
      allmenus.components [9]->setVisible (false);
      allmenus.components [10]->setVisible (false);
      allmenus.components [i + 1 + 7]->setVisible (true);
      if (i + 1 + 7 == 10)
      {
        if (controls == CONTROLS_KEYBOARD) allmenus.components [11]->setVisible (true);
        else if (controls == CONTROLS_JOYSTICK) allmenus.components [13]->setVisible (true);
        else allmenus.components [12]->setVisible (true);
      }
      else
      {
        allmenus.components [11]->setVisible (false);
        allmenus.components [12]->setVisible (false);
        allmenus.components [13]->setVisible (false);
      }
      currentoptmenu = allmenus.components [i + 1 + 7];
      return;
    }
}

#endif
