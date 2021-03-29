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
#include "loadmodel/Model3dRegistry.h"
//#include "loadmodel/Model3dFactory.h"

#include <stdio.h>
#include <string.h>



MissionDogfight3::MissionDogfight3 ()
{
  id = MISSION_DOGFIGHT3;
  strcpy (name, "DOGFIGHT");
  strcpy (briefing, "ENEMY FIGHTERS HAVE BEEN SIGHTED. YOU HAVE TO INTERCEPT THEM IMMEDIATELY. EXPECT BOMBERS WITHIN THEIR WING.");
  autoLFBriefing ();
  alliedfighters = 4;
  maxtime = 3500 * timestep;
  selfighter [1] = Hawk2Descriptor;
  alliedpilot [0] = PILOT_PRIMETIME;
  alliedpilot [1] = PILOT_SHADOW;
  alliedpilot [2] = PILOT_LARA;
}

void MissionDogfight3::start ()
{
  int i;
  day = 1;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 20;
  heading = 300;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_DESERT, NULL);
  playerInit ();
  fplayer->trafo.translation.x = 0;
  fplayer->trafo.translation.z = 100;
  alliedInit (FalconDescriptor, alliedpilot [0]);
  fighter [1]->trafo.translation.x = 5;
  fighter [1]->trafo.translation.z = 105;
  alliedInit (FalconDescriptor, alliedpilot [1]);
  fighter [2]->trafo.translation.x = 10;
  fighter [2]->trafo.translation.z = 110;
  alliedInit (FalconDescriptor, alliedpilot [2]);
  fighter [3]->trafo.translation.x = 15;
  fighter [3]->trafo.translation.z = 115;
  for (i = 4; i <= 8; i ++)
  {
    objectInit (new Fighter (CrowDescriptor), 0, 400 - i * 25);
    fighter [i]->target = fighter [Math::random (4)];
    fighter [i]->trafo.translation.x = -i * 10;
    fighter [i]->trafo.translation.z = -i * 10;
  }
  for (i = 9; i <= 11; i ++)
  {
    objectInit (new Fighter (SwallowDescriptor), 0, 400 - i * 20);
    fighter [i]->target = fighter [Math::random (4)];
    fighter [i]->trafo.translation.x = -i * 10;
    fighter [i]->trafo.translation.z = -i * 10;
    fighter [i]->deactivate ();
  }
  invertZ (); // only invert if NO objects are mapped to flat ground
}

/*int MissionDogfight3::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
  for (i = 0; i <= 15; i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party == 0)
        b = true;
  }
  if (b) return 0;
  return 1;
}*/

void MissionDogfight3::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
  if (timer >= 800 * timestep && state == 0)
  {
    state = 1;
    fighter [9]->activate ();
    fighter [10]->activate ();
    fighter [11]->activate ();
    fighter [9]->trafo.translation.x = fplayer->trafo.translation.x + 55;
    fighter [9]->trafo.translation.z = fplayer->trafo.translation.z + 55;
    fighter [9]->trafo.translation.y = l->getHeight (fighter [9]->trafo.translation.x, fighter [9]->trafo.translation.z) + 10;
    fighter [10]->trafo.translation.x = fplayer->trafo.translation.x + 60;
    fighter [10]->trafo.translation.z = fplayer->trafo.translation.z + 60;
    fighter [10]->trafo.translation.y = l->getHeight (fighter [10]->trafo.translation.x, fighter [10]->trafo.translation.z) + 10;
    fighter [11]->trafo.translation.x = fplayer->trafo.translation.x + 65;
    fighter [11]->trafo.translation.z = fplayer->trafo.translation.z + 65;
    fighter [11]->trafo.translation.y = l->getHeight (fighter [11]->trafo.translation.x, fighter [11]->trafo.translation.z) + 10;
  }
  if (timer >= 800 * timestep && timer <= 1000 * timestep)
  {
    font1->drawTextCentered (0, 7, -3, "BOMBERS ARE ENTERING THE REGION", textcolor);
    if (state == 0)
    {
      state ++;
      for (unsigned i = 9; i <= 11; i ++)
      {
        fighter [i]->activate ();
      }
    }
  }
}

#endif
