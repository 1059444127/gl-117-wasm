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

/* To fully understand the GL-117 code, please read the methods from bottom to top! */

/*
TODO list:
- BlackSmoke is not drawn!
- joystick update (mail)
- joystick in menu
- mig29 model
- correct transport2 (left side corrupt in VRML) and add mission with transport2
- southern seashore landscape (additional missions)
- alpine snow landscape (additional missions)
- tree colors (fall, spring), draw more tree textures
- particle systems: rain, clouds to fly through
- James Stone: music
- Eugeni Andreeschev: 3d model
*/

#include "SDL_video.h"
#ifndef IS_MAIN_H

#include "game/globals.h"
#include "menu/Component.h"
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
#include "game/GameState.h"
#include "game/Events.h"
#include "game/Callbacks.h"
#include "loadmodel/Load3ds.h"
#include "loadmodel/Model3dFactory.h"
#include "loadmodel/Model3dRegistry.h"
#include "logging/Logging.h"
#include "util/Util.h"

#include <ctype.h>
#include <cassert>



GameState *gamestate;
StatePlay stateplay;
StateMenu statemenu;
StateMission statemission;
StateFame statefame;
StateInit stateinit;
StateFinish statefinish;
StateQuit statequit;
StatePause statepause;
StateCredits statecredits;
StateStats statestats;
StateFighter statefighter;
StateCreate statecreate;
StateJoin statejoin;

Events events;

int mousex, mousey;

// joystick constants
const int maxjaxis = 10;
const int maxjoysticks = 10;

int debug = 1;
bool multiplayer = false;

Server *server;
Client *client;

Rotation camrot;
Vector3 cam;
float view_x = 0;
float view_y = 0;

float blackout = 0;
float redout = 0;

// pre-defined screen resolutions (x, y, bpp, fullscreen)
int resolution [4] [4] =
        { { 1024, 768, 32, 1 },
          { 800, 600, 32, 1 },
          { 640, 480, 32, 1 },
          { 640, 480, 32, 0 } };

float nearclippingplane = 0.25; // do NOT lower this!

SoundSystem *sound = NULL;

Texture *texradar1, *texradar2, *texarrow;

MapLoader *maploader;

float visibleangle = 80.0F;







Texture *texsun, *texflare1, *texflare2, *texflare3, *texflare4, *texmoon = NULL, *texcross, *texcross2, *texranks, *texmedals;
Texture *texclouds1, *texclouds2, *texclouds3;
Texture *textitle;

GlLandscape *l = NULL;
Font *font1, *font2;

Uint32 lasttime = 0;

PilotList *pilots;

ObjectList<Explosion *> explosion;
ObjectList<BlackSmoke *> blacksmoke;
ObjectList<DynamicObj *> flare;
ObjectList<DynamicObj *> chaff;
ObjectList<AiObj *> missile;
ObjectList<DynamicObj *> laser;
ObjectList<AiObj *> fighter;
std::vector<Star *> star; // not an object => not in ObjectList
ObjectList<DynamicObj *> groundobj;
Flash *flash1;

Space *space;

AiObj *fplayer;
SpaceObj *sphere;
Sphere *objsphere;
HighClouds *highclouds;
HighClouds *highclouds2;

Model3d *obj, *objlaser, *objmissile;
Vector3 tlnull (0, 0, 0);
Vector3 tlinf (1E10, 1E10, 1E10);
Vector3 tlminf (-1E10, -1E10, -1E10);
Rotation rotnull (0, 0, 0);
Rotation rotmissile (90, 0, 270);

GLenum polygonMode = GL_FILL;

GLfloat light_position0[] = { 0.0, 0.5, -1.0, 0 };

#define max(x,y) ((x)>(y)?(x):(y))

int NX, NY;

int joysticks;

Mission *mission = NULL;
Mission *missionnew = NULL;

Container allmenus;
Container famemenu;
Container fightermenu;
Container missionmenu;
Container quitmenu;
Container statsmenu;
Component *currentsubmenu = NULL;
Component *currentoptmenu = NULL;
Container *mainmenu, *submenu [10], *optmenu [5], *controlsmenu [5];
Button *mainbutton [10];
Button *optbutton [10];



bool firststart = false;

bool missionactive = false;

void drawRank (float xp, float yp, float zp, int rank, float zoom)
{
  rank --;
  if (rank < 0) return;
  float x = xp / 10.0, y = yp / 10.0, z = zp;
  float tx1 = 0.5 * (rank % 2);
  float tx2 = tx1 + 0.5;
  float ty1 = 0.755 - 0.25 * (rank / 2);
  float ty2 = ty1 + 0.24;
  zoom /= 10;
  gl.enableTexture (texranks->textureID);
  if (antialiasing) gl.enableLinearTexture (texranks->textureID, texranks->mipmap);
  else gl.disableLinearTexture (texranks->textureID, texranks->mipmap);
  gl.enableAlphaBlending ();
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, 0.35);

  glBegin (GL_QUADS);
  glColor4ub (255, 255, 255, 200);
  glTexCoord2f (tx1, ty1);
  glVertex3f (x, y, z);
  glTexCoord2f (tx2, ty1);
  glVertex3f (x + zoom * 2, y, z);
  glTexCoord2f (tx2, ty2);
  glVertex3f (x + zoom * 2, y + zoom, z);
  glTexCoord2f (tx1, ty2);
  glVertex3f (x, y + zoom, z);
  glEnd ();

  glDisable (GL_ALPHA_TEST);
  gl.disableAlphaBlending ();
  glDisable (GL_TEXTURE_2D);

// Example how to pass the glBegin()...glEnd() code using vertex lists
/*  float vertex [sizeof (float) * 9];
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer (3,	GL_FLOAT,	sizeof (float) * 9, &vertex [0]);
	glColorPointer (4, GL_FLOAT, sizeof (float) * 9, &vertex [3]);
	glTexCoordPointer (2, GL_FLOAT, sizeof (float) * 9, &vertex [7]);

  int mz = 0;
  vertex [mz ++] = x; vertex [mz ++] = y; vertex [mz ++] = z;
  vertex [mz ++] = 1; vertex [mz ++] = 1; vertex [mz ++] = 1; vertex [mz ++] = 200.0F/255.0F;
  vertex [mz ++] = tx1; vertex [mz ++] = ty1;
  vertex [mz ++] = x + zoom * 2; vertex [mz ++] = y; vertex [mz ++] = z;
  vertex [mz ++] = 1; vertex [mz ++] = 1; vertex [mz ++] = 1; vertex [mz ++] = 200.0F/255.0F;
  vertex [mz ++] = tx2; vertex [mz ++] = ty1;
  vertex [mz ++] = x + zoom * 2; vertex [mz ++] = y + zoom; vertex [mz ++] = z;
  vertex [mz ++] = 1; vertex [mz ++] = 1; vertex [mz ++] = 1; vertex [mz ++] = 200.0F/255.0F;
  vertex [mz ++] = tx2; vertex [mz ++] = ty2;
  vertex [mz ++] = x; vertex [mz ++] = y + zoom; vertex [mz ++] = z;
  vertex [mz ++] = 1; vertex [mz ++] = 1; vertex [mz ++] = 1; vertex [mz ++] = 200.0F/255.0F;
  vertex [mz ++] = tx1; vertex [mz ++] = ty2;

  glDrawArrays (GL_QUADS, 0, 4);*/
}

void drawMedal (float xp, float yp, float zp, int medal, float zoom, int missionid)
{
  if (medal < 0) return;
  if (missionid < MISSION_CAMPAIGN1 || missionid > MISSION_CAMPAIGN2) return;
  float x = xp / 10.0 - zoom / 20, y = yp / 10.0 - zoom / 20, z = zp;
  float tx1 = 0.5 * (medal % 2);
  float tx2 = tx1 + 0.5;
  float ty1 = 0.5 * (medal / 2);
  float ty2 = ty1 + 0.5;
  zoom /= 10;
  gl.enableTexture (texmedals->textureID);
  if (antialiasing) gl.enableLinearTexture (texmedals->textureID, texmedals->mipmap);
  else gl.disableLinearTexture (texmedals->textureID, texmedals->mipmap);
  gl.enableAlphaBlending ();
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, 0.1);
  glBegin (GL_QUADS);
  glColor4ub (255, 255, 255, 200);
  glTexCoord2f (tx1, ty1);
  glVertex3f (x, y, z);
  glTexCoord2f (tx2, ty1);
  glVertex3f (x + zoom, y, z);
  glTexCoord2f (tx2, ty2);
  glVertex3f (x + zoom, y + zoom, z);
  glTexCoord2f (tx1, ty2);
  glVertex3f (x, y + zoom, z);
  glEnd ();
  glDisable (GL_ALPHA_TEST);
  gl.disableAlphaBlending ();
  glDisable (GL_TEXTURE_2D);
}

void drawMouseCursor ()
{
  glPushMatrix ();
  glMatrixMode (GL_PROJECTION);
  glPushMatrix ();
  glLoadIdentity ();
  //gluOrtho2D (0, width, 0, height);
  glScalef (1, -1, 1);
  glTranslatef (0, -height, 0);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  gl.enableAlphaBlending ();
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, 0.1);
  glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  gl.enableTexture (texcross2->textureID);
  int crossradius = width / 35;

  glTranslatef (mousex, mousey, 0);
  glBegin (GL_QUADS);
  glColor4ub (255, 255, 255, 255);
  glTexCoord2i (0, 0);
  glVertex2d (-crossradius, -crossradius);
  glTexCoord2i (0, 1);
  glVertex2d (-crossradius, crossradius);
  glTexCoord2i (1, 1);
  glVertex2d (crossradius, crossradius);
  glTexCoord2i (1, 0);
  glVertex2d (crossradius, -crossradius);
  glEnd ();
  glDisable (GL_ALPHA_TEST);
  glDisable (GL_TEXTURE_2D);
  gl.disableAlphaBlending ();
  glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glMatrixMode (GL_PROJECTION);
  glPopMatrix ();
  glMatrixMode (GL_MODELVIEW);
  glPopMatrix ();

//  glPopMatrix ();
}

int getMedal (int score)
{
  if (score >= 350) return 1;
  else if (score >= 300) return 0;
  else if (score >= 250) return 3;
  else if (score >= 200) return 2;
  return -1;
}

/*void gl117_rotateColors (int inittimer_gl117)
{
  int i;
  for (i = 0; i < model_gl117.numObjects; i ++)
  {
    for (int i2 = 0; i2 < model_gl117.object [i]->numVertices; i2 ++)
    {
      model_gl117.object [i]->vertex [i2].color.c [0] = (int) (75.0F * SIN(i2 * 100 + inittimer_gl117 / 2) + 155.0F);
      model_gl117.object [i]->vertex [i2].color.c [1] = model_gl117.object [i]->vertex [i2].color.c [0];
      model_gl117.object [i]->vertex [i2].color.c [2] = 100;
    }
  }
}*/

void adjustBrightness ()
{
  // adjust brightness setting (blending)
  if (brightness < 0)
  {
    glColor4ub (0, 0, 0, -brightness);
    float xf = 2.0, yf = 1.5, zf = 1.0;
    glDisable (GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin (GL_QUADS);
    glVertex3f (-xf, -yf, -zf);
    glVertex3f (-xf, yf, -zf);
    glVertex3f (xf, yf, -zf);
    glVertex3f (xf, -yf, -zf);
    glEnd ();
    glDisable (GL_BLEND);
//    glEnable (GL_DEPTH_TEST);
  }
  else if (brightness > 0)
  {
    glColor4ub (255, 255, 255, brightness);
    float xf = 2.0, yf = 1.5, zf = 1.0;
    glDisable (GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin (GL_QUADS);
    glVertex3f (-xf, -yf, -zf);
    glVertex3f (-xf, yf, -zf);
    glVertex3f (xf, yf, -zf);
    glVertex3f (xf, -yf, -zf);
    glEnd ();
    glDisable (GL_BLEND);
//    glEnable (GL_DEPTH_TEST);
  }
}

SDL_Window *sdlwindow = NULL;
static SDL_GLContext context = NULL;

SDL_Joystick *sdljoystick [maxjoysticks];
int sdljoystickaxes [maxjoysticks];

int sdldisplay = true;
int sdlreshape = true;
Cockpit *cockpit;
float fps = 30;

int glutwindow;



bool initing = true;
int flash = 0;
int lastshield = 0;

int getTrainingIdFromValue (int n)
{
  int z = 0;
  if (n == z ++) return MISSION_TUTORIAL;
  else if (n == z ++) return MISSION_TUTORIAL2;
  else if (n == z ++) return MISSION_DOGFIGHT;
  else if (n == z ++) return MISSION_TUTORIAL3;
  else if (n == z ++) return MISSION_FREEFLIGHT1;
  else if (n == z ++) return MISSION_DEATHMATCH1;
  else if (n == z ++) return MISSION_DEATHMATCH2;
  else if (n == z ++) return MISSION_DEATHMATCH3;
  else if (n == z ++) return MISSION_TEAMBASE1;
  else if (n == z ++) return MISSION_WAVES1;
  else
  {
    return MISSION_CUSTOM1 + n - 10;
  }
  return 0;
}

int getCampaignIdFromValue (int n)
{
  int z = 0;
  if (n == z ++) return MISSION_TEST1;
  else if (n == z ++) return MISSION_TEST2;
  else if (n == z ++) return MISSION_TRANSPORT;
  else if (n == z ++) return MISSION_CONVOY;
  else if (n == z ++) return MISSION_DOGFIGHT2;
  else if (n == z ++) return MISSION_AIRBATTLE;
  else if (n == z ++) return MISSION_SADEFENSE;
  else if (n == z ++) return MISSION_SCOUT;
  else if (n == z ++) return MISSION_BASE;
  else if (n == z ++) return MISSION_DEPOT;
  else if (n == z ++) return MISSION_DEFEND1;
  else if (n == z ++) return MISSION_DOGFIGHT3;
  else if (n == z ++) return MISSION_TANK1;
  else if (n == z ++) return MISSION_CONVOY2;
  else if (n == z ++) return MISSION_SHIP1;
  else if (n == z ++) return MISSION_SHIP2;
  else if (n == z ++) return MISSION_SHIP3;
  else if (n == z ++) return MISSION_CANYON1;
  else if (n == z ++) return MISSION_CANYON2;
  else if (n == z ++) return MISSION_TUNNEL1;
  else if (n == z ++) return MISSION_CANYON3;
  else if (n == z ++) return MISSION_MOON1;
  else if (n == z ++) return MISSION_MOONBATTLE;
  else if (n == z ++) return MISSION_MOON2;
  else if (n == z ++) return MISSION_MOON3;
  return 0;
}

int getCampaignValueFromId (int n)
{
  int z = 0;
  if (n == MISSION_TEST1) z = 0;
  else if (n == MISSION_TEST2) z = 1;
  else if (n == MISSION_TRANSPORT) z = 2;
  else if (n == MISSION_CONVOY) z = 3;
  else if (n == MISSION_DOGFIGHT2) z = 4;
  else if (n == MISSION_AIRBATTLE) z = 5;
  else if (n == MISSION_SADEFENSE) z = 6;
  else if (n == MISSION_SCOUT) z = 7;
  else if (n == MISSION_BASE) z = 8;
  else if (n == MISSION_DEPOT) z = 9;
  else if (n == MISSION_DEFEND1) z = 10;
  else if (n == MISSION_DOGFIGHT3) z = 11;
  else if (n == MISSION_TANK1) z = 12;
  else if (n == MISSION_CONVOY2) z = 13;
  else if (n == MISSION_SHIP1) z = 14;
  else if (n == MISSION_SHIP2) z = 15;
  else if (n == MISSION_SHIP3) z = 16;
  else if (n == MISSION_CANYON1) z = 17;
  else if (n == MISSION_CANYON2) z = 18;
  else if (n == MISSION_TUNNEL1) z = 19;
  else if (n == MISSION_CANYON3) z = 20;
  else if (n == MISSION_MOON1) z = 21;
  else if (n == MISSION_MOONBATTLE) z = 22;
  else if (n == MISSION_MOON2) z = 23;
  else if (n == MISSION_MOON3) z = 24;
  return z;
}

void setPlaneVolume ()
{
  if (gamestate == &stateplay)
  {
    int lev = (int) ((float) 128 * fplayer->thrust / fplayer->getPrototype ()->maxthrust) - 32;
    sound->stop (SOUND_PLANE1);
    sound->engine = (int) (fplayer->thrust / fplayer->getPrototype ()->maxthrust * 20) - 10;
    sound->setVolume (SOUND_PLANE1, lev);
    sound->playLoop (SOUND_PLANE1);
  }
  else
  {
    sound->setVolume (SOUND_PLANE1, 0);
  }
}

void setLightSource (int gamma)
{
  light_position0 [0] = -COS(gamma);
  light_position0 [1] = SIN(gamma);
  light_position0 [2] = 0;

  glLightfv (GL_LIGHT0, GL_POSITION, light_position0);
}

/*Model3d *getModel (const UnitDescriptor &id)
{
  if (id == FalconDescriptor) return Model3dRegistry::get ("FalconDescriptor");
  else if (id == SwallowDescriptor) return Model3dRegistry::get ("SwallowDescriptor");
  else if (id == HawkDescriptor) return Model3dRegistry::get ("HawkDescriptor");
  else if (id == Hawk2Descriptor) return Model3dRegistry::get ("Hawk2Descriptor");
  else if (id == BuzzardDescriptor) return Model3dRegistry::get ("BuzzardDescriptor");
  else if (id == CrowDescriptor) return Model3dRegistry::get ("CrowDescriptor");
  else if (id == PhoenixDescriptor) return Model3dRegistry::get ("PhoenixDescriptor");
  else if (id == RedArrowDescriptor) return Model3dRegistry::get ("RedArrowDescriptor");
  else if (id == BlackBirdDescriptor) return Model3dRegistry::get ("BlackBirdDescriptor");
  else if (id == StormDescriptor) return Model3dRegistry::get ("StormDescriptor");
  else if (id == TransportDescriptor) return Model3dRegistry::get ("TransportDescriptor");
  else if (id == Transport2Descriptor) return Model3dRegistry::get ("Transport2Descriptor");
  else if (id == AamHs1Descriptor) return Model3dRegistry::get ("AamHs1Descriptor");
  else if (id == AamHs2Descriptor) return Model3dRegistry::get ("AamHs2Descriptor");
  else if (id == AamHs3Descriptor) return Model3dRegistry::get ("AamHs3Descriptor");
  else if (id == Agm1Descriptor) return Model3dRegistry::get ("Agm1Descriptor");
  else if (id == Agm2Descriptor) return Model3dRegistry::get ("Agm2Descriptor");
  else if (id == DfmDescriptor) return Model3dRegistry::get ("DfmDescriptor");
  else if (id == AamFf1Descriptor) return Model3dRegistry::get ("AamFf1Descriptor");
  else if (id == AamFf2Descriptor) return Model3dRegistry::get ("AamFf2Descriptor");
  else if (id == WieselDescriptor) return Model3dRegistry::get ("WieselDescriptor");
  else if (id == PantherDescriptor) return Model3dRegistry::get ("PantherDescriptor");
  else if (id == MobileSamDescriptor) return Model3dRegistry::get ("MobileSamDescriptor");
  else if (id == PickupDescriptor) return Model3dRegistry::get ("PickupDescriptor");
  else if (id == TruckDescriptor) return Model3dRegistry::get ("TruckDescriptor");
  else if (id == Truck2Descriptor) return Model3dRegistry::get ("Truck2Descriptor");
  else if (id == CruiserDescriptor) return Model3dRegistry::get ("CruiserDescriptor");
  else if (id == LightDestroyerDescriptor) return Model3dRegistry::get ("LightDestroyerDescriptor");
  else if (id == SacDescriptor) return Model3dRegistry::get ("SacDescriptor");
  else if (id == SamDescriptor) return Model3dRegistry::get ("SamDescriptor");
  else if (id == TentDescriptor) return Model3dRegistry::get ("TentDescriptor");
  else if (id == BigTentDescriptor) return Model3dRegistry::get ("BigTentDescriptor");
  else if (id == ContainerDescriptor) return Model3dRegistry::get ("Container");
  else if (id == HallDescriptor) return Model3dRegistry::get ("HallDescriptor");
  else if (id == Hall2Descriptor) return Model3dRegistry::get ("Hall2Descriptor");
  else if (id == OilrigDescriptor) return Model3dRegistry::get ("OilrigDescriptor");
  else if (id == ComplexDescriptor) return Model3dRegistry::get ("ComplexDescriptor");
  else if (id == RadarDescriptor) return Model3dRegistry::get ("RadarDescriptor");
  else if (id == MoonBaseDescriptor) return Model3dRegistry::get ("MoonBaseDescriptor");
  else if (id == DepotDescriptor) return Model3dRegistry::get ("DepotDescriptor");
  else if (id == LaserBarrierDescriptor) return Model3dRegistry::get ("LaserBarrierDescriptor");
  else if (id == AsteroidDescriptor) return Model3dRegistry::get ("AsteroidDescriptor");
  return Model3dRegistry::get ("FalconDescriptor");
}*/

int game_levelInit ()
{
  unsigned i;

  space->removeAllObjects ();

  fighter.clear ();
  laser.clear ();
  missile.clear ();
  flare.clear ();
  chaff.clear ();
  groundobj.clear ();
  star.clear ();

  // init all objects
/*  for (unsigned i = 0; i < fighter.size (); i ++)
  {
    fighter [i]->init ();
    fighter [i]->explode = 0;
    fighter [i]->trafo.translation.x = 0;
    fighter [i]->trafo.translation.z = -i * 10;
    fighter [i]->party = 0;
    fighter [i]->trafo.scaling.set (0.35, 0.35, 0.35);
    fighter [i]->deactivate ();
    fighter [i]->recheight = 15;
    fighter [i]->recthrust = 0.2;
    fighter [i]->recrot.theta = 0;
    fighter [i]->o = Model3dRegistry::get (FalconDescriptor.name);
  }
  for (i = 0; i < groundobj.size (); i ++)
  {
    groundobj [i]->init ();
    groundobj [i]->trafo.scaling.set (1, 1, 1);
    groundobj [i]->id = StaticPassiveBeginDescriptor;
    groundobj [i]->deactivate ();
  }*/

//  if (l != NULL) delete l;
  if (!multiplayer || isserver || !isserver) // clients do not need the mission
  {
    Mission *missionold = mission;
    mission = missionnew;
    missionnew = NULL;
    mission->difficulty = difficulty;
    mission->start ();
    if (mission->id >= MISSION_CUSTOM1 && mission->id <= MISSION_CUSTOM2)
    {
      if (((MissionCustom *) mission)->reterror)
      {
        DISPLAY_ERROR("Could not startup mission");
        delete mission;
        mission = missionold;
        return 0;
      }
    }
    if (missionold != NULL)
    { delete missionold; missionold = NULL; }
  }

  initing = true;
  flash = 0;

  if (clouds == 0) highclouds->setTexture (NULL);
  else if (clouds == 1) highclouds->setTexture (texclouds1);
  else if (clouds == 2) highclouds->setTexture (texclouds2);
  else if (clouds == 3) highclouds->setTexture (texclouds3);

  if (clouds == 0) highclouds2->setTexture (NULL);
  else if (clouds == 1) highclouds2->setTexture (texclouds2);
  else if (clouds == 2) highclouds2->setTexture (NULL);
  else if (clouds == 3) highclouds2->setTexture (NULL);

  // place missiles to racks
/*  for (i = 0; i < maxfighter; i ++)
  {
    if (fighter [i]->id >= FighterBeginDescriptor && fighter [i]->id <= AirEndDescriptor)
      for (i2 = 0; i2 < 4; i2 ++)
      {
        int type = fighter [i]->missilerack [i2];
        Model3d *rm = getModel (MissileBeginDescriptor + type);
        fighter [i]->refModel [i2 * 3] = rm;
        fighter [i]->refModel [i2 * 3 + 1] = rm;
        fighter [i]->refModel [i2 * 3 + 2] = rm;
        fighter [i]->refTl [i2 * 3 + 1].z = fighter [i]->refTl [i2 * 3].z - 0.04;
        fighter [i]->refTl [i2 * 3 + 2].z = fighter [i]->refTl [i2 * 3].z + 0.04;
        fighter [i]->refTl [i2 * 3].y = fighter [i]->refTl [i2 * 3 + 1].y - 0.04;
        int tmp = fighter [i]->missilerackn [i2];
        fighter [i]->refScale [i2 * 3] = 0.25;
        fighter [i]->refScale [i2 * 3 + 1] = 0.25;
        fighter [i]->refScale [i2 * 3 + 2] = 0.25;
        if (tmp < 3) fighter [i]->refScale [i2 * 3] = 0;
        if (tmp < 2) fighter [i]->refScale [i2 * 3 + 1] = 0;
      }
  }*/

  // initialize object's height over the surface
  for (i = 0; i < fighter.size (); i ++)
  {
    if (fighter [i]->id >= StaticAaBeginDescriptor && fighter [i]->id <= StaticAaEndDescriptor)
    {
      fighter [i]->trafo.translation.y = l->getExactHeight (fighter [i]->trafo.translation.x, fighter [i]->trafo.translation.z) + fighter [i]->trafo.scaling.x;
    }
    else if (fighter [i]->id == LightDestroyerDescriptor)
    {
      fighter [i]->trafo.translation.y = l->getExactHeight (fighter [i]->trafo.translation.x, fighter [i]->trafo.translation.z) + fighter [i]->trafo.scaling.x / 4;
    }
    else if (fighter [i]->id == CruiserDescriptor)
    {
      fighter [i]->trafo.translation.y = l->getExactHeight (fighter [i]->trafo.translation.x, fighter [i]->trafo.translation.z) + fighter [i]->trafo.scaling.x / 4;
    }
    else if (fighter [i]->id >= TankBeginDescriptor && fighter [i]->id <= TankEndDescriptor)
    {
      fighter [i]->trafo.translation.y = l->getExactHeight (fighter [i]->trafo.translation.x, fighter [i]->trafo.translation.z) + fighter [i]->trafo.scaling.x * 0.55;
    }
    else if (fighter [i]->id == TentDescriptor)
    {
      fighter [i]->trafo.translation.y = l->getExactHeight (fighter [i]->trafo.translation.x, fighter [i]->trafo.translation.z);
    }
    else if (fighter [i]->id == ContainerDescriptor)
    {
      fighter [i]->trafo.translation.y = l->getExactHeight (fighter [i]->trafo.translation.x, fighter [i]->trafo.translation.z) + fighter [i]->trafo.scaling.x / 2;
    }
    else if (fighter [i]->id == HallDescriptor)
    {
      fighter [i]->trafo.translation.y = l->getExactHeight (fighter [i]->trafo.translation.x, fighter [i]->trafo.translation.z) + fighter [i]->trafo.scaling.x / 3;
    }
    else if (fighter [i]->id == Hall2Descriptor)
    {
      fighter [i]->trafo.translation.y = l->getExactHeight (fighter [i]->trafo.translation.x, fighter [i]->trafo.translation.z) + fighter [i]->trafo.scaling.x / 3;
    }
    else if (fighter [i]->id == BigTentDescriptor)
    {
      fighter [i]->trafo.translation.y = l->getExactHeight (fighter [i]->trafo.translation.x, fighter [i]->trafo.translation.z) + fighter [i]->trafo.scaling.x / 3;
    }
    else if (fighter [i]->id == OilrigDescriptor)
    {
      fighter [i]->trafo.translation.y = l->getExactHeight (fighter [i]->trafo.translation.x, fighter [i]->trafo.translation.z) + fighter [i]->trafo.scaling.x / 5;
    }
    else if (fighter [i]->id == ComplexDescriptor)
    {
      fighter [i]->trafo.translation.y = l->getExactHeight (fighter [i]->trafo.translation.x, fighter [i]->trafo.translation.z) + 0.55 * fighter [i]->trafo.scaling.x;
    }
    else if (fighter [i]->id == RadarDescriptor)
    {
      fighter [i]->trafo.translation.y = l->getExactHeight (fighter [i]->trafo.translation.x, fighter [i]->trafo.translation.z) + 0.7 * fighter [i]->trafo.scaling.x;
    }
    else if (fighter [i]->id == MoonBaseDescriptor)
    {
      fighter [i]->trafo.translation.y = l->getExactHeight (fighter [i]->trafo.translation.x, fighter [i]->trafo.translation.z) + 0.5 * fighter [i]->trafo.scaling.x;
    }
    else if (fighter [i]->id == DepotDescriptor)
    {
      fighter [i]->trafo.translation.y = l->getExactHeight (fighter [i]->trafo.translation.x, fighter [i]->trafo.translation.z) + 0.5 * fighter [i]->trafo.scaling.x;
    }
    else if (fighter [i]->id == LaserBarrierDescriptor)
    {
      fighter [i]->trafo.translation.y = l->getExactHeight (fighter [i]->trafo.translation.x, fighter [i]->trafo.translation.z) + 0.3 * fighter [i]->trafo.scaling.x;
    }
    else if (fighter [i]->id == MineDescriptor)
    {
      fighter [i]->trafo.translation.y = l->getHeight (fighter [i]->trafo.translation.x, fighter [i]->trafo.translation.z) + 5 + Math::random (20);
    }
    else if (fighter [i]->id == AsteroidDescriptor)
    {
      ;
    }
    else
    {
      fighter [i]->trafo.translation.y = l->getHeight (fighter [i]->trafo.translation.x, fighter [i]->trafo.translation.z) + 20;
    }
  }

/*  for (unsigned i = 0; i < laser.size (); i ++)
  {
    laser [i]->deactivate ();
    if (day)
    {
      laser [i]->o = Model3dRegistry::get ("Cannon1");
      laser [i]->trafo.scaling.set (0.08, 0.08, 0.08);
      laser [i]->drawLight = false;
    }
    else
    {
      laser [i]->o = Model3dRegistry::get ("Cannon2");
      laser [i]->trafo.scaling.set (0.1, 0.1, 0.1);
    }
  }

  for (i = 0; i < missile.size (); i ++)
  {
    missile [i]->init ();
    missile [i]->deactivate ();
  }

  for (i = 0; i < flare.size (); i ++)
  {
    flare [i]->init ();
    flare [i]->deactivate ();
  }

  for (i = 0; i < chaff.size (); i ++)
  {
    chaff [i]->init ();
    chaff [i]->deactivate ();
  } */

  for (i = 0; i < maxstar; i ++)
  {
    star.push_back (new Star (Math::random (360), Math::random (85), 0.6 + 0.15 * Math::random (8)));
  }

  Color skycolor;

  if (day && weather == WEATHER_SUNNY)
  {
    if (sungamma < 35)
    {
      skycolor.set ((unsigned short) (127 + 70 - 2 * sungamma), 127, 127);
      objsphere->setColor (skycolor);
    }
    else
    {
      skycolor.set (127, 127, 127 + 5);
      objsphere->setColor (skycolor);
    }
    if (sungamma < 35)
    {
      gl.setFogColor (static_cast<int>(127 + 70 - 2 * sungamma), 127, 127);
    }
    else
    {
      gl.setFogColor (127, 127, 132);
    }
    skycolor.set (50, 200, 255);
    objsphere->setNorthPoleColor (skycolor, 1.8);
    if (sungamma < 35)
    {
      skycolor.set ((unsigned short) (180 + 70 - 2 * sungamma), 180, 180);
      objsphere->setPoleColor (270, (int) (90 - sungamma), skycolor, 0.3);
    }
    else
    {
      skycolor.set (200, 200, 200);
      objsphere->setPoleColor (270, (int) (90 - sungamma), skycolor, 0.3);
    }
  }
  else if (!day && weather == WEATHER_SUNNY)
  {
    skycolor.set (64, 64, 64);
    objsphere->setColor (skycolor);
    gl.setFogColor (64, 64, 64);
    if (l->type != LAND_MOON)
    {
      skycolor.set (0, 0, 170);
      objsphere->setNorthPoleColor (skycolor, 1.8);
      skycolor.set (64, 64, 64);
      objsphere->setPoleColor (270, (int) (90 - sungamma), skycolor, 0.3);
    }
    else
    {
      skycolor.set (0, 0, 0);
      objsphere->setNorthPoleColor (skycolor, 1.8);
    }
  }
  else if (day && weather == WEATHER_THUNDERSTORM)
  {
    skycolor.set (102, 102, 102);
    objsphere->setColor (skycolor);
    gl.setFogColor (102, 102, 102);
    skycolor.set (102, 102, 102);
    objsphere->setNorthPoleColor (skycolor, 1.8);
  }
  else if (!day && weather == WEATHER_THUNDERSTORM)
  {
    skycolor.set (40, 40, 40);
    objsphere->setColor (skycolor);
    gl.setFogColor (40, 40, 40);
    skycolor.set (40, 40, 40);
    objsphere->setNorthPoleColor (skycolor, 1.8);
  }
  else if (weather == WEATHER_CLOUDY)
  {
    skycolor.set (20, 20, 20);
    objsphere->setColor (skycolor);
    gl.setFogColor (20, 20, 20);
  }
/*  glDeleteLists (objsphere->list1, 1);
  glDeleteLists (objsphere->list2, 1);
  glDeleteLists (objsphere->list3, 1);
  objsphere->list1 = -1;
  objsphere->list2 = -1;
  objsphere->list3 = -1; */

//  glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient );
/*  float light_specular [3] = {0.5, 0.5, 0.5};
  glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular );*/
  setLightSource ((int) sungamma);
  glLightModeli (GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
//  glLightModeli (GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
  glEnable (GL_LIGHT0);                // Turn on a light with defaults set
  float light_ambient [4] = {0.2, 0.2, 0.2, 1.0};
  float light_diffuse [4] = {1.0, 1.0, 1.0, 1.0};
  float light_specular [4] = {1.0, 1.0, 1.0, 1.0};
  glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular);
  glEnable (GL_LIGHTING);                // Turn on lighting
  glEnable (GL_COLOR_MATERIAL);            // Allow color

  glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
//  glDisable (GL_DITHER);

  fplayer->missiletype = fplayer->firstMissile ();
  initing = false;
  lastshield = (int) fplayer->shield;

  fps = 30;

  redout = 0;
  blackout = 0;

  if (!fplayer->ai)
    fplayer->realism = physics;

/*#ifdef HAVE_SDL_NET
  if (multiplayer)
  {
    if (threadnet != NULL)
      SDL_KillThread (threadnet);
    threadnet = SDL_CreateThread (net_thread_main, NULL);
  }
#endif*/

  return 1;
}

void playRandomMusic ()
{
  int r = Math::random (10) + 1;
  sound->loadMusic (static_cast<MusicId>(r));
  sound->playMusic (1);
#ifdef HAVE_SDL_MIXER
  Mix_HookMusicFinished (playRandomMusic);
#endif
}

void switch_menu ()
{
  gamestate = &statemenu;
  lasttime = 0;
  setLightSource ((int) sungamma);
  statemenu.reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
    playRandomMusic ();
  allmenus.visible = true;
  allmenus.components [0]->setVisible (true);
  if (currentsubmenu)
  {
    currentsubmenu->setVisible (true);
  }
  if (missionactive)
    mainbutton [6]->setVisible (true);
  else
    mainbutton [6]->setVisible (false);
  SDL_SetWindowGrab(sdlwindow, SDL_FALSE);
}

bool ispromoted;
int statsitemselected = 0;

void switch_stats ()
{
  gamestate = &statestats;
  lasttime = 0;
  setLightSource ((int) sungamma);
  statsitemselected = 0;
  int lastrank = pilots->pilot [pilots->aktpilot]->ranking;
  (void) pilots->pilot [pilots->aktpilot]->getRank (MISSION_CAMPAIGN1, MISSION_CAMPAIGN2 - 1);
  ispromoted = false;
  if (lastrank < pilots->pilot [pilots->aktpilot]->ranking)
    ispromoted = true;
  statestats.reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
  {
    int missionstate = mission->processtimer (1);
    if (missionstate == 1)
      sound->loadMusic (MUSIC_WINNER1);
    else
      sound->loadMusic (MUSIC_LOSER1);
    sound->playMusic ();
  }
}

void createMission (int missionid)
{
  if (missionid >= MISSION_MULTIPLAYER)
  {
    multiplayer = true;
  }
  else
  {
    multiplayer = false;
    isserver = false;
  }
  if (missionnew != NULL) delete missionnew;
  if (missionid == MISSION_DEMO) missionnew = new MissionDemo1 ();
  else if (missionid == MISSION_TEST1) missionnew = new MissionTest1 ();
  else if (missionid == MISSION_TEST2) missionnew = new MissionTest2 ();
  else if (missionid == MISSION_TRANSPORT) missionnew = new MissionTransport ();
  else if (missionid == MISSION_CONVOY) missionnew = new MissionConvoy ();
  else if (missionid == MISSION_DOGFIGHT2) missionnew = new MissionDogfight2 ();
  else if (missionid == MISSION_AIRBATTLE) missionnew = new MissionAirBattle ();
  else if (missionid == MISSION_SADEFENSE) missionnew = new MissionGround1 ();
  else if (missionid == MISSION_SCOUT) missionnew = new MissionScout ();
  else if (missionid == MISSION_BASE) missionnew = new MissionBase ();
  else if (missionid == MISSION_DEPOT) missionnew = new MissionDepot ();
  else if (missionid == MISSION_DEFEND1) missionnew = new MissionDefend1 ();
  else if (missionid == MISSION_DOGFIGHT3) missionnew = new MissionDogfight3 ();
  else if (missionid == MISSION_TANK1) missionnew = new MissionTank1 ();
  else if (missionid == MISSION_CONVOY2) missionnew = new MissionConvoy2 ();
  else if (missionid == MISSION_SHIP1) missionnew = new MissionShip1 ();
  else if (missionid == MISSION_SHIP2) missionnew = new MissionShip2 ();
  else if (missionid == MISSION_SHIP3) missionnew = new MissionShip3 ();
  else if (missionid == MISSION_CANYON1) missionnew = new MissionCanyon1 ();
  else if (missionid == MISSION_CANYON2) missionnew = new MissionCanyon2 ();
  else if (missionid == MISSION_CANYON3) missionnew = new MissionCanyon3 ();
  else if (missionid == MISSION_TUNNEL1) missionnew = new MissionTunnel1 ();
  else if (missionid == MISSION_MOON1) missionnew = new MissionMoonDefense1 ();
  else if (missionid == MISSION_MOONBATTLE) missionnew = new MissionMoonBattle ();
  else if (missionid == MISSION_MOON2) missionnew = new MissionMoonDogfight1 ();
  else if (missionid == MISSION_MOON3) missionnew = new MissionMoonBase1 ();
  else if (missionid == MISSION_TUTORIAL) missionnew = new MissionTutorial1 ();
  else if (missionid == MISSION_TUTORIAL2) missionnew = new MissionTutorial2 ();
  else if (missionid == MISSION_DOGFIGHT) missionnew = new MissionDogfight1 ();
  else if (missionid == MISSION_TUTORIAL3) missionnew = new MissionTutorial3 ();
  else if (missionid == MISSION_FREEFLIGHT1) missionnew = new MissionFreeFlight1 ();
  else if (missionid == MISSION_DEATHMATCH1) missionnew = new MissionDeathmatch1 ();
  else if (missionid == MISSION_DEATHMATCH2) missionnew = new MissionDeathmatch2 ();
  else if (missionid == MISSION_DEATHMATCH3) missionnew = new MissionDeathmatch3 ();
  else if (missionid == MISSION_TEAMBASE1) missionnew = new MissionTeamBase1 ();
  else if (missionid == MISSION_WAVES1) missionnew = new MissionWaves1 ();
  else if (missionid == MISSION_MULTIPLAYER_DOGFIGHT) missionnew = new MissionMultiDogfight1 ();
  else if (missionid >= MISSION_CUSTOM1 && missionid <= MISSION_CUSTOM2)
  {
    missionnew = new MissionCustom ();
    missionnew->id = missionid;
    ((MissionCustom *) missionnew)->init ();
  }
  if (mission != NULL)
  {
    if (mission->id == missionnew->id)
    {
      missionnew->wantfighter = mission->wantfighter;
      missionnew->wantweapon = mission->wantweapon;
    }
  }
}

void switch_mission (int missionid)
{
  gamestate = &statemission;
  lasttime = 0;
  setLightSource (60);
  createMission (missionid);
  if (missionid >= MISSION_CUSTOM1 && missionid <= MISSION_CUSTOM2)
  {
    if (((MissionCustom *) missionnew)->reterror)
    {
      DISPLAY_ERROR("Could not init mission");
      // play error sound
      switch_menu ();
      return;
    }
  }
  gamestate->reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
    playRandomMusic ();
}

void switch_fame ()
{
  gamestate = &statefame;
  lasttime = 0;
  setLightSource (60);
  gamestate->reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
    playRandomMusic ();
}

void switch_fighter ()
{
  gamestate = &statefighter;
  lasttime = 0;
  setLightSource (60);
  gamestate->reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
    playRandomMusic ();
}

void switch_create ()
{
  gamestate = &statecreate;
  lasttime = 0;
  setLightSource (60);
  isserver = true;
  if (server != NULL) delete server;
  server = new Server ();
  gamestate->reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
    playRandomMusic ();
}

void switch_join ()
{
  gamestate = &statejoin;
  lasttime = 0;
  setLightSource (60);
  isserver = false;
  if (client != NULL) delete client;
  client = new Client ();
  gamestate->reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
    playRandomMusic ();
}

int creditstimer, finishtimer;

void switch_credits ()
{
  gamestate = &statecredits;
  lasttime = 0;
  creditstimer = 50 * timestep;
  gamestate->reshape ();
  sound->stop (SOUND_PLANE1);
  sound->stop (SOUND_CANNON1);
}

void switch_finish ()
{
  gamestate = &statefinish;
  lasttime = 0;
  creditstimer = 50 * timestep;
  gamestate->reshape ();
  if (sound->musicplaying)
    sound->haltMusic ();
  sound->loadMusic (MUSIC_ELECTRO1);
  sound->playMusic ();
  sound->stop (SOUND_PLANE1);
  sound->stop (SOUND_CANNON1);
}

void switch_quit ()
{
  gamestate = &statequit;
  lasttime = 0;
  conf.saveSaveConfig (); // this configuration seems to work => save it
}

void switch_game ()
{
  gamestate = &stateplay;
  lasttime = 0;
  setLightSource ((int) sungamma);
  gamestate->reshape ();
  if (sound->musicplaying)
    sound->haltMusic ();
  sound->playLoop (SOUND_PLANE1);
  setPlaneVolume ();
  SDL_SetWindowGrab(sdlwindow, SDL_TRUE);
}

bool startcannon = false;

void Events::setAntialiasing ()
{
  if (antialiasing)
  {
    gl.enableLinearTexture (font1->texture->textureID, font1->texture->mipmap);
    gl.enableLinearTexture (font2->texture->textureID, font2->texture->mipmap);
    if (texmoon != NULL)
    {
      gl.enableLinearTexture (texmoon->textureID, texmoon->mipmap);
      gl.enableLinearTexture (texsun->textureID, texsun->mipmap);
      gl.enableLinearTexture (texearth->textureID, texearth->mipmap);
    }
  }
  else
  {
    gl.disableLinearTexture (font1->texture->textureID, font1->texture->mipmap);
    gl.disableLinearTexture (font2->texture->textureID, font2->texture->mipmap);
    if (texmoon != NULL)
    {
      gl.disableLinearTexture (texmoon->textureID, texmoon->mipmap);
      gl.disableLinearTexture (texsun->textureID, texsun->mipmap);
      gl.disableLinearTexture (texearth->textureID, texearth->mipmap);
    }
  }
}

void Events::fireCannon ()
{
  if (!fplayer->active) return;
  if (fplayer->firecannonttl > 0) return;
  if (fplayer->ammo == 0) return;
  fplayer->autofire = true;
}

void Events::stopCannon ()
{
  fplayer->autofire = false;
}

void Events::fireMissile ()
{
  if (!fplayer->active) return;
  if (fplayer->firemissilettl > 0) return;
  UnitDescriptor desc = UnitDescriptorRegistry::get (MissileBeginDescriptor + fplayer->missiletype);
  if (fplayer->fireMissile (desc, missile))
    sound->play (SOUND_MISSILE1, false);
}

void Events::fireChaff ()
{
  if (!fplayer->active) return;
  if (fplayer->firechaffttl > 0) return;
  Fighter *f = dynamic_cast<Fighter *>(fplayer);
  if (f && f->fireChaff (chaff, missile))
    sound->play (SOUND_CHAFF1, false);
}

void Events::fireFlare ()
{
  if (!fplayer->active) return;
  if (fplayer->fireflarettl > 0) return;
  Fighter *f = dynamic_cast<Fighter *>(fplayer);
  if (f && f->fireFlare (flare, missile))
    sound->play (SOUND_CHAFF1, false);
}

void Events::selectMissile ()
{
  if (!fplayer->active) return;
  fplayer->missiletype = fplayer->nextMissile (fplayer->missiletype);
}

void Events::targetNearest ()
{
  if (!fplayer->active) return;
  fplayer->targetNearestEnemy (fighter);
  sound->play (SOUND_CLICK1, false);
}

void Events::targetNext ()
{
  if (!fplayer->active) return;
  fplayer->targetNext (fighter);
  sound->play (SOUND_CLICK1, false);
}

void Events::targetPrevious ()
{
  if (!fplayer->active) return;
  fplayer->targetPrevious (fighter);
  sound->play (SOUND_CLICK1, false);
}

void Events::targetLocking ()
{
  if (!fplayer->active) return;
  fplayer->targetLockingEnemy (fighter);
  sound->play (SOUND_CLICK1, false);
}

void Events::thrustUp ()
{
  if (!fplayer->active) return;
  fplayer->thrustUp ();
  fplayer->thrustUp ();
}

void Events::thrustDown ()
{
  if (!fplayer->active) return;
  fplayer->thrustDown ();
  fplayer->thrustDown ();
}

int lastmousex = 0;

int lastmx = 0, lastmy = 0;

int jaxis [maxjaxis * maxjoysticks];

int getJoystickAxisIndex (int n)
{
  int idx = (n / 1000) * 10 + (n % 1000);
  if (idx < 0) idx = 0;
  if (idx >= maxjaxis) idx = 0;
  return idx;
}

int frames = 0, time2 = 0, time1 = 0;

void frame ()
{
  frames ++;
  time2 = SDL_GetTicks ();
  if (time2 - time1 > 1000)
  {
    if (time2 - time1 < 1500)
      fps = (float) frames * 1000.0 / (time2 - time1);
    time1 = time2;
    frames = 0;
  }
}

int missionending = 0;
int missionstate = 0;

int menuitem = 0;
int menutimer = 0;
int menuitemselected = -1;
int missionmenutimer;

void drawCircles (Color *colorstd)
{
  int i;
  float zf = -3, yf, xf;
  int t = 360 - (missionmenutimer * 10 / timestep) % 360;
  for (i = 0; i < 20; i ++)
  {
    yf = -3 + 0.3 * i; // - (float) (missionmenutimer & 63) / 64.0;
    glBegin (GL_QUAD_STRIP);
    for (int i2 = 0; i2 < 20; i2 ++)
    {
      xf = -3 + 0.3 * i2;
      yf = -3 + 0.3 * i;
      float cola = SIN((int) (sqrt (xf * xf + yf * yf) * 200 + t) % 360) / 10 + 0.2;
      if (colorstd == &colorblue) glColor3f (0, 0, cola);
      else glColor3f (cola, 0, 0);
      glVertex3f (xf, yf, zf);
      yf = -3 + 0.3 * (i + 1);
      cola = SIN((int) (sqrt (xf * xf + yf * yf) * 200 + t) % 360) / 10 + 0.2;
      if (colorstd == &colorblue) glColor3f (0, 0, cola);
      else glColor3f (cola, 0, 0);
      glVertex3f (xf, yf, zf);
    }
    glEnd ();
  }
}

void drawQuads (Color *colorstd)
{
  int i;
  float zf = -3;
  for (i = 0; i < 14; i ++)
  {
    glBegin (GL_QUAD_STRIP);
    for (int i2 = 0; i2 < 14; i2 ++)
    {
      float cola = SIN((i * 100+missionmenutimer*4 / timestep)) / 10 + SIN((i2 * 100)) / 10 + 0.2;
      if (colorstd == &colorblue) glColor3f (0, 0, cola);
      else glColor3f (cola, 0, 0);
      glVertex3f (-3 + 0.5 * i2, -3 + 0.5 * i, zf + SIN((i * 100)) / 2);
      cola = SIN(((i+1) * 100+missionmenutimer*4 / timestep)) / 10 + SIN((i2 * 100)) / 10 + 0.2;
      if (colorstd == &colorblue) glColor3f (0, 0, cola);
      else glColor3f (cola, 0, 0);
      glVertex3f (-3 + 0.5 * i2, -2.5 + 0.5 * i, zf + SIN(((i+1) * 100)) / 2);
    }
    glEnd ();
  }
}

void drawPlasma (Color *colorstd)
{
  int i;
  float yf;
  float zf1 = -2.98;
  float yind = 200.0 * sin (0.003 * missionmenutimer / timestep) + 200.0;
  float xind = 200.0 * cos (0.003 * missionmenutimer / timestep) + 200.0;
  float yind2 = -200.0 * sin (0.003 * missionmenutimer / timestep) + 200.0;
  float xind2 = -200.0 * cos (0.003 * missionmenutimer / timestep) + 200.0;
  for (i = 0; i < 21; i ++)
  {
    yf = -2.5 + 0.25 * i;
    float yif = yind + i * 5;
    int yi = (int) yif;
    float yr = yif - yi;
    float yif2 = yind2 + i * 5;
    int yi2 = (int) yif2;
    float yr2 = yif2 - yi2;
    glBegin (GL_QUAD_STRIP);
    for (int i2 = 0; i2 < 21; i2 ++)
    {
      float xf = -2.5 + 0.25 * i2;
      float xif = xind + i2 * 5;
      int xi = (int) xif;
      float xr = xif - xi;
      float xif2 = xind2 + i2 * 5;
      int xi2 = (int) xif2;
      float xr2 = xif2 - xi2;
      float h1 = xr * yr * l->h [yi + 1] [xi + 1] + (1.0 - xr) * (1.0 - yr) * l->h [yi] [xi] +
                 xr * (1.0 - yr) * l->h [yi] [xi + 1] + (1.0 - xr) * yr * l->h [yi + 1] [xi];
      float h2 = xr * yr * l->h [yi + 1 + 5] [xi + 1] + (1.0 - xr) * (1.0 - yr) * l->h [yi + 5] [xi] +
                 xr * (1.0 - yr) * l->h [yi + 5] [xi + 1] + (1.0 - xr) * yr * l->h [yi + 1 + 5] [xi];
      float h3 = xr2 * yr2 * l->h [yi2 + 1] [xi2 + 1] + (1.0 - xr2) * (1.0 - yr2) * l->h [yi2] [xi2] +
                 xr2 * (1.0 - yr2) * l->h [yi2] [xi2 + 1] + (1.0 - xr2) * yr2 * l->h [yi2 + 1] [xi2];
      float h4 = xr2 * yr2 * l->h [yi2 + 1 + 5] [xi2 + 1] + (1.0 - xr2) * (1.0 - yr2) * l->h [yi2 + 5] [xi2] +
                 xr2 * (1.0 - yr2) * l->h [yi2 + 5] [xi2 + 1] + (1.0 - xr2) * yr2 * l->h [yi2 + 1 + 5] [xi2];
      float intens = sin (0.15 * (h1 / 256 + 0.5 * missionmenutimer / timestep)) * 0.14 + 0.14;
      float intens2 = sin (0.15 * (h3 / 256 + 0.5 * missionmenutimer / timestep)) * 0.06 + 0.06;
      if (colorstd == &colorblue) glColor3f (intens2, 0, intens);
      else glColor3f (intens, 0, intens2);
      glVertex3f (xf, yf, zf1);
      intens = sin (0.15 * (h2 / 256 + 0.5 * missionmenutimer / timestep)) * 0.16 + 0.16;
      intens2 = sin (0.15 * (h4 / 256 + 0.5 * missionmenutimer / timestep)) * 0.08 + 0.08;
      if (colorstd == &colorblue) glColor3f (intens2, 0, intens);
      else glColor3f (intens, 0, intens2);
      glVertex3f (xf, yf + 0.25, zf1);
    }
    glEnd ();
  }
}

void pleaseWait ()
{
  float xf = 1.0, yf = 0.2, zf = -2.5;
  int c1 = 100, c2 = 10, c3 = 180;
  glBegin (GL_QUADS);
  glColor4ub (c2, c2, c2, 255);
  glVertex3f (-xf, -yf, zf);
  glColor4ub (c1, c1, c1, 255);
  glVertex3f (xf, -yf, zf);
  glColor4ub (c2, c2, c2, 255);
  glVertex3f (xf, yf, zf);
  glColor4ub (c1, c1, c1, 255);
  glVertex3f (-xf, yf, zf);
  glEnd ();
  glLineWidth (LINEWIDTH(1));
  glBegin (GL_LINE_STRIP);
  glColor4ub (c3, c3, c3, 255);
  glVertex3f (-xf, -yf, zf);
  glVertex3f (xf, -yf, zf);
  glVertex3f (xf, yf, zf);
  glVertex3f (-xf, yf, zf);
  glVertex3f (-xf, -yf, zf);
  glEnd ();

  font1->drawTextCentered (0, -0.5, -1.5, "PLEASE WAIT...", colorwhite);
  stateplay.view ();
}

const char *getModelText (const UnitDescriptor &id)
{
  if (id == FalconDescriptor)
    return "THE FALCON IS A NIMBLE AND\nFAST FIGHTER PROVIDING\nBOTH AIR-AIR AND\nAIR-GROUND-MISSILES.";
  else if (id == HawkDescriptor)
    return "THE HAWK IS A TYPICAL\nALLROUNDER AND DESIGNED\nFOR GROUND ATTACKS.";
  else if (id == Hawk2Descriptor)
    return "THE HAWK II IS A ...";
  return "NO INFO";
}

/*char *getModelName (int id)
{
  if (id == FalconDescriptor)
    return "GL-16 FALCON";
  else if (id == HawkDescriptor)
    return "GL-22 HAWK";
  else if (id == Hawk2Descriptor)
    return "GL-23 HAWK II";
  else if (id == RedArrowDescriptor)
    return "GL-15 REDARROW";
  else if (id == PhoenixDescriptor)
    return "GL-117 PHOENIX";
  else if (id == StormDescriptor)
    return "GL-50 STORM";
  else if (id == CrowDescriptor)
    return "CROW";
  else if (id == BuzzardDescriptor)
    return "BUZZARD";
  else if (id == SwallowDescriptor)
    return "SWALLOW";
  else if (id == BlackBirdDescriptor)
    return "BLACKBIRD";
  else if (id == AamHs1Descriptor)
    return "AirBeginDescriptor-AirBeginDescriptor MISSILE PACK";
  else if (id == Agm1Descriptor)
    return "AirBeginDescriptor-GROUND MISSILE PACK";
  else if (id == DfmDescriptor)
    return "DUMB FIRE MISSILE PACK";
  return "NO INFO";
} */

void drawArrow (float x, float y, float w, float h)
{
  float zf = -3;
  gl.enableTexture (texarrow->textureID);
  gl.enableAlphaBlending ();
  glBegin (GL_QUADS);
  glColor3ub (180, 180, 180);

  if (w > h)
  {
    glTexCoord2f (0, 1);
    glVertex3f (x, y, zf);
    glTexCoord2f (0, 0);
    glVertex3f (x + w, y, zf);
    glTexCoord2f (1, 0);
    glVertex3f (x + w, y + h, zf);
    glTexCoord2f (1, 1);
    glVertex3f (x, y + h, zf);
  }
  else
  {
    glTexCoord2f (1, 0);
    glVertex3f (x, y - h, zf);
    glTexCoord2f (1, 1);
    glVertex3f (x, y, zf);
    glTexCoord2f (0, 1);
    glVertex3f (x + w, y, zf);
    glTexCoord2f (0, 0);
    glVertex3f (x + w, y - h, zf);
  }

  glEnd ();
  glDisable (GL_BLEND);
  glDisable (GL_TEXTURE_2D);
}

int aktfighter = 0;

//Model3d *explsphere;
Sphere *mysphere;

void game_quit ()
{
  unsigned i;
  volumesound = sound->volumesound;
  volumemusic = sound->volumemusic;
  conf.saveConfig ();
  conf.saveConfigInterface ();
  pilots->save (Directory::getSaves ("pilots"));
  DISPLAY_INFO("Pilots saved");
  for (i = 0; i < laser.size (); i ++)
    delete (laser [i]);
  for (i = 0; i < missile.size (); i ++)
    delete (missile [i]);
  for (i = 0; i < flare.size (); i ++)
    delete (flare [i]);
  for (i = 0; i < chaff.size (); i ++)
    delete (chaff [i]);
  for (i = 0; i < explosion.size (); i ++)
    delete (explosion [i]);
  for (i = 0; i < star.size (); i ++)
    delete (star [i]);
  for (i = 0; i < groundobj.size (); i ++)
    delete (groundobj [i]);
  for (i = 0; i < blacksmoke.size (); i ++)
    delete (blacksmoke [i]);
  delete pilots;
//  delete explsphere;
  delete objsphere;
  delete sphere;
  delete flash1;
  delete cockpit;
  delete font1;
  delete font2;
  delete space;
  if (l != NULL) delete l;
//  delete dirs;
//  delete gl;
//  SDL_CloseAudio();
//  SDL_FreeWAV(wave.sound);
//  SDL_Quit (); // done atexit()
  delete sound;
  exit (EXIT_NORMAL);
}



/****************************************************************************
  DISPLAY EVENTS
****************************************************************************/

void drawMissionElement (float x, float y, float z, int thismissionid, int missionid, int selected, char *string)
{
  Pilot *p = pilots->pilot [pilots->aktpilot];
  drawMedal (x - 0.8, y + 0.6, z, getMedal (p->mission_score [thismissionid]), 1.0, thismissionid);
}

void test ()
{
  view ++;
  if (view > VIEW_MAX) view = VIEW_MIN;
}

int selectMouse (int x, int y, int motionx, int motiony, int mode, bool shift)
{
//  glutSwapBuffers ();
	GLuint selectBuff [20 * 4];
	GLint hits, viewport [4];

	// Select buffer parameters
	glSelectBuffer (20 * 4, selectBuff);
	glGetIntegerv (GL_VIEWPORT, viewport);

	// Enter to selection mode
	glEnable (GL_DEPTH_TEST);
	glDisable (GL_LIGHTING);
	glMatrixMode (GL_PROJECTION);
	glPushMatrix ();
	glLoadIdentity ();

	// Set-up pick matrix
  //gluPickMatrix (x, viewport [3] - y, 5, 5, viewport);

	// Set our perpective transformation matrix
  ////gluPerspective (visibleangle, 1.0, nearclippingplane * GLOBALSCALE, view * GLOBALSCALE);
//  ////gluPerspective (60.0, 1.0, 0.2, 200.0);

	glMatrixMode (GL_MODELVIEW);

	// Render all scene and fill selection buffer
	glRenderMode (GL_SELECT);
  statemenu.display ();

	// Get hits and go back to normal rendering
	hits = glRenderMode (GL_RENDER);

	// Get first model in selection buffer stack
  GLuint *ptr = selectBuff;
  int mypicks = 0;
  int pickz2 = -1;
	if (hits > 0)
  {
    int i;
    GLuint z = 0xFFFFFFFF;
    for (i = 0; i < hits; i ++)
    {
      int names = *ptr;
      ptr ++;
      GLuint z1 = *ptr;
      ptr ++;
      ptr ++;
      if (motionx < 0 && motiony < 0)
      {
        if (z1 < z)
        {
          z = z1; pickz2 = *ptr; mypicks = 1;
        }
      }
      else
      {
        mypicks ++;
      }
      ptr += names;
      if (mypicks >= 20) break;
    }
  }

	glMatrixMode (GL_PROJECTION);

	glPopMatrix ();
	glMatrixMode (GL_MODELVIEW);

  DISPLAY_DEBUG(FormatString ("selectMouse: picks=%d, pickz=%d, shift=%d", mypicks, pickz2, shift));
  return pickz2;
}



/****************************************************************************
  GAME DATA INITIALIZATION
****************************************************************************/

// calculate missile rack positions for a fighter
/*void setMissiles (Model3d *model)
{
  int i;
  Vector3 tlmissile (0, 0.3, 0.3);
  for (i = 0; i < model->numObjects; i ++)
  {
    if (model->object [i]->numVertices == 4)
    {
      Object3d *o = model->object [i];
      float sumx = 0, sumz = 0;
      float maxy = 2;
      int i2;
      for (i2 = 0; i2 < o->numVertices; i2 ++)
      {
        sumx += o->vertex [i2].vector.x;
        if (o->vertex [i2].vector.y < maxy)
          maxy = o->vertex [i2].vector.y;
        sumz += o->vertex [i2].vector.z;
      }
      tlmissile.x = sumx / 4.0F;
      tlmissile.y = maxy;
      tlmissile.z = sumz / 4.0F;

      tlmissile.y = maxy;
      model->addRefPoint (tlmissile);
    }
  }
}*/

// initialize game data
void myInit ()
{
//  int i, i2;

  // initialize all global variables

/*  for (i = 0; i < maxgroundobj; i ++)
  {
    groundobj [i] = new DynamicObj (TentDescriptor, space, Model3dRegistry::get (TentDescriptor.name), 3);
  }*/

/*  for (i = 0; i < maxexplosion; i ++)
  {
    explosion [i] = new Explosion (space, explsphere);
  }

  for (i = 0; i < maxblacksmoke; i ++)
  {
    blacksmoke [i] = new BlackSmoke (space);
  } */

  // TODO: will be removed
/*  for (unsigned i = 0; i < fighter.size (); i ++)
  {
    fighter.add (new AiObj (FalconDescriptor, space, Model3dRegistry::get (FalconDescriptor.name), 0.4));
    fighter [i]->ref.clear ();
    for (unsigned i2 = 0; i2 < 12; i2 ++)
      fighter [i]->addRefModel (SpaceObj (Model3dRegistry::get (AamHs1Descriptor.name), Transformation (tlnull, rotmissile, Vector3 (0.2, 0.2, 0.2))));
  }*/

  highclouds = new HighClouds (25);
  highclouds->setTexture (texclouds3);

  highclouds2 = new HighClouds (25);
  highclouds2->setTexture (texclouds3);

  objsphere = new Sphere (1, 9, 1, 1, 1);
  sphere = new SpaceObj (objsphere, Transformation (Vector3 (), Rotation (), Vector3 (10.0, 10.0, 10.0)));
  sphere->trafo.rotation.gamma = 90;
  sphere->trafo.rotation.theta = 90;
  sphere->trafo.rotation.phi = 270;
  sphere->draw = 2;
  sphere->drawLight = false;

  flash1 = new Flash ();
/*
  for (unsigned i = 0; i < laser.size (); i ++)
  {
    laser.push_back (new DynamicObj (Cannon1Descriptor, space, Model3dRegistry::get ("Cannon1"), 0.07));
  }

  for (i = 0; i < missile.size (); i ++)
  {
    missile.push_back (new AiObj (AamHs1Descriptor, space, Model3dRegistry::get (AamHs1Descriptor.name), 0.1));
  }

  for (i = 0; i < flare.size (); i ++)
  {
    flare.push_bask (new DynamicObj (FlareDescriptor, space, Model3dRegistry::get (FlareDescriptor.name), 0.1));
  }

  for (i = 0; i < chaff.size (); i ++)
  {
    chaff.push_back (new DynamicObj (ChaffDescriptor, space, Model3dRegistry::get (ChaffDescriptor.name), 0.1));
  }*/

  for (unsigned i = 0; i < star.size (); i ++)
  {
    star.push_back (new Star (Math::random (360), Math::random (85), 0.4 + 0.1 * Math::random (8)));
  }

  cockpit = new Cockpit ();

  pleaseWait ();
  createMission (MISSION_DEMO);
  game_levelInit ();

  statemenu.reshape ();
}



/****************************************************************************
  INTRO
****************************************************************************/

Rotation rot;
Rotation rot2;
Vector3 tl;
Vector3 tl2;
int initexplode = 0;
int initexplode1 = 0;
int i;
int inittimer = 0;
int inittimer_gl117 = 0;

void initModel (const UnitDescriptor &desc, const std::string &filename)
{
  DISPLAY_DEBUG(std::string (" * ").append (desc.name));
  Model3d *model = Model3dFactory::getModel (Directory::getModels (filename.c_str ()));
  Model3dRegistry::add (desc.name, model);
  UnitDescriptorRegistry::add (desc);
}

// load game data (this method does not really belong to the intro itself)
void myFirstInit ()
{
  DISPLAY_DEBUG("Creating calculation tables");
//  mathtab_init ();

  DISPLAY_DEBUG("Creating advanced OpenGL methods");
//  gl = new GlPrimitives ();
//  frustum = new Frustum ();

  // create textures (OpenGL)
  DISPLAY_DEBUG("Loading textures");
  Load3ds load3ds;
  load3ds.setTextureDir (Directory::getTextures (""));
  texgrass = new Texture (Directory::getTextures ("grass1.tga"), 0, 1, false);
  texrocks = new Texture (Directory::getTextures ("rocks1.tga"), 0, 1, false);
  texwater = new Texture (Directory::getTextures ("water1.tga"), 0, 1, false);
  texsand = new Texture (Directory::getTextures ("sand1.tga"), 0, 1, false);
  texredsand = new Texture (Directory::getTextures ("redsand1.tga"), 0, 1, false);
  texredstone = new Texture (Directory::getTextures ("redstone2.tga"), 0, 1, false);
  texgravel1 = new Texture (Directory::getTextures ("gravel1.tga"), 0, 1, false);
  texglitter1 = new Texture (Directory::getTextures ("glitter.tga"), -1, 0, true);
  textree = new Texture (Directory::getTextures ("tree1.tga"), -1, 1, true);
  textreeu = new Texture (Directory::getTextures ("treeu1.tga"), -1, 1, true);
  textree2 = new Texture (Directory::getTextures ("tree2.tga"), -1, 1, true);
  textreeu2 = new Texture (Directory::getTextures ("treeu2.tga"), -1, 1, true);
  textree3 = new Texture (Directory::getTextures ("tree3.tga"), 3, 1, true);
  textreeu3 = new Texture (Directory::getTextures ("treeu3.tga"), 3, 1, true);
  textree4 = new Texture (Directory::getTextures ("tree4.tga"), 3, 1, true);
  textreeu4 = new Texture (Directory::getTextures ("treeu4.tga"), 3, 1, true);
  textree5 = new Texture (Directory::getTextures ("tree5.tga"), -1, 1, true);
  textreeu5 = new Texture (Directory::getTextures ("treeu5.tga"), -1, 1, true);
  texcactus1 = new Texture (Directory::getTextures ("cactus1.tga"), 3, 1, true);
  texcactusu1 = new Texture (Directory::getTextures ("cactusu1.tga"), 3, 1, true);
  texsmoke = new Texture (Directory::getTextures ("smoke1.tga"), -1, 1, true);
  texsmoke2 = new Texture (Directory::getTextures ("smoke2.tga"), -1, 1, true);
  texsmoke3 = new Texture (Directory::getTextures ("smoke3.tga"), 5, 1, true);
  texsun = new Texture (Directory::getTextures ("sun2.tga"), -1, 0, true);
  texmoon = new Texture (Directory::getTextures ("moon1.tga"), 2, 0, true);
  texearth = new Texture (Directory::getTextures ("earth.tga"), 0, 0, true);
  // TODO: why doesn't !mipmap work for the flares???
  texflare1 = new Texture (Directory::getTextures ("flare1.tga"), -1, 1, true);
  texflare2 = new Texture (Directory::getTextures ("flare2.tga"), -1, 1, true);
  texflare3 = new Texture (Directory::getTextures ("flare3.tga"), -1, 1, true);
  texflare4 = new Texture (Directory::getTextures ("flare4.tga"), -1, 1, true);
  texcross = new Texture (Directory::getTextures ("cross.tga"), -1, 1, true);
  texcross2 = new Texture (Directory::getTextures ("cross2.tga"), -1, 1, true);
  texranks = new Texture (Directory::getTextures ("ranks.tga"), 0, 0, true);
  texmedals = new Texture (Directory::getTextures ("medals.tga"), 0, 0, true);
  texclouds1 = new Texture (Directory::getTextures ("clouds1.tga"), -1, 1, true);
  texclouds2 = new Texture (Directory::getTextures ("clouds2.tga"), 4, 1, true);
  texclouds3 = new Texture (Directory::getTextures ("clouds3.tga"), 6, 1, true);
  texradar1 = new Texture (Directory::getTextures ("radar2.tga"), -1, 0, true);
  texradar2 = new Texture (Directory::getTextures ("radar1.tga"), -1, 0, true);
  texarrow = new Texture (Directory::getTextures ("arrow.tga"), -1, 0, true);

  DISPLAY_DEBUG("Loading Fonts");
  font1 = new Font (Directory::getTextures ("font1.tga"), 32, '!', 64);
//  font1 = new Font (Directory::getTextures ("font3.tga"), 37, '!', 100);
  font2 = new Font (Directory::getTextures ("font2.tga"), 32, '!', 64);

  DISPLAY_DEBUG("Loading 3ds models:");

  initModel (FalconDescriptor, "falcon.3ds");
  initModel (HawkDescriptor, "hawk.3ds");
  initModel (SwallowDescriptor, "swallow.3ds");
  initModel (Hawk2Descriptor, "hawk2.3ds");
  initModel (BuzzardDescriptor, "buzzard.3ds");
  initModel (CrowDescriptor, "crow.3ds");
  initModel (PhoenixDescriptor, "phoenix.3ds");
  initModel (RedArrowDescriptor, "redarrow.3ds");
  initModel (BlackBirdDescriptor, "blackbird.3ds");
  Model3d *model = Model3dRegistry::get (BlackBirdDescriptor.name);
  model->scaleTexture (0.3, 0.3);
  initModel (StormDescriptor, "storm.3ds");
  initModel (TransportDescriptor, "transport.3ds");
  initModel (Transport2Descriptor, "transport2.3ds");

  // cannon at daylight
  float cannoncube = 0.025;
  DISPLAY_DEBUG(" * cannon1.3ds");
  model = Model3dFactory::getModel (Directory::getModels ("cannon1.3ds"));
//  model->setName ("CANNON");
  model->nolight = true;
  model->cube.set (cannoncube, cannoncube, cannoncube);
  Model3dRegistry::add ("Cannon1", model);

  DISPLAY_DEBUG(" * cannon1b.3ds");
  model = Model3dFactory::getModel (Directory::getModels ("cannon1b.3ds"));
  model->nolight = true;
//  model->setName ("CANNON");
  model->cube.set (cannoncube, cannoncube, cannoncube);
  Model3dRegistry::add ("Cannon1b", model);

  // cannon at night
  DISPLAY_DEBUG(" * cannon2.3ds");
  model = Model3dFactory::getModel (Directory::getModels ("cannon2.3ds"));
  model->nolight = true;
  model->alpha = true;
  for (i = 0; i < model->object [0]->numVertices; i ++)
  {
    model->object [0]->vertex [i].color.c [0] = 255;
    model->object [0]->vertex [i].color.c [1] = 255;
    model->object [0]->vertex [i].color.c [2] = 0;
    model->object [0]->vertex [i].color.c [3] = 255;
  }
  model->object [0]->vertex [1].color.c [3] = 50;
  model->object [0]->vertex [2].color.c [3] = 50;
//  model->setName ("CANNON");
  model->cube.set (cannoncube, cannoncube, cannoncube);
  Model3dRegistry::add ("Cannon2", model);

  DISPLAY_DEBUG(" * cannon2b.3ds");
  model = Model3dFactory::getModel (Directory::getModels ("cannon2b.3ds"));
  model->nolight = true;
  model->alpha = true;
  for (int i2 = 0; i2 < 2; i2 ++)
  {
    for (i = 0; i < 4; i ++)
    {
      model->object [i2]->vertex [i].color.c [0] = 255;
      model->object [i2]->vertex [i].color.c [1] = 255;
      model->object [i2]->vertex [i].color.c [2] = 0;
      model->object [i2]->vertex [i].color.c [3] = 255;
    }
    model->object [i2]->vertex [1].color.c [3] = 50;
    model->object [i2]->vertex [2].color.c [3] = 50;
  }
  model->cube.set (cannoncube, cannoncube, cannoncube);
//  model->setName ("CANNON");
  Model3dRegistry::add ("Cannon2b", model);

  initModel (FlareDescriptor, "flare1.3ds");
  model = Model3dRegistry::get (FlareDescriptor.name);
  model->alpha = true;
  model->nolight = true;
  initModel (ChaffDescriptor, "chaff1.3ds");
  model = Model3dRegistry::get (ChaffDescriptor.name);
  model->alpha = true;
  model->nolight = true;

  initModel (AamHs1Descriptor, "missile1.3ds");
  initModel (AamHs2Descriptor, "missile2.3ds");
  initModel (AamHs3Descriptor, "missile3.3ds");
  initModel (Agm1Descriptor, "missile4.3ds");
  initModel (Agm2Descriptor, "missile5.3ds");
  initModel (DfmDescriptor, "missile6.3ds");
  initModel (AamFf1Descriptor, "missile7.3ds");
  initModel (AamFf2Descriptor, "missile8.3ds");

  initModel (SacDescriptor, "flak2.3ds");
  initModel (SamDescriptor, "flarak1.3ds");
  initModel (CruiserDescriptor, "ship1.3ds");
  initModel (TentDescriptor, "tent1.3ds");
//  initModel ("Gl-117 Logo", "gl-117.3ds");
  initModel (WieselDescriptor, "tank1.3ds");
  model = Model3dRegistry::get (WieselDescriptor.name);
  model->scaleTexture (0.5, 0.5);
  initModel (ContainerDescriptor, "container1.3ds");
  initModel (LightDestroyerDescriptor, "ship2.3ds");
  initModel (TruckDescriptor, "truck1.3ds");
  initModel (Truck2Descriptor, "truck2.3ds");
  initModel (MobileSamDescriptor, "trsam.3ds");
  initModel (PickupDescriptor, "pickup1.3ds");
  initModel (Pickup2Descriptor, "pickup2.3ds");
  initModel (PantherDescriptor, "tank2.3ds");
  model = Model3dRegistry::get (PantherDescriptor.name);
  model->scaleTexture (0.5, 0.5);
  initModel (BigTentDescriptor, "tent4.3ds");
  initModel (HallDescriptor, "hall1.3ds");
  initModel (Hall2Descriptor, "hall2.3ds");
  initModel (OilrigDescriptor, "oilrig.3ds");
  model = Model3dRegistry::get (OilrigDescriptor.name);
  model->alpha = true;
  initModel (ComplexDescriptor, "complex.3ds");
  model = Model3dRegistry::get (ComplexDescriptor.name);
  model->scaleTexture (0.08, 0.08);
  initModel (RadarDescriptor, "radar.3ds");
  initModel (MineDescriptor, "mine1.3ds");
  initModel (AsteroidDescriptor, "aster1.3ds");
  initModel (MoonBaseDescriptor, "base1.3ds");
  initModel (LaserBarrierDescriptor, "barrier.3ds");
  model = Model3dRegistry::get (LaserBarrierDescriptor.name);
  model->scaleTexture (10.0, 10.0);
  model->alpha = true;
  initModel (DepotDescriptor, "depot1.3ds");
  model = Model3dRegistry::get (DepotDescriptor.name);
  model->scaleTexture (2.0, 2.0);
  initModel (RubbleDescriptor, "rubble.3ds");
  initModel (HouseDescriptor, "house1.3ds");

  Sphere *explsphere = new Sphere ();
  explsphere->init (1, 9);
  Color explcolor (255, 255, 1);
  explsphere->setColor (explcolor);
  explsphere->alpha = true;
  for (i = 0; i < explsphere->object [0]->numVertices; i ++)
  {
    explsphere->object [0]->vertex [i].color.set (Math::random (100) + 155, Math::random (100) + 100, 0, Math::random (3) / 2 * 255);
  }
  Model3dRegistry::add (ExplosionDescriptor.name, explsphere);

  // enable Z-Buffer
  glEnable (GL_DEPTH_TEST);

  // fill polygons (GL_LINE for wireframe models)
  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

  DISPLAY_DEBUG("Setting up world geometry");
  space = new Space ();
//  space->drawLight = true;
//  space->z1.set (-ZOOM, -ZOOM, -ZOOM);
//  space->z2.set (ZOOM, ZOOM, ZOOM);
/*  clip1 = space->z1;
  clip2 = space->z2;
  clip1->x = -ZOOM;
  clip1->y = -ZOOM;
  clip1->z = -ZOOM;
  clip2->x = ZOOM;
  clip2->y = ZOOM;
  clip2->z = ZOOM;*/

  // prepare intro
  stateinit.reshape ();

  tl.x = 0.0;
  tl.y = 0.0;
  tl.z = -5.0;
  tl2.x = 0.0;
  tl2.y = 0.0;
  tl2.z = -5.0;
  rot.gamma = 270;
  rot2.phi = 90;
  rot2.gamma = 270;
  rot2.theta = 270;
  initexplode = 0;
  initexplode1 = 0;

  textitle = new Texture (Directory::getTextures ("patents.tga"), 0, false, true);

  sungamma = 60;
  setLightSource (60);

  events.setAntialiasing ();

  glLightModeli (GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
//  glLightModeli (GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
  glEnable (GL_LIGHT0);
  glEnable (GL_LIGHTING);
  glEnable (GL_COLOR_MATERIAL);

  glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
}

const int maxfx = 256;
const int maxfy = 64;

int heat [maxfy] [maxfx];
int heat2 [maxfy] [maxfx];

void genFireLine ()
{
  int i, i2;
  for (i = 0; i < maxfx; i ++)
  {
    heat [maxfy - 1] [i] = Math::random (400);
  }
  for (i = 0; i < 5; i ++)
  {
    int r = Math::random (maxfx - 7) + 3;
    for (i2 = -3; i2 <= 3; i2 ++)
    {
      heat [maxfy - 1] [r + i2] = 1200; // insert hot spots at the bottom line
    }
  }
}

unsigned char firetex [maxfx * maxfy * 4];

void proceedFire ()
{
  int i, i2;
  for (i = maxfy - 2; i >= 1; i --)
    for (i2 = 2; i2 < maxfx - 2; i2 ++)
    {
      heat2 [i] [i2] = 0 * heat [i + 1] [i2 - 2] + 4 * heat [i + 1] [i2 - 1] + 16 * heat [i + 1] [i2] + 4 * heat [i + 1] [i2 + 1] + 0 * heat [i + 1] [i2 + 2]; // heat diffusion
      heat2 [i] [i2] += 0 * heat [i] [i2 - 2] + 4 * heat [i] [i2 - 1] + 16 * heat [i] [i2] + 4 * heat [i] [i2 + 1] + 0 * heat [i] [i2 + 2]; // heat diffusion
      heat2 [i] [i2] /= 48;
      heat2 [i] [i2] -= (int) (300.0F / maxfy); // heat sink
      if (heat2 [i] [i2] < 0) heat2 [i] [i2] = 0;
    }

  memcpy (heat, heat2, maxfx * maxfy * sizeof (int)); // copy back buffer to heat array

  for (i = 0; i < maxfy; i ++)
  {
    for (i2 = 0; i2 < maxfx + 1; i2 ++)
    {
      // rotate through fire colors (white-yellow-red-black-blue-black)
      // col in [0...512]
      int yind = i;
      int h = heat [yind] [i2];
      int b = h * 5;
      if (h > 30) b = (60 - h) * 5;
      if (h >= 60) b = 0;
      h -= 50;
      int r = h * 2; // blend out late for red->black
      if (r > 255) r = 255;
      else if (r < 0) r = 0;
      h -= 127;
      int g = h * 2; // blend out for yellow->red
      if (g > 255) g = 255;
      else if (g < 0) g = 0;
      h -= 127;
      if (h > 0)
      {
        b = h - 256; // blend out early to get white->yellow
        if (b > 255) b = 255;
      }
      else if (b < 0) b = 0;
      int a = r >= b ? r : b; // alpha value: transparent after yellow-red phase
      glColor4ub (r, g, b, a);
      firetex [(i * maxfx + i2) * 4] = r;
      firetex [(i * maxfx + i2) * 4 + 1] = g;
      firetex [(i * maxfx + i2) * 4 + 2] = b;
      firetex [(i * maxfx + i2) * 4 + 3] = a;
    }
    glEnd ();
  }
  glPopMatrix ();
  glBindTexture (GL_TEXTURE_2D, 5000);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D (GL_TEXTURE_2D, 0, 4, maxfx, maxfy, 0, GL_RGBA, GL_UNSIGNED_BYTE, firetex);
}



/****************************************************************************
  ABSTRACT EVENT BRANCHES
****************************************************************************/

static void myKeyboardFunc (unsigned char uckey, int x, int y)
{
  int key = (int) uckey;
  if (key >= 'a' && key <= 'z') key = toupper (key);
  if (gamestate == &statemenu)
    allmenus.eventKey (key);
  gamestate->key (key, x, y);
}

static void myKeyboardFuncUp (unsigned char key, int x, int y)
{
  if (gamestate == &stateplay || gamestate == &statepause)
    gamestate->keyUp ((int) key, x, y);
}

static void mySpecialFunc (int key, int x, int y)
{
  if (gamestate == &stateplay || gamestate == &statepause)
    gamestate->key (key + 256, x, y);
  if (gamestate == &statemenu)
    allmenus.eventSpecial (key);
}

static void mySpecialFuncUp (int key, int x, int y)
{
  if (gamestate == &stateplay || gamestate == &statepause)
    gamestate->keyUp (key + 256, x, y);
}

static void myPassiveRelMotionFunc (int xrel, int yrel)
{
  if (gamestate == &stateplay && controls == CONTROLS_MOUSE)
    gamestate->mouseRelMotion (xrel, yrel);
}

static void myPassiveMotionFunc (int x, int y)
{
  mousex = x;
  mousey = y;

  if (gamestate == &stateplay)
  {
    gamestate->mouseMotion (x, y);
  }
  else if (gamestate == &statemenu)
  {
    allmenus.eventMouse (x, y, 0);
    gamestate->mouse (-1, -1, x, y);
  }
  else if (gamestate == &statestats)
  {
    statsmenu.eventMouse (x, y, 0);
    gamestate->mouse (-1, -1, x, y);
  }
  else if (gamestate == &statemission)
  {
    missionmenu.eventMouse (x, y, 0);
    gamestate->mouse (-1, -1, x, y);
  }
  else if (gamestate == &statefame)
  {
    famemenu.eventMouse (x, y, 0);
    gamestate->mouse (-1, -1, x, y);
  }
  else if (gamestate == &statefighter)
  {
    fightermenu.eventMouse (x, y, 0);
    gamestate->mouse (-1, -1, x, y);
  }
  else if (gamestate == &statecreate)
    gamestate->mouse (-1, -1, x, y);
  else if (gamestate == &statejoin)
    gamestate->mouse (-1, -1, x, y);
  else if (gamestate == &statequit)
  {
    quitmenu.eventMouse (x, y, 0);
    gamestate->mouse (-1, -1, x, y);
  }
}

static void myMouseFunc (int button, int state, int x, int y)
{
  if (gamestate == &stateplay && controls == CONTROLS_MOUSE)
  {
    gamestate->mouse (button, state, x, y);
  }
  else if (gamestate == &stateinit)
  {
    gamestate->mouse (button, state, x, y);
  }
  else if (gamestate == &statemenu)
  {
    allmenus.eventMouse (x, y, button);
//    gamestate->mouse (button, state, x, y);
  }
  else if (gamestate == &statemission)
  {
    missionmenu.eventMouse (x, y, button);
    gamestate->mouse (button, state, x, y);
  }
  else if (gamestate == &statestats)
  {
    statsmenu.eventMouse (x, y, button);
//    gamestate->mouse (button, state, x, y);
  }
  else if (gamestate == &statefame)
  {
    famemenu.eventMouse (x, y, button);
//    gamestate->mouse (button, state, x, y);
  }
  else if (gamestate == &statefighter)
  {
    fightermenu.eventMouse (x, y, button);
//    gamestate->mouse (button, state, x, y);
  }
  else if (gamestate == &statecreate)
  {
    gamestate->mouse (button, state, x, y);
  }
  else if (gamestate == &statejoin)
  {
    gamestate->mouse (button, state, x, y);
  }
  else if (gamestate == &statequit)
  {
    quitmenu.eventMouse (x, y, button);
//    gamestate->mouse (button, state, x, y);
  }
  else if (gamestate == &statecredits)
  {
    gamestate->mouse (button, state, x, y);
  }
}

static void myReshapeFunc (int width, int height)
{
  ::width = width;
  ::height = height;
  ::wantwidth = width;
  ::wantheight = height;
  gamestate->reshape ();
}

static void myDisplayFunc ()
{
  if (gamestate == &stateinit)
  {
    gamestate->display ();
    stateplay.view ();
  }
  else if (gamestate == &stateplay)
  {
    stateplay.display ();
    stateplay.view ();
  }
  else if (gamestate == &statepause)
  {
    stateplay.display ();
    gamestate->display ();
    stateplay.view ();
  }
  else if (gamestate == &statemenu)
  {
    if (camera == 50) camera = 0;
    stateplay.display ();
    gamestate->display ();
    stateplay.view ();
  }
  else if (gamestate == &statecredits)
  {
    if (camera == 50) camera = 0;
    stateplay.display ();
    gamestate->display ();
    stateplay.view ();
  }
  else if (gamestate == &statemission)
  {
    if (camera == 50) camera = 0;
    stateplay.display ();
    gamestate->display ();
    stateplay.view ();
  }
  else if (gamestate == &statestats)
  {
    if (camera == 50) camera = 0;
    stateplay.display ();
    gamestate->display ();
    stateplay.view ();
  }
  else if (gamestate == &statefame)
  {
    stateplay.display ();
    gamestate->display ();
    stateplay.view ();
  }
  else if (gamestate == &statefighter)
  {
    stateplay.display ();
    gamestate->display ();
    stateplay.view ();
  }
  else if (gamestate == &statecreate)
  {
    stateplay.display ();
    stateplay.view ();
  }
  else if (gamestate == &statejoin)
  {
    stateplay.display ();
    stateplay.view ();
  }
  else if (gamestate == &statefinish)
  {
    if (camera == 50) camera = 0;
    stateplay.display ();
    gamestate->display ();
    stateplay.view ();
  }
  else if (gamestate == &statequit)
  {
    stateplay.display ();
    gamestate->display ();
    stateplay.view ();
  }
}

static void myJoystickAxisFunc (int x, int y, int t, int r)
{
  if (gamestate == &stateplay && controls == CONTROLS_JOYSTICK)
    gamestate->joystickAxis (x, y, t, r);
}

static void myJoystickButtonFunc (int button)
{
  if (gamestate == &stateplay && controls == CONTROLS_JOYSTICK)
    gamestate->joystickButton (button);
  else if (gamestate == &statemenu)
  {
    allmenus.eventJoystick (button);
  }
}

static void myJoystickHatFunc (int hat)
{
  int normhat = hat;
  if (hat % 1000 == SDL_HAT_RIGHT) normhat = 100;
  if (hat % 1000 == SDL_HAT_UP) normhat = 101;
  if (hat % 1000 == SDL_HAT_LEFT) normhat = 102;
  if (hat % 1000 == SDL_HAT_DOWN) normhat = 103;
  normhat += (hat / 1000) * 1000;
  if (gamestate == &stateplay && controls == CONTROLS_JOYSTICK)
    gamestate->joystickHat (normhat);
  else if (gamestate == &statemenu)
  {
    allmenus.eventJoystick (normhat);
  }
}

static void myTimerFunc (int value)
{
  Uint32 akttime, dt;
  akttime = SDL_GetTicks ();
  if (lasttime == 0) dt = 1;
  else dt = akttime - lasttime;
  lasttime = akttime;

  if (dt > 1000)
  {
    dt = 1;
    if (gamestate == &stateplay && multiplayer)
    {
      DISPLAY_ERROR("Out of sync");
      switch_menu ();
    }
  }

  if (gamestate == &statemenu || gamestate == &statecredits || gamestate == &statemission ||
      gamestate == &statefame || gamestate == &statefighter || gamestate == &statequit)
  {
    if (!missionactive)
      stateplay.timer (dt);
    gamestate->timer (dt);
  }
  else
  {
    gamestate->timer (dt);
  }
}

Uint32 nexttime = 0;

int joystickbutton = -1;
bool joystickfirebutton = false;

// This loop emulates the glutMainLoop() of GLUT using SDL!!!
void sdlMainLoop ()
{
  int sym = 0;
  SDL_Event event;

    while (SDL_PollEvent (&event)) // process events
    {
      switch (event.type)
      {
        case SDL_MOUSEMOTION:
          myPassiveMotionFunc (event.motion.x, event.motion.y);
          if (gamestate == &stateplay && controls == CONTROLS_MOUSE && mouse_relative)
          {
            fplayer->rolleffect = 0;
            fplayer->elevatoreffect = 0;
            fplayer->ruddereffect = 0;
          }
          myPassiveRelMotionFunc (event.motion.xrel, event.motion.yrel);
          break;

        case SDL_MOUSEBUTTONDOWN:
          myMouseFunc (event.button.button, event.button.state, event.button.x, event.button.y);
          break;

        case SDL_KEYDOWN:
          if (event.key.keysym.sym & SDLK_SCANCODE_MASK)
            mySpecialFunc (event.key.keysym.sym, 0, 0);
          else
            myKeyboardFunc (event.key.keysym.sym, 0, 0);
          break;

        case SDL_KEYUP:
          sym = event.key.keysym.sym;
          if (sym == 8 || sym == 9 || sym == 13 || (sym >= 32 && sym <= 'z'))
            myKeyboardFuncUp (event.key.keysym.sym, 0, 0);
          else
            mySpecialFuncUp (event.key.keysym.sym, 0, 0);
          break;

        case SDL_JOYAXISMOTION:
          if (abs (event.jaxis.value) < 2000)
          {
            jaxis [event.jaxis.axis + event.jaxis.which * 10] = 0;
          }
          else
          {
            if (event.jaxis.value < 0)
              event.jaxis.value += 2500;
            else
              event.jaxis.value -= 2500;
            jaxis [event.jaxis.axis + event.jaxis.which * 10] = (int) event.jaxis.value * 32768 / 30268;
          }
          break;

        case SDL_JOYBUTTONDOWN:
          joystickbutton = event.jbutton.button + event.jbutton.which * 1000;
          if (joystickbutton == (int) joystick_firecannon)
            joystickfirebutton = true;
          myJoystickButtonFunc (joystickbutton);
          joystickbutton = -1;
          break;

        case SDL_JOYBUTTONUP:
          joystickbutton = event.jbutton.button + event.jbutton.which * 1000;
          if (joystickbutton == (int) joystick_firecannon)
            joystickfirebutton = false;
          joystickbutton = -1;
          break;

        case SDL_JOYHATMOTION:
          myJoystickHatFunc (event.jhat.value + event.jhat.which * 1000);
          break;

        case SDL_WINDOWEVENT:
          switch (event.window.event)
          {
            case SDL_WINDOWEVENT_SHOWN:
            case SDL_WINDOWEVENT_FOCUS_GAINED:
              sdlreshape = true;
              sdldisplay = true;
              break;
          }
          break;
      }
    }

    if (controls == CONTROLS_JOYSTICK)
    {
      int x = jaxis [getJoystickAxisIndex (joystick_aileron)];
      int y = jaxis [getJoystickAxisIndex (joystick_elevator)];
      int rudder = jaxis [getJoystickAxisIndex (joystick_rudder)];
      int throttle = jaxis [getJoystickAxisIndex (joystick_throttle)];
      myJoystickAxisFunc (x, y, rudder, throttle);
      view_x = (float) jaxis [getJoystickAxisIndex (joystick_view_x)]/(-328.0);
      view_y = (float) jaxis [getJoystickAxisIndex (joystick_view_y)]/(-328.0);
      if (joystickfirebutton)
        myJoystickButtonFunc (joystick_firecannon);
    }

    if (sdldisplay)
      myDisplayFunc ();
    sdldisplay = false;
    if (sdlreshape)
      myReshapeFunc (width, height);
    sdlreshape = false;
    myTimerFunc (1); // dummy value
}



/****************************************************************************
  STARTUP METHODS
****************************************************************************/

// set screen to (width, height, bpp, fullscreen), return 0 on error
int setScreen (int w, int h, int b, int f)
{
  int rgb_size [3];
  switch (b)
  {
    case 8:
      rgb_size [0] = 2;
      rgb_size [1] = 3;
      rgb_size [2] = 3;
      break;
    case 15:
    case 16:
      rgb_size [0] = 5;
      rgb_size [1] = 5;
      rgb_size [2] = 5;
      break;
    default:
      rgb_size [0] = 8;
      rgb_size [1] = 8;
      rgb_size [2] = 8;
      break;
  }
  SDL_GL_SetAttribute (SDL_GL_RED_SIZE, rgb_size [0]);
  SDL_GL_SetAttribute (SDL_GL_GREEN_SIZE, rgb_size [1]);
  SDL_GL_SetAttribute (SDL_GL_BLUE_SIZE, rgb_size [2]);
  SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
  if (sdlwindow)
  {
    int display = SDL_GetWindowDisplayIndex(sdlwindow);
    if (f)
    {
      if (SDL_SetWindowFullscreen(sdlwindow, SDL_WINDOW_FULLSCREEN) < 0)
      {
        return 0;
      }
      fullscreen = f;
      wantfullscreen = f;
      int num_modes = SDL_GetNumDisplayModes(display);
      for (int i = 0; i < num_modes; i++)
      {
        SDL_DisplayMode mode;
        if (SDL_GetDisplayMode(display, i, &mode) != 0)
        {
          if (mode.w == w && mode.h == h)
          {
            if (SDL_SetWindowDisplayMode(sdlwindow, &mode) < 0)
            {
              return 0;
            }
            width = w;
            height = h;
            wantwidth = w;
            wantheight = h;
            glViewport (0, 0, (GLint) w, (GLint) h);
          }
        }
      }
    }
    else
    {
      if (SDL_SetWindowFullscreen(sdlwindow, 0) < 0)
      {
        return 0;
      }
      fullscreen = f;
      wantfullscreen = f;
      SDL_SetWindowSize(sdlwindow, w, h);
      width = w;
      height = h;
      wantwidth = w;
      wantheight = h;
      glViewport (0, 0, (GLint) w, (GLint) h);
    }

    SDL_GLContext newcontext = SDL_GL_CreateContext(sdlwindow);
    if (!newcontext)
    {
      return 0;
    }
    SDL_GL_DeleteContext(context);
    context = newcontext;
    bpp = b;
  }
  else
  {
    Uint32 video_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    if (f)
    {
      video_flags |= SDL_WINDOW_FULLSCREEN;
    }
    sdlwindow = SDL_CreateWindow("GL-117", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, w, h, video_flags);

    if (!sdlwindow)
    {
      return 0;
    }
    context = SDL_GL_CreateContext(sdlwindow);
    if (!context)
    {
      SDL_DestroyWindow(sdlwindow);
      return 0;
    }

    glViewport (0, 0, (GLint) w, (GLint) h);

    // take over results in global variables
    width = w;
    height = h;
    bpp = b;
    fullscreen = f;
    wantwidth = w; // requested values for next restart
    wantheight = h;
    wantfullscreen = f;
  }
  return 1;
}

// get approximate speed by drawing polygons (obsolete)
int speedTest ()
{
  int frames = 0, time2 = 0, time1 = 0;

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  ////gluPerspective (visibleangle, 1.0, nearclippingplane, 20.0); // should be sqrt(2) or 1.5
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

    time1 = SDL_GetTicks ();

  while (time2 - time1 < 1000)
  {
    frames ++;
    time2 = SDL_GetTicks ();

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glRotatef (1, 0, 0, 1);
    for (float zf = -13; zf <= -3; zf += 0.05)
    {
      glBegin (GL_QUADS);
      glColor4d (255, 0, 0, 255);
      glVertex3f (-1, -1, zf);
      glVertex3f (-1, 1, zf);
      glVertex3f (1, 1, zf);
      glVertex3f (1, -1, zf);
      glEnd ();
    }

    gl.swapBuffers ();

  }
  return frames;
}

bool configinit = false; // has GLUT/SDL already been inited?

// test screen settings automatically
void config_test (int argc, char **argv)
{
  DISPLAY_INFO("No configuration file found. Testing...");
  int bppi [4];

  DISPLAY_INFO("Using SDL");
  if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0)
  {
    //assert (false);
    DISPLAY_FATAL(FormatString ("Couldn't initialize SDL: %s", SDL_GetError ()));
    exit (EXIT_INIT);
  }
  configinit = true;

  int valids = -1; // valid screen mode? (-1 = no mode)
  int n = 0;
  while (n < 4)
  {
    if (setScreen (resolution [n] [0], resolution [n] [1], resolution [n] [2], resolution [n] [3]))
    {
      bppi [n] = bpp; // store bpp setting
      valids = n;
      break; // first mode found => exit loop
    }
    else
    {
      bppi [n] = -1;
    }
    n ++;
  }

  if (valids == -1)
  {
    DISPLAY_FATAL("No working display modes found! Try editing the file conf yourself. You may not be able to play this game.");
    //assert (false);
    exit (EXIT_INIT);
  }

  // start with lowest quality/view settings
  quality = 0;
  view = 20;
  width = resolution [valids] [0];
  height = resolution [valids] [1];
  bpp = bppi [valids];
  fullscreen = resolution [valids] [3];
  wantwidth = width; // requested values for next restart
  wantheight = height;
  wantfullscreen = fullscreen;
}

// get startup help screen
void viewParameters ()
{
  DISPLAY(" ", LOG_NONE);
  DISPLAY("Usage: gl-117 [-h -v -dLEVEL]", LOG_NONE);
  DISPLAY(" ", LOG_NONE);
  DISPLAY("-h: Display this help screen and quit", LOG_NONE);
  DISPLAY("-v: Display version string and quit", LOG_NONE);
  DISPLAY("-dLEVEL: Set debug LEVEL to 0=silent...5=log all", LOG_NONE);
  DISPLAY(" ", LOG_NONE);
}

void checkargs (int argc, char **argv)
{
  int i; // temp counter

  for (i = 1; i < argc; i ++) // for each arg (argument/parameter)
  {
    if (argv [i] [1] == 'd') // change log/debug level
    {
      char *ptr = &argv [i] [2];
      Logging::setLevel (static_cast<LogLevel>(atoi (ptr)));
      if (Logging::loglevel < LOG_NONE || Logging::loglevel > LOG_DEBUG) // look at common.h for the constants
      {
        DISPLAY_FATAL("Invalid debug level");
        viewParameters ();
        exit (EXIT_COMMAND);
      }
      else
      {
        DISPLAY_INFO(FormatString ("Entering debug level %d", Logging::loglevel));
      }
    }
    else if (argv [i] [1] == 'v') // display version string
    {
      DISPLAY(PACKAGE_VERSION, LOG_NONE);
      exit (EXIT_NORMAL);
    }
    else if (argv [i] [1] == 'h') // display startup help screen
    {
      viewParameters ();
      exit (EXIT_NORMAL);
    }
    else
    {
      DISPLAY_FATAL("Invalid command line parameter");
      viewParameters ();
      exit (EXIT_COMMAND);
    }
  }
}

void textMouseButton (char *buf, int button)
{
  if (button == 0) strcpy (buf, "NONE");
  else snprintf (buf, sizeof(buf), "BUTTON%d", button);
}

int campaignstartid;
int trainingstartid;

void setJoystickAxisString (int joysetting, char *joystr)
{
  joystr [3] = 0;
  if (joysetting < 0)
  {
    strcpy (joystr, "N/A");
  }
  else
  {
    joystr [0] = 'A' + (joysetting / 1000);
    joystr [1] = 32;
    joystr [2] = '0' + (joysetting % 1000);
  }
}

char mainmenunames [10] [25];
char submenu1names [20] [25];
char submenu2names [30] [25];
char optmenunames [5] [25];
char controlsmenu0names [15] [25];

void createMenu ()
{
  char buf [256];
  char buf2 [256];
  int i;
  int z = 0;
  float xf = -2;
  float xfstep = 4;
  float yf = 12;
  float yfstep = 1.0;
  float xsubmenu = -1;
  float ysubmenu = 12;
  Button *button;
  Label *label;
  TextField *textfield;
  EditKey *editkey;
  EditField *editfield;
  EditJoystick *editjoystick;

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register main menu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  strcpy (mainmenunames [0], "PILOTS");
  strcpy (mainmenunames [1], "TRAINING");
  strcpy (mainmenunames [2], "CAMPAIGN");
  strcpy (mainmenunames [3], "OPTIONS");
#ifdef SHOW_SOFTWARE_PATENTS
  strcpy (mainmenunames [4], "SOFTWARE PATENTS");
#else
  strcpy (mainmenunames [4], "CREDITS");
#endif
  strcpy (mainmenunames [5], "QUIT");
  strcpy (mainmenunames [6], "RETURN");

  mainmenu = new Container ();
//  mainmenu->setBounds (-14.3, 12.2, 10.6, 7.4);
  allmenus.add (mainmenu);

  for (i = 0; i < 7; i ++)
  {
    submenu [i] = new Container ();
    allmenus.add (submenu [i]);
  }

/*  submenu [0]->setBounds (-1.3, 12.2, 14.1, 12.4);
  submenu [1]->setBounds (-1.3, 12.2, 12.6, 7.4);
  submenu [2]->setBounds (-1.3, 12.2, 12.6, 1.4);
  submenu [3]->setBounds (-1.3, 12.2, 14.1, 1.4);*/

  for (i = 0; i < 7; i ++)
  {
    mainbutton [i] = new Button (mainmenunames [i]);
    mainbutton [i]->setBounds (-14, yf, 10, yfstep - 0.1);
    yf -= yfstep;
    mainbutton [i]->setFunction (callbackMainMenu);
    mainmenu->add (mainbutton [i]);
  }

  currentsubmenu = submenu [0];
  mainbutton [4]->setFunction (callbackCredits);
  mainbutton [5]->setFunction (callbackQuit);
  mainbutton [6]->setVisible (false);
  mainbutton [6]->setFunction (callbackReturn);

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register pilots submenu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  xf = xsubmenu; yf = ysubmenu; xfstep = 13.5; yfstep = 1;
  snprintf (buf, sizeof(buf), "ACTIVE:");
  label = new Label (buf);
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  submenu [0]->add (label);
  yf -= yfstep;

  snprintf (buf, sizeof(buf), "     %s %s", pilots->pilot [pilots->aktpilot]->getShortRank (MISSION_CAMPAIGN1, MISSION_CAMPAIGN2 - 1).c_str (), pilots->pilot [pilots->aktpilot]->name.c_str ());
  label = new Label (buf);
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  submenu [0]->add (label);

  yf -= 2;
  for (i = 0; i < 5; i ++)
  {
    if (i < pilots->aktpilots)
      snprintf (buf, sizeof(buf), "     %s %s", pilots->pilot [i]->getShortRank (MISSION_CAMPAIGN1, MISSION_CAMPAIGN2 - 1).c_str (), pilots->pilot [i]->name.c_str ());
    else
      snprintf (buf, sizeof(buf), "N/A");
    button = new Button (buf);
    button->setFunction (callbackPilotsList);
    button->setBounds (xf, yf, xfstep, yfstep - 0.1);
    submenu [0]->add (button);
    yf -= yfstep;
  }

  yf -= 0.5;
  button = new Button ("DELETE (RIGHT MB)");
  button->setFunction (callbackPilotsDelete);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  submenu [0]->add (button);
  yf -= yfstep;

  yf -= 0.5;
  button = new Button ("ADD");
  button->setFunction (callbackPilotsAdd);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  submenu [0]->add (button);
  yf -= yfstep;

  editfield = new EditField (15);
  editfield->setBounds (xf, yf, xfstep, yfstep - 0.1);
  submenu [0]->add (editfield);
  yf -= yfstep;

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register training submenu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  z = 0;
  strcpy (submenu1names [z ++], "TUTORIAL: PILOTING");
  strcpy (submenu1names [z ++], "TUTORIAL: BOMBER");
  strcpy (submenu1names [z ++], "TUTORIAL: FIGHTER");
  strcpy (submenu1names [z ++], "TUTORIAL: AERODYNAMICS");
  strcpy (submenu1names [z ++], "FREE FLIGHT");
  strcpy (submenu1names [z ++], "DEATHMATCH");
  strcpy (submenu1names [z ++], "TEAM DEATHMATCH");
  strcpy (submenu1names [z ++], "CANNON DEATHMATCH");
  strcpy (submenu1names [z ++], "TEAM BASE");
  strcpy (submenu1names [z ++], "WAVES");

  xf = xsubmenu; yf = ysubmenu - 2; xfstep = 12; yfstep = 0.8;
  for (i = 0; i < 10; i ++)
  {
    button = new Button (submenu1names [i]);
    button->setBounds (xf, yf, xfstep, yfstep - 0.1);
    button->setFunction (callbackTraining);
    yf -= yfstep;
    if (i == 3) yf -= yfstep;
    submenu [1]->add (button);
  }

  trainingstartid = submenu [1]->components [0]->id;

  yf -= yfstep;
  for (i = 0; i < maploader->getMapNumber (); i ++)
  {
    char str [256];
    strcpy (str, maploader->getNextMap ());
    maploader->toUpper (str);
    button = new Button (str);
    button->setBounds (xf, yf, xfstep, yfstep - 0.1);
    button->setFunction (callbackTraining);
    yf -= yfstep;
    submenu [1]->add (button);
  }

  xf = xsubmenu; yf = ysubmenu; yfstep = 1;
  button = new Button ("FIGHTER INFO");
  button->setFunction (callbackFighter);
  button->setBounds (xf, yf, xfstep - 0.2, yfstep - 0.1);
  submenu [1]->add (button);
  xf += xfstep;

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register campaign submenu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  z = 0;
  strcpy (submenu2names [z ++], "FIRST TEST");
  strcpy (submenu2names [z ++], "SECOND TEST");
  strcpy (submenu2names [z ++], "TRANSPORT");
  strcpy (submenu2names [z ++], "CONVOY");
  strcpy (submenu2names [z ++], "DOGFIGHT");
  strcpy (submenu2names [z ++], "AIR BATTLE");
  strcpy (submenu2names [z ++], "SURFACE-AIR DEFENSE");
  strcpy (submenu2names [z ++], "VETERAN DOGFIGHT");
  strcpy (submenu2names [z ++], "BASE ATTACK");
  strcpy (submenu2names [z ++], "DEPOTS");
  strcpy (submenu2names [z ++], "DEFEND SAM");
  strcpy (submenu2names [z ++], "DESERT DOGFIGHT");
  strcpy (submenu2names [z ++], "TANK ASSAUT");
  strcpy (submenu2names [z ++], "SAM CONVOY");
  strcpy (submenu2names [z ++], "DESTROYERS");
  strcpy (submenu2names [z ++], "OILRIG");
  strcpy (submenu2names [z ++], "CRUISER");
  strcpy (submenu2names [z ++], "RADAR BASE");
  strcpy (submenu2names [z ++], "CANYON BATTLE");
  strcpy (submenu2names [z ++], "TUNNEL");
  strcpy (submenu2names [z ++], "MAIN BASE");
  strcpy (submenu2names [z ++], "TURRETS");
  strcpy (submenu2names [z ++], "MOON BATTLE");
  strcpy (submenu2names [z ++], "ELITE DOGFIGHT");
  strcpy (submenu2names [z ++], "SNEAKING");

  xf = xsubmenu; yf = ysubmenu - 2; xfstep = 12; yfstep = 0.8;
  for (i = 0; i < z; i ++)
  {
    button = new Button (submenu2names [i]);
    button->setFunction (callbackCampaign);
    button->setBounds (xf, yf, xfstep, yfstep - 0.1);
    yf -= yfstep;
    submenu [2]->add (button);
  }

  campaignstartid = submenu [2]->components [0]->id;

  xf = xsubmenu; yf = ysubmenu; yfstep = 1;
  button = new Button ("PILOTS RANKING");
  button->setFunction (callbackPilots);
  button->setBounds (xf, yf, xfstep - 0.2, yfstep - 0.1);
  submenu [2]->add (button);
  xf += xfstep;

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register options submenu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  xf = xsubmenu; yf = 12; xfstep = 4.6;
  strcpy (optmenunames [0], "VIDEO");
  strcpy (optmenunames [1], "AUDIO");
  strcpy (optmenunames [2], "GAME");

  for (i = 0; i < 3; i ++)
  {
    optmenu [i] = new Container ();
    allmenus.add (optmenu [i]);
  }

  for (i = 0; i < 3; i ++)
  {
    optbutton [i] = new Button (optmenunames [i]);
    optbutton [i]->setBounds (xf, yf, xfstep - 0.2, 1);
    xf += xfstep;
    optbutton [i]->setFunction (callbackMainMenu);
    submenu [3]->add (optbutton [i]);
  }

  currentoptmenu = optmenu [0];

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register video options submenu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  float xftab = 3;
  xf = xsubmenu; yf = 10; xfstep = 13.6; yfstep = 1;
  button = new Button ("QUALITY");
  button->setFunction (callbackQuality);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [0]->add (button);

  snprintf (buf, sizeof(buf), "%d", quality);
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  optmenu [0]->add (label);
  yf -= yfstep;

  button = new Button ("VIEW");
  button->setFunction (callbackView);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [0]->add (button);

  snprintf (buf, sizeof(buf), "%d", (int) view);
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  optmenu [0]->add (label);
  yf -= yfstep;

  button = new Button ("DITHERING");
  button->setFunction (callbackDithering);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [0]->add (button);

  if (dithering) strcpy (buf, "ON");
  else strcpy (buf, "OFF");
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  optmenu [0]->add (label);
  yf -= yfstep;

  button = new Button ("ANTIALIASING");
  button->setFunction (callbackAntialiasing);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [0]->add (button);

  if (antialiasing) strcpy (buf, "ON");
  else strcpy (buf, "OFF");
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  optmenu [0]->add (label);
  yf -= yfstep;

  button = new Button ("SPECIAL EFFECTS");
  button->setFunction (callbackSpecialEffects);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [0]->add (button);

  if (specialeffects) strcpy (buf, "ON");
  else strcpy (buf, "OFF");
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  optmenu [0]->add (label);
  yf -= yfstep;

  button = new Button ("DYN LIGHTING");
  button->setFunction (callbackDynamicLighting);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [0]->add (button);

  if (dynamiclighting) strcpy (buf, "ON");
  else strcpy (buf, "OFF");
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  optmenu [0]->add (label);
  yf -= yfstep;

  button = new Button ("BRIGHTNESS");
  button->setFunction (callbackBrightness);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [0]->add (button);

  snprintf (buf, sizeof(buf), "%d%%", brightness);
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  optmenu [0]->add (label);
  yf -= yfstep;

  yf -= yfstep / 2;
  snprintf (buf, sizeof(buf), "NEED RESTART:");
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf, yf, 2, yfstep - 0.3);
  optmenu [0]->add (label);
  yf -= yfstep;

  button = new Button ("RESOLUTION");
  button->setFunction (callbackResolution);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [0]->add (button);

  snprintf (buf, sizeof(buf), "%d*%d", wantwidth, wantheight);
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab * 2, yf, 2, yfstep - 0.1);
  optmenu [0]->add (label);
  yf -= yfstep;

  button = new Button ("FULLSCREEN");
  button->setFunction (callbackFullscreen);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [0]->add (button);

  if (wantfullscreen) snprintf (buf, sizeof(buf), "YES");
  else snprintf (buf, sizeof(buf), "NO");
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  optmenu [0]->add (label);
  yf -= yfstep;

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register sound options submenu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  yf = 10; xfstep = 13.6; yfstep = 1;
  button = new Button ("SOUND VOLUME");
  button->setFunction (callbackSound);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [1]->add (button);

  if (sound->audio) snprintf (buf, sizeof(buf), "%d%%", volumesound);
  else snprintf (buf, sizeof(buf), "N/A");
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  optmenu [1]->add (label);
  yf -= yfstep;

  button = new Button ("MUSIC VOLUME");
  button->setFunction (callbackMusic);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [1]->add (button);

  if (sound->audio) snprintf (buf, sizeof(buf), "%d%%", volumemusic);
  else snprintf (buf, sizeof(buf), "N/A");
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  optmenu [1]->add (label);
  yf -= yfstep;

  if (!sound->audio)
  {
    optmenu [1]->components [0]->setActive (false);
    optmenu [1]->components [1]->setActive (false);
    optmenu [1]->components [2]->setActive (false);
    optmenu [1]->components [3]->setActive (false);
  }

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register controls options submenu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  xftab = 6.8;
  xf = xsubmenu; yf = 10; xfstep = 13.5; yfstep = 1;
  button = new Button ("DIFFICULTY");
  button->setFunction (callbackDifficulty);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [2]->add (button);

  if (difficulty == 0) snprintf (buf, sizeof(buf), "%s", "EASY");
  else if (difficulty == 1) snprintf (buf, sizeof(buf), "%s", "NORMAL");
  else if (difficulty == 2) snprintf (buf, sizeof(buf), "%s", "HARD");
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  optmenu [2]->add (label);
  yf -= yfstep;

  button = new Button ("PHYSICS");
  button->setFunction (callbackPhysics);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [2]->add (button);

  if (!physics) snprintf (buf, sizeof(buf), "%s", "ACTION");
  else snprintf (buf, sizeof(buf), "%s", "SIMULATION");
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  optmenu [2]->add (label);
  yf -= yfstep;

  button = new Button ("CONTROLS");
  button->setFunction (callbackControls);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [2]->add (button);

  textControls (buf);
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  optmenu [2]->add (label);
  yf -= yfstep;

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register keyboard control submenu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  xf = xsubmenu + 1; yf = ysubmenu - 6; xfstep = 12.5; yfstep = 0.8;
  for (i = 0; i < 3; i ++)
  {
    controlsmenu [i] = new Container ();
    allmenus.add (controlsmenu [i]);
  }

  label = new Label ("ROLL:       \t   LEFT, RIGHT");
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [0]->add (label);
  yf -= yfstep;

  label = new Label ("ELEVATOR: \t   UP, DOWN");
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [0]->add (label);
  yf -= yfstep;

  label = new Label ("RUDDER:  \t   LSHIFT+ROLL");
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [0]->add (label);
  yf -= yfstep;

  label = new Label ("THROTTLE: \t   1...9");
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [0]->add (label);
  yf -= yfstep;

  label = new Label ("CAMERA:  \t   F1...F8");
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [0]->add (label);
  yf -= yfstep;

  z = 0;
  strcpy (controlsmenu0names [z ++], "FIRE CANNON");
  strcpy (controlsmenu0names [z ++], "FIRE MISSILE");
  strcpy (controlsmenu0names [z ++], "DROP FLARE");
  strcpy (controlsmenu0names [z ++], "DROP CHAFF");
  strcpy (controlsmenu0names [z ++], "SELECT MISSILE");
  strcpy (controlsmenu0names [z ++], "TARGET NEAREST");
  strcpy (controlsmenu0names [z ++], "TARGET LOCKING");
  strcpy (controlsmenu0names [z ++], "TARGET NEXT");
  strcpy (controlsmenu0names [z ++], "TARGET PREVIOUS");
  strcpy (controlsmenu0names [z ++], "INC THRUST");
  strcpy (controlsmenu0names [z ++], "DEC THRUST");

  int keys [11];
  keys [0] = key_firecannon;
  keys [1] = key_firemissile;
  keys [2] = key_dropflare;
  keys [3] = key_dropchaff;
  keys [4] = key_selectmissile;
  keys [5] = key_targetnearest;
  keys [6] = key_targetlocking;
  keys [7] = key_targetnext;
  keys [8] = key_targetprevious;
  keys [9] = key_thrustup;
  keys [10] = key_thrustdown;

  yf -= 0.5;
  for (i = 0; i < 11; i ++)
  {
    editkey = new EditKey (controlsmenu0names [i]);
    editkey->setKey (keys [i]);
    editkey->setFunction (callbackKeyboard);
    editkey->setBounds (xf, yf, xfstep, yfstep - 0.1);
    controlsmenu [0]->add (editkey);
    yf -= yfstep;
  }

  yf -= 1; yfstep = 1.0;
  button = new Button ("LOAD DEFAULTS");
  button->setFunction (callbackDefaults);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [0]->add (button);

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register mouse control submenu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  xftab = 3.5;
  xf = xsubmenu + 1; yf = ysubmenu - 6; xfstep = 12.5; yfstep = 0.8;
  button = new Button ("SENSITIVITY");
  button->setFunction (callbackMouseSensitivity);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [1]->add (button);

  snprintf (buf, sizeof(buf), "%d%%", (int) mouse_sensitivity);
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  controlsmenu [1]->add (label);
  yf -= yfstep;

  button = new Button ("REVERSE");
  button->setFunction (callbackMouseReverse);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [1]->add (button);

  if (mouse_reverse) snprintf (buf, sizeof(buf), "ON");
  else snprintf (buf, sizeof(buf), "OFF");
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  controlsmenu [1]->add (label);
  yf -= yfstep;

  button = new Button ("RELATIVE");
  button->setFunction (callbackMouseRelative);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [1]->add (button);

  if (mouse_relative) snprintf (buf, sizeof(buf), "ON");
  else snprintf (buf, sizeof(buf), "OFF");
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  controlsmenu [1]->add (label);
  yf -= yfstep;

  button = new Button ("AUTORUDDER");
  button->setFunction (callbackMouseAutorudder);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [1]->add (button);

  snprintf (buf, sizeof(buf), "%d%%", mouse_autorudder);
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  controlsmenu [1]->add (label);
  yf -= yfstep;

  yf -= 0.5;
  textMouseButton (buf2, mouse_firecannon);
  snprintf (buf, sizeof(buf), "FIRE CANNON:            \t %s", buf2);
  label = new Label (buf);
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [1]->add (label);
  yf -= yfstep;

  textMouseButton (buf2, mouse_firemissile);
  snprintf (buf, sizeof(buf), "FIRE MISSILE:            \t %s", buf2);
  label = new Label (buf);
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [1]->add (label);
  yf -= yfstep;

  textMouseButton (buf2, mouse_selectmissile);
  snprintf (buf, sizeof(buf), "SELECT MISSILE:      \t %s", buf2);
  label = new Label (buf);
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [1]->add (label);
  yf -= yfstep;

  yf -= 1; yfstep = 1.0;
  button = new Button ("LOAD DEFAULTS");
  button->setFunction (callbackMouseDefaults);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [1]->add (button);

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register joystick control submenu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  char joystr [4];

  xf = xsubmenu + 1; yf = ysubmenu - 6; xfstep = 12.5; yfstep = 0.8;
  button = new Button ("AILERON");
  button->setFunction (callbackJoystickAxis);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [2]->add (button);
  setJoystickAxisString (joystick_aileron, joystr);
  label = new Label (joystr);
  label->setBounds (xf + xfstep - xftab, yf, xftab, yfstep - 0.1);
  label->setTransparent (true);
  controlsmenu [2]->add (label);
  yf -= yfstep;

  button = new Button ("ELEVATOR");
  button->setFunction (callbackJoystickAxis);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [2]->add (button);
  setJoystickAxisString (joystick_elevator, joystr);
  label = new Label (joystr);
  label->setBounds (xf + xfstep - xftab, yf, xftab, yfstep - 0.1);
  label->setTransparent (true);
  controlsmenu [2]->add (label);
  yf -= yfstep;

  button = new Button ("THROTTLE");
  button->setFunction (callbackJoystickAxis);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [2]->add (button);
  setJoystickAxisString (joystick_throttle, joystr);
  label = new Label (joystr);
  label->setBounds (xf + xfstep - xftab, yf, xftab, yfstep - 0.1);
  label->setTransparent (true);
  controlsmenu [2]->add (label);
  yf -= yfstep;

  button = new Button ("RUDDER");
  button->setFunction (callbackJoystickAxis);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [2]->add (button);
  setJoystickAxisString (joystick_rudder, joystr);
  label = new Label (joystr);
  label->setBounds (xf + xfstep - xftab, yf, xftab, yfstep - 0.1);
  label->setTransparent (true);
  controlsmenu [2]->add (label);
  yf -= yfstep;

  button = new Button ("VIEW X");
  button->setFunction (callbackJoystickAxis);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [2]->add (button);
  setJoystickAxisString (joystick_view_x, joystr);
  label = new Label (joystr);
  label->setBounds (xf + xfstep - xftab, yf, xftab, yfstep - 0.1);
  label->setTransparent (true);
  controlsmenu [2]->add (label);
  yf -= yfstep;

  button = new Button ("VIEW Y");
  button->setFunction (callbackJoystickAxis);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [2]->add (button);
  setJoystickAxisString (joystick_view_y, joystr);
  label = new Label (joystr);
  label->setBounds (xf + xfstep - xftab, yf, xftab, yfstep - 0.1);
  label->setTransparent (true);
  controlsmenu [2]->add (label);
  yf -= yfstep;

  int buttons [11];
  buttons [0] = joystick_firecannon;
  buttons [1] = joystick_firemissile;
  buttons [2] = joystick_dropflare;
  buttons [3] = joystick_dropchaff;
  buttons [4] = joystick_selectmissile;
  buttons [5] = joystick_targetnearest;
  buttons [6] = joystick_targetlocking;
  buttons [7] = joystick_targetnext;
  buttons [8] = joystick_targetprevious;
  buttons [9] = joystick_thrustup;
  buttons [10] = joystick_thrustdown;

  yf -= 0.5;
  for (i = 0; i < 11; i ++)
  {
    editjoystick = new EditJoystick (controlsmenu0names [i]);
    editjoystick->setButton (buttons [i]);
    editjoystick->setFunction (callbackJoystick);
    editjoystick->setBounds (xf, yf, xfstep, yfstep - 0.1);
    controlsmenu [2]->add (editjoystick);
    yf -= yfstep;
  }

  yf -= 1; yfstep = 1.0;
  button = new Button ("LOAD DEFAULTS");
  button->setFunction (callbackDefaultsJoystick);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [2]->add (button);

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register top pilots menu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  xf = -10; yf = 9; xfstep = 20; yfstep = 1.2;
  snprintf (buf, sizeof(buf), "         PILOTS RANKING");
  label = new Label (buf);
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  famemenu.add (label);
  yf -= yfstep + 0.25;

  yfstep = 1.0;
  textfield = new TextField ("", yfstep - 0.1);
  textfield->setBounds (xf, yf, xfstep, (yfstep - 0.1) * 13);
  famemenu.add (textfield);
  yf -= yfstep * 13;

  yfstep = 1.1;
  snprintf (buf, sizeof(buf), "         BACK TO MAIN MENU");
  button = new Button (buf);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  button->setFunction (callbackSwitchMainMenu);
  famemenu.add (button);
  yf -= yfstep + 0.25;

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register fighter menu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  xf = -10; yf = 12; xfstep = 20; yfstep = 1.2;
  snprintf (buf, sizeof(buf), "          FIGHTER INFO");
  label = new Label (buf);
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  fightermenu.add (label);
  yf -= yfstep + 0.25;

  yfstep = 1.0;
  buf [0] = 0;
  label = new Label (buf);
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  fightermenu.add (label);
  yf -= yfstep + 0.25;

  yfstep = 1.1;
  textfield = new TextField ("", yfstep - 0.1);
  textfield->setBounds (xf, yf, xfstep, (yfstep - 0.1) * 5 + 0.15);
  fightermenu.add (textfield);
  yf -= yfstep * 5 + 0.05;

  button = new Button ("<");
  button->setBounds (xf, yf, 1.5, 1.5);
  button->setFunction (callbackFighterPrev);
  fightermenu.add (button);
  button = new Button (">");
  button->setBounds (-xf -1.5, yf, 1.5, 1.5);
  button->setFunction (callbackFighterNext);
  fightermenu.add (button);
  textfield = new TextField ("", yfstep - 0.1);
  textfield->setBounds (xf + 1.5, yf, 17, 12.5);
  fightermenu.add (textfield);
  yf -= 13;

  snprintf (buf, sizeof(buf), "         BACK TO MAIN MENU");
  button = new Button (buf);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  button->setFunction (callbackSwitchMainMenu);
  fightermenu.add (button);
  yf -= yfstep + 0.25;

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register mission menu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  xf = -12.5; yf = 11; xfstep = 25; yfstep = 1.3;
  buf [0] = 0;
  label = new Label (buf);
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  missionmenu.add (label);
  yf -= yfstep + 0.15;

  yfstep = 1.0;
  buf [0] = 0;
  label = new Label (buf);
  label->setBounds (xf, yf, xfstep, 3.5);
  missionmenu.add (label);
  yf -= 3.75;

  buf [0] = 0;
  label = new Label (buf);
  label->setBounds (xf, yf, xfstep / 2 - 0.1, 5.7);
  missionmenu.add (label);

  buf [0] = 0;
  label = new Label (buf);
  label->setBounds (xf + xfstep / 2 + 0.1, yf, xfstep / 2 - 0.1, 5.7);
  missionmenu.add (label);
  yf -= 5.95;

  buf [0] = 0;
  label = new Label (buf);
  label->setBounds (xf, yf, xfstep / 2 - 0.1, 6.8);
  missionmenu.add (label);

  buf [0] = 0;
  label = new Label (buf);
  label->setBounds (xf + xfstep / 2 + 0.1, yf, xfstep / 2 - 0.1, 6.8);
  missionmenu.add (label);
  yf -= 7.15;

  yfstep = 1.0;
  button = new Button (" BACK TO MAIN MENU");
  button->setBounds (xf, yf, xfstep / 2 - 0.1, yfstep - 0.1);
  button->setFunction (callbackSwitchMainMenu);
  missionmenu.add (button);

  button = new Button ("    START MISSION");
  button->setBounds (xf + xfstep / 2 + 0.1, yf, xfstep / 2 - 0.1, yfstep - 0.1);
  button->setFunction (callbackSwitchStartMission);
  missionmenu.add (button);

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register quit dialog
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  xf = -6; yf = 2; xfstep = 12; yfstep = 1.2;
  buf [0] = 0;
  label = new Label (buf);
  label->setBounds (xf, yf, xfstep, 3);
  quitmenu.add (label);
  yf -= 3.25;

  yfstep = 1.0; xfstep = 12;
  button = new Button ("    YES");
  button->setBounds (xf, yf, xfstep / 2 - 0.1, yfstep - 0.1);
  button->setFunction (callbackQuitNow);
  quitmenu.add (button);

  button = new Button ("     NO");
  button->setBounds (xf + xfstep / 2 + 0.1, yf, xfstep / 2 - 0.1, yfstep - 0.1);
  button->setFunction (callbackSwitchMainMenu);
  quitmenu.add (button);

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register stats dialog
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  xf = -10; yf = 9; xfstep = 20; yfstep = 1.2;
  buf [0] = 0;
  textfield = new TextField (buf, 1);
  textfield->setBounds (xf, yf, xfstep, 15.8);
  statsmenu.add (textfield);
  yf -= 16;

  yfstep = 1.0; xfstep = 20;
  button = new Button ("                  CONTINUE");
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  button->setFunction (callbackStats);
  statsmenu.add (button);



  famemenu.setVisible (false);
  fightermenu.setVisible (false);
  missionmenu.setVisible (false);
  quitmenu.setVisible (false);
  statsmenu.setVisible (false);
  allmenus.setVisible (false);
  allmenus.visible = true;
  allmenus.components [0]->setVisible (true);
}



/****************************************************************************
  GL-117 ENTRY POINT
****************************************************************************/

int main (int argc, char **argv)
{
  char buf [4096]; // temp buffer

  Logging::setLevel (LOG_DEBUG);
  checkargs (argc, argv); // process command line parameters

//  dirs = new Dirs (argv [0]); // get data directory (DATADIR, defined via autoconf)
  Directory::init (argv [0]);

  Logging::setFile (Directory::getSaves ("logfile.txt"));

  snprintf (buf, sizeof(buf), "Startup %s, %s ... ", argv [0], PACKAGE_VERSION);
  DISPLAY_INFO(buf);

#ifdef _MSC_VER
  DISPLAY_INFO("Windows detected");
#endif

  DISPLAY_DEBUG("Getting directory locations");

  if (!conf.loadConfig ()) // try to load conf file (conf.cpp) and validate settings
  {
    // no conf file found => create new one
    DISPLAY_DEBUG("Creating new configuration");
    config_test (argc, argv); // do screen test
    firststart = true; // enable adjusting quality/view/graphics automatically by the game
  }

  conf.saveConfig (); // save conf file (validated)

  conf.loadConfigInterface (); // load interface settings from conf.interface and validate
  conf.saveConfigInterface (); // save interface settings

  maploader = new MapLoader ();

// here srand should be called to initialize the random number generator
// this is currently done by grabbing random numbers via the init methods (not very elegant)

  server = NULL;
  client = NULL;

  DISPLAY_DEBUG("Creating/Loading pilots list");
  pilots = new PilotList (Directory::getSaves ("pilots")); // look at pilots.h

  gamestate = &stateinit;

  DISPLAY_INFO("Using SDL");
  if (!configinit)
    if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0)
    {
      snprintf (buf, sizeof(buf), "Couldn't initialize SDL: %s", SDL_GetError ());
      //assert (false);
      DISPLAY_FATAL(buf);
      exit (EXIT_INIT);
    }
  atexit (SDL_Quit);

// SDL_NET INSTALLED?
#ifdef HAVE_SDL_NET
  if (SDLNet_Init () == -1) // initialize SDL_net
  {
    DISPLAY_FATAL(FormatString ("SDLNet_Init: %s", SDLNet_GetError ()));
    exit (EXIT_INIT);
  }
  DISPLAY_INFO("Using SDL_net");
#endif

  if (!configinit)
  {
    if (!setScreen (width, height, bpp, fullscreen))
    {
      conf.loadSaveConfig ();
      if (!setScreen (width, height, bpp, fullscreen))
      {
        //assert (false);
        DISPLAY_FATAL(FormatString ("No working display mode %dx%d found.", width, height));
        exit (EXIT_INIT);
      }
    }
  }

  DISPLAY_DEBUG("Setting SDL caption");

  SDL_ShowCursor (0);

  DISPLAY_DEBUG("Creating sound system");
  sound = new SoundSystem (); // look at audio.cpp
  sound->volumesound = volumesound;
  sound->volumemusic = volumemusic;
  sound->setVolume (); // set all sound volumes
  sound->setVolumeMusic (); // set all music volumes

  DISPLAY_DEBUG("Playing startup music");
  sound->playMusic (1);
#ifdef HAVE_SDL_MIXER
  Mix_HookMusicFinished (playRandomMusic);
#endif

  DISPLAY_DEBUG("Calling main initialization method");
  myFirstInit ();
  myReshapeFunc (width, height);

  DISPLAY_DEBUG("Querying joystick");
  joysticks = SDL_NumJoysticks ();
  memset (jaxis, 0, maxjaxis * maxjoysticks * sizeof (int));
  if (joysticks > 0)
  {
    for (i = 0; i < joysticks; i ++)
    {
      SDL_JoystickEventState (SDL_ENABLE);
      sdljoystick [i] = SDL_JoystickOpen (i);
      sdljoystickaxes [i] = SDL_JoystickNumAxes (sdljoystick [i]);
      snprintf (buf, sizeof(buf), "Joystick \"%s\" detected", SDL_JoystickNameForIndex (i));
      DISPLAY_INFO(buf);
    }
  }
  else
  {
    DISPLAY_INFO("No joystick found");
//    sdljoystick [0] = NULL;
    if (controls == CONTROLS_JOYSTICK) // no joystick available, so switch to mouse controls
      controls = CONTROLS_MOUSE;
  }

// disable joystick manually
//  joysticks = 0;

/*  joysticks = 2;
  sdljoystickaxes [0] = 4;
  sdljoystickaxes [1] = 2;*/

// Restrict mouse to SDL window
//  SDL_WM_GrabInput (SDL_GRAB_ON);

  createMenu ();

  DISPLAY_DEBUG("Entering SDL main loop");
  emscripten_set_main_loop(sdlMainLoop,0,1); // simulate GLUT's main loop (above)

  return 0; // exit without signaling errors
}

#endif
