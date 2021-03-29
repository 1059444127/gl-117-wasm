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

#include "GameState.h"
#include "Game.h"
#include "gllandscape/GlLandscape.h"
#include "mission/Mission.h"
#include "loadmodel/Model3dRegistry.h"
#include "render/Render.h"

/*#include "menu/Component.h"
#include "mission/MapLoader.h"
#include "configuration/Directory.h"
#include "opengl/GlPrimitives.h"
#include "landscape/Landscape.h"
#include "net/net.h"
#include "util/Math.h"
#include "cockpit/Cockpit.h"
#include "configuration/Configuration.h"
#include "gllandscape/GlLandscape.h"
#include "logging/Logging.h"
#include "game/Game.h"
#include "game/Events.h"
#include "game/Callbacks.h"

#include <ctype.h>*/

int keyb_elev = 0;
int keyb_roll = 0;
int keyb_rudder = 0;
int keyb_lshift = 0;
int missionmenuitemselected = 0;
int missionmenufighterselected = -1;
int missionmenuweaponselected = -1;

Color colorwhite (255, 255, 255, 255);
Color colorblue (50, 50, 255, 255);
Color colorgreen (100, 255, 100, 255);
Color colororange (255, 150, 100, 255);
Color colorred (255, 0, 0, 255);
Color coloryellow (255, 255, 0, 200);
Color colorgrey (150, 150, 150, 200);
Color colorlightgrey (210, 210, 210, 255);

int vibration = 0;
bool sunblinding = false;
float sunlight = 1.0;
float sunlight_dest = 1.0;
int starttime;
int gametimer;
float lastthrust;
int aktcam = 0;
float dtheta = 0;
float dgamma = 0;
int newcamera = 0;
float lastfps = -1;
int initsynchrotimer = 0;

void StatePlay::reshape ()
{
  glViewport(0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  float v = l->getView ();
  //gluPerspective (visibleangle, (float) width / height, nearclippingplane * GLOBALSCALE, v * GLOBALSCALE);
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);

  SDL_ShowCursor (0);
}

void StatePause::reshape ()
{
  glViewport(0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  float v = l->getView ();
  //gluPerspective (visibleangle, (float) width / height, nearclippingplane * GLOBALSCALE, v * GLOBALSCALE);
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);

  SDL_ShowCursor (0);
}

void StateMenu::reshape ()
{
  glViewport(0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  float v = l->getView ();
  //gluPerspective (visibleangle, 1.0, nearclippingplane * GLOBALSCALE, v * GLOBALSCALE);
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);

  SDL_ShowCursor (0);
}

void StateFame::reshape ()
{
  glViewport(0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  float v = l->getView ();
  //gluPerspective (visibleangle, 1.0, nearclippingplane * GLOBALSCALE, v * GLOBALSCALE);
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);

  SDL_ShowCursor (0);
}

void StateJoin::reshape ()
{
  glViewport(0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  float v = l->getView ();
  //gluPerspective (visibleangle, 1.0, nearclippingplane * GLOBALSCALE, v * GLOBALSCALE);
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);

  SDL_ShowCursor (0);
}

void StateCreate::reshape ()
{
  glViewport(0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  float v = l->getView ();
  //gluPerspective (visibleangle, 1.0, nearclippingplane * GLOBALSCALE, v * GLOBALSCALE);
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);

  SDL_ShowCursor (0);
}

void StateFighter::reshape ()
{
  glViewport(0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  float v = l->getView ();
  //gluPerspective (visibleangle, 1.0, nearclippingplane * GLOBALSCALE, v * GLOBALSCALE);
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);

  SDL_ShowCursor (0);
}

void StateMission::reshape ()
{
  glViewport(0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  float v = l->getView ();
  //gluPerspective (visibleangle, 1.0, nearclippingplane * GLOBALSCALE, v * GLOBALSCALE);
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);

  SDL_ShowCursor (0);
}

void StateQuit::reshape ()
{
  glViewport(0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  float v = l->getView ();
  //gluPerspective (visibleangle, 1.0, nearclippingplane * GLOBALSCALE, v * GLOBALSCALE);
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);

  SDL_ShowCursor (0);
}

void StateCredits::reshape ()
{
  glViewport(0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  float v = l->getView ();
  //gluPerspective (visibleangle, 1.0, nearclippingplane * GLOBALSCALE, v * GLOBALSCALE);
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);

  SDL_ShowCursor (0);
}

void StateFinish::reshape ()
{
  glViewport(0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  float v = l->getView ();
  //gluPerspective (visibleangle, 1.0, nearclippingplane * GLOBALSCALE, v * GLOBALSCALE);
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);

  SDL_ShowCursor (0);
}

void StateStats::reshape ()
{
  glViewport(0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  float v = l->getView ();
  if (camera == 50) v = 100000.0;
  gluPerspective (visibleangle, 1.0, nearclippingplane * GLOBALSCALE, v * GLOBALSCALE);
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);

  SDL_ShowCursor (0);
}

void StatePlay::key (int key, int x, int y)
{
  unsigned int hikey = key;
  unsigned int lokey = key;

  if (key == 27)
  {
    switch_menu ();
  }
  else if (hikey == key_firemissile || lokey == key_firemissile)
  {
    events.fireMissile ();
  }
  else if (hikey == key_dropflare || lokey == key_dropflare)
  {
    events.fireFlare ();
  }
  else if (hikey == key_dropchaff || lokey == key_dropchaff)
  {
    events.fireChaff ();
  }
  else if (hikey == key_firecannon || lokey == key_firecannon)
  {
    events.fireCannon ();
  }
  else if (hikey == key_selectmissile || lokey == key_selectmissile)
  {
    events.selectMissile ();
  }
  else if (hikey == key_thrustup || lokey == key_thrustup)
  {
    events.thrustUp ();
  }
  else if (hikey == key_thrustdown || lokey == key_thrustdown)
  {
    events.thrustDown ();
  }
  else if (key >= '1' && key <= '9')
  {
    fplayer->recthrust = fplayer->getPrototype ()->maxthrust * (1.0 / 18.0 * (key - '0') + 0.5);
    sound->play (SOUND_CLICK1, false);
  }
  else if (hikey == key_targetnearest || lokey == key_targetnearest)
  {
    events.targetNearest ();
  }
  else if (hikey == key_targetnext || lokey == key_targetnext)
  {
    events.targetNext ();
  }
  else if (hikey == key_targetprevious || lokey == key_targetprevious)
  {
    events.targetPrevious ();
  }
  else if (hikey == key_targetlocking || lokey == key_targetlocking)
  {
    events.targetLocking ();
  }
  else if (key == 'K')
  {
    if (fplayer->target != NULL)
      fplayer->target->shield = -1;
  }
/*  else if (key == 'V')
  {
    visibleangle += 10.0;
    if (visibleangle > 120.0) visibleangle = 50.0;
  }*/
  else if (hikey == key_firecannon || lokey == key_firecannon)
  {
    fplayer->autofire = false;
  }

  switch (key - 256) // standard special keys
  {
    case KEY_LSHIFT:
      keyb_lshift = 1;
      break;
    case KEY_UP:
      keyb_elev = -1;
      break;
    case KEY_DOWN:
      keyb_elev = 1;
      break;
    case KEY_LEFT:
      if (keyb_lshift) keyb_rudder = -1;
      else keyb_roll = 1;
      break;
    case KEY_RIGHT:
      if (keyb_lshift) keyb_rudder = 1;
      else keyb_roll = -1;
      break;
    case KEY_F1:
      camera = 0;
      stateplay.reshape ();
      break;
    case KEY_F2:
      camera = 1;
      stateplay.reshape ();
      break;
    case KEY_F3:
      camera = 2;
      stateplay.reshape ();
      break;
    case KEY_F4:
      camera = 6;
      stateplay.reshape ();
      break;
    case KEY_F5:
      camera = 7;
      stateplay.reshape ();
      break;
    case KEY_F6:
      camera = 9;
      stateplay.reshape ();
      break;
    case KEY_F7:
      camera = 8;
      stateplay.reshape ();
      break;
    case KEY_F8:
      camera = 5;
      stateplay.reshape ();
      break;
    default:
      return;
  }

  sdldisplay = true;
}

void StatePlay::keyUp (int key, int x, int y)
{
  unsigned int hikey = (unsigned int) key;
  unsigned int lokey = (unsigned int) key;

  if (hikey == key_firecannon || lokey == key_firecannon)
  {
    events.stopCannon ();
  }

  switch (key - 256) // standard special keys
  {
    case KEY_LSHIFT:
      keyb_lshift = 0;
      break;
    case KEY_UP:
      keyb_elev = 0;
      break;
    case KEY_DOWN:
      keyb_elev = 0;
      break;
    case KEY_LEFT:
      keyb_roll = 0;
      keyb_rudder = 0;
      break;
    case KEY_RIGHT:
      keyb_roll = 0;
      keyb_rudder = 0;
      break;
    case KEY_LALT:
      keyb_rudder = 0;
      break;
    case KEY_LCTRL:
      keyb_rudder = 0;
      break;
  }

    sdldisplay = true;
}

void StatePlay::mouse (int button, int state, int x, int y)
{
  if (state == MOUSE_DOWN && fplayer->active)
  {
    if (button == mouse_firecannon)
    {
      fplayer->autofire = !fplayer->autofire;
    }
    else if (button == mouse_firemissile)
    {
      events.fireMissile ();
    }
    else if (button == mouse_selectmissile)
    {
      events.selectMissile ();
    }
  }
}

void StatePlay::mouseRelMotion (int xrel, int yrel)
{
  if (controls != CONTROLS_MOUSE || !mouse_relative) return;
  float xr = (float) xrel / width, yr = (float) yrel / height;

  float roll = (float) -xr * 20;

  if (roll > 1.0F) roll = 1.0F;
  else if (roll < -1.0F) roll = -1.0F;
  fplayer->rolleffect = roll;
  if (roll < 1E-3)
    fplayer->ruddereffect = (float) xr * 200;
  else
    fplayer->ruddereffect = 0.0f;
  if (fplayer->ruddereffect > 1.0) fplayer->ruddereffect = 1.0;
  else if (fplayer->ruddereffect < -1.0) fplayer->ruddereffect = -1.0;

  fplayer->elevatoreffect = (float) yr * fabs (yr) * 20000;
  if (!mouse_reverse) fplayer->elevatoreffect *= -1;
  if (fplayer->elevatoreffect > 1.0f) fplayer->elevatoreffect = 1.0f;
  else if (fplayer->elevatoreffect < -0.5f) fplayer->elevatoreffect = -0.5f;
}

void StatePlay::mouseMotion (int x, int y)
{
  if (controls != CONTROLS_MOUSE || mouse_relative) return;

  float mx = width / 2, my = height / 2;
  float dx = x - mx, dy = my - y;
  dx *= mouse_sensitivity / 70.0F;
  dy *= mouse_sensitivity / 70.0F;
  if (mouse_reverse)
    dy *= -1;

// mouse interface code added by Lourens Veen
  float nx = dx / width; // normalised x-coordinate, -1 at lefthand
                         // side of the screen, 1 at righthand side
  if (mouse_autorudder)
  {
    if (fabs(nx) < (1.0f/3.0f))
    if (nx < 0.0f)
      fplayer->ruddereffect = 729.0f*nx*(nx+1.0f/3.0f)*(nx+1.0f/3.0f)/4.0f;
    else
      fplayer->ruddereffect = 729.0f*nx*(nx-1.0f/3.0f)*(nx-1.0f/3.0f)/4.0f;
    else
      fplayer->ruddereffect = 0.0f;
  }
  else
  {
    fplayer->ruddereffect = 0;
  }

  const float roll_deadarea = (float) mouse_autorudder / 1000.0F; //(1.0f/30.0f);

  if (fabs(nx) > roll_deadarea)
  {
    if (nx > 0.0f) nx -= roll_deadarea;
    else nx += roll_deadarea;

    if (nx > 0)
      fplayer->rolleffect = -(exp(log(nx) * 1.3f)) * 3.0f;
    else
      fplayer->rolleffect = (exp(log(-nx) * 1.3f)) * 3.0f;

    if (fplayer->rolleffect < -1.0F) fplayer->rolleffect = -1.0F;
    if (fplayer->rolleffect > 1.0F) fplayer->rolleffect = 1.0F;
  }
  else
    fplayer->rolleffect = 0.0f;

  fplayer->elevatoreffect = dy / height * 2.5;
  if (fplayer->elevatoreffect > 1.0f) fplayer->elevatoreffect = 1.0f;
  else if (fplayer->elevatoreffect < -0.5f) fplayer->elevatoreffect = -0.5f;

  sdldisplay = true;
}

void StatePlay::joystickAxis (int x, int y, int rudder, int throttle)
{
  if (fplayer->ai) return;
  float xx = x / 32768.0;
  xx *= fabs (xx);
  fplayer->rolleffect = (float) -xx;
  fplayer->elevatoreffect = (float) y / 30000;
  if (fplayer->elevatoreffect > 1.0) fplayer->elevatoreffect = 1.0;
  else if (fplayer->elevatoreffect < -1.0) fplayer->elevatoreffect = -1.0;
  if (fplayer->elevatoreffect < -0.5) fplayer->elevatoreffect = -0.5;
  fplayer->ruddereffect = (float) rudder / 30000;
  if (fplayer->ruddereffect > 1.0) fplayer->ruddereffect = 1.0;
  else if (fplayer->ruddereffect < -1.0) fplayer->ruddereffect = -1.0;
  fplayer->recthrust = fplayer->getPrototype ()->maxthrust * 0.75 - fplayer->getPrototype ()->maxthrust / 4 * throttle / 32638;
}

void StatePlay::joystickButton (int button)
{
  if (!fplayer->active) return;
  if (button == joystick_firecannon)
  {
    fplayer->fireCannon (laser);
#ifdef HAVE_SDL_MIXER
    if (!startcannon)
    {
      sound->playLoop (SOUND_CANNON1);
      startcannon = true;
    }
#else
    sound->play (SOUND_CANNON1, 1);
#endif
  }
  if (button == joystick_firemissile)
  {
    events.fireMissile ();
  }
  if (button == joystick_dropflare)
  {
    events.fireFlare ();
  }
  if (button == joystick_dropchaff)
  {
    events.fireChaff ();
  }
  if (button == joystick_selectmissile)
  {
    events.selectMissile ();
  }
  if (button == joystick_targetnearest)
  {
    events.targetNearest ();
  }
  if (button == joystick_targetlocking)
  {
    events.targetLocking ();
  }
  if (button == joystick_targetnext)
  {
    events.targetNext ();
  }
  if (button == joystick_targetprevious)
  {
    events.targetPrevious ();
  }
  if (button == joystick_thrustup)
  {
    events.thrustUp ();
  }
  if (button == joystick_thrustdown)
  {
    events.thrustDown ();
  }
}

void StatePlay::joystickHat (int normhat)
{
  gamestate->joystickButton (normhat);
}

void StatePlay::view ()
{
  frame ();
  gl.swapBuffers ();
}

void StateMenu::key (int key, int x, int y)
{
  if (key == 27)
  {
    if (missionactive)
    {
      switch_game ();
    }
    else
    {
      switch_quit ();
    }
  }
}

void StateStats::key (int key, int x, int y)
{
  if (key == 27)
  {
    pleaseWait ();
    DISPLAY_INFO("Initing new mission");
    missionactive = false;
    createMission (MISSION_DEMO);
    game_levelInit ();
    sound->haltMusic ();
    switch_menu ();
  }
}

void StateCredits::key (int key, int x, int y)
{
  if (key)
  {
//    sound->haltMusic ();
    switch_menu ();
  }
}

void StateFinish::key (int key, int x, int y)
{
  if (key)
  {
//    sound->haltMusic ();
    switch_menu ();
  }
}

void StateMission::key (int key, int x, int y)
{
  if (key == 27)
  {
    switch_menu ();
  }
  else if (key == 13)
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
}

void StateFame::key (int key, int x, int y)
{
  if (key == 27)
  {
    switch_menu ();
  }
}

void StateCreate::key (int key, int x, int y)
{
  if (key == 's')
  {
//    server->sendMessage (0, "s", 1);
    if (server->checkStart())
    {
      printf ("Starting..");
      createMission (MISSION_MULTIPLAYER_DOGFIGHT);
      game_levelInit ();
      switch_game ();
      missionactive = true;
    }
  }
  if (key == 27)
  {
    switch_menu ();
  }
}

void StateJoin::key (int key, int x, int y)
{
  if (key == 's')
  {
    createMission (MISSION_MULTIPLAYER_DOGFIGHT);
    game_levelInit ();
    switch_game ();
    missionactive = true;
  }
  if (key == 27)
  {
    switch_menu ();
  }
}

void StateFighter::key (int key, int x, int y)
{
  if (key == 27)
  {
    switch_menu ();
  }
}

void StateMission::mouse (int button, int state, int x, int y)
{
  float rx = (float) x / width;
  float ry = (float) y / height;
  missionmenuitemselected = -1;

  missionmenufighterselected = -1;
  if (ry >= 0.53 && ry <= 0.65)
  {
    if (rx >= 0.18 && rx < 0.28)
    {
      missionmenufighterselected = 0;
      if (state == MOUSE_DOWN)
        missionnew->wantfighter = 0;
    }
    if (rx >= 0.28 && rx < 0.38 && missionnew->selfighters >= 2)
    {
      missionmenufighterselected = 1;
      if (state == MOUSE_DOWN)
        missionnew->wantfighter = 1;
    }
    if (rx >= 0.38 && rx < 0.48 && missionnew->selfighters >= 3)
    {
      missionmenufighterselected = 2;
      if (state == MOUSE_DOWN)
        missionnew->wantfighter = 2;
    }
  }

  missionmenuweaponselected = -1;
  if (ry >= 0.53 && ry <= 0.65)
  {
    if (rx >= 0.54 && rx < 0.64)
    {
      missionmenuweaponselected = 0;
      if (state == MOUSE_DOWN)
        missionnew->wantweapon = 0;
    }
    if (rx >= 0.64 && rx < 0.74 && missionnew->selweapons >= 2)
    {
      missionmenuweaponselected = 1;
      if (state == MOUSE_DOWN)
        missionnew->wantweapon = 1;
    }
    if (rx >= 0.74 && rx < 0.84 && missionnew->selweapons >= 3)
    {
      missionmenuweaponselected = 2;
      if (state == MOUSE_DOWN)
        missionnew->wantweapon = 2;
    }
  }
}

void StateMission::display ()
{
  allmenus.setVisible (false);
  missionmenu.setVisible (true);
  missionmenu.draw ();

  char buf [256];
  int i;
  float piloty = 5;
  Pilot *p = pilots->pilot [pilots->aktpilot];
  Color *colorstd = &colorred;
  if (p->mission_state [missionnew->id] == 1)
    colorstd = &colorblue;
  Color *col = &StandardTextColor;
  Font *font2 = font1;

  Vector3 vec;
  Vector3 tl (-4.5, 5.5, -8.0);
  Rotation rot;
  rot.gamma = 270;
  rot.theta = (270 - missionmenutimer * 4 / timestep) % 360;
  rot.phi = 90;

  // Draw dummy missile
/*  glEnable (GL_LIGHTING);
  Model3dRealizer mr;
  mr.draw (*Model3dRegistry::get (AamHs1Descriptor.name), Transformation(tl, rot, Vector3(0.05)), 1.0, 0);
  glDisable (GL_LIGHTING); */

  glEnable (GL_LIGHTING);
  glEnable (GL_DEPTH_TEST);

  tl.x = -0.24; tl.y = -0.075; tl.z = -0.5;
  rot.gamma = 300;
  rot.theta = 0;
  for (i = 0; i < missionnew->selfighters; i ++)
  {
    tl.x = -0.24 + (float) i * 0.085;
    if (missionnew->wantfighter == i)
      rot.phi = (5 + missionmenutimer * 4 / timestep) % 360;
    else
      rot.phi = 5;
    Model3dRealizer mr;
    mr.draw (*Model3dRegistry::get (missionnew->selfighter [i].name), Transformation (tl, rot, Vector3(0.04)), 0.1, 0);
  }

  tl.x = 0; tl.y = -0.075; tl.z = -0.5;
  rot.gamma = 300;
  rot.theta = 0;
  for (i = 0; i < missionnew->selweapons; i ++)
  {
    tl.x = 0.08 + (float) i * 0.07;
    if (missionnew->wantweapon == i)
      rot.phi = (5 + missionmenutimer * 4 / timestep) % 360;
    else
      rot.phi = 5;
    Model3dRealizer mr;
    mr.draw (*Model3dRegistry::get (missionnew->selweapon [i].name), Transformation (tl, rot, Vector3(0.04)), 0.1, 0);
  }
  glDisable (GL_DEPTH_TEST);
  glDisable (GL_LIGHTING);


  float textx = -12, texty = 9.8;
  font1->zoom = 0.105;
  font2->zoom = 0.105;
  float fontscale = 1.05;
  font1->drawTextCentered (0, texty / fontscale, -2, missionnew->name, *col);
  texty -= 1.1;
  font1->zoom = 0.05;
  font2->zoom = 0.05;
  fontscale = 0.5;
  font1->drawText (textx / fontscale, texty / fontscale, -2, "BRIEFING:", *col);
  texty -= 1;
  font1->drawText (textx / fontscale, texty / fontscale, -2, missionnew->briefing, *col);

  float xstats = 0.8, ystats = 5;
  float xstatstab = 5;
  font1->drawText (xstats / fontscale, ystats / fontscale, -2, "STATUS:", *col);
  if (p->mission_state [missionnew->id] == 1)
    font1->drawText (xstatstab / fontscale, ystats / fontscale, -2, "SUCCESS", *colorstd);
  else if (p->mission_state [missionnew->id] == 2)
    font1->drawText (xstatstab / fontscale, ystats / fontscale, -2, "FAILED", *colorstd);
  else
    font1->drawText (xstatstab / fontscale, ystats / fontscale, -2, "EMPTY", *colorstd);
  if (missionnew->id >= MISSION_CAMPAIGN1 && missionnew->id <= MISSION_CAMPAIGN2)
  {
    texty = ystats - 0.7;
    font1->drawText (xstats / fontscale, texty / fontscale, -2, "SCORE:", *col);
    int score = p->mission_score [missionnew->id];
    if (score < -10000 || score > 100000) score = 0;
    sprintf (buf, "%d", score);
    font1->drawText (xstatstab / fontscale, texty / fontscale, -2, buf, *colorstd);
    texty -= 0.7;
    font1->drawText (xstats / fontscale, texty / fontscale, -2, "KILLS:", *col);
    sprintf (buf, "%d AIRCRAFTS", p->mission_fighterkills [missionnew->id]);
    font1->drawText (xstatstab / fontscale, texty / fontscale, -2, buf, *colorstd);
//    drawMedal (xstatstab + 2, ystats - 3, -2, getMedal (p->mission_score [missionnew->id]), 2, mission->id);
  }
  else
  {
    texty = ystats - 0.7;
    font1->drawText (xstats / fontscale, texty / fontscale, -2, "SCORE:", *col);
    font1->drawText (xstatstab / fontscale, texty / fontscale, -2, "TRAINING", *col);
  }

  font1->drawText (textx / fontscale, piloty / fontscale, -2, "PILOTS:", *col);
  strcpy (buf, pilots->pilot [pilots->aktpilot]->getShortRank (MISSION_CAMPAIGN1, MISSION_CAMPAIGN2 - 1).c_str ());
  strcat (buf, " ");
  strcat (buf, pilots->pilot [pilots->aktpilot]->name.c_str ());
  font2->drawText ((textx + 1.5) / fontscale, (piloty - 0.8) / fontscale, -2, buf, *col);
  drawRank (textx, piloty - 0.8, -2, pilots->pilot [pilots->aktpilot]->ranking, 0.5);
  for (i = 1; i < missionnew->alliedfighters; i ++)
    if (missionnew->alliedpilot [i - 1] >= 0 && missionnew->alliedpilot [i - 1] < 100)
    {
      drawRank (textx, piloty - 1 - 0.6 * i, -2, pilots->pilot [pilots->aktpilot]->tp [missionnew->alliedpilot [i - 1]]->ranking, 0.5);
      font2->drawText ((textx + 1.5) / fontscale, (piloty - 1 - 0.6 * i) / fontscale, -2, pilots->pilot [pilots->aktpilot]->tp [missionnew->alliedpilot [i - 1]]->getShortName (), *col);
    }

  font1->drawText (textx / fontscale, -1 / fontscale, -2, "CHOOSE FIGHTER:", *col);
  font1->drawText (xstats / fontscale, -1 / fontscale, -2, "CHOOSE WEAPON PACK:", *col);
  font2->drawText (textx / fontscale, -6 / fontscale, -2, missionnew->selfighter [missionnew->wantfighter].displayedName, *col);
  font2->drawText (xstats / fontscale, -6 / fontscale, -2, missionnew->selweapon [missionnew->wantweapon].displayedName, *col);
  font1->zoom = 0.1;
  font2->zoom = 0.1;

  drawMouseCursor ();
}

void StateFame::mouse (int button, int state, int x, int y)
{
  missionmenuitemselected = -1;
}

void StateCreate::mouse (int button, int state, int x, int y)
{
  float rx = (float) x / width;
  float ry = (float) y / height;
  missionmenuitemselected = -1;

  if (ry >= 0.8 && ry <= 0.9)
  {
    if (rx >= 0.4 && rx <= 0.6)
    {
      missionmenuitemselected = 0;
      if (state == MOUSE_DOWN)
      {
        key (27, 0, 0);
      }
    }
  }
}

void StateCreate::display ()
{
  Color *colorstd = &colorblue;

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  glPushMatrix ();

  drawQuads (colorstd);

#ifdef HAVE_SDL_NET
  float my = 0;
  int i;
  for (i = 0; i < server->num_clients; i ++)
  {
    font1->drawTextCentered (0, my, -2.5, server->clients [i].name);
    my -= 2;
  }
#endif

  font1->drawTextCentered (0, 9, -1.5, "CREATE GAME");

  if (missionmenuitemselected == 0)
    font1->drawTextScaled (-2, -12, -2, "BACK", *colorstd, -missionmenutimer * 5);
  else
    font1->drawText (-2, -12, -2, "BACK");

  glPopMatrix ();

  adjustBrightness ();

  drawMouseCursor ();
}

void StateJoin::mouse (int button, int state, int x, int y)
{
  float rx = (float) x / width;
  float ry = (float) y / height;
  missionmenuitemselected = -1;

  if (ry >= 0.8 && ry <= 0.9)
  {
    if (rx >= 0.4 && rx <= 0.6)
    {
      missionmenuitemselected = 0;
      if (state == MOUSE_DOWN)
      {
        key (27, 0, 0);
      }
    }
  }
}

void StateJoin::display ()
{
  Color *colorstd = &colorblue;

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  glPushMatrix ();

  drawQuads (colorstd);

  font1->drawTextCentered (0, 9, -1.5, "JOIN GAME");

  if (missionmenuitemselected == 0)
    font1->drawTextScaled (-2, -12, -2, "BACK", *colorstd, -missionmenutimer * 5);
  else
    font1->drawText (-2, -12, -2, "BACK");

  glPopMatrix ();

  adjustBrightness ();

  drawMouseCursor ();
}

void StateFighter::mouse (int button, int state, int x, int y)
{
}

void StateFighter::display ()
{
  allmenus.setVisible (false);
  fightermenu.setVisible (true);
  fightermenu.draw ();
  AiObj ffighter (FalconDescriptor);

  char buf [256];
  int i;

  Vector3 vec;
  Vector3 tl;
  tl.y = -0.04;
  tl.z = -0.4;
  Rotation rot;
  rot.gamma = 300;
  rot.theta = 0;
  rot.phi = (5 + missionmenutimer * 4 / timestep) % 360;
  Model3d *model = NULL;
  UnitDescriptor id;
  if (aktfighter == 0) { id = FalconDescriptor; }
  else if (aktfighter == 1) { id = CrowDescriptor; }
  else if (aktfighter == 2) { id = HawkDescriptor; }
  else if (aktfighter == 3) { id = StormDescriptor; }
  else if (aktfighter == 4) { id = SwallowDescriptor; }
  else if (aktfighter == 5) { id = BuzzardDescriptor; }
  else if (aktfighter == 6) { id = Hawk2Descriptor; }
  else if (aktfighter == 7) { id = RedArrowDescriptor; }
  else if (aktfighter == 8) { id = PhoenixDescriptor; }
  else if (aktfighter == 9) { id = BlackBirdDescriptor; }
  model = Model3dRegistry::get (id.name);

  glEnable (GL_DEPTH_TEST);
  glEnable (GL_LIGHTING);
  Model3dRealizer mr;
  mr.draw (*model, Transformation (tl, rot, Vector3(0.11)), 0.5, 0);
//  model->draw (vec, tl, rot, 0.11, 0.5, 0);
  glDisable (GL_LIGHTING);
  glDisable (GL_DEPTH_TEST);

  ffighter.o = model;
  ffighter.newinit (id, 1, 0);

  Color *col = &StandardTextColor;
  float fontzoom = 0.7;
  float textx = -9.5;
  font1->zoom = 0.07;
  font1->drawText (textx / fontzoom, 9.7 / fontzoom, -2, id.displayedName, *col);
  float yf = 9.6 - 1.35;
  strcpy (buf, "TYPE: ");
  if (ffighter.id == FalconDescriptor || ffighter.id == CrowDescriptor || ffighter.id == BuzzardDescriptor || ffighter.id == RedArrowDescriptor || ffighter.id == BlackBirdDescriptor)
    strcat (buf, "FIGHTER");
  else if (ffighter.id == HawkDescriptor || ffighter.id == Hawk2Descriptor)
    strcat (buf, "FIGHTER-BOMBER");
  else
    strcat (buf, "BOMBER");
  font1->drawText (textx / fontzoom, yf / fontzoom, -2, buf, *col);
  yf -= 1;
  strcpy (buf, "SPEED: ");
  int stars = (int) ((ffighter.getPrototype ()->maxthrust - 0.2) * 40);
  font1->drawText (textx / fontzoom, yf / fontzoom, -2, buf, *col);
  for (i = 0; i < stars; i ++)
    drawMedal (4 + i * 1.1, yf + 0.7, -2, 0, 1, MISSION_CAMPAIGN1);
  yf -= 1;
  strcpy (buf, "getPrototype ()->nimbility: ");
  stars = (int) ((ffighter.getPrototype ()->manoeverability - 0.3) * 20 + 1);
  font1->drawText (textx / fontzoom, yf / fontzoom, -2, buf, *col);
  for (i = 0; i < stars; i ++)
    drawMedal (4 + i * 1.1, yf + 0.7, -2, 0, 1, MISSION_CAMPAIGN1);
  yf -= 1;
  strcpy (buf, "SHIELD: ");
  stars = (int) ((ffighter.getPrototype ()->maxshield - 30) / 30);
  font1->drawText (textx / fontzoom, yf / fontzoom, -2, buf, *col);
  for (i = 0; i < stars; i ++)
    drawMedal (4 + i * 1.1, yf + 0.7, -2, 0, 1, MISSION_CAMPAIGN1);
  yf -= 1;
  strcpy (buf, "FIREPOWER: ");
  stars = ffighter.statfirepower;
  font1->drawText (textx / fontzoom, yf / fontzoom, -2, buf, *col);
  for (i = 0; i < stars; i ++)
    drawMedal (4 + i * 1.1, yf + 0.7, -2, 0, 1, MISSION_CAMPAIGN1);
  font1->zoom = 0.1;

  drawMouseCursor ();
}

void StateFame::display ()
{
  allmenus.setVisible (false);
  famemenu.setVisible (true);
  famemenu.draw ();

  char buf [256];
  int i, i2;
  Pilot *p = pilots->pilot [pilots->aktpilot];

  Color *col = &StandardTextColor;
  float textx = -10;
  int sum = 0;
  for (i = MISSION_CAMPAIGN1; i < MISSION_CAMPAIGN2; i ++)
    sum += p->mission_fighterkills [i];
  p->tp [10]->fighterkills = sum;
  p->tp [10]->name = p->name;
  p->tp [10]->ranking = p->ranking;
  int index [11];
  for (i = 0; i < 11; i ++)
    index [i] = i;
  for (i = 0; i < 11; i ++)
    for (i2 = 0; i2 < i; i2 ++)
    {
      if (p->tp [index [i]]->fighterkills < p->tp [index [i2]]->fighterkills)
      {
        int temp = index [i];
        index [i] = index [i2];
        index [i2] = temp;
      }
    }

  for (i = 0; i < 11; i ++)
  {
    font1->zoom = 0.07;
    drawRank (textx + 0.2, i - 3.7, -2, p->tp [index [i]]->ranking, 0.7);
    sprintf (buf, "%s %s", p->tp [index [i]]->getRank ().c_str (), p->tp [index [i]]->name.c_str ());
    font1->drawText ((textx + 2) / 0.7, (i - 3.7) / 0.7, -2, buf, *col);
    sprintf (buf, "%d", p->tp [index [i]]->fighterkills);
    font1->drawText ((textx + 18) / 0.7, (i - 3.7) / 0.7, -2, buf, *col);
    font1->zoom = 0.1;
  }

  drawMouseCursor ();
}

void StateQuit::key (int key, int x, int y)
{
  if (key == 'y' || key == 'Y')
    game_quit ();
  else
    switch_menu ();
}

void StateQuit::mouse (int button, int state, int x, int y)
{
  float rx = (float) x / width;
  float ry = (float) y / height;
  missionmenuitemselected = -1;

  if (ry >= 0.5 && ry <= 0.55)
  {
    if (rx >= 0.35 && rx <= 0.45)
    {
      missionmenuitemselected = 0;
      if (state == MOUSE_DOWN)
      {
        key ('y', 0, 0);
      }
    }
    if (rx >= 0.55 && rx <= 0.65)
    {
      missionmenuitemselected = 1;
      if (state == MOUSE_DOWN)
      {
        key (27, 0, 0);
      }
    }
  }
}

void StateMenu::mouse (int button, int state, int x, int y)
{
}

void StateStats::mouse (int button, int state, int x, int y)
{
}

void StateStats::display ()
{
  allmenus.setVisible (false);
  statsmenu.setVisible (true);
  statsmenu.draw ();

  float yf = 1.4, zf = -2.5;

  drawMedal (-5.5, 5.8, -1.5, getMedal (fplayer->score), 1.6, mission->id);
  drawMedal (5.5, 5.8, -1.5, getMedal (fplayer->score), 1.6, mission->id);

  char buf [100];
  float xf1 = -12, xf2 = 0, xf3 = 8;
  float linedist = 1.3, linedist2 = 1.1;
  yf = 6.5; zf = -2.5;
  Color *color;
  Pilot *p = pilots->pilot [pilots->aktpilot];

  color = &StandardTextColor;
  if (missionstate == 1)
  {
    font1->drawTextCentered (0, 7, -2, "SUCCESS", colorblue);
  }
  else
  {
    font1->drawTextCentered (0, 7, -2, "FAILED", colorred);
  }

  font1->drawText (xf1, yf, zf, "SCORE:", *color);
  sprintf (buf, "%d", fplayer->score);
  font1->drawText (xf3, yf, zf, buf, *color);
  yf -= linedist * 1.2;

  font1->drawText (xf1, yf, zf, "DIFFICULTY:", *color);
  if (mission->difficulty == 0) font1->drawText (xf2, yf, zf, "EASY", *color);
  else if (mission->difficulty == 1) font1->drawText (xf2, yf, zf, "NORMAL", *color);
  else if (mission->difficulty == 2) font1->drawText (xf2, yf, zf, "HARD", *color);
  if (mission->difficulty == 0) font1->drawText (xf3, yf, zf, "-25", *color);
  else if (mission->difficulty == 1) font1->drawText (xf3, yf, zf, "+25", *color);
  else if (mission->difficulty == 2) font1->drawText (xf3, yf, zf, "+75", *color);
  yf -= linedist;

  font1->drawText (xf1, yf, zf, "FLIGHT MODEL:", *color);
  if (!fplayer->realism) font1->drawText (xf2, yf, zf, "ACTION", *color);
  else font1->drawText (xf2, yf, zf, "SIM", *color);
  if (!fplayer->realism) font1->drawText (xf3, yf, zf, "-25", *color);
  else font1->drawText (xf3, yf, zf, "+25", *color);

  int timebonus = 0;
  if (mission->timer < mission->maxtime)
    timebonus = (mission->maxtime - mission->timer) * 100 / mission->maxtime;
  yf -= linedist;
  font1->drawText (xf1, yf, zf, "TIME BONUS:", *color);
  sprintf (buf, "%d%%", timebonus);
  font1->drawText (xf2, yf, zf, buf, *color);
  sprintf (buf, "+%d", timebonus);
  font1->drawText (xf3, yf, zf, buf, *color);
  int shieldbonus = (int) (fplayer->shield * 100 / fplayer->getPrototype ()->maxshield);
  yf -= linedist;
  font1->drawText (xf1, yf, zf, "SHIELD BONUS:", *color);
  sprintf (buf, "%d%%", shieldbonus);
  font1->drawText (xf2, yf, zf, buf, *color);
  sprintf (buf, "+%d", shieldbonus);
  font1->drawText (xf3, yf, zf, buf, *color);
  yf -= linedist;
  font1->drawText (xf1, yf, zf, "KILLS:", *color);
  if (fplayer->stat.fighterkills > 0)
  {
    sprintf (buf, "%d AIR", fplayer->stat.fighterkills);
    font1->drawText (xf2, yf, zf, buf, *color);
    sprintf (buf, "+%d", fplayer->stat.fighterkills * 20);
    font1->drawText (xf3, yf, zf, buf, *color);
    yf -= linedist2;
  }
  if (fplayer->stat.tankkills > 0)
  {
    sprintf (buf, "%d TANKS", fplayer->stat.tankkills);
    font1->drawText (xf2, yf, zf, buf, *color);
    sprintf (buf, "+%d", fplayer->stat.tankkills * 10);
    font1->drawText (xf3, yf, zf, buf, *color);
    yf -= linedist2;
  }
  if (fplayer->stat.shipkills > 0)
  {
    sprintf (buf, "%d SHIPS", fplayer->stat.shipkills);
    font1->drawText (xf2, yf, zf, buf, *color);
    sprintf (buf, "+%d", fplayer->stat.shipkills * 12);
    font1->drawText (xf3, yf, zf, buf, *color);
    yf -= linedist2;
  }
  if (fplayer->stat.otherkills > 0)
  {
    sprintf (buf, "%d OTHERS", fplayer->stat.otherkills);
    font1->drawText (xf2, yf, zf, buf, *color);
    sprintf (buf, "+%d", fplayer->stat.otherkills * 5);
    font1->drawText (xf3, yf, zf, buf, *color);
    yf -= linedist2;
  }
  yf -= (linedist - linedist2);

  if (ispromoted)
  {
    yf = -6;
    font1->drawTextCentered (0, yf, zf, "PROMOTED TO", *color);
    yf -= 1.5;
    sprintf (buf, "%s", p->getRank (MISSION_CAMPAIGN1, MISSION_CAMPAIGN2 - 1).c_str ());
    font1->drawTextCentered (0, yf, zf, buf, *color);
    drawRank (-11, yf + 0.2, zf, pilots->pilot [pilots->aktpilot]->ranking, 2);
    drawRank (7, yf + 0.2, zf, pilots->pilot [pilots->aktpilot]->ranking, 2);
  }
  yf = -9;

  drawMouseCursor ();
}

void StateMenu::display ()
{
  int i;

  glInitNames ();
  glPushName (0);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  int menutimernorm = menutimer * 5 / timestep;
  if (menutimernorm != 0) menutimernorm %= 360;
  if (menutimernorm < 0) menutimernorm *= -1;
  Color color2 (255, 255, (int) (255.0 * COS(menutimernorm)), 255);

  Pilot *p = pilots->pilot [pilots->aktpilot];

  if (allmenus.components [3]->isVisible ())
  {
    ((Container *) allmenus.components [3])->components [0]->setVisible (true);
    for (i = 1; i < 25; i ++)
    {
      if (p->mission_state [getCampaignIdFromValue (i - 1)] == 1)
        ((Container *) allmenus.components [3])->components [i]->setVisible (true);
      else
        ((Container *) allmenus.components [3])->components [i]->setVisible (false);
    }
  }

  allmenus.draw ();

  if (allmenus.components [1]->isVisible ())
  {
    float textx2 = -0.7, yf = 10.15, zf = -2, ydiff = 1;
    drawRank (textx2, yf, zf, pilots->pilot [pilots->aktpilot]->ranking, 0.8);
    yf -= 2;
    for (i = 0; i < pilots->aktpilots; i ++)
    {
      drawRank (textx2, yf, zf, pilots->pilot [i]->ranking, 0.8);
      yf -= ydiff;
    }
  }

  if (allmenus.components [3]->isVisible ())
  {
    float textx2 = 11, yf = 9.05, zf = -2, ydiff = 0.8;
    for (i = 0; i < 24; i ++)
    {
      drawMedal (textx2 - 0.8, yf + 0.6, zf, getMedal (p->mission_score [getCampaignIdFromValue (i)]), 0.8, getCampaignIdFromValue (i));
      yf -= ydiff;
    }
  }

  drawMouseCursor ();
}

void StatePause::display ()
{
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  glPushMatrix ();

  font1->drawText (-3, -1, -1, "PAUSED", colorwhite);

  glPopMatrix ();
}

void StateCredits::mouse (int button, int state, int x, int y)
{
  switch_menu ();
}

void StateCredits::display ()
{
#ifdef SHOW_SOFTWARE_PATENTS
  float xt = -22, yt = 12, zf = -2.4, ydist = 0.7;
  glPushMatrix ();
//  glTranslatef (0, -3.5 + 0.014 * (float) creditstimer / timestep, 0);
  glTranslatef (0, -3.4 + 0.004 * (float) creditstimer / timestep, 0);
  Color *col = &colorwhite;
  Color *col2 = &coloryellow;
  float fontzoom = 1.0;
  font1->zoom = 0.1;
  font2->zoom = 0.1;
  font1->drawTextCentered (0, (yt -= 2 * ydist) / fontzoom, zf, "MONEY VERSUS DEMOCRACY", *col2);
  font1->drawTextCentered (0, (yt -= 3 * ydist) / fontzoom, zf, "PATENTS ON SOFTWARE", *col2);
  fontzoom = 0.8;
  font1->zoom = 0.08;
  font2->zoom = 0.08;

  font1->drawText (xt, (yt -= 5 * ydist) / fontzoom, zf, "WHAT ARE PATENTS?", *col2);
  font2->drawText (xt, (yt -= 2 * ydist) / fontzoom, zf, "PATENTS IN EUROPE COST ABOUT 30000-50000 EURO (= US$).", *col);
  font2->drawText (xt, (yt -= 1.5 * ydist) / fontzoom, zf, "THEY GRANT PROPERTY FOR SOMETHING, THAT NOONE CAN PUBLISH", *col);
  font2->drawText (xt, (yt -= 1.5 * ydist) / fontzoom, zf, "EXCEPT THE PATENT OWNER.", *col);

  font1->drawText (xt, (yt -= 4 * ydist) / fontzoom, zf, "WHO CAN AFFORD PATENTS?", *col2);
  font2->drawText (xt, (yt -= 2 * ydist) / fontzoom, zf, "ONLY HUGE COMPANIES.", *col);

  font1->drawText (xt, (yt -= 4 * ydist) / fontzoom, zf, "WHAT CAN BE PATENTED?", *col2);
  font2->drawText (xt, (yt -= 2 * ydist) / fontzoom, zf, "CURRENTLY EVERYTHING. AMAZON HAS A PATENT TO SELL PRODUCTS", *col);
  font2->drawText (xt, (yt -= 1.5 * ydist) / fontzoom, zf, "WITH ONE MOUSE CLICK. THERE ARE EUROPEAN PATENTS ON THE", *col);
  font2->drawText (xt, (yt -= 1.5 * ydist) / fontzoom, zf, "PROGRESS BAR, ON THE UNDO FUNCTION, ETC.", *col);
  font2->drawText (xt, (yt -= 1.5 * ydist) / fontzoom, zf, "CURRENTLY EURO SOFTWARE PATENTS ARE STOPPED UNTIL THE END", *col);
  font2->drawText (xt, (yt -= 1.5 * ydist) / fontzoom, zf, "OF SUMMER 2004.", *col);

  font1->drawText (xt, (yt -= 4 * ydist) / fontzoom, zf, "AN EXAMPLE FOR PATENTS ON COURT?", *col2);
  font2->drawText (xt, (yt -= 2 * ydist) / fontzoom, zf, "IN 2001 MICROSOFT HAD TO PAY 500000000 US$, BECAUSE ANOTHER", *col);
  font2->drawText (xt, (yt -= 1.5 * ydist) / fontzoom, zf, "COMPANY HAD A PATENT ON BROWSER PLUGINS (US).", *col);

  font1->drawText (xt, (yt -= 4 * ydist) / fontzoom, zf, "WHAT IS SOFTWARE?", *col2);
  font2->drawText (xt, (yt -= 2 * ydist) / fontzoom, zf, "SOFTWARE IS BASED ON ABSTRACT DESCRIPTIONS, WHICH EVERYONE", *col);
  font2->drawText (xt, (yt -= 1.5 * ydist) / fontzoom, zf, "CAN FORMULATE IN PROSE LANGUAGE. CODE IS JUST A MIRROR", *col);
  font2->drawText (xt, (yt -= 1.5 * ydist) / fontzoom, zf, "IMAGE OF THE DESCRIPTION.", *col);

  font1->drawText (xt, (yt -= 4 * ydist) / fontzoom, zf, "WHY DOES MICROSOFT WANT PATENTS?", *col2);
  font2->drawText (xt, (yt -= 2 * ydist) / fontzoom, zf, "TO PATENT CONCEPTS, AND DICTATE WHICH SYSTEMS MAY USE THEM.", *col);
  font2->drawText (xt, (yt -= 1.5 * ydist) / fontzoom, zf, "EVENTUALLY TO ELIMINATE LINUX AND OPEN SOURCE.", *col);

  font1->drawText (xt, (yt -= 4 * ydist) / fontzoom, zf, "WHY CAN EVERYTHING BE PATENTED?", *col2);
  font2->drawText (xt, (yt -= 2 * ydist) / fontzoom, zf, "HUGE COMPANIES ARE PUSHING FOR EXECUTIVE POWERS.", *col);
  font2->drawText (xt, (yt -= 1.5 * ydist) / fontzoom, zf, "PATENT LAWYERS ARE TAKING THE MONEY.", *col);

  font1->drawText (xt, (yt -= 4 * ydist) / fontzoom, zf, "WHAT IS DEMOCRACY?", *col2);
  font2->drawText (xt, (yt -= 2 * ydist) / fontzoom, zf, "YOUR POWER TO STOP THIS ABUSE.", *col);
  font2->drawText (xt, (yt -= 1.5 * ydist) / fontzoom, zf, "VOTE AGAINST SOFTWARE PATENTS ON HTTP://PETITION.EUROLINUX.ORG.", *col);
  font2->drawText (xt, (yt -= 1.5 * ydist) / fontzoom, zf, "TELL YOUR FRIENDS AND SEARCH THE INTERNET FOR MORE INSANITY.", *col);
#else
  float yt = 12, zf = -2.4, ydist = 0.7;
  glPushMatrix ();
  glTranslatef (0, -3.5 + 0.014 * (float) creditstimer / timestep, 0);
  Color *col = &colorwhite;
  Color *col2 = &coloryellow;
  float fontzoom = 1;
  font2->drawTextCentered (0, (yt -= 2 * ydist) / fontzoom, zf, "GAME PROGRAMMING,", *col);
  font2->drawTextCentered (0, (yt -= 2 * ydist) / fontzoom, zf, "GRAPHICS, MODELS, SOUND & MUSIC", *col);
  font1->drawTextCentered (0, (yt -= 2 * ydist) / fontzoom, zf, "THOMAS A. DREXL", *col2);
  font1->zoom = 0.08;
  font2->zoom = 0.08;
  fontzoom = 0.8;
  font2->drawTextCentered (0, (yt -= 4 * ydist) / fontzoom, zf, "SPECIAL THANKS TO...", *col);
  font2->drawTextCentered (0, (yt -= 4 * ydist) / fontzoom, zf, "LENS FLARES & FURTHER DEBUGGING", *col);
  font1->drawTextCentered (0, (yt -= 2 * ydist) / fontzoom, zf, "PIOTR PAWLOW", *col2);
  font2->drawTextCentered (0, (yt -= 4 * ydist) / fontzoom, zf, "MOUSE INTERFACE & LANDSCAPE IMPROVEMENTS", *col);
  font1->drawTextCentered (0, (yt -= 2 * ydist) / fontzoom, zf, "LOURENS VEEN", *col2);
  font2->drawTextCentered (0, (yt -= 4 * ydist) / fontzoom, zf, "PUBLISHING & FURTHER GAME IDEAS", *col);
  font1->drawTextCentered (0, (yt -= 2 * ydist) / fontzoom, zf, "BERNHARD KAINDL", *col2);
  font2->drawTextCentered (0, (yt -= 4 * ydist) / fontzoom, zf, "MOON TERRAIN", *col);
  font1->drawTextCentered (0, (yt -= 2 * ydist) / fontzoom, zf, "NORBERT DREXL", *col2);
  font2->drawTextCentered (0, (yt -= 4 * ydist) / fontzoom, zf, "PHYSICAL MODEL (ACTION) & COCKPIT IMPROVEMENTS", *col);
  font1->drawTextCentered (0, (yt -= 2 * ydist) / fontzoom, zf, "ARNE REINERS", *col2);
  font2->drawTextCentered (0, (yt -= 4 * ydist) / fontzoom, zf, "...AND THANKS TO ALL PEOPLE GIVING FEEDBACK AND ADVICE", *col);
  float xf = -10;
  font1->zoom = 0.1;
  font2->zoom = 0.1;
  fontzoom = 1;
  font1->drawTextCentered (0, (yt -= 10 * ydist) / fontzoom, zf, "***********************", *col);
  font1->drawTextCentered (0, (yt -= 10 * ydist) / fontzoom, zf, "DO YOU WANT TO CONTRIBUTE?", *col);
  font1->zoom = 0.08;
  font2->zoom = 0.08;
  fontzoom = 0.8;
  font2->drawText (xf / fontzoom, (yt -= 4 * ydist) / fontzoom, zf, "LOTS OF THINGS ARE STILL NEEDED:", *col);
  font2->drawText (xf / fontzoom, (yt -= 2 * ydist) / fontzoom, zf, "- DRAW TEXTURES OF TREES, BUILDINGS, ETC", *col);
  font2->drawText (xf / fontzoom, (yt -= 2 * ydist) / fontzoom, zf, "- DESIGN 3D MODELS", *col);
  font2->drawText (xf / fontzoom, (yt -= 2 * ydist) / fontzoom, zf, "- CREATE SOUNDTRACKS", *col);
  font2->drawText (xf / fontzoom, (yt -= 2 * ydist) / fontzoom, zf, "- MAKE MISSION SUGGESTIONS", *col);
  font2->drawText (xf / fontzoom, (yt -= 2 * ydist) / fontzoom, zf, "- GIVE FEEDBACK AND ADVICE", *col);
  font1->zoom = 0.1;
  font2->zoom = 0.1;
  glPopMatrix ();
#endif
}

void StateFinish::display ()
{
  glTranslatef (0, -3.5 + 0.01 * (float) finishtimer / timestep, 0);
  Color *col = &colorwhite;
  font1->drawTextCentered (0, 12, -3, "CONGRATULATIONS!", *col);
  font1->drawTextCentered (0, 10, -3, "THE WORLD HAS BEEN SAVED YET AGAIN.", *col);
  font1->drawTextCentered (0, 6, -3, "HOPE YOU HAD FUN PLAYING GL-117!", *col);
  font1->drawTextCentered (0, 4, -3, "THIS GAME HAS ORIGINALLY BEEN DEVELOPED AS PART", *col);
  font1->drawTextCentered (0, 2, -3, "OF THE COURSE \"APPLICATIONS OF COMPUTER GRAPHICS\"", *col);
  font1->drawTextCentered (0, 0, -3, "AT THE TECHNICAL UNIVERSITY OF MUNICH, GERMANY.", *col);
  font1->drawTextCentered (0, -2, -3, "IN FEBRUARY 2002 THE WORK WAS DONE AND I", *col);
  font1->drawTextCentered (0, -4, -3, "PRESENTED THE PROTOTYPE OF A FLIGHT SIM,", *col);
  font1->drawTextCentered (0, -6, -3, "YET WITHOUT TEXTURES, JOYSTICK, SOUNDS, MUSIC,", *col);
  font1->drawTextCentered (0, -8, -3, "NO CAMPAIGN!", *col);
  font1->drawTextCentered (0, -10, -3, "TWO MONTHS LATER THE FIRST RELEASE OF GL-117 WAS READY.", *col);
  font1->drawTextCentered (0, -12, -3, "FURTHER RELEASES FOLLOWED, FEEDBACK ARRIVED, AND", *col);
  font1->drawTextCentered (0, -14, -3, "PROGRAMMERS JOINED (LISTED IN THE CREDITS SECTION).", *col);
  font1->drawTextCentered (0, -16, -3, "SPECIAL THANKS TO JOSEF DREXL FOR THE MODELING IDEAS,", *col);
  font1->drawTextCentered (0, -18, -3, "AND THE UNIX AWARD GOES TO WOLFGANG HOMMEL ;-)", *col);
}

void StateQuit::display ()
{
  allmenus.setVisible (false);
  quitmenu.setVisible (true);
  quitmenu.draw ();

  font1->drawTextCentered (0, 0, -2, "REALLY QUIT?", StandardTextColor);

  drawMouseCursor ();
}

void StatePlay::calcVibration (Rotation &mycamrot)
{
  mycamrot.set (camrot.gamma + view_y, camrot.theta, camrot.phi + view_x);

  if (vibration > 0)
  {
    if (camera == 0)
    {
      float sinvib = sin ((float) vibration / timestep);
      mycamrot.phi += 0.2 * vibration * sinvib / timestep;
      mycamrot.gamma += 0.2 * vibration * sinvib / timestep;
    }
  }
}

void StatePlay::calcLightFactor (float &pseudoview)
{
  if (camera == 0 && sunblinding && day && weather == WEATHER_SUNNY)
  {
    float np = fplayer->currot.phi - 180;
    if (np >= 180) np -= 360;
    float sunfactor = fabs (np) + fabs (fplayer->currot.gamma - 180 - sungamma);
    if (sunfactor < 50)
    {
      sunfactor = (50 - sunfactor) / 10;
      if (sunfactor < 1.0) sunfactor = 1.0;
    }
    else sunfactor = 1.0;
    sunlight_dest = sunfactor;
    if (gamestate == &stateplay)
      pseudoview /= sunfactor;
  }
  else
  {
    if (day) sunlight_dest = 1.0;
    else sunlight_dest = 0.75;
    if (flash > 0)
    {
      sunlight_dest = (float) flash / timestep;
    }
  }
}

void StatePlay::calcFlash ()
{
  for (unsigned i = 0; i < fighter.size (); i ++)
  {
    if (fighter [i]->draw)
      if (fighter [i]->explode > 0)
        if (fighter [i] != fplayer)
        {
          float dgamma = fabs ((float) fplayer->getAngleH (fighter [i]));
          float dphi = fabs ((float) fplayer->getAngle (fighter [i]));
          if (dphi < 45 && dgamma < 45)
          {
            float ddist = fplayer->distance (fighter [i]);
            if (ddist < 40)
            {
              ddist /= 15;
              ddist ++;
              dphi /= 25;
              dphi ++;
              dgamma /= 25;
              dgamma ++;
              if (fighter [i]->explode < 8 * timestep)
                sunlight_dest = (float) fighter [i]->explode / timestep * 4 / ddist / dphi / dgamma;
              else if (fighter [i]->explode < 16 * timestep)
                sunlight_dest = (16.0 - fighter [i]->explode / timestep) * 4 / ddist / dphi / dgamma;
            }
          }
        }
  }
}

void StatePlay::display ()
{
  unsigned i;
  double sunx = 0, suny = 0, sunz;
  bool sunvisible = false;

  Rotation mycamrot;
  calcVibration (mycamrot); // add vibration to camera if player is hit

  float pseudoview = l->getView ();
  calcLightFactor (pseudoview); // calculate sunlight factor from angle to sun

  if (dynamiclighting)
  {
    calcFlash (); // show a short flash when an object explodes => add to sunfactor
  }

  // sunlight for glittering does not look good
/* if (camera == 0 && l->glittering > 1.0F)
  {
    sunlight += l->glittering * 1.0F - 1.0F;
  }*/
  l->glittering = 0;

  // turn down global sunlight when menu is showing
  if (gamestate != &stateplay && sunlight > 0.9F) sunlight = 0.9F;
  l->sunlight = sunlight;

  // start rendering

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  glScalef (GLOBALSCALE, GLOBALSCALE, GLOBALSCALE);

  if (dithering) glEnable (GL_DITHER);
  else glDisable (GL_DITHER);
  glShadeModel (GL_SMOOTH);

  glPushMatrix (); // PUSH -> 1

  if (camera != 1 && camera != 5)
    glRotatef (-mycamrot.theta, 0.0, 0.0, 1.0);
  glRotatef (mycamrot.gamma, 1.0, 0.0, 0.0);
  glRotatef (-mycamrot.phi, 0.0, 1.0, 0.0);

  // draw sky

  glDisable (GL_FOG);
  glDisable (GL_DEPTH_TEST);

  float mylight = sunlight;
  if (!day) mylight /= 0.75;
  if (mylight > 1.0 && day)
    mylight = mylight / 5.0 + 0.8;
  else if (mylight > 1.0 && !day)
    mylight = mylight / 5.0 + 0.8;
  gl.setFogLuminance (mylight);
  Model3dRealizer mr;
  mr.drawNoTexture (*sphere->o, sphere->trafo, mylight, 0);

  if (weather == WEATHER_SUNNY || weather == WEATHER_CLOUDY)
  {
    if (!day)
    {
      unsigned stars = maxstar;
      if (weather != WEATHER_CLOUDY)
        stars = maxstar / 2;
      for (unsigned i = 0; i < stars; i ++)
      {
        glPushMatrix (); // PUSH -> 2
        glRotatef (star [i]->phi, 0.0, 1.0, 0.0);
        glRotatef (star [i]->gamma, 1.0, 0.0, 0.0);
        glTranslatef (0, 0, -10);
        star [i]->draw ();
        glPopMatrix ();
      }
    }
  }

  glEnable (GL_DEPTH_TEST);
  glEnable (GL_FOG);

  if (quality >= 1 && clouds > 0)
  {
    float cloudfog = pseudoview;
    if (cloudfog > 110) cloudfog = 110;
    gl.enableFog (cloudfog * GLOBALSCALE, quality <= 5);

    highclouds->trafo.scaling.set (400, 400, 400);
    float ch2 = -382 - fplayer->trafo.translation.y / 10.0;
    Vector3 tlsphere2 (0, ch2, 0);
    highclouds->drawGL (tlsphere2, fplayer->trafo.translation);

    glDisable (GL_FOG);
  }

// draw sun or moon (or earth)
  float fac = ::view, zfac = ::view * 0.2;
  if (weather == WEATHER_SUNNY || weather == WEATHER_CLOUDY)
  {
    float zf = -11;
    if (day)
      zf = -10;
    if (l->type == LAND_MOON && !day)
      zf = -8; // diplay bigger earth

    glRotatef (180, 0.0, 1.0, 0.0);
    if (camera == 0)
      glRotatef (sungamma, 1.0, 0.0, 0.0);
    else
      glRotatef (mycamrot.gamma + sungamma, 1.0, 0.0, 0.0);
    glTranslatef (0, 0, zf);

    frustum.extractFrustum ();
    if (frustum.isPointInFrustum (-1, 1, 0) || frustum.isPointInFrustum (-1, -1, 0) ||
        frustum.isPointInFrustum (1, -1, 0) || frustum.isPointInFrustum (1, 1, 0))
    {
      glDisable (GL_DEPTH_TEST);
      if (day) gl.enableTexture (texsun->textureID);
      else if (l->type != LAND_MOON) gl.enableTexture (texmoon->textureID);
      else gl.enableTexture (texearth->textureID);
      if (day && l->type != 1)
        glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      gl.enableAlphaBlending ();
      glEnable (GL_ALPHA_TEST);
      float alphamax = 0.1;
      if (day) alphamax = 0.1;
      glAlphaFunc (GL_GEQUAL, alphamax);
      glBegin (GL_QUADS);
      fac = ::view; zfac = ::view * 3.5;
      if (day && l->type != LAND_MOON)
      {
        float gm = (40.0 - sungamma) / 80.0;
        if (gm < 0) gm = 0;
        if (gm > 0.5) gm = 0.5;
        glColor4f (1.0, 1.0 - gm, 0.8 - gm, 1.0);
      }
      else
        glColor4f (1.0, 1.0, 1.0, 1.0);
      glTexCoord2d (0, 1);
      glVertex3f (-1, 1, 0);
      glTexCoord2d (1, 1);
      glVertex3f (1, 1, 0);
      glTexCoord2d (1, 0);
      glVertex3f (1, -1, 0);
      glTexCoord2d (0, 0);
      glVertex3f (-1, -1, 0);
      glEnd ();

      // Where is the sun?
      double proj[16];
      double modl[16];
      int vp[4];
      glGetDoublev( GL_PROJECTION_MATRIX, proj );
      glGetDoublev( GL_MODELVIEW_MATRIX, modl );
      glGetIntegerv( GL_VIEWPORT, vp );
      //gluProject (0, 0, 0, modl, proj, vp, &sunx, &suny, &sunz);
      if ((sunx>=vp[0])&&(suny>=vp[1])&&(sunx<(vp[0]+vp[2]))&&(suny<(vp[1]+vp[3])))
        sunvisible = true;

      glDisable (GL_ALPHA_TEST);
      glEnable (GL_DEPTH_TEST);
      glDisable (GL_TEXTURE_2D);
      gl.disableAlphaBlending ();
    }
  }

  glPopMatrix (); // POP -> 0

  glPushMatrix (); // PUSH -> 1

  if (camera != 1 && camera != 5)
    glRotatef (-mycamrot.theta, 0.0, 0.0, 1.0);

  glRotatef (mycamrot.gamma, 1.0, 0.0, 0.0);

  glDisable (GL_FOG);

  glRotatef (-mycamrot.phi, 0.0, 1.0, 0.0);
  glScalef (1, 1, 1);

  glTranslatef (-cam.x, -cam.y, -cam.z);

  if (camera != 50)
  {
    gl.enableFog (pseudoview * GLOBALSCALE, quality <= 5);
  }

  // draw terrain
  if (dynamiclighting)
  {
    memset (l->dl, 0, (MAXX + 1) * (MAXX + 1));
    for (i = 0; i < explosion.size (); i ++)
    {
      if (explosion [i]->ttl > 0)
        l->calcDynamicLight (explosion [i], 50.0F, 100.0F, 2.0F);
    }
    if (!day)
    {
      for (i = 0; i < laser.size (); i ++)
      {
        if (laser [i]->draw)
          l->calcDynamicLight (laser [i], 15.0F, 75.0F, 5.0F);
      }
      for (i = 0; i < missile.size (); i ++)
      {
        if (missile [i]->draw)
          l->calcDynamicLight (missile [i], 15.0F, 75.0F, 5.0F);
      }
      for (i = 0; i < flare.size (); i ++)
      {
        if (flare [i]->draw)
          l->calcDynamicLight (flare [i], 15.0F, 75.0F, 5.0F);
      }
    }
  }
  glEnable (GL_CULL_FACE);
  glCullFace (GL_FRONT);
  l->draw (cam, (int) mycamrot.phi, (int) (-mycamrot.gamma + 180.0));
  glDisable (GL_CULL_FACE);

  // draw objects
  frustum.extractFrustum ();
  if (camera != 50)
  {
//    space->lum = sunlight;
    float dayfac = 1.0;
    if (!day) dayfac = 0.5;
    if (weather == WEATHER_SUNNY || weather == WEATHER_CLOUDY)
    {
      for (i = 0; i < space->o.size (); i ++)
      {
        if (space->o [i]->trafo.translation.y < l->getExactRayHeight (space->o [i]->trafo.translation.x, space->o [i]->trafo.translation.z))
          space->o [i]->lum = 0.5 * dayfac;
        else
          space->o [i]->lum = 1.0 * dayfac;
      }
    }
    else
    {
      for (i = 0; i < space->o.size (); i ++)
        space->o [i]->lum = dayfac;
    }
//    printf ("%2.1f*%2.1f ", fplayer->lum, sunlight);
    if (flash > 7 * timestep)
    {
      if (antialiasing)
        flash1->draw ();
      else
        flash1->drawHQ ();
    }
    if (lighting)
    {
      glEnable( GL_LIGHTING);
      glEnable( GL_LIGHT0 );
      glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      OpenGlRenderer renderer;
      renderer.draw(*space, *l);
//      space->drawGL (); // draw all objects
      glDisable (GL_LIGHTING);
      glDepthMask (GL_FALSE);
      for (i = 0; i < space->o.size (); i ++)
      {
        AiObj *dobj = (AiObj *) space->o [i];
        if (dobj->id >= MissileBeginDescriptor)
          if (dobj->draw && dobj->drawLight && dobj->active)
          {
            if (dobj->smoke)
              if ((dobj->id >= MissileBeginDescriptor && dobj->id <= MissileEndDescriptor) || (dobj->id >= FighterBeginDescriptor && dobj->id <= AirEndDescriptor))
                if (!(dobj->ttl == 0 && dobj->id >= MissileBeginDescriptor && dobj->id <= MissileEndDescriptor))
                {
                  dobj->smoke->draw (camrot.phi, camrot.gamma);
                }
          }
      }
      glDepthMask (GL_TRUE);
    }
    else
    {
//      space->drawGL ();
    }
  }
  glDisable (GL_TEXTURE_2D);

  glDisable (GL_LIGHTING);

  if (camera == 0)
  {
    glDisable (GL_DEPTH_TEST);
    if (antialiasing)
      glEnable (GL_LINE_SMOOTH);
    cockpit->drawTargeter ();
    if (antialiasing)
      glDisable (GL_LINE_SMOOTH);
    glEnable (GL_DEPTH_TEST);
  }

  glPopMatrix (); // POP -> 0

// draw flares
  if (specialeffects)
    if (sunvisible && /*camera == 0 &&*/ sunblinding && day)
    {
      Texture* fl_texture[]= {texflare1,texflare3,texflare2,texflare4,texflare2,texflare4,texflare3,0};
      double fl_position[]=   {0.2,      1.6,      3.2,      8.1,      -1.4,     -2.2,     -3.5,     0};
      double fl_size[]=       {0.8,      0.6,      0.5,      0.4,      0.8,      0.4,      0.5,      0};
      double proj[16];
      double modl[16];
      double dr[2];
      int vp[4];
      double objx,objy,objz;

      glGetDoublev( GL_PROJECTION_MATRIX, proj );
      glGetDoublev( GL_MODELVIEW_MATRIX, modl );
      glGetIntegerv( GL_VIEWPORT, vp );
      glGetDoublev( GL_DEPTH_RANGE, dr );

      double cx=vp[2]/2+vp[0];
      double cy=vp[3]/2+vp[1];

      glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      int i=0;
      double flarez_eye=4;
      double flarez_win=(-proj[10]+proj[14]/flarez_eye)*(dr[1]-dr[0])/2+(dr[1]+dr[0])/2;
      while (Texture *tex=fl_texture[i])
      {
        double position=fl_position[i];
        double flarex,flarey,size=fl_size[i]/GLOBALSCALE;

        i++;

        flarex=(cx-sunx)*position+sunx;
        flarey=(cy-suny)*position+suny;

        if (gluUnProject (flarex, flarey, flarez_win, modl, proj, vp, &objx, &objy, &objz)==GL_TRUE)
        {
          gl.enableTexture (tex->textureID);
          gl.enableAlphaBlending ();
          glDisable (GL_ALPHA_TEST);
          glDisable (GL_DEPTH_TEST);
          glDisable (GL_FOG);

          glBegin (GL_QUADS);
          glColor4f (1.0, 1.0, 1.0, 1.0);
          glTexCoord2d (0, 1);
          glVertex3f (-size+objx, size+objy, objz);
          glTexCoord2d (1, 1);
          glVertex3f (size+objx, size+objy, objz);
          glTexCoord2d (1, 0);
          glVertex3f (size+objx, -size+objy, objz);
          glTexCoord2d (0, 0);
          glVertex3f (-size+objx, -size+objy, objz);
          glEnd ();
        }
      }
      glEnable (GL_DEPTH_TEST);
      glDisable (GL_TEXTURE_2D);
      gl.disableAlphaBlending ();
      glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }

// sunblinding test

  if (sunvisible)
  {
    GLfloat zbuf[1];
    glReadPixels((int)sunx,(int)suny,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,zbuf);
    sunblinding=(zbuf[0]<1)?false:true;
  }
  else
  {
    sunblinding=false;
  }

  // draw cockpit
  if (camera == 0)
  {
    if (antialiasing)
      glEnable (GL_LINE_SMOOTH);
    glPushMatrix (); // PUSH -> 1
    glRotatef (view_y, 1, 0, 0);
    glRotatef (-view_x, 0, 1, 0);
    cockpit->drawCross ();
    cockpit->drawHeading ();
    glPopMatrix (); // POP -> 0
    if (antialiasing)
      glDisable (GL_LINE_SMOOTH);
    cockpit->drawRadar ();
    cockpit->drawTargetedElement ();
    cockpit->drawWeapon ();
    cockpit->drawCounter (gamestate == &stateplay);
    cockpit->drawThrustBar ();
    cockpit->drawRelativeHeightBar ();
   }

  // draw blackout/redout (blending)
  if (camera == 0)
  {
    int black = (int) blackout;
    if (black > 255) black = 255;
    int red = (int) redout;
    if (red > 255) red = 255;
    float xf = 2.0, yf = 1.5, zf = 1.0;
    if (black > 0)
    {
      glColor4ub (0, 0, 0, black);
    }
    else if (red > 0)
    {
      glColor4ub (255, 0, 0, red);
    }
    if (black > 0 || red > 0)
    {
      glDisable (GL_DEPTH_TEST);
      glEnable (GL_BLEND);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glBegin (GL_QUADS);
      glVertex3f (-xf, -yf, -zf);
      glVertex3f (-xf, yf, -zf);
      glVertex3f (xf, yf, -zf);
      glVertex3f (xf, -yf, -zf);
      glEnd ();
      gl.disableAlphaBlending ();
      glDisable (GL_BLEND);
      glEnable (GL_DEPTH_TEST);
    }
  }

  if (brightness)
  {
    adjustBrightness ();
    glEnable (GL_DEPTH_TEST);
  }

  // draw mission dependant informations
  mission->draw ();

  glDisable (GL_DEPTH_TEST);
  char buf [25];
  sprintf (buf, "FPS: %d", (int) fps);
  font1->drawText (-25, 25, -3.5, buf, colorwhite);

  bool write = false;
  if (firststart)
  {
    int akttime;
    akttime = SDL_GetTicks ();
    if (akttime - starttime < 20000)
      if ((akttime - starttime) / 300 % 3)
      {
        font1->drawTextCentered (0, 0, -1.8, "PLEASE WAIT WHILE", colorred);
        font1->drawTextCentered (0, -1, -1.8, "ADJUSTING QUALITY", colorred);
        write = true;
      }
  }
  if (fps >= 5 && fps <= 20 && !write)
  {
    font1->drawTextCentered (0, -8, -2, "FPS TOO LOW", colorred);
    font1->drawTextCentered (0, -9, -2, "TURN DOWN VIEW OR QUALITY", colorred);
  }

  if (fps < 5 && !write)
  {
    font1->drawTextCentered (0, -8, -2, "FPS FAR TOO LOW", colorred);
    font1->drawTextCentered (0, -9, -2, "SEE \"README\" OR \"FAQ\" FILES", colorred);
    font1->drawTextCentered (0, -10, -2, "HIT \"ESC\" AND 'Y' TO EXIT THE GAME", colorred);
  }

  if (controls == CONTROLS_MOUSE && !mouse_relative)
    drawMouseCursor ();
}

void StatePlay::timer (Uint32 dt)
{
//  if (multiplayer) return;
  unsigned i, i2;

  // make sure that Mission::processTimer() is called after move() and before this cleanUp()!
  // so, call cleanUp() first of all
  explosion.cleanUp ();
  blacksmoke.cleanUp ();
  flare.cleanUp ();
  chaff.cleanUp ();
  missile.cleanUp ();
  laser.cleanUp ();
//  fighter.cleanUp (); // do not cleanup fighter objects, we still need them for mission stats
  groundobj.cleanUp ();

  sunlight += (sunlight_dest - sunlight) / 10 * dt / timestep;
  l->sunlight = sunlight;

  gametimer += dt;
  cockpit->dt = dt;
  l->lsticker += dt;
  if (l->lsticker >= 36000000)
    l->lsticker = 0;

  if (vibration > 0)
  {
    vibration -= dt;
  }

  if (fplayer->autofire && fplayer->active)
  {
    if (fplayer->ammo != 0)
    {
      fplayer->fireCannon (laser);
#ifdef HAVE_SDL_MIXER
      if (!startcannon)
      {
        sound->playLoop (SOUND_CANNON1);
        startcannon = true;
      }
#else
      sound->play (SOUND_CANNON1, 1);
#endif
    }
  }
#ifdef HAVE_SDL_MIXER
  else
  {
    if (startcannon)
    {
      startcannon = false;
      sound->stop (SOUND_CANNON1);
    }
  }
#endif

  if (lastshield > fplayer->shield && !fplayer->ai)
  {
    sound->play (SOUND_HIT1, false);
    lastshield = (int) fplayer->shield;
    vibration = 25 * timestep;
  }

  // create flash during thunderstorm
  if (weather == WEATHER_THUNDERSTORM && flash <= 0 && !Math::random (2000 / dt))
  {
    flash = 18 * timestep;
    int fphi = (int) camrot.phi + Math::random (50) - 25;
    if (fphi < 0) fphi += 360;
    else if (fphi >= 360) fphi -= 360;
    float pseudoview = l->getView ();
    float fdist = Math::random ((int) pseudoview - 20) + 10;
    float fx = fplayer->trafo.translation.x - SIN(fphi) * fdist;
    float fz = fplayer->trafo.translation.z - COS(fphi) * fdist;
    flash1->set (fx, l->getHeight (fx, fz), fz, (int) camrot.phi);
    int lev = (int) (128.0 - 80.0 * fdist / (pseudoview - 10));
    sound->setVolume (SOUND_THUNDER1, lev);
    sound->play (SOUND_THUNDER1, false);
  }

  if (initing) return;

  if (flash > 0)
    flash -= dt;
  if (flash <= 7 * timestep && flash > 0)
    flash -= dt;

  if (!fplayer->ai)
    if (sound->engine != (int) ((fplayer->thrust / fplayer->getPrototype ()->maxthrust * 20) - 10))
    {
      setPlaneVolume ();
      lastthrust = fplayer->thrust;
    }

  // collision tests
  for (i = 0; i < fighter.size (); i ++)
  {
    for (i2 = 0; i2 < laser.size (); i2 ++)
      if (laser [i2]->active)
        fighter [i]->collide (laser [i2], dt);
    for (i2 = 0; i2 < missile.size (); i2 ++)
      if (missile [i2]->active)
        fighter [i]->collide (missile [i2], dt);
    for (i2 = 0; i2 < i; i2 ++)
      if (fighter [i2]->active)
        if (i != i2)
          fighter [i]->collide (fighter [i2], dt);
  }

  for (i = 0; i < flare.size (); i ++)
  {
    for (i2 = 0; i2 < missile.size (); i2 ++)
      if (missile [i2]->active)
        flare [i]->collide (missile [i2], dt);
  }

  for (i = 0; i < chaff.size (); i ++)
  {
    for (i2 = 0; i2 < missile.size (); i2 ++)
      if (missile [i2]->active)
        chaff [i]->collide (missile [i2], dt);
  }

  // move objects
  for (i = 0; i < fighter.size (); i ++)
  {
    fighter [i]->aiAction (dt, fighter, missile, laser, flare, chaff, camrot.phi, camrot.gamma);
    float lev;
    if (fighter [i]->explode == 1 && (lev = fplayer->distance (fighter [i])) < 32)
    {
      sound->setVolume (SOUND_EXPLOSION1, 128);
      float angle = fplayer->getAngle (fighter [i]);
      angle = 180 - angle;
      sound->setPosition (SOUND_EXPLOSION1, (int) angle, (int) (lev * 4.0));
      sound->play (SOUND_EXPLOSION1, false);
    }
  }
  for (i = 0; i < laser.size (); i ++)
  {
    laser [i]->move (dt, camrot.phi, camrot.gamma);
  }
  for (i = 0; i < missile.size (); i ++)
  {
    missile [i]->aiAction (dt, fighter, missile, laser, flare, chaff, camrot.phi, camrot.gamma);
  }
  for (i = 0; i < flare.size (); i ++)
  {
    flare [i]->move (dt, camrot.phi, camrot.gamma);
  }
  for (i = 0; i < chaff.size (); i ++)
  {
    chaff [i]->move (dt, camrot.phi, camrot.gamma);
  }

  for (i = 0; i < explosion.size (); i ++)
    explosion [i]->move (dt, camrot.phi, camrot.gamma);
  for (i = 0; i < blacksmoke.size (); i ++)
    blacksmoke [i]->move (dt, camrot.phi, camrot.gamma);

  // show blackout/redout
  if (blackout > 0) blackout -= 3.0F * dt / timestep;
  if (blackout < 0) blackout = 0;
  if (redout > 0) redout -= 3.0F * dt / timestep;
  if (redout < 0) redout = 0;
  if (blackout > 400) blackout = 400;
  if (redout > 400) redout = 400;
  float testout;
  if (redout < 1)
  {
    testout = (fplayer->realspeed * fplayer->elevatoreffect - 0.13F) * 40.0F;
    if (testout > 0) blackout += testout * dt / timestep;
  }
  if (blackout < 1)
  {
    testout = (fplayer->realspeed * fplayer->elevatoreffect + 0.065F) * 80.0F;
    if (testout < 0) redout -= testout * dt / timestep;
  }

  // show own explosion from chase cam
  if (fplayer->shield <= 0)
    camera = 1;

  float cf = -fplayer->trafo.scaling.x / 2;
  camrot.theta = fplayer->currot.theta;
  if (camera == 0)  // cockpit
  {
    float cgamma = fplayer->currot.gamma + 25.0F * COS(fplayer->currot.theta);
    float cphi = fplayer->currot.phi + 25.0F * SIN(fplayer->currot.theta);
    float fac = fplayer->trafo.scaling.x / 2;
    cam.x = fplayer->trafo.translation.x + COS(cgamma) * SIN(cphi) * fac;
    cam.y = fplayer->trafo.translation.y - SIN(cgamma) * fac;
    cam.z = fplayer->trafo.translation.z + COS(cgamma) * COS(cphi) * fac;
    camrot.phi = fplayer->currot.phi;
    camrot.gamma = -fplayer->currot.gamma + 180;
    fplayer->draw = 0;
  }
  if (camera == 1) // chase
  {
    cf = fplayer->trafo.scaling.x * 3;
    cam.x = fplayer->trafo.translation.x + cf * SIN(fplayer->currot.phi);
    cam.y = fplayer->trafo.translation.y + fplayer->trafo.scaling.x;
    cam.z = fplayer->trafo.translation.z + cf * COS(fplayer->currot.phi);
    camrot.phi = fplayer->currot.phi;
    fplayer->draw = 1;
    camrot.gamma = 20;
  }
  else if (camera == 2) // backwards
  {
    cf = -fplayer->trafo.scaling.x * 3;
    cam.x = fplayer->trafo.translation.x + cf * SIN(fplayer->currot.phi);
    cam.y = fplayer->trafo.translation.y + fplayer->trafo.scaling.x;
    cam.z = fplayer->trafo.translation.z + cf * COS(fplayer->currot.phi);
    camrot.phi = fplayer->currot.phi + 180.0;
    fplayer->draw = 1;
    camrot.gamma = 20;
  }
  else if (camera == 3) // other players
  {
    cf = fighter [aktcam]->trafo.scaling.x * 3;
    cam.x = fighter [aktcam]->trafo.translation.x + cf * SIN(fighter [aktcam]->currot.phi);
    cam.y = fighter [aktcam]->trafo.translation.y + fighter [aktcam]->trafo.scaling.x;
    cam.z = fighter [aktcam]->trafo.translation.z + cf * COS(fighter [aktcam]->currot.phi);
    camrot.phi = fighter [aktcam]->currot.phi;
    camrot.gamma = 20;
    camrot.theta = fighter [aktcam]->currot.theta;
    fplayer->draw = 1;
  }
  else if (camera == 4) // missile
  {
    cf = missile [0]->trafo.scaling.x * 10;
    cam.x = missile [0]->trafo.translation.x + cf * SIN(missile [0]->currot.phi);
    cam.y = missile [0]->trafo.translation.y + fplayer->trafo.scaling.x * 2;
    cam.z = missile [0]->trafo.translation.z + cf * COS(missile [0]->currot.phi);
    camrot.phi = missile [0]->currot.phi;
    fplayer->draw = 1;
  }
  else if (camera == 5) // top
  {
    cf = fplayer->trafo.scaling.x * 15;
    cam.x = fplayer->trafo.translation.x + cf * SIN(fplayer->currot.phi);
    cam.y = fplayer->trafo.translation.y + 5.5;
    cam.z = fplayer->trafo.translation.z + cf * COS(fplayer->currot.phi);
    camrot.phi = fplayer->currot.phi;
    fplayer->draw = 1;
    camrot.gamma = 50;
  }
  else if (camera == 6) // left
  {
    cf = fplayer->trafo.scaling.x * 2;
    camrot.phi = fplayer->currot.phi + 90.0;
    if (camrot.phi >= 360) camrot.phi -= 360;
    else if (camrot.phi < 0) camrot.phi += 360;
    cam.x = fplayer->trafo.translation.x + cf * SIN(camrot.phi);
    cam.y = fplayer->trafo.translation.y + fplayer->trafo.scaling.x;
    cam.z = fplayer->trafo.translation.z + cf * COS(camrot.phi);
    fplayer->draw = 1;
    camrot.gamma = 20;
  }
  else if (camera == 7) // right
  {
    cf = fplayer->trafo.scaling.x * 2;
    camrot.phi = fplayer->currot.phi + 270.0;
    if (camrot.phi >= 360) camrot.phi -= 360;
    else if (camrot.phi < 0) camrot.phi += 360;
    cam.x = fplayer->trafo.translation.x + cf * SIN(camrot.phi);
    cam.y = fplayer->trafo.translation.y + fplayer->trafo.scaling.x;
    cam.z = fplayer->trafo.translation.z + cf * COS(camrot.phi);
    fplayer->draw = 1;
    camrot.gamma = 20;
  }
  else if (camera == 8) // top near
  {
    cf = fplayer->trafo.scaling.x * 5;
    cam.x = fplayer->trafo.translation.x + cf * SIN(fplayer->currot.phi);
    cam.y = fplayer->trafo.translation.y + 2.5;
    cam.z = fplayer->trafo.translation.z + cf * COS(fplayer->currot.phi);
    camrot.phi = fplayer->currot.phi;
    fplayer->draw = 1;
    camrot.gamma = 50;
  }
  else if (camera == 9) // top very near
  {
    cf = fplayer->trafo.scaling.x * 2;
    cam.x = fplayer->trafo.translation.x + cf * SIN(fplayer->currot.phi);
    cam.y = fplayer->trafo.translation.y + 1.0;
    cam.z = fplayer->trafo.translation.z + cf * COS(fplayer->currot.phi);
    camrot.phi = fplayer->currot.phi;
    fplayer->draw = 1;
    camrot.gamma = 50;
  }
  else if (camera == 50)
  {
    cam.x = 20;
    cam.z = 80;
    cam.y = 250;
    camrot.phi = 20;
    camrot.gamma = 75;
    gamestate = &statepause;
  }
  if (camrot.phi >= 360.0) camrot.phi -= 360.0;
  sdldisplay = true;

  fplayer->recrot.theta -= dtheta;
  fplayer->recrot.gamma += dgamma;

  missionstate = mission->processtimer (dt);

  if (missionstate == 2)
  {
    missionending ++;
    if (missionending >= 25)
    {
      missionending = 0;
      fplayer->score = mission->getScore (missionstate);
      if (mission->id != MISSION_DEMO)
      {
        switch_stats ();
      }
      else
      {
        pleaseWait ();
        createMission (MISSION_DEMO);
        game_levelInit ();
      }
    }
  }

  if (missionstate == 1)
  {
    missionending ++;
    if (missionending >= 25)
    {
      missionending = 0;
      fplayer->score = mission->getScore (missionstate);
      if (mission->id != MISSION_DEMO)
      {
        if (mission->id != MISSION_MOON3)
          switch_stats ();
        else
          switch_finish ();
      }
      else
      {
        pleaseWait ();
        createMission (MISSION_DEMO);
        game_levelInit ();
      }
    }
  }

  if (controls == CONTROLS_KEYBOARD)
  {
    float fac = 0.005;
    if (keyb_elev == 0) fplayer->elevatoreffect = 0;
    else fplayer->elevatoreffect += (float) dt * keyb_elev * fac;
    if (keyb_roll == 0) fplayer->rolleffect = 0;
    else fplayer->rolleffect += (float) dt * keyb_roll * fac;
    if (keyb_rudder == 0) fplayer->ruddereffect = 0;
    else fplayer->ruddereffect += (float) dt * keyb_rudder * fac;
    if (fplayer->elevatoreffect > 1.0) fplayer->elevatoreffect = 1.0;
    if (fplayer->elevatoreffect < -0.5) fplayer->elevatoreffect = -0.5;
    if (fplayer->rolleffect > 1.0) fplayer->rolleffect = 1.0;
    if (fplayer->rolleffect < -1.0) fplayer->rolleffect = -1.0;
    if (fplayer->ruddereffect > 1.0) fplayer->ruddereffect = 1.0;
    if (fplayer->ruddereffect < -1.0) fplayer->ruddereffect = -1.0;
  }
}

void StateMenu::timer (Uint32 dt)
{
  char buf [256];
  menutimer += dt;
  int cycle = (menutimer / timestep) % 256;
  if (cycle == 0)
  {
    newcamera = 0;
  }
  if (cycle == 200 && !newcamera)
  {
    newcamera = 1;
    if (camera == 5)
    {
      camera = 1;
      if (!missile.empty () && missile [0]->active)
        camera = 4;
    }
    else if (camera == 1 || camera == 4) camera = 2;
    else if (camera == 2) camera = 5;
  }

  int akttime;
  akttime = SDL_GetTicks ();
  if (firststart)
    if (akttime - starttime < 20000)
    {
      if (lastfps != fps)
      {
        lastfps = fps;
        if (fps > 40)
        {
          if (::view < quality * 20 + 60 && ::view < 100)
            ::view += 20;
          else if (quality < 5)
          {
            quality ++;
            ::view = quality * 20 + 30;
            if (quality >= 1)
            { antialiasing = 1; specialeffects = 1; dithering = 1; dynamiclighting = 0; }
          }
          sprintf (buf, "%d", (int) quality);
          ((Label *) optmenu [0]->components [1])->setText (buf);
          sprintf (buf, "%d", (int) ::view);
          ((Label *) optmenu [0]->components [3])->setText (buf);
        }
        else if (fps < 30)
        {
          if (::view > quality * 20 + 30 && ::view > 20)
            ::view -= 20;
          else if (quality > 0)
          {
            quality --;
            ::view = quality * 20 + 60;
            if (quality < 1)
            { antialiasing = 0; specialeffects = 0; dithering = 0; dynamiclighting = 0; }
          }
          sprintf (buf, "%d", (int) quality);
          ((Label *) optmenu [0]->components [1])->setText (buf);
          sprintf (buf, "%d", (int) ::view);
          ((Label *) optmenu [0]->components [3])->setText (buf);
        }
        statemenu.reshape ();
      }
    }
  sdldisplay = true;
}

void StateStats::timer (Uint32 dt)
{
  menutimer += dt;
  sdldisplay = true;
}

void StateMission::timer (Uint32 dt)
{
  missionmenutimer += dt;
  sdldisplay = true;
}

void StateCredits::timer (Uint32 dt)
{
  creditstimer += dt;
#ifdef SHOW_SOFTWARE_PATENTS
  if (creditstimer > 2500 * timestep)
    creditstimer = 0;
#else
  if (creditstimer > 700 * timestep)
    creditstimer = 0;
#endif

  sdldisplay = true;
}

void StateFinish::timer (Uint32 dt)
{
  finishtimer += dt;
  if (finishtimer > 800 * timestep)
    finishtimer = 0;
  sdldisplay = true;
}

void StateQuit::timer (Uint32 dt)
{
  sdldisplay = true;
}

void StateCreate::timer (Uint32 dt)
{
//  server->createSocketSet ();
  server->getClient ();
  SDL_Delay (100);
  statemission.timer (dt);
}

void StateJoin::timer (Uint32 dt)
{
#ifdef HAVE_SDL_NET
  char buf [4096];
  if (client->sock == NULL) client->getServer ("127.0.0.1", "client1");
  else
  {
    SDL_Delay (100);
    statemission.timer (dt);
    client->getMessage (buf);

    if (buf [0] == 's')
    {
      createMission (MISSION_MULTIPLAYER_DOGFIGHT);
      game_levelInit ();
      switch_game ();
      missionactive = true;
    }
  }
#endif
}

void StateInit::reshape ()
{
  // use whole window
  glViewport (0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  // angle, aspectratio, nearclip, farclip
  //gluPerspective (visibleangle, 1.0, nearclippingplane, 80);
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);
}

void StateInit::key (int key, int x, int y)
{
  gl.clearBuffers (); // exit intro
  myInit ();
  switch_menu ();
  fplayer->ai = true;
  camera = 5;
  starttime = SDL_GetTicks ();
}

void StateInit::mouse (int button, int state, int x, int y)
{
  key (32, x, y);
}

void StateInit::display ()
{
  Vector3 vec;
  Color color (200, 200, 200, 255);

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  glPushMatrix ();

  glEnable (GL_DEPTH_TEST);
  glEnable (GL_LIGHTING);

  // draw fighter
  glPushMatrix ();
  glTranslatef (0, 0, -5);
/*  tl.z = 0;
  rot.phi = (inittimer_gl117 % (360*4)) / 4;
  rot.theta = 0;
  rot.gamma = 0; */
  Model3dRealizer mr;
  Model3d *model = Model3dRegistry::get (FalconDescriptor.name);
  mr.draw (*model, Transformation(tl, rot, Vector3(1.0)), 2.0, initexplode1);
/*  glRotatef (rot.phi, 0, 1, 0);
  glBegin (GL_QUADS);
  glNormal3f (-1.0, -1.0, 0.0);
  glVertex3f (-1.0, -1.0, 0.0);
  glNormal3f (-1.0, -1.0, 0.0);
  glVertex3f (1.0, -1.0, 0.0);
  glNormal3f (-1.0, -1.0, 0.0);
  glVertex3f (1.0, 1.0, 0.0);
  glNormal3f (-1.0, -1.0, 0.0);
  glVertex3f (-1.0, 1.0, 0.0);
  glNormal3f (-1.0, -1.0, 0.0);
  glVertex3f (-1.0, -1.0, 0.0);
  glEnd (); */
  glPopMatrix ();

  glDisable (GL_DEPTH_TEST);
  glEnable (GL_BLEND);

  if (inittimer_gl117 > 2000)
  {
    float xf = 1.3F, yf = 0.65F, zf = 2.0F;
    int col = (inittimer_gl117 - 2000) / 2;
    if (col < 0 || col > 255) col = 255;
    glColor3ub (col, col, col);
    glPushMatrix ();
    glTranslatef (0, 0.5F, 0);
    gl.enableTexture (textitle->textureID);
    if (antialiasing)
      gl.enableLinearTexture (textitle->textureID, textitle->mipmap);
    else
      gl.disableLinearTexture (textitle->textureID, textitle->mipmap);
    glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBegin (GL_QUADS);
    glTexCoord2d (0, 1);
    glVertex3f (-xf, yf, -zf);
    glTexCoord2d (1, 1);
    glVertex3f (xf, yf, -zf);
    glTexCoord2d (1, 0);
    glVertex3f (xf, -yf, -zf);
    glTexCoord2d (0, 0);
    glVertex3f (-xf, -yf, -zf);
    glEnd ();
    glPopMatrix ();
    glDisable (GL_BLEND);
  }

  glDisable (GL_LIGHTING);
  glPopMatrix ();

  // draw fire (heat array)
  glDisable (GL_DEPTH_TEST);
  glEnable (GL_BLEND);

  float xf = 1.75F, yf = 1.78F, zf = 2.0F;
  glPushMatrix ();
  gl.enableTexture (5000);
  glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glBegin (GL_QUADS);
  glTexCoord2d (0, 1);
  glVertex3f (-xf, -yf, -zf);
  glTexCoord2d (1, 1);
  glVertex3f (xf, -yf, -zf);
  glTexCoord2d (1, 0);
  glVertex3f (xf, yf, -zf);
  glTexCoord2d (0, 0);
  glVertex3f (-xf, yf, -zf);
  glEnd ();
  glPopMatrix ();
  glDisable (GL_BLEND);

  font2->drawText (20, -20, -3, PACKAGE_VERSION, color);
}

void StateInit::timer (Uint32 dt)
{
  inittimer_gl117 += dt;
  initsynchrotimer += dt;
  if (initsynchrotimer > 20)
  {
    initsynchrotimer -= 20;
    dt = 20;
  }
  else return;

  int r = Math::random (100);
  if (r == 50) r = Math::random (100); // do not optimize this: random number generator initialization

  tl.x = 6.0 * pow ((float) 1.5, (float) -(5 + tl.z));
  tl.y = (tl.z + 3) * (tl.z + 3) * 0.02 - 0.8; //0.9 * tl.x;
  tl.z += 0.14;

  tl2.y = 0.3F;
  if (initexplode1 == -1 && tl2.z < 3)
  {
    tl2.z += 0.25;
  }
  if (tl2.z < 3)
  {
    rot2.theta -= 20;
    if (rot2.theta < 0) rot2.theta += 360;
  }
  if (tl.z > -4.0)
  {
    rot.theta ++;
  }

  if (inittimer == 0)
  {
    memset (heat, 0, maxfx * maxfy * sizeof (int));
    memset (heat2, 0, maxfx * maxfy * sizeof (int));
  }

  genFireLine ();
  proceedFire ();

  if (inittimer >= 75)
  {
    initexplode1 = -1;
  }

  if (inittimer >= 350)
  {
    initexplode += dt;
  }

  if (inittimer >= 400)
  {
    stateinit.key (27, 0, 0); // switch to menu
  }

  inittimer ++;

  sdldisplay = true;
}

#endif
