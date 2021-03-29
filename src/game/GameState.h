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

#ifndef IS_GAMESTATE_H
#define IS_GAMESTATE_H

#include "configuration/Configuration.h"
#include "model3d/Model3d.h"

extern int keyb_elev;
extern int keyb_roll;
extern int keyb_rudder;
extern int keyb_lshift;
extern int missionmenuitemselected;
extern int missionmenufighterselected;
extern int missionmenuweaponselected;

extern Color colorwhite;
extern Color colorblue;
extern Color colorgreen;
extern Color colororange;
extern Color colorred;
extern Color coloryellow;
extern Color colorgrey;
extern Color colorlightgrey;

class GameState
{
  public:

    GameState () {}
    virtual ~GameState () {}

    virtual void reshape () {}
    virtual void key (int key, int x, int y) {}
    virtual void keyUp (int key, int x, int y) {}
    virtual void mouse (int button, int state, int x, int y) {}
    virtual void mouseRelMotion (int xrel, int yrel) {}
    virtual void mouseMotion (int x, int y) {}
    virtual void joystickAxis (int x, int y, int rudder, int throttle) {}
    virtual void joystickButton (int button) {}
    virtual void joystickHat (int normhat) {}
    virtual void view () {}
    virtual void display () {}
    virtual void timer (Uint32 dt) {}
};

class StatePlay : public GameState
{
  public:

    virtual void reshape ();
    virtual void key (int key, int x, int y);
    virtual void keyUp (int key, int x, int y);
    virtual void mouse (int button, int state, int x, int y);
    virtual void mouseRelMotion (int xrel, int yrel);
    virtual void mouseMotion (int x, int y);
    virtual void joystickAxis (int x, int y, int rudder, int throttle);
    virtual void joystickButton (int button);
    virtual void joystickHat (int normhat);
    virtual void view ();
    virtual void display ();
    virtual void timer (Uint32 dt);

  protected:
    void calcVibration (Rotation &mycamrot);
    void calcLightFactor (float &pseudoview);
    void calcFlash ();
};

class StateMenu : public GameState
{
  public:

    virtual void reshape ();
    virtual void key (int key, int x, int y);
    virtual void mouse (int button, int state, int x, int y);
    virtual void display ();
    virtual void timer (Uint32 dt);
};

class StateMission : public GameState
{
  public:

    virtual void reshape ();
    virtual void key (int key, int x, int y);
    virtual void mouse (int button, int state, int x, int y);
    virtual void display ();
    virtual void timer (Uint32 dt);
};

class StateFinish : public GameState
{
  public:

    virtual void reshape ();
    virtual void key (int key, int x, int y);
    virtual void display ();
    virtual void timer (Uint32 dt);
};

class StateCreate : public GameState
{
  public:

    virtual void reshape ();
    virtual void key (int key, int x, int y);
    virtual void mouse (int button, int state, int x, int y);
    virtual void display ();
    virtual void timer (Uint32 dt);
};

class StateJoin : public GameState
{
  public:

    virtual void reshape ();
    virtual void key (int key, int x, int y);
    virtual void mouse (int button, int state, int x, int y);
    virtual void display ();
    virtual void timer (Uint32 dt);
};

class StateStats : public GameState
{
  public:

    virtual void reshape ();
    virtual void key (int key, int x, int y);
    virtual void mouse (int button, int state, int x, int y);
    virtual void display ();
    virtual void timer (Uint32 dt);
};

class StateFame : public GameState
{
  public:

    virtual void reshape ();
    virtual void key (int key, int x, int y);
    virtual void mouse (int button, int state, int x, int y);
    virtual void display ();
};

class StateFighter : public GameState
{
  public:

    virtual void reshape ();
    virtual void key (int key, int x, int y);
    virtual void mouse (int button, int state, int x, int y);
    virtual void display ();
};

class StateQuit : public GameState
{
  public:

    virtual void reshape ();
    virtual void key (int key, int x, int y);
    virtual void mouse (int button, int state, int x, int y);
    virtual void display ();
    virtual void timer (Uint32 dt);
};

class StatePause : public GameState
{
  public:

    virtual void reshape ();
    virtual void keyUp (int key, int x, int y) {}
    virtual void display ();
};

class StateCredits : public GameState
{
  public:

    virtual void reshape ();
    virtual void key (int key, int x, int y);
    virtual void mouse (int button, int state, int x, int y);
    virtual void display ();
    virtual void timer (Uint32 dt);
};

class StateInit : public GameState
{
  public:

    virtual void reshape ();
    virtual void key (int key, int x, int y);
    virtual void mouse (int button, int state, int x, int y);
    virtual void display ();
    virtual void timer (Uint32 dt);
};

#endif

