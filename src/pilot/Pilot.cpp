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

#ifndef IS_PILOTS_H

#include "pilot/Pilot.h"
#include "configuration/Directory.h"
#include "logging/Logging.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>



void Pilot::load ()
{
  int i;
  for (i = 0; i < maxpilotdata; i ++)
  {
    mission_state [i] = 0;
    mission_time [i] = 0;
    mission_fighterkills [i] = 0;
    mission_shipkills [i] = 0;
    mission_tankkills [i] = 0;
    mission_otherkills [i] = 0;
    mission_shield [i] = 0;
    mission_points [i] = 0;
    mission_score [i] = 0;
  }
  if (tp == NULL)
  {
    tp = new TeamPilot *[11];
    tp [0] = new TeamPilot (5, "PRIMETIME", 200, 100, 150, 20);
    tp [1] = new TeamPilot (1, "HEPTARGON", 80, 220, 300, 1);
    tp [2] = new TeamPilot (2, "KARA", 180, 160, 50, 4);
    tp [3] = new TeamPilot (6, "SHARK", 70, 90, 120, 22);
    tp [4] = new TeamPilot (3, "BOSS", 250, 180, 80, 10);
    tp [5] = new TeamPilot (2, "DR DOOM", 320, 210, 20, 2);
    tp [6] = new TeamPilot (4, "SHADOW", 130, 200, 320, 15);
    tp [7] = new TeamPilot (7, "MATRIX", 40, 80, 180, 25);
    tp [8] = new TeamPilot (1, "FIREBIRD", 250, 140, 100, 1);
    tp [9] = new TeamPilot (4, "THUNDER", 150, 170, 60, 18);
    tp [10] = new TeamPilot (0, "PLAYER", 0, 0, 0, 0);
  }

  char buf [4096];
  strcpy (buf, Directory::getSaves (name).c_str ());
  FILE *in = fopen (buf, "rb");
  if (in == NULL)
  {
    DISPLAY_WARN("Could not load pilot");
    return;
  }
  char saveversion [20];
  fread (saveversion, sizeof (char), strlen (SAVEVERSION), in);
  fread (mission_state, sizeof (int), maxpilotdata, in);
  fread (mission_time, sizeof (int), maxpilotdata, in);
  fread (mission_fighterkills, sizeof (int), maxpilotdata, in);
  fread (mission_shipkills, sizeof (int), maxpilotdata, in);
  fread (mission_tankkills, sizeof (int), maxpilotdata, in);
  fread (mission_otherkills, sizeof (int), maxpilotdata, in);
  fread (mission_shield, sizeof (int), maxpilotdata, in);
  fread (mission_points, sizeof (int), maxpilotdata, in);
  fread (mission_score, sizeof (int), maxpilotdata, in);
  for (i = 0; i < 10; i ++)
    tp [i]->load (in);
  fclose (in);
}

void Pilot::save ()
{
  int i;
  char buf [4096];
  strncpy (buf, Directory::getSaves (name).c_str (), 4096);
  FILE *out = fopen (buf, "wb");
  if (out == NULL)
  {
    DISPLAY_WARN("Could not write pilot");
    return;
  }
  fwrite (SAVEVERSION, sizeof (char), strlen (SAVEVERSION), out);
  fwrite (mission_state, sizeof (int), maxpilotdata, out);
  fwrite (mission_time, sizeof (int), maxpilotdata, out);
  fwrite (mission_fighterkills, sizeof (int), maxpilotdata, out);
  fwrite (mission_shipkills, sizeof (int), maxpilotdata, out);
  fwrite (mission_tankkills, sizeof (int), maxpilotdata, out);
  fwrite (mission_otherkills, sizeof (int), maxpilotdata, out);
  fwrite (mission_shield, sizeof (int), maxpilotdata, out);
  fwrite (mission_points, sizeof (int), maxpilotdata, out);
  fwrite (mission_score, sizeof (int), maxpilotdata, out);
  for (i = 0; i < 10; i ++)
    tp [i]->save (out);
  fclose (out);
}

std::string Pilot::getRank (int lomissionid, int himissionid)
{
  int i, sum = 0;
  for (i = lomissionid; i <= himissionid; i ++)
  {
    if (mission_score [i] > 0 && mission_score [i] < 2000)
      sum += mission_score [i];
  }
  if (sum < 500)
  { ranking = 0; return "AIRMAN BASIC"; }
  else if (sum < 1000)
  { ranking = 1; return "AIRMAN"; }
  else if (sum < 1500)
  { ranking = 2; return "AIRMAN 1ST CLASS"; }
  else if (sum < 2500)
  { ranking = 3; return "2ND LIEUTNANT"; }
  else if (sum < 3500)
  { ranking = 4; return "1ST LIEUTNANT"; }
  else if (sum < 5000)
  { ranking = 5; return "CAPTAIN"; }
  else if (sum < 7000)
  { ranking = 6; return "MAJOR"; }
  else if (sum < 9000)
  { ranking = 7; return "COLONEL"; }
  else
  { ranking = 8; return "GENERAL"; }
}

/*
US RANKS:
Second Lieutenant, 2nd Lt
First Lieutenant, 1st Lt
Captain, Capt
Major, Maj
Lieutenant Colonel, Lt Col
Colonel, Col
Brigadier General, Brig Gen
Major General, Maj Gen
Lieutenant General, Lt Gen
General, Gen
*/

std::string Pilot::getShortRank (int lomissionid, int himissionid)
{
  int i, sum = 0;
  for (i = lomissionid; i < himissionid; i ++)
  {
    if (mission_score [i] > 0 && mission_score [i] < 2000)
      sum += mission_score [i];
  }
  if (sum < 500)
  { ranking = 0; return "AB"; }
  else if (sum < 1000)
  { ranking = 1; return "AMN"; }
  else if (sum < 1500)
  { ranking = 2; return "A1C"; }
  else if (sum < 2500)
  { ranking = 3; return "2ND LT"; }
  else if (sum < 3500)
  { ranking = 4; return "1ST LT"; }
  else if (sum < 5000)
  { ranking = 5; return "CAPT"; }
  else if (sum < 7000)
  { ranking = 6; return "MAJ"; }
  else if (sum < 9000)
  { ranking = 7; return "COL"; }
  else
  { ranking = 8; return "GEN"; }
}

Pilot::Pilot (const std::string &name)
{
  tp = NULL;
  this->name = name;
  load ();
}

Pilot::~Pilot ()
{
  int i;
  for (i = 0; i < 11; i ++)
    delete (tp [i]);
}

#endif
