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
#define IS_CONFIGURATION_H

#include "../config.h" // defines HAVE_SDL, HAVE_SDL_MIXER
#include "opengl/includegl.h"

// HAVE_SDL_MIXER means SDL and SDL_mixer are available
// HAVE_SDL_NET means SDL_net is available

//#undef HAVE_SDL_NET // dummy
//#undef HAVE_SDL_MIXER // dummy

#define GLOBALSCALE 1

#define LINEWIDTH(x) (float) x * height / 600.0F

// controls
#define CONTROLS_KEYBOARD 0
#define CONTROLS_MOUSE 1
#define CONTROLS_JOYSTICK 2

// view range borders
#define VIEW_MIN 30
#define VIEW_MAX 300

extern int width;
extern int height;
extern int bpp;
extern int wantwidth; // requested values for next restart
extern int wantheight;
extern int wantfullscreen;

extern int volumesound;
extern int volumemusic;
extern int dithering;
extern int dynamiclighting;
extern int fullscreen;
extern int lighting; // 1=on, 0=off (obsolete)

extern int difficulty; // 0=easy, 1=normal, 2=hard
extern int physics; // 0=action, 1=realistic
extern int brightness; // brightness correction
extern int controls; // see CONTROLS-constants
extern int game; // see GAME-constants

extern int clouds;
extern int camera;
extern bool isserver;
//extern int loglevel;

// TODO: these constants should go somewhere else
// maximum constants for objects
//const int maxexplosion = 30;
//const int maxfighter = 30;
//const int maxlaser = 150;
//const int maxmissile = 30;
const int maxstar = 70;
//const int maxgroundobj = 10;
//const int maxblacksmoke = 50;
//const int maxflare = 30;
//const int maxchaff = 30;

// game states
/*#define GAME_INIT 0
#define GAME_PLAY 1
#define GAME_PAUSE 2
#define GAME_MENU 3
#define GAME_CREDITS 4
#define GAME_CREATE 5
#define GAME_JOIN 6
#define GAME_MISSION 7
#define GAME_STATS 8
#define GAME_FAME 9
#define GAME_FIGHTER 10
#define GAME_FINISH 11
#define GAME_QUIT 12*/

#define MOUSE_BUTTON_LEFT SDL_BUTTON_LEFT
#define MOUSE_BUTTON_MIDDLE SDL_BUTTON_MIDDLE
#define MOUSE_BUTTON_RIGHT SDL_BUTTON_RIGHT
#define MOUSE_UP SDL_RELEASED
#define MOUSE_DOWN SDL_PRESSED
#define KEY_F1 SDLK_F1
#define KEY_F2 SDLK_F2
#define KEY_F3 SDLK_F3
#define KEY_F4 SDLK_F4
#define KEY_F5 SDLK_F5
#define KEY_F6 SDLK_F6
#define KEY_F7 SDLK_F7
#define KEY_F8 SDLK_F8
#define KEY_F9 SDLK_F9
#define KEY_F10 SDLK_F10
#define KEY_UP SDLK_UP
#define KEY_DOWN SDLK_DOWN
#define KEY_LEFT SDLK_LEFT
#define KEY_RIGHT SDLK_RIGHT
#define KEY_PGUP SDLK_PAGEUP
#define KEY_PGDOWN SDLK_PAGEDOWN
#define KEY_LALT SDLK_LALT
#define KEY_RALT SDLK_RALT
#define KEY_LCTRL SDLK_LCTRL
#define KEY_RCTRL SDLK_RCTRL
#define KEY_LSHIFT SDLK_LSHIFT
#define KEY_RSHIFT SDLK_RSHIFT
#define KEY_CAPSLOCK SDLK_CAPSLOCK
#define KEY_DELETE SDLK_DELETE
#define KEY_INSERT SDLK_INSERT
#define KEY_HOME SDLK_HOME
#define KEY_END SDLK_END

#define KEY_BACKSPACE 8
#define KEY_TAB 9
#define KEY_ENTER 13
#define KEY_SPACE 32

// interface definitions
extern unsigned int key_firecannon, key_firemissile, key_dropchaff, key_dropflare;
extern unsigned int key_selectmissile, key_thrustup, key_thrustdown;
extern unsigned int key_targetnearest, key_targetnext, key_targetprevious, key_targetlocking;

extern int joystick_firecannon, joystick_firemissile, joystick_dropchaff, joystick_dropflare;
extern int joystick_selectmissile, joystick_thrustup, joystick_thrustdown;
extern int joystick_targetnearest, joystick_targetnext, joystick_targetprevious, joystick_targetlocking;
extern int joystick_aileron, joystick_elevator, joystick_rudder, joystick_throttle;
extern int joystick_view_x, joystick_view_y;

extern unsigned char mouse_firecannon, mouse_firemissile;
extern unsigned char mouse_selectmissile;
extern unsigned int mouse_sensitivity;
extern bool mouse_reverse;
extern bool mouse_relative;
extern int mouse_autorudder;

/**
* This class represents an abstract config file as text file in memory.
*/
class ConfigFile
{
  public:

    char buf [32000]; // max file size
    int length;
    FILE *out;

    ConfigFile ();
    ConfigFile (const char *fname);

    const char *skipwhite (const char *str);
    const char *skipnum (const char *str);
    const char *skipalphanum (const char *str);
    char *getString (char *dest, const char *str);
    int getValue (const char *str);
    int openOutput (const char *fname);
    int write (const char *str1, int n);
    int write (const char *str1, char c);
    void writeText (const char *str);
    void close ();
};

/**
* This class saves/loads special config files of GL-117: conf and conf.interface
*/
class Configuration
{
  public:

    Configuration ();

    void saveConfig ();
    void saveSaveConfig ();
    int loadConfig ();
    int loadSaveConfig ();
    void saveConfigInterface ();
    int loadConfigInterface ();

    void key2string (int key, char *buf);
    void joystick2string (int key, char *buf);

  protected:

    void writeJoystick (ConfigFile *cf, char *str, int jn);
    int getKey (char *str, int n);
    int getJoystick (char *str, int n);
};

extern Configuration conf;



#include <map>
#include <string>

#include "filetokenizer/FileTokenizer.h"

typedef std::map <std::string, std::string> OptionList;

class OptionFile
{
  public:

    FileTokenizer file;
    OptionList optionList;

    OptionFile (const std::string &filename);
    ~OptionFile ();

    bool getString (const std::string &name, std::string &value);
    bool getBoolean (const std::string &name, bool &value);
    bool getInteger (const std::string &name, int &value);
    bool getFloat (const std::string &name, float &value);
};

typedef std::map<std::string, OptionFile *> OptionFileList;

class OptionFileFactory
{
  public:

    static OptionFileList optionFileList;
    static OptionFile *get (const std::string &filename);
};

#endif
