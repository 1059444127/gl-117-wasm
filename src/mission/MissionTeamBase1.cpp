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



MissionTeamBase1::MissionTeamBase1 ()
{
  id = MISSION_TEAMBASE1;
  strcpy (name, "TEAM BASE");
  strcpy (briefing, "ATTACK YOUR OPPONENTS BASE AND DEFEND YOUR OWN ONE. TWO TEAMS - ONE FIGHTER AND ONE BOMBER EACH.");
  autoLFBriefing ();
  alliedfighters = 1;
  selweapons = 2;
  maxtime = 10000 * timestep;
}

void MissionTeamBase1::start ()
{
  day = 1;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 50;
  heading = 180;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_ALPINE_EROSION, NULL);
  int px, py;
  int px1, py1;
  int px2, py2;
  l->searchPlain (1, 1, &px1, &py1);
  px = px1; py = py1;
  l->flatten (px, py, 8, 8);
  team1x = px; team1y = py + 50;
  playerInit ();
  fplayer->trafo.translation.x = px;
  fplayer->trafo.translation.z = py;
  fplayer->currot.phi = 180;
  if (fplayer->id == FalconDescriptor)
  {
    objectInit (new Fighter (HawkDescriptor), 1, 200);
  }
  else
  {
    objectInit (new Fighter (FalconDescriptor), 1, 200);
  }
  fighter [1]->trafo.translation.x = px + 5;
  fighter [1]->trafo.translation.z = py + 5;

  l->searchPlain (1, 2, &px2, &py2);
  px = px2; py = py2;
  l->flatten (px, py, 8, 8);
  team2x = px; team2y = py - 50;
  objectInit (new Fighter (BuzzardDescriptor), 2, 200);
  fighter [2]->trafo.translation.x = px;
  fighter [2]->trafo.translation.z = py;
  if (fplayer->id == FalconDescriptor)
    fighter [2]->target = fighter [1];
  else
    fighter [2]->target = fighter [0];
  objectInit (new Fighter (SwallowDescriptor), 2, 200);
  fighter [3]->trafo.translation.x = px + 5;
  fighter [3]->trafo.translation.z = py + 5;
  fighter [3]->target = NULL;

  px = px1; py = py1;
  int n = 4;
  objectInit (new StaticPassive (Hall2Descriptor), 1, 200);
  fighter [n]->trafo.translation.x = px + 2;
  fighter [n]->trafo.translation.z = py + 5;
  fighter [n]->target = fighter [0];
  n ++;
  objectInit (new StaticPassive (HallDescriptor), 1, 200);
  fighter [n]->trafo.translation.x = px;
  fighter [n]->trafo.translation.z = py - 1;
  fighter [n]->target = fighter [0];
  n ++;
  objectInit (new StaticPassive (HallDescriptor), 1, 200);
  fighter [n]->trafo.translation.x = px + 2;
  fighter [n]->trafo.translation.z = py - 1;
  fighter [n]->target = fighter [0];
  n ++;
  objectInit (new StaticPassive (BigTentDescriptor), 1, 200);
  fighter [n]->trafo.translation.x = px - 1.5;
  fighter [n]->trafo.translation.z = py - 4.5;
  fighter [n]->target = fighter [0];
  n ++;
  objectInit (new StaticAa (SacDescriptor), 1, 200);
  fighter [n]->trafo.translation.x = px - 6;
  fighter [n]->trafo.translation.z = py + 6;
  fighter [n]->target = fighter [0];
  fighter [n]->currot.phi = 90;
  fighter [n]->getPrototype ()->maxrot.theta = 0;
  n ++;
  objectInit (new StaticAa (SacDescriptor), 1, 200);
  fighter [n]->trafo.translation.x = px - 6;
  fighter [n]->trafo.translation.z = py - 6;
  fighter [n]->target = fighter [0];
  fighter [n]->currot.phi = 0;
  fighter [n]->getPrototype ()->maxrot.theta = 0;
  n ++;
  objectInit (new StaticAa (SamDescriptor), 1, 300);
  fighter [n]->trafo.translation.x = px - 15;
  fighter [n]->trafo.translation.z = py - 20;
  fighter [n]->target = NULL;
  n ++;
  objectInit (new StaticAa (SamDescriptor), 1, 300);
  fighter [n]->trafo.translation.x = px + 0;
  fighter [n]->trafo.translation.z = py - 20;
  fighter [n]->target = NULL;
  n ++;
  objectInit (new StaticAa (SamDescriptor), 1, 300);
  fighter [n]->trafo.translation.x = px + 15;
  fighter [n]->trafo.translation.z = py - 20;
  fighter [n]->target = NULL;

  px = px2; py = py2;
  objectInit (new StaticPassive (Hall2Descriptor), 2, 200);
  fighter [n]->trafo.translation.x = px + 2;
  fighter [n]->trafo.translation.z = py + 5;
  fighter [n]->target = fighter [0];
  n ++;
  objectInit (new StaticPassive (HallDescriptor), 2, 200);
  fighter [n]->trafo.translation.x = px;
  fighter [n]->trafo.translation.z = py - 1;
  fighter [n]->target = fighter [0];
  n ++;
  objectInit (new StaticPassive (Hall2Descriptor), 2, 200);
  fighter [n]->trafo.translation.x = px + 2;
  fighter [n]->trafo.translation.z = py - 1;
  fighter [n]->target = fighter [0];
  n ++;
  objectInit (new StaticPassive (BigTentDescriptor), 2, 200);
  fighter [n]->trafo.translation.x = px - 1.5;
  fighter [n]->trafo.translation.z = py - 4.5;
  fighter [n]->target = fighter [0];
  n ++;
  objectInit (new StaticAa (SacDescriptor), 2, 200);
  fighter [n]->trafo.translation.x = px - 6;
  fighter [n]->trafo.translation.z = py + 6;
  fighter [n]->target = fighter [0];
  fighter [n]->currot.phi = 90;
  fighter [n]->getPrototype ()->maxrot.theta = 0;
  n ++;
  objectInit (new StaticAa (SacDescriptor), 2, 200);
  fighter [n]->trafo.translation.x = px - 6;
  fighter [n]->trafo.translation.z = py - 6;
  fighter [n]->target = fighter [0];
  fighter [n]->currot.phi = 0;
  fighter [n]->getPrototype ()->maxrot.theta = 0;
  n ++;
  objectInit (new StaticAa (SamDescriptor), 2, 200);
  fighter [n]->trafo.translation.x = px - 15;
  fighter [n]->trafo.translation.z = py + 20;
  fighter [n]->target = NULL;
  n ++;
  objectInit (new StaticAa (SamDescriptor), 2, 200);
  fighter [n]->trafo.translation.x = px + 0;
  fighter [n]->trafo.translation.z = py + 20;
  fighter [n]->target = NULL;
  n ++;
  objectInit (new StaticAa (SamDescriptor), 2, 200);
  fighter [n]->trafo.translation.x = px + 15;
  fighter [n]->trafo.translation.z = py + 20;
  fighter [n]->target = NULL;

  if (fplayer->id == FalconDescriptor)
  {
    fighter [1]->target = NULL;
  }
  else
  {
    fighter [1]->target = fighter [3];
  }

  state = 0;
  laststate = 0;
  texttimer = 0;
}

int MissionTeamBase1::processtimer (Uint32 dt)
{
//  bool b = false;
  int i;
  if (texttimer >= 200 * timestep) texttimer = 0;
  if (texttimer > 0) texttimer += dt;
  timer += dt;

  if (fplayer->id == FalconDescriptor)
  {
    fighter [2]->target = fighter [1];
  }
  else
  {
    fighter [2]->target = fighter [0];
    fighter [1]->target = fighter [3];
  }

  bool testb1 = false, testb2 = false;
  for (i = 4; i < fighter.size (); i ++)
  {
    if (fighter [i]->active)
    {
      if (fighter [i]->party == 1)
        testb1 = true;
      if (fighter [i]->party == 2)
        testb2 = true;
    }
  }
  if (!testb1) return 2;
  if (!testb2) return 1;

  for (i = 0; i <= 3; i ++)
  {
    if (!fighter [i]->active && fighter [i]->explode >= 35 * timestep)
    {
      reincarnateFighter (i);
      if (i <= 1)
      {
        fighter [i]->trafo.translation.x = team1x;
        fighter [i]->trafo.translation.z = team1y;
      }
      else
      {
        fighter [i]->trafo.translation.x = team2x;
        fighter [i]->trafo.translation.z = team2y;
      }
      fighter [i]->trafo.translation.y = l->getHeight (fighter [i]->trafo.translation.x, fighter [i]->trafo.translation.z) + 15;
      camera = 0;
    }
  }
  return 0;
}

void MissionTeamBase1::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
}

#endif
