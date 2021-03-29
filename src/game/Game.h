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

#ifndef IS_GAME_H
#define IS_GAME_H

#include "globals.h"
#include "GameState.h"
#include "Events.h"
#include "menu/Component.h"
#include "logging/Logging.h"
#include "net/net.h"
#include "cockpit/Cockpit.h"

extern GameState *gamestate;
extern StatePlay stateplay;
extern StateMenu statemenu;
extern StateMission statemission;
extern StateFame statefame;
extern StateInit stateinit;
extern StateFinish statefinish;
extern StateQuit statequit;
extern StatePause statepause;
extern StateCredits statecredits;
extern StateStats statestats;
extern StateFighter statefighter;
extern StateCreate statecreate;
extern StateJoin statejoin;

extern Events events;

extern Container *controlsmenu [5];

extern int joysticks;

const int maxjoysticks = 10;


extern SDL_Window *sdlwindow;
extern SDL_Joystick *sdljoystick [maxjoysticks];
extern int sdljoystickaxes [maxjoysticks];

extern bool missionactive;
extern int aktfighter;

extern Container allmenus;
extern Container famemenu;
extern Container fightermenu;
extern Container missionmenu;
extern Container quitmenu;
extern Container statsmenu;
extern Component *currentsubmenu;
extern Component *currentoptmenu;
extern Container *mainmenu;
extern Container *submenu [10];
extern Container *optmenu [5];

extern int campaignstartid;
extern int trainingstartid;

extern Button *mainbutton [10];
extern Button *optbutton [10];

extern float visibleangle;
extern float nearclippingplane;
extern GLenum polygonMode;
extern int sdldisplay;
extern int sdlreshape;
extern bool startcannon;
extern float fps;

extern Server *server;
extern Client *client;

extern Rotation camrot;
extern Vector3 cam;

extern float view_x;
extern float view_y;

extern int flash;

extern Space *space;
extern SpaceObj *sphere;
extern Sphere *objsphere;
extern HighClouds *highclouds;
extern HighClouds *highclouds2;

extern Vector3 tlnull;
extern Vector3 tlinf;
extern Vector3 tlminf;
extern Rotation rotnull;

extern Cockpit *cockpit;

extern float blackout;
extern float redout;

extern bool firststart;

extern int lastshield;
extern bool initing;

extern Texture *textitle;

extern void drawRank (float xp, float yp, float zp, int rank, float zoom);
extern void drawMedal (float xp, float yp, float zp, int medal, float zoom, int missionid);
extern void drawMouseCursor ();
extern int getMedal (int score);
extern void adjustBrightness ();

extern int getTrainingIdFromValue (int n);
extern int getCampaignIdFromValue (int n);
extern int getCampaignValueFromId (int n);
extern void setPlaneVolume ();
extern void setLightSource (int gamma);
//extern Model3d *getModel (int id);

extern int game_levelInit ();

extern void playRandomMusic ();

extern void switch_menu ();

extern bool ispromoted;
extern int statsitemselected;

extern void switch_stats ();
extern void createMission (int missionid);
extern void switch_mission (int missionid);
extern void switch_fame ();
extern void switch_fighter ();
extern void switch_create ();
extern void switch_join ();

extern int creditstimer;
extern int finishtimer;

extern void switch_credits ();
extern void switch_finish ();
extern void switch_quit ();
extern void switch_game ();

extern int getJoystickAxisIndex (int n);

extern int frames;
extern int time2;
extern int time1;

extern void frame ();

extern int missionending;
extern int missionstate;

extern int menuitem;
extern int menutimer;
extern int menuitemselected;

extern int missionmenutimer;

extern void drawCircles (Color *colorstd);
extern void drawQuads (Color *colorstd);
extern void drawPlasma (Color *colorstd);
extern void pleaseWait ();
extern char *getModelText (const UnitDescriptor &id);
//extern char *getModelName (const UnitDescriptor &id);
extern void drawArrow (float x, float y, float w, float h);

extern void game_quit ();
extern void drawMissionElement (float x, float y, float z, int thismissionid, int missionid, int selected, char *string);
extern void test ();
extern int selectMouse (int x, int y, int motionx, int motiony, int mode, bool shift);

extern void setMissiles (Model3d *model);
extern void myInit ();

extern Rotation rot;
extern Rotation rot2;
extern Vector3 tl;
extern Vector3 tl2;
extern int initexplode;
extern int initexplode1;
extern int inittimer;
extern int inittimer_gl117;

const int maxfx = 256;
const int maxfy = 64;

extern int heat [maxfy] [maxfx];
extern int heat2 [maxfy] [maxfx];

extern void myFirstInit ();

extern void genFireLine ();

extern void proceedFire ();

#endif
