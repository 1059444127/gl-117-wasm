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

#ifndef IS_CONFIGURATION_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "configuration/Configuration.h"
#include "configuration/Directory.h"
#include "logging/Logging.h"
#include "util/Util.h"



// one object of the Configuration class
Configuration conf;

// definition of global variables needed by basic packages
int timestep = 34;
int quality = 3;
float view = 50.0;
int antialiasing = 1;
int specialeffects = 1;
int day = 1;
int weather = 0;
float sungamma = 45.0;

// initialization
int width = 800;
int height = 600;
int bpp = 32;
int wantwidth = 800; // requested values for next restart
int wantheight = 600;
int wantfullscreen = 1;

int volumesound = 100;
int volumemusic = 100;
int dithering = 1;
int dynamiclighting = 1;
int fullscreen = 1;
int lighting = 1; // 1=on, 0=off (obsolete)

int difficulty = 1; // 0=easy, 1=normal, 2=hard
int physics = 0; // 0=action, 1=realistic
int brightness = 0; // brightness correction
int contrast = 0; // contrast/gamma correction (not yet configurable)
int controls = CONTROLS_MOUSE; // see CONTROLS-constants
//int game = GAME_INIT; // see GAME-constants

int clouds = 0;
int camera = 0;
bool isserver = false;
//int loglevel = LOG_DEBUG;

// use 0...255 for one byte keys, 256... for special (two byte) beys
unsigned int key_firecannon = 32, key_firemissile = 13, key_dropchaff = 'C', key_dropflare = 'F';
unsigned int key_selectmissile = 'M', key_thrustup = 'S', key_thrustdown = 'X';
unsigned int key_targetnearest = 'E', key_targetnext = 'T', key_targetprevious = 'P', key_targetlocking = 'L';

int joystick_firecannon = 0, joystick_firemissile = 2, joystick_dropchaff = 3, joystick_dropflare = 3;
int joystick_selectmissile = 1, joystick_thrustup = 4, joystick_thrustdown = 5;
int joystick_targetnearest = 101, joystick_targetnext = 100, joystick_targetprevious = 102, joystick_targetlocking = 103;
int joystick_aileron = 0, joystick_elevator = 1, joystick_rudder = 3, joystick_throttle = 2;
int joystick_view_x = 4, joystick_view_y = 5;

unsigned char mouse_firecannon = MOUSE_BUTTON_LEFT, mouse_firemissile = MOUSE_BUTTON_RIGHT;
unsigned char mouse_selectmissile = MOUSE_BUTTON_MIDDLE;
unsigned int mouse_sensitivity = 100;
bool mouse_reverse = false;
bool mouse_relative = false;
int mouse_autorudder = 30;



Configuration::Configuration ()
{
}

void Configuration::saveConfig ()
{
  ConfigFile *cf = new ConfigFile ();
  std::string confname = Directory::getSaves ("conf");
  DISPLAY_INFO(FormatString ("Saving %s ", confname.c_str ()));
  int ret1 = cf->openOutput (confname.c_str ());
  if (ret1 == 0)
  {
    DISPLAY_ERROR("Could not save configuration");
    fflush (stderr);
    return;
  }
  cf->writeText ("# Configuration\n");
  cf->writeText ("# Some possible width x height values for fullscreen mode:");
  cf->writeText ("#  640x480, 800x600, 1024x768, 1152x864, 1280x768, 1280x960, 1280x1024");
  cf->write (" width", wantwidth);
  cf->write (" height", wantheight);
  cf->writeText ("# Possible bits per pixel values (color depth):");
  cf->writeText ("#  8 (not recommended), 16, 24, 32");
  cf->write (" bpp", bpp);
  cf->writeText ("# Try to go fullscreen = 1, game in window = 0");
  cf->write (" fullscreen", wantfullscreen);
  cf->writeText ("# Quality: 0=software rendered up to 5=best (default=2)");
  cf->write (" quality", quality);
  cf->writeText ("# Far clipping plane: 20..100 (default=50)");
  cf->write (" view", (int) view);
  cf->writeText ("# Dithering: 0=off, 1=on (default=1)");
  cf->write (" dithering", dithering);
  cf->writeText ("# Antialiasing: 0=off, 1=on (default=1)");
  cf->write (" antialiasing", antialiasing);
  cf->writeText ("# Special effects: 0=off, 1=on (default=1)");
  cf->write (" specialeffects", specialeffects);
  cf->writeText ("# Dynamic lighting: 0=off, 1=on (default=1)");
  cf->write (" dynamiclighting", dynamiclighting);
#ifdef HAVE_SDL_MIXER
  cf->writeText ("# Sound volume: 0..100 (default=100) per cent");
  cf->write (" sound", (int) volumesound);
  cf->writeText ("# Music volume: 0..100 (default=100) per cent");
  cf->write (" music", (int) volumemusic);
#endif
  cf->writeText ("# Piloting controls: 0=keyboard, 1=mouse easy, 2=joystick");
  cf->write (" controls", controls);
  cf->writeText ("# Difficulty level: 0=easy, 1=medium, 2=hard");
  cf->write (" difficulty", difficulty);
  cf->writeText ("# Brightness: -50..50 per cent (default=0)");
  cf->write (" brightness", brightness);
  cf->writeText ("# Physics: 0=action, 1=realistic");
  cf->write (" physics", physics);
  cf->writeText ("\n# This file is meant to give sensible startup settings");
  cf->writeText ("# as graphic cards and drivers may differ some 100 times in speed");
  cf->writeText ("\n# To get back to default settings, just delete this file!");
  cf->close ();
  delete cf;
}

void Configuration::saveSaveConfig ()
{
  ConfigFile *cf = new ConfigFile ();
  std::string confname = Directory::getSaves ("saveconf");
  DISPLAY_INFO(FormatString ("Saving %s ", confname.c_str ()));
  int ret1 = cf->openOutput (confname.c_str ());
  if (ret1 == 0)
  {
    DISPLAY_ERROR("Could not save working graphics mode configuration");
    fflush (stderr);
    return;
  }
  cf->writeText ("# Working backup configuration\n");
  cf->writeText ("# Do not edit this file. It is only used to restore a broken configuration!");
  cf->write (" width", width);
  cf->write (" height", height);
  cf->write (" bpp", bpp);
  cf->write (" fullscreen", fullscreen);
  cf->close ();
  delete cf;
}

int Configuration::loadConfig ()
{
  char ret [256];
  char *str;
  std::string confname = Directory::getSaves ("conf");
  DISPLAY_INFO(FormatString ("Loading %s ", confname.c_str ()));
  ConfigFile *cf = new ConfigFile (confname.c_str ());

  if (cf->buf [0] == 0) // no file found
  {
    delete cf;
    return 0;
  }

  str = cf->getString (ret, "width");
  if (str == NULL)
  { width = 800; }
  else
  { width = atoi (str); }
  if (width < 100) width = 100;
  else if (width > 3000) width = 1024;
  wantwidth = width;

  str = cf->getString (ret, "height");
  if (str == NULL)
  { height = 600; }
  else
  { height = atoi (str); }
  if (height < 100) height = 100;
  else if (height > 2000) height = 768;
  wantheight = height;

  str = cf->getString (ret, "bpp");
  if (str == NULL)
  { bpp = 32; }
  else
  { bpp = atoi (str); }
  if (bpp != 8 && bpp != 16 && bpp != 24 && bpp != 32)
    bpp = 32;

  str = cf->getString (ret, "fullscreen");
  if (str == NULL)
  { fullscreen = 1; }
  else
  { fullscreen = atoi (str); }
  if (fullscreen) fullscreen = 1;
  wantfullscreen = fullscreen;

  str = cf->getString (ret, "quality");
  if (str == NULL)
  { quality = 2; }
  else
  { quality = atoi (str); }
  if (quality < 0) quality = 0;
  else if (quality > 5) quality = 5;

  str = cf->getString (ret, "view");
  if (str == NULL)
  { view = 50; }
  else
  { view = atoi (str); }
  if (view < VIEW_MIN)
  {
    view = VIEW_MIN;
  }
  else if (view > VIEW_MAX)
  {
    view = VIEW_MAX;
  }

  str = cf->getString (ret, "dithering");
  dithering = (str == NULL) ? 1 : atoi (str);
  if (dithering) dithering = 1;

  str = cf->getString (ret, "antialiasing");
  antialiasing = (str == NULL) ? 1 : atoi (str);
  if (antialiasing) antialiasing = 1;

  str = cf->getString (ret, "specialeffects");
  specialeffects = (str == NULL) ? 1 : atoi (str);
  if (specialeffects) specialeffects = 1;

  str = cf->getString (ret, "dynamiclighting");
  dynamiclighting = (str == NULL) ? 1 : atoi (str);
  if (dynamiclighting) dynamiclighting = 1;

#ifdef HAVE_SDL_MIXER
  str = cf->getString (ret, "sound");
  if (str == NULL)
  { volumesound = 100; }
  else
  { volumesound = atoi (str); }
  if (volumesound < 0) volumesound = 0;
  else if (volumesound > 100) volumesound = 100;

  str = cf->getString (ret, "music");
  if (str == NULL)
  { volumemusic = 100; }
  else
  { volumemusic = atoi (str); }
  if (volumemusic < 0) volumemusic = 0;
  else if (volumemusic > 100) volumemusic = 100;
#endif

  str = cf->getString (ret, "controls");
  if (str == NULL)
  { controls = CONTROLS_MOUSE; }
  else
  { controls = atoi (str); }
  if (controls < 0) controls = 0;
  else if (controls > 2) controls = 0;

  str = cf->getString (ret, "difficulty");
  if (str == NULL)
  { difficulty = 1; }
  else
  { difficulty = atoi (str); }
  if (difficulty < 0) difficulty = 0;
  else if (difficulty > 2) difficulty = 0;

  str = cf->getString (ret, "brightness");
  if (str == NULL)
  { brightness = 0; }
  else
  { brightness = atoi (str); }
  if (brightness < -50) brightness = -50;
  else if (brightness > 50) brightness = 50;

  str = cf->getString (ret, "physics");
  if (str == NULL)
  { physics = 0; }
  else
  { physics = atoi (str); }
  if (physics < 0) physics = 0;
  else if (physics > 1) physics = 1;

  delete cf;
  return 1;
}

int Configuration::loadSaveConfig ()
{
  char ret [256];
  char *str;
  const char *confname = Directory::getSaves ("saveconf").c_str ();
  DISPLAY_INFO(FormatString ("Loading %s ", confname));
  ConfigFile *cf = new ConfigFile (confname);

  if (cf->buf [0] == 0) // no file found
  {
    delete cf;
    return 0;
  }

  str = cf->getString (ret, "savewidth");
  if (str == NULL)
  { width = 800; }
  else
  { width = atoi (str); }
  if (width < 100) width = 100;
  else if (width > 3000) width = 1024;

  str = cf->getString (ret, "saveheight");
  if (str == NULL)
  { height = 600; }
  else
  { height = atoi (str); }
  if (height < 100) height = 100;
  else if (height > 2000) height = 768;

  str = cf->getString (ret, "savebpp");
  if (str == NULL)
  { bpp = 32; }
  else
  { bpp = atoi (str); }
  if (bpp != 8 && bpp != 16 && bpp != 24 && bpp != 32)
    bpp = 32;

  str = cf->getString (ret, "savefullscreen");
  if (str == NULL)
  { fullscreen = 1; }
  else
  { fullscreen = atoi (str); }
  if (fullscreen) fullscreen = 1;

  delete cf;
  return 1;
}

void Configuration::writeJoystick (ConfigFile *cf, char *str, int jn)
{
  char buf [4096];
  sprintf (buf, "%s = %c%d", str, 'A' + (jn / 1000), jn % 1000);
  cf->writeText (buf);
}

void Configuration::saveConfigInterface ()
{
  ConfigFile *cf = new ConfigFile ();
  std::string confname = Directory::getSaves ("conf.interface");
  DISPLAY_INFO(FormatString ("Saving %s ", confname.c_str ()));
  int ret1 = cf->openOutput (confname.c_str ());
  if (ret1 == 0)
  {
    DISPLAY_ERROR("Could not save interface configuration");
    return;
  }
  cf->writeText ("# Interface configuration\n");
  cf->writeText ("# ---------------------------------------------------------------------");
  cf->writeText ("# Keyboard section");
  cf->writeText ("# ---------------------------------------------------------------------\n");
  cf->writeText ("# Use ASCII-Code values or letters to remap");
  cf->writeText ("#  8=BACKSPACE, 13=ENTER, 32=SPACE, 65=A...90=Z (NOT case sensitive)");
  cf->write (" key_firecannon", (int) key_firecannon);
  cf->write (" key_firemissile", (int) key_firemissile);
  cf->write (" key_dropflare", (int) key_dropflare);
  cf->write (" key_dropchaff", (int) key_dropchaff);
  cf->write (" key_selectmissile", (int) key_selectmissile);
  cf->writeText ("#  Target nearest enemy with priority for enemies in front of you");
  cf->write (" key_targetnearest", (int) key_targetnearest);
  cf->writeText ("#  Target next enemy who has locked you");
  cf->write (" key_targetlocking", (int) key_targetlocking);
  cf->write (" key_targetnext", (int) key_targetnext);
  cf->write (" key_targetprevious", (int) key_targetprevious);
  cf->write (" key_incthrust", (int) key_thrustup);
  cf->write (" key_decthrust", (int) key_thrustdown);
  cf->writeText ("# All other piloting keys (CURSORS, PGUP/DN) are fixed.");
  cf->writeText ("\n# ---------------------------------------------------------------------");
  cf->writeText ("# Mouse section");
  cf->writeText ("# ---------------------------------------------------------------------\n");
  cf->writeText ("# Sensitivity: 70...200%, 70%=full screen area, 200%=max sensitivity");
  cf->write (" mouse_sensitivity", (int) mouse_sensitivity);
  cf->writeText ("# Reverse y-axis: 0 (no) or 1 (yes)");
  cf->write (" mouse_reverse", (int) mouse_reverse);
  cf->writeText ("# Relative coordinates: 0 (no) or 1 (yes)");
  cf->write (" mouse_relative", (int) mouse_relative);
  cf->writeText ("# Auto rudder on x-axis, dead area for rolls: 0...100");
  cf->write (" mouse_autorudder", (int) mouse_autorudder);
  cf->writeText ("\n# Buttons: 1=Left, 2=Middle, 3=Right");
  int mousebutton = 1;
  if (mouse_firecannon == MOUSE_BUTTON_MIDDLE) mousebutton = 2;
  else if (mouse_firecannon == MOUSE_BUTTON_RIGHT) mousebutton = 3;
  else mousebutton = 1;
  cf->write (" mouse_firecannon", mousebutton);
  if (mouse_firemissile == MOUSE_BUTTON_MIDDLE) mousebutton = 2;
  else if (mouse_firemissile == MOUSE_BUTTON_RIGHT) mousebutton = 3;
  else mousebutton = 1;
  cf->write (" mouse_firemissile", mousebutton);
  if (mouse_selectmissile == MOUSE_BUTTON_MIDDLE) mousebutton = 2;
  else if (mouse_selectmissile == MOUSE_BUTTON_RIGHT) mousebutton = 3;
  else mousebutton = 1;
  cf->write (" mouse_selectmissile", mousebutton);
  cf->writeText ("\n# ---------------------------------------------------------------------");
  cf->writeText ("# Joystick section");
  cf->writeText ("# ---------------------------------------------------------------------\n");
  cf->writeText ("# The number of axes, buttons, and the coolie hat depends on your joystick!");
  cf->writeText ("# Numbers start with A=first joystick, B=second joystick...J=10th joystick");
  cf->writeText ("#  followed by a number to identify axes, buttons, and coolie hat\n");
  cf->writeText ("# Axis: 0...MAX-1 (maybe 0=aileron 1=elevator 2=throttle 3=rudder)");
  writeJoystick (cf, " joystick_aileron", joystick_aileron);
  writeJoystick (cf, " joystick_elevator", joystick_elevator);
  writeJoystick (cf, " joystick_throttle", joystick_throttle);
  writeJoystick (cf, " joystick_rudder", joystick_rudder);
  writeJoystick (cf, " joystick_view_x", joystick_view_x);
  writeJoystick (cf, " joystick_view_y", joystick_view_y);
  cf->writeText ("\n# Buttons: 0...MAX-1, Coolie: 100=Right, 101=Up, 102=Left, 103=Down");
  writeJoystick (cf, " joystick_firecannon", joystick_firecannon);
  writeJoystick (cf, " joystick_firemissile", joystick_firemissile);
  cf->writeText ("# Dropping chaff AND flare may be preferred");
  writeJoystick (cf, " joystick_dropflare", joystick_dropflare);
  writeJoystick (cf, " joystick_dropchaff", joystick_dropchaff);
  writeJoystick (cf, " joystick_selectmissile", joystick_selectmissile);
  writeJoystick (cf, " joystick_targetnearest", joystick_targetnearest);
  writeJoystick (cf, " joystick_targetlocking", joystick_targetlocking);
  writeJoystick (cf, " joystick_targetnext", joystick_targetnext);
  writeJoystick (cf, " joystick_targetprevious", joystick_targetprevious);
  writeJoystick (cf, " joystick_incthrust", joystick_thrustup);
  writeJoystick (cf, " joystick_decthrust", joystick_thrustdown);
  cf->writeText ("\n# This file is meant to give sensible custom interface settings");
  cf->writeText ("\n# To get back to default settings, just delete this file!");
  cf->close ();
  delete cf;
}

int Configuration::getKey (char *str, int n)
{
  int tmp;
  if (str == NULL) return n;
  tmp = atoi (str);
  if (tmp == 0)
  {
    if (str [0] >= 33)
    {
      return toupper (str [0]);
    }
    return n;
  }
  return tmp;
}

int Configuration::getJoystick (char *str, int n)
{
  int tmp, jn = 0;
  if (str == NULL) return n;
  int str0 = toupper (str [0]) - (int) 'A';
  if (str0 >= -1 && str0 < 10)
    jn = str0;
  else
    return n;
  tmp = atoi (&str [1]);
  return jn * 1000 + tmp;
}

int Configuration::loadConfigInterface ()
{
  char ret [256];
  char *str;
  std::string confname = Directory::getSaves ("conf.interface");
  DISPLAY_INFO(FormatString ("Loading %s ", confname.c_str ()));
  ConfigFile *cf = new ConfigFile (confname.c_str ());

  str = cf->getString (ret, "key_firecannon");
  key_firecannon = getKey (str, 32);

  str = cf->getString (ret, "key_firemissile");
  key_firemissile = getKey (str, 13);

  str = cf->getString (ret, "key_dropchaff");
  key_dropchaff = getKey (str, 'C');

  str = cf->getString (ret, "key_dropflare");
  key_dropflare = getKey (str, 'F');

  str = cf->getString (ret, "key_selectmissile");
  key_selectmissile = getKey (str, 'M');

  str = cf->getString (ret, "key_targetnearest");
  key_targetnearest = getKey (str, 'E');

  str = cf->getString (ret, "key_targetlocking");
  key_targetlocking = getKey (str, 'L');

  str = cf->getString (ret, "key_targetnext");
  key_targetnext = getKey (str, 'T');

  str = cf->getString (ret, "key_targetprevious");
  key_targetprevious = getKey (str, 'P');

  str = cf->getString (ret, "key_incthrust");
  key_thrustup = getKey (str, 'S');

  str = cf->getString (ret, "key_decthrust");
  key_thrustdown = getKey (str, 'X');

  str = cf->getString (ret, "mouse_sensitivity");
  if (str == NULL)
  { mouse_sensitivity = 100; }
  else
  { mouse_sensitivity = atoi (str); }
  if (mouse_sensitivity < 70) mouse_sensitivity = 70;
  else if (mouse_sensitivity > 200) mouse_sensitivity = 200;

  str = cf->getString (ret, "mouse_reverse");
  if (str == NULL)
  { mouse_reverse = false; }
  else
  { mouse_reverse = (atoi (str) != 0); }

  str = cf->getString (ret, "mouse_relative");
  if (str == NULL)
  { mouse_relative = false; }
  else
  { mouse_relative = (atoi (str) != 0); }

  str = cf->getString (ret, "mouse_autorudder");
  if (str == NULL)
  { mouse_autorudder = 30; }
  else
  { mouse_autorudder = atoi (str); }
  if (mouse_autorudder < 0) mouse_autorudder = 0;
  else if (mouse_autorudder > 100) mouse_autorudder = 100;

  int mousebutton = 1;
  str = cf->getString (ret, "mouse_firecannon");
  if (str == NULL)
  { mousebutton = 1; }
  else
  { mousebutton = atoi (str); }
  if (mousebutton == 2) mouse_firecannon = MOUSE_BUTTON_MIDDLE;
  else if (mousebutton == 3) mouse_firecannon = MOUSE_BUTTON_RIGHT;
  else mouse_firecannon = MOUSE_BUTTON_LEFT;

  str = cf->getString (ret, "mouse_firemissile");
  if (str == NULL)
  { mousebutton = 3; }
  else
  { mousebutton = atoi (str); }
  if (mousebutton == 2) mouse_firemissile = MOUSE_BUTTON_MIDDLE;
  else if (mousebutton == 3) mouse_firemissile = MOUSE_BUTTON_RIGHT;
  else mouse_firemissile = MOUSE_BUTTON_LEFT;

  str = cf->getString (ret, "mouse_selectmissile");
  if (str == NULL)
  { mousebutton = 2; }
  else
  { mousebutton = atoi (str); }
  if (mousebutton == 2) mouse_selectmissile = MOUSE_BUTTON_MIDDLE;
  else if (mousebutton == 3) mouse_selectmissile = MOUSE_BUTTON_RIGHT;
  else mouse_selectmissile = MOUSE_BUTTON_LEFT;

  str = cf->getString (ret, "joystick_aileron");
  joystick_aileron = getJoystick (str, 0);

  str = cf->getString (ret, "joystick_elevator");
  joystick_elevator = getJoystick (str, 1);

  str = cf->getString (ret, "joystick_throttle");
  joystick_throttle = getJoystick (str, 2);

  str = cf->getString (ret, "joystick_rudder");
  joystick_rudder = getJoystick (str, 3);

  str = cf->getString (ret, "joystick_view_x");
  joystick_view_x = getJoystick (str, 4);

  str = cf->getString (ret, "joystick_view_y");
  joystick_view_y = getJoystick (str, 5);

  str = cf->getString (ret, "joystick_firecannon");
  joystick_firecannon = getJoystick (str, 0);

  str = cf->getString (ret, "joystick_firemissile");
  joystick_firemissile = getJoystick (str, 2);

  str = cf->getString (ret, "joystick_dropchaff");
  joystick_dropchaff = getJoystick (str, 3);

  str = cf->getString (ret, "joystick_dropflare");
  joystick_dropflare = getJoystick (str, 3);

  str = cf->getString (ret, "joystick_selectmissile");
  joystick_selectmissile = getJoystick (str, 1);

  str = cf->getString (ret, "joystick_targetnearest");
  joystick_targetnearest = getJoystick (str, 101);

  str = cf->getString (ret, "joystick_targetlocking");
  joystick_targetlocking = getJoystick (str, 103);

  str = cf->getString (ret, "joystick_targetnext");
  joystick_targetnext = getJoystick (str, 100);

  str = cf->getString (ret, "joystick_targetprevious");
  joystick_targetprevious = getJoystick (str, 102);

  str = cf->getString (ret, "joystick_incthrust");
  joystick_thrustup = getJoystick (str, 4);

  str = cf->getString (ret, "joystick_decthrust");
  joystick_thrustdown = getJoystick (str, 5);

  if (cf->buf [0] == 0) // no file found
  {
    delete cf;
    return 0;
  }

  delete cf;
  return 1;
}

void Configuration::key2string (int key, char *buf)
{
  buf [0] = '\0';

  // try one byte keys
  if (key > 32 && key <= 'z') sprintf (buf, "%c", key);
  else if (key == KEY_SPACE) strcpy (buf, "SPACE");
  else if (key == KEY_TAB) strcpy (buf, "TAB");
  else if (key == KEY_ENTER) strcpy (buf, "ENTER");
  else if (key == KEY_BACKSPACE) strcpy (buf, "BSPACE");
  if (key < 256) return;

  // try special keys
  key -= 256;
  if (key == KEY_F1) strcpy (buf, "F1");
  else if (key == KEY_F2) strcpy (buf, "F2");
  else if (key == KEY_F3) strcpy (buf, "F3");
  else if (key == KEY_F4) strcpy (buf, "F4");
  else if (key == KEY_F5) strcpy (buf, "F5");
  else if (key == KEY_F6) strcpy (buf, "F6");
  else if (key == KEY_F7) strcpy (buf, "F7");
  else if (key == KEY_F8) strcpy (buf, "F8");
  else if (key == KEY_F9) strcpy (buf, "F9");
  else if (key == KEY_F10) strcpy (buf, "F10");
  else if (key == KEY_F10) strcpy (buf, "F11");
  else if (key == KEY_F10) strcpy (buf, "F12");
  else if (key == KEY_UP) strcpy (buf, "UP");
  else if (key == KEY_DOWN) strcpy (buf, "DOWN");
  else if (key == KEY_LEFT) strcpy (buf, "LEFT");
  else if (key == KEY_RIGHT) strcpy (buf, "RIGHT");
  else if (key == KEY_PGUP) strcpy (buf, "PGUP");
  else if (key == KEY_PGDOWN) strcpy (buf, "PGDOWN");
  else if (key == KEY_LALT) strcpy (buf, "LALT");
  else if (key == KEY_RALT) strcpy (buf, "RALT");
  else if (key == KEY_LCTRL) strcpy (buf, "LCTRL");
  else if (key == KEY_RCTRL) strcpy (buf, "RCTRL");
  else if (key == KEY_LSHIFT) strcpy (buf, "LSHIFT");
  else if (key == KEY_RSHIFT) strcpy (buf, "RSHIFT");
  else if (key == KEY_CAPSLOCK) strcpy (buf, "CAPS");
  else if (key == KEY_DELETE) strcpy (buf, "DEL");
  else if (key == KEY_HOME) strcpy (buf, "HOME");
  else if (key == KEY_INSERT) strcpy (buf, "INS");
  else if (key == KEY_END) strcpy (buf, "END");
}

void Configuration::joystick2string (int button, char *buf)
{
  buf [0] = '\0';
  char joystick = 'A' + button / 1000;
  int b = button % 1000;

  // try one byte keys
  if (b >= 0 && b < 100) sprintf (buf, "%c BTN%d", joystick, b);
  else if (b >= 100) sprintf (buf, "%c HAT%d", joystick, b - 100);
}

#endif
