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



void PilotList::load (const std::string &fname)
{
  char buf [4096];
  int i;
  FILE *in = fopen (fname.c_str (), "rb");
  if (in == NULL)
  {
    DISPLAY_WARN("Could not load saves/pilots");
    aktpilots = 0;
    add ("PLAYER");
    return;
  }
  fscanf (in, "%d %d ", &aktpilots, &aktpilot);
  unsigned z = 0;
  while (z < maxpilots)
  {
    fscanf (in, "%s ", buf);
    pilot [z] = new Pilot (buf);
    z ++;
  }
  fclose (in);
}

void PilotList::save (const std::string &fname)
{
  char buf [256];
  int i;

  FILE *out = fopen (fname.c_str (), "wb");
  if (out == NULL)
  {
    DISPLAY_WARN("Could not write saves/pilots");
    return;
  }
  fprintf (out, "%d %d ", aktpilots, aktpilot);
  for (i = 0; i < aktpilots; i ++)
  {
    fprintf (out, "%s ", pilot [i]->name.c_str ());
  }
  fclose (out);

  for (i = 0; i < aktpilots; i ++)
  {
    pilot [i]->save ();
  }
}

PilotList::PilotList (const std::string &fname)
{
  int i;
  for (i = 0; i < maxpilots; i ++)
    pilot [i] = NULL;
  load (fname);
}

PilotList::~PilotList ()
{
  int i;
  for (i = 0; i < aktpilots; i ++)
  {
    if (pilot [i] != NULL)
      delete pilot [i];
  }
}

void PilotList::rm ()
{
  int i, z = 0;
  for (i = 0; i < maxpilots; i ++)
  {
    if (pilot [i] != NULL) z ++;
  }
  if (z <= 1) return;
  if (pilot [aktpilot] != NULL)
  {
    char buf [4096];
    strcpy (buf, Directory::getSaves (pilot [aktpilot]->name).c_str ());
    remove (buf);
    delete pilot [aktpilot];
    pilot [aktpilot] = NULL;
  }
  for (i = aktpilot; i < aktpilots; i ++)
    pilot [i] = pilot [i + 1];
  pilot [aktpilots] = NULL;
  aktpilots --;
  if (aktpilots < 0) aktpilots = 0;
  aktpilot = 0;
}

void PilotList::add (const std::string &name)
{
  if (aktpilots >= maxpilots) return;
  aktpilot = aktpilots;
  pilot [aktpilot] = new Pilot (name);
  aktpilots ++;
}

#endif
