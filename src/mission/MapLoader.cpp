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

/* This file contains a map loader. */

#ifndef IS_MAPLOADER_H

#include "MapLoader.h"
#include "game/globals.h"
#include "loadbitmap/Bitmap.h"
#include "filetokenizer/FileTokenizer.h"
#include "configuration/Directory.h"
#include "gllandscape/GlLandscape.h"
#include "logging/Logging.h"
#include "util/Util.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <cassert>



int mapPilotname2Num (char *str)
{
  if (!strcmp (str, "PRIMETIME")) return PILOT_PRIMETIME;
  else if (!strcmp (str, "HEPTARGON")) return PILOT_HEPTARGON;
  else if (!strcmp (str, "LARA")) return PILOT_LARA;
  else if (!strcmp (str, "SHARK")) return PILOT_SHARK;
  else if (!strcmp (str, "BOSS")) return PILOT_BOSS;
  else if (!strcmp (str, "DRDOOM")) return PILOT_DRDOOM;
  else if (!strcmp (str, "SHADOW")) return PILOT_SHADOW;
  else if (!strcmp (str, "MATRIX")) return PILOT_MATRIX;
  else if (!strcmp (str, "FIREBIRD")) return PILOT_FIREBIRD;
  else if (!strcmp (str, "THUNDER")) return PILOT_THUNDER;
  return -1;
}

void MapLoader::readMapFile ()
{
  char token [TOKENLEN];
  mapcount = 0;

  if (!file.open (Directory::getMaps ("maps.txt").c_str ()))
    return;

  while (file.nextToken (token, TOKENLEN))
  {
    strcpy (mapfile [mapcount], token);
    mapcount ++;
    if (mapcount >= MAXMAPS) break;
  }

  file.close ();
}

MapLoader::MapLoader ()
{
  // initialize map file loader
  file.init ();
  file.setWhitespace (" \t\r\n");
  file.addComment ("#", "\n");
  file.setQuotes ("\"'`");
  readMapFile ();
  mapptr = 0;
}

int MapLoader::getMapNumber ()
{
  return mapcount;
}

char *MapLoader::getNextMap ()
{
  return mapfile [mapptr ++];
}

void MapLoader::toUpper (char *str)
{
  if (str == NULL) return;
  int i;
  for (i = 0; i < (int) strlen (str); i ++)
    if (str [i] >= 'a' && str [i] <= 'z')
      str [i] -= 32;
}

Mission *MapLoader::getMission ()
{
  Mission *m = new MissionCustom ();
  m->id = mapptr;
//  m->init ();
  return m;
}



MissionCustom::MissionCustom ()
  : Mission ()
{
  reterror = 0;
  numobjects = 0;
  numtext = 0;
}

void MissionCustom::error (const char *msg)
{
  reterror = 1;
  DISPLAY_ERROR(FormatString ("Line %d: %s", maploader->file.getLine (), msg));
}

void MissionCustom::toUpper (char *str)
{
  if (str == NULL) return;
  int i;
  for (i = 0; i < (int) strlen (str); i ++)
    if (str [i] >= 'a' && str [i] <= 'z')
      str [i] -= 32;
}

int MissionCustom::readAttribute (char *attribute, char *value, char *casevalue)
{
  if (!maploader->file.nextToken (token, TOKENLEN))
    return 0;
  toUpper (token);
  if (isGlobal (token))
    return 0;
  strcpy (attribute, token);
  if (!maploader->file.nextToken (token, TOKENLEN))
    return 0;
  if (strcmp (token, "="))
  {
    error ("Assignment symbol '=' is missing");
    if (!synchronize ())
      return 0;
    return -1;
  }
  if (!maploader->file.nextToken (token, TOKENLEN))
  {
    error ("Unexpected end of file");
    return 0;
  }
  strcpy (casevalue, token);
  toUpper (token);
  strcpy (value, token);
  return 1;
}

int MissionCustom::synchronize ()
{
  if (!maploader->file.find ("\n")) return 0;
  return 1;
}

int MissionCustom::isInteger (char *str)
{
  int i;
  if (str [0] == 0)
    return 0;
  for (i = 0; i < (int) strlen (str); i ++)
    if (str [i] < '0' || str [i] > '9')
      return 0;
  return 1;
}

int MissionCustom::isDouble (char *str)
{
  if (str [0] == 0)
    return 0;
  char *ptr;
  (void) strtod (str, &ptr);
  if ((int) (ptr - str) != (int) strlen (str)) return 0;
  return 1;
}

double MissionCustom::toDouble (char *str)
{
  if (str [0] == 0)
    return 0;
  char *ptr;
  return strtod (str, &ptr);
}

int MissionCustom::parseMapData ()
{
  char attr [TOKENLEN], value [TOKENLEN], casevalue [TOKENLEN];
  int ret;
  map_type = LANDSCAPE_ALPINE;
  save_map_type = map_type;
  heading = 0;
  weather = WEATHER_SUNNY;
  clouds = 0;

  while (true)
  {
    while ((ret = readAttribute (attr, value, casevalue)) == -1);
    if (!ret) return 0;
    if (!strcmp (attr, "TYPE"))
    {
      if (!strcmp (value, "ALPINE")) map_type = LANDSCAPE_ALPINE;
      else if (!strcmp (value, "ALPINE_NOLAKE")) map_type = LANDSCAPE_ALPINE_NOLAKE;
      else if (!strcmp (value, "ALPINE_LOW")) map_type = LANDSCAPE_LOW_ALPINE;
      else if (!strcmp (value, "ALPINE_EROSION")) map_type = LANDSCAPE_ALPINE_EROSION;
      else if (!strcmp (value, "ALPINE_SEA")) map_type = LANDSCAPE_ALPINE_SEA;
      else if (!strcmp (value, "ALPINE_ROCKY")) map_type = LANDSCAPE_ALPINE_ROCKY;
      else if (!strcmp (value, "SEA")) map_type = LANDSCAPE_SEA;
      else if (!strcmp (value, "MOON")) map_type = LANDSCAPE_MOON;
      else if (!strcmp (value, "MOON_FLAT")) map_type = LANDSCAPE_FLAT_MOON;
      else if (!strcmp (value, "CANYON")) map_type = LANDSCAPE_CANYON;
      else if (!strcmp (value, "DESERT")) map_type = LANDSCAPE_DESERT;
      else
      {
        error (FormatString ("Invalid attribute %s", attr).c_str ());
      }
      save_map_type = map_type;
    }
    else if (!strcmp (attr, "HEADING"))
    {
      if (!isInteger (value))
      {
        error ("Heading value must be an integer");
      }
      else
      {
        heading = atoi (value);
      }
    }
    else if (!strcmp (attr, "WEATHER"))
    {
      if (!strcmp (value, "SUNNY")) { weather = WEATHER_SUNNY; clouds = 0; }
      else if (!strcmp (value, "CLEAR")) { weather = WEATHER_SUNNY; clouds = 2; }
      else if (!strcmp (value, "CLOUDY")) { weather = WEATHER_SUNNY; clouds = 1; }
      else if (!strcmp (value, "DULL")) { weather = WEATHER_CLOUDY; clouds = 1; }
      else if (!strcmp (value, "THUNDERSTORM")) { weather = WEATHER_THUNDERSTORM; clouds = 3; }
      else
      {
        error (FormatString ("Invalid attribute %s", attr).c_str ());
      }
    }
    else if (!strcmp (attr, "TIME"))
    {
      if (!isInteger (value))
      {
        error ("Time value must be an integer in [0...24]");
      }
      else
      {
        int time = atoi (value);
        if (time < 0 || time > 24)
        {
          error ("Time value must be an integer in [0...24]");
        }
        else
        {
          if (time < 6 || time > 18)
          {
            day = 0;
            if (time > 18) time = 24 - time;
            sungamma = (6 - time) * 10;
          }
          else
          {
            day = 1;
            time = abs (12 - time);
            sungamma = (6 - time) * 10;
          }
        }
      }
    }
    else if (!strcmp (attr, "FILE"))
    {
      FILE *in = fopen (Directory::getMaps (casevalue).c_str (), "rb");
      if (in == NULL)
      {
        error (FormatString ("Cannot open file %s", Directory::getMaps (casevalue)).c_str ());
      }
      else
      {
        fseek (in, 16, SEEK_SET);
        char bpp = fgetc (in);
        fclose (in);
        if (bpp != 32)
        {
          error ("Only 32 bpp TGA files are valid");
        }
        else
        {
          map_type = -1;
          strcpy (mapfile, casevalue);
        }
      }
    }
    else if (!strcmp (attr, "SCALEHEIGHT"))
    {
      if (!isDouble (value))
      {
        error ("Scaleheight value must be a float number in [0...1]");
      }
      else
      {
        float test = toDouble (value);
        if (test < 0 || test > 1)
        {
          error ("Scaleheight value must be a float number in [0...1]");
        }
        else
        {
          scaleheight = test;
        }
      }
    }
    else if (strstr (attr, "TREE"))
    {
      int num = atoi (&attr [4]);
      if (num < 1 || num > 3)
      {
        error ("Tree needs a number suffix like \"Tree2\" with number in [1...3]");
      }
      else
      {
        if (!isInteger (value))
        {
          error ("Tree value must be an integer in [0...100]");
        }
        else
        {
          int test = atoi (value);
          if (test < 0 || test > 100)
            error ("Tree value must be an integer in [0...100]");
          else
            tree [num - 1] = test;
        }
      }
    }
    else
    {
      if (isGlobal (attr))
        break;
    }
  }
  return 1;
}

int MissionCustom::parseObjectData ()
{
  char attr [TOKENLEN], value [TOKENLEN], casevalue [TOKENLEN];
  int ret;
  AiObj *aiobj = &obj [numobjects];

  if (numobjects >= maxfighter)
  {
    error ("Too many objects defined");
    return 0;
  }

  while (true)
  {
    while ((ret = readAttribute (attr, value, casevalue)) == -1);
    if (!ret)
    {
      numobjects ++;
      return 0;
    }
    if (!strcmp (attr, "TYPE"))
    {
      if (!strcmp (value, "FALCON")) aiobj->id = FalconDescriptor;
      else if (!strcmp (value, "SWALLOW")) aiobj->id = SwallowDescriptor;
      else if (!strcmp (value, "HAWK")) aiobj->id = HawkDescriptor;
      else if (!strcmp (value, "HAWK2")) aiobj->id = Hawk2Descriptor;
      else if (!strcmp (value, "BUZZARD")) aiobj->id = BuzzardDescriptor;
      else if (!strcmp (value, "CROW")) aiobj->id = CrowDescriptor;
      else if (!strcmp (value, "PHOENIX")) aiobj->id = PhoenixDescriptor;
      else if (!strcmp (value, "REDARROW")) aiobj->id = RedArrowDescriptor;
      else if (!strcmp (value, "BLACKBIRD")) aiobj->id = BlackBirdDescriptor;
      else if (!strcmp (value, "STORM")) aiobj->id = StormDescriptor;
      else if (!strcmp (value, "TRANSPORT")) aiobj->id = TransportDescriptor;
      else if (!strcmp (value, "WIESEL")) aiobj->id = WieselDescriptor;
      else if (!strcmp (value, "PANTHER")) aiobj->id = PantherDescriptor;
      else if (!strcmp (value, "SAMTRUCK")) aiobj->id = MobileSamDescriptor;
      else if (!strcmp (value, "PICKUP")) aiobj->id = PickupDescriptor;
      else if (!strcmp (value, "TRUCK")) aiobj->id = TruckDescriptor;
      else if (!strcmp (value, "TRUCK2")) aiobj->id = Truck2Descriptor;
      else if (!strcmp (value, "CRUISER")) aiobj->id = CruiserDescriptor;
      else if (!strcmp (value, "DESTROYER")) aiobj->id = LightDestroyerDescriptor;
      else if (!strcmp (value, "SAC")) aiobj->id = SacDescriptor;
      else if (!strcmp (value, "SAM")) aiobj->id = SamDescriptor;
      else if (!strcmp (value, "TENT")) aiobj->id = TentDescriptor;
      else if (!strcmp (value, "BIGTENT")) aiobj->id = BigTentDescriptor;
      else if (!strcmp (value, "CONTAINER")) aiobj->id = ContainerDescriptor;
      else if (!strcmp (value, "HALL")) aiobj->id = HallDescriptor;
      else if (!strcmp (value, "HALL2")) aiobj->id = Hall2Descriptor;
      else if (!strcmp (value, "OILRIG")) aiobj->id = OilrigDescriptor;
      else if (!strcmp (value, "COMPLEX")) aiobj->id = ComplexDescriptor;
      else if (!strcmp (value, "RADAR")) aiobj->id = RadarDescriptor;
      else if (!strcmp (value, "MOONBASE")) aiobj->id = MoonBaseDescriptor;
      else if (!strcmp (value, "DEPOT")) aiobj->id = DepotDescriptor;
      else if (!strcmp (value, "LASERBARRIER")) aiobj->id = LaserBarrierDescriptor;
      else if (!strcmp (value, "ASTEROID")) aiobj->id = AsteroidDescriptor;
      else
      {
        error (FormatString ("Invalid attribute %s", attr).c_str ());
      }
    }
    else if (!strcmp (attr, "PARTY"))
    {
      if (!isInteger (value))
      {
        error ("Party value must be an integer");
      }
      else
      {
        aiobj->party = atoi (value);
      }
    }
    else if (!strcmp (attr, "LEVEL"))
    {
      if (!isInteger (value))
      {
        error ("Level value must be an integer in [0...10]");
      }
      else
      {
        int level = atoi (value);
        if (level < 0 || level > 10)
          error ("Level value must be an integer in [0...10]");
        aiobj->intelligence = 400 - level * 40;
      }
    }
    else if (!strcmp (attr, "X"))
    {
      if (!isDouble (value))
      {
        error ("X value must be a float number");
      }
      else
      {
        aiobj->trafo.translation.x = -toDouble (value);
      }
    }
    else if (!strcmp (attr, "Y"))
    {
      if (!isDouble (value))
      {
        error ("Y value must be a float number");
      }
      else
      {
        aiobj->trafo.translation.z = -toDouble (value);
      }
    }
    else if (!strcmp (attr, "INITTIME"))
    {
      if (!isDouble (value))
      {
        error ("Inittime value must be a float number");
      }
      else
      {
        float z = toDouble (value);
        aiobj->ttl = (int) (z * 1000 * timestep / 34);
      }
    }
    else if (!strcmp (attr, "CRITICAL"))
    {
      if (!isInteger (value))
      {
        error ("Critical value must be an integer");
      }
      else
      {
        int b = atoi (value);
        if (b) aiobj->explode = 1;
        else aiobj->explode = -1;
      }
    }
    else
    {
      if (isGlobal (attr))
        break;
    }
  }
  numobjects ++;
  return 1;
}

int MissionCustom::parseTextData ()
{
  char attr [TOKENLEN], value [TOKENLEN], casevalue [TOKENLEN];
  int ret;

  if (numtext >= maxtext)
  {
    error ("Too many text blocks");
  }

  while (true)
  {
    while ((ret = readAttribute (attr, value, casevalue)) == -1);
    if (!ret)
    {
      numtext ++;
      return 0;
    }
    if (!strcmp (attr, "MESSAGE"))
    {
      strcpy (text [numtext], value);
    }
    else if (!strcmp (attr, "STARTTIME"))
    {
      if (!isDouble (value))
      {
        error ("Starttime value must be a float number");
      }
      else
      {
        float z = toDouble (value);
        textstart [numtext] = (int) (z * 1000 * timestep / 34);
      }
    }
    else if (!strcmp (attr, "DELAY"))
    {
      if (!isDouble (value))
      {
        error ("Delay value must be a float number");
      }
      else
      {
        float z = toDouble (value);
        textdelay [numtext] = (int) (z * 1000 * timestep / 34);
      }
    }
    else
    {
      if (isGlobal (attr))
        break;
    }
  }
  numtext ++;
  return 1;
}

int MissionCustom::parseRelCoords ()
{
  char attr [TOKENLEN], value [TOKENLEN], casevalue [TOKENLEN];
  int ret;

  while (true)
  {
    while ((ret = readAttribute (attr, value, casevalue)) == -1);
    if (!ret) return 0;
    if (!strcmp (attr, "FIND"))
    {
      if (!strcmp (value, "LASTOBJECT"))
      {
        if (numobjects <= 0)
        {
          error ("No object defined yet: coords origin could not be set");
        }
        else
        {
          obj [numobjects].idle = 1;
        }
      }
      else if (!strcmp (value, "PLAYER"))
      {
        if (numobjects <= 0)
        {
          error ("No object defined yet: coords origin could not be set");
        }
        else
        {
          obj [numobjects].idle = 2;
        }
      }
      else if (!strcmp (value, "FLATTERRAIN"))
      {
        obj [numobjects].idle = 100;
      }
      else
      {
        error (FormatString ("Invalid attribute %s", attr).c_str());
      }
    }
    else if (!strcmp (attr, "QUADRANT"))
    {
      if (!isInteger (value))
      {
        error ("Quadrant value must be an integer in [1...16]");
      }
      else
      {
        int quad = atoi (value);
        if (quad < 1 || quad > 16)
          error ("Quadrant value must be an integer in [1...16]");
        else
          obj [numobjects].idle += quad;
      }
    }
    else
    {
      if (isGlobal (attr))
        break;
    }
  }
  return 1;
}

int MissionCustom::isGlobal (char *token)
{
  bool b = !strcmp (token, "INFO") || !strcmp (token, "TERRAIN") || !strcmp (token, "OBJECT") || !strcmp (token, "RELCOORDS") || !strcmp (token, "TEXT");
  return b;
}

int MissionCustom::parseInfoData ()
{
  char attr [TOKENLEN], value [TOKENLEN], casevalue [TOKENLEN];
  int ret;

  while (true)
  {
    while ((ret = readAttribute (attr, value, casevalue)) == -1);
    if (!ret) return 0;
    if (!strcmp (attr, "NAME"))
    {
      strcpy (name, value);
    }
    else if (!strcmp (attr, "BRIEFING"))
    {
      strcpy (briefing, value);
    }
    else if (strstr (attr, "PILOT"))
    {
      int num = atoi (&attr [5]);
      if (num < 1 || num > maxpilots)
      {
        error ("Pilot needs a number suffix like \"Pilot5\" with number in [1...maxpilots]");
      }
      else
      {
        int test = mapPilotname2Num (value);
        if (test == -1)
        {
          error ("Invalid pilot name");
        }
        else
        {
          alliedpilot [num - 1] = test;
        }
      }
    }
    else if (strstr (attr, "FIGHTER"))
    {
      int num = atoi (&attr [7]);
      if (num < 1 || num > maxpilots)
      {
        error ("Fighter needs a number suffix like \"Fighter1\" with number in [1...3]");
      }
      else
      {
        if (!strcmp (value, "FALCON")) selfighter [num - 1] = FalconDescriptor;
        else if (!strcmp (value, "HAWK")) selfighter [num - 1] = HawkDescriptor;
        else if (!strcmp (value, "HAWK2")) selfighter [num - 1] = Hawk2Descriptor;
        else if (!strcmp (value, "STORM")) selfighter [num - 1] = StormDescriptor;
        else if (!strcmp (value, "REDARROW")) selfighter [num - 1] = RedArrowDescriptor;
        else if (!strcmp (value, "PHOENIX")) selfighter [num - 1] = PhoenixDescriptor;
        else
        {
          error ("Invalid fighter type");
        }
      }
    }
    else if (strstr (attr, "WEAPON"))
    {
      int num = atoi (&attr [6]);
      if (num < 1 || num > maxpilots)
      {
        error ("Weapon needs a number suffix like \"Weapon\" with number in [1...3]");
      }
      else
      {
        if (!strcmp (value, "AAMPACK")) selweapon [num - 1] = AamHs1Descriptor;
        else if (!strcmp (value, "AGMPACK")) selweapon [num - 1] = Agm1Descriptor;
        else if (!strcmp (value, "DFPACK")) selweapon [num - 1] = DfmDescriptor;
        else
        {
          error ("Invalid weapon type");
        }
      }
    }
    else if (strstr (attr, "MAXTIME"))
    {
      if (!isDouble (value))
      {
        error ("Maxtime value must be a float number");
      }
      else
      {
        float z = toDouble (value);
        maxtime = (int) (z * 1000 * timestep / 34);
      }
    }
    else
    {
      error (FormatString ( "Invalid attribute %s", attr).c_str());
    }
  }
  return 1;
}

void MissionCustom::init ()
{
  int i;
  bool readtoken = true;

  reterror = 0;
  if (!maploader->file.open (Directory::getMaps (maploader->mapfile [id - MISSION_CUSTOM1]).c_str ()))
  {
    error ("Could not open map file");
    return;
  }

  for (i = 0; i < maxfighter; i ++)
  {
    obj [i].init ();
  }

  for (i = 0; i < maxtext; i ++)
  {
    text [i] [0] = 0;
    textstart [i] = -1;
    textdelay [i] = 0;
  }

  for (i = 0; i < maxpilots; i ++)
    alliedpilot [i] = i;

  maxtime = 2500 * timestep;

  tree [0] = 20; tree [1] = 20; tree [2] = 50;
  scaleheight = 1;

  memset (token, 0, TOKENLEN);
  strcpy (name, "");
  strcpy (briefing, "");
  strcpy (mapfile, "");
  while (true)
  {
    if (readtoken)
    {
      if (!maploader->file.nextToken (token, TOKENLEN))
        break;
    }
    readtoken = true;
    toUpper (token);
    if (!strcmp (token, "INFO"))
    {
      strcpy (token, "");
      parseInfoData ();
      readtoken = false;
    }
    else if (!strcmp (token, "TERRAIN"))
    {
      strcpy (token, "");
      parseMapData ();
      readtoken = false;
    }
    else if (!strcmp (token, "OBJECT"))
    {
      strcpy (token, "");
      parseObjectData ();
      readtoken = false;
    }
    else if (!strcmp (token, "RELCOORDS"))
    {
      strcpy (token, "");
      parseRelCoords ();
      readtoken = false;
    }
    else if (!strcmp (token, "TEXT"))
    {
      strcpy (token, "");
      parseTextData ();
      readtoken = false;
    }
    else
    {
      if (strlen (token))
      {
        error (FormatString ("Unknown token %s", token).c_str());
      }
      // error in map file, must be closed, mission freed
    }
  }

  maploader->file.close ();

  autoLFBriefing ();
  alliedfighters = 1;
  for (i = 1; i < numobjects; i ++)
    if (obj [i].party == fighter [0]->party)
      alliedfighters ++;
}

void MissionCustom::getLakeMin (short x, short y)
{
  if (x <= 0 || y <= 0 || x >= MAXX || y >= MAXX) return;
  l->f [x] [y] = DEEPWATER;
  if (l->h [x] [y] < minh) minh = l->h [x] [y];
  if (l->f [x-1] [y] == WATER) getLakeMin (x-1, y);
  if (l->f [x+1] [y] == WATER) getLakeMin (x+1, y);
  if (l->f [x] [y-1] == WATER) getLakeMin (x, y-1);
  if (l->f [x] [y+1] == WATER) getLakeMin (x, y+1);
}

void MissionCustom::setLakeMin (short x, short y)
{
  if (x <= 0 || y <= 0 || x >= MAXX || y >= MAXX) return;
  l->hw [x] [y] = minh;
  l->f [x] [y] = SHALLOWWATER;
  if (l->h [x] [y] >= minh) l->h [x] [y] = minh - 50;
  if (l->f [x-1] [y] == DEEPWATER) setLakeMin (x-1, y);
  if (l->f [x+1] [y] == DEEPWATER) setLakeMin (x+1, y);
  if (l->f [x] [y-1] == DEEPWATER) setLakeMin (x, y-1);
  if (l->f [x] [y+1] == DEEPWATER) setLakeMin (x, y+1);
}

void MissionCustom::start ()
{
  int i, i2;
  int relx = 0, rely = 0;

  if (l != NULL) delete l;
  if (strlen (mapfile) == 0)
  {
    l = new GlLandscape (map_type, NULL);
  }
  else
  {
    l = new GlLandscape (map_type, NULL);

    unsigned char *map;
    int mapx, mapy;
    Bitmap *bitmap = BitmapFactory::getBitmap (Directory::getMaps (mapfile));
    map = bitmap->data;
    mapx = bitmap->width;
    mapy = bitmap->height;
//    map = LoadTga::load (Directory::getMaps (mapfile), &mapx, &mapy);
    if (map == NULL)
    {
      DISPLAY_FATAL("Map has a valid bpp entry but seems to be corrupt");
      assert (false);
      exit (1);
    }

    int mapcx = mapx, mapcy = mapy;
    if (mapcx > MAXX+1) mapcx = MAXX+1;
    if (mapcy > MAXX+1) mapcy = MAXX+1;

    memset ((unsigned short *) l->h, 0, (MAXX+1) * (MAXX+1) * sizeof (unsigned short));
    memset ((unsigned char *) l->f, 0, (MAXX+1) * (MAXX+1) * sizeof (unsigned char));
    int minh = 65535, maxh = 0;
    for (i = 0; i < mapcy; i ++)
      for (i2 = 0; i2 < mapcx; i2 ++)
      {
        int x = i2, y = MAXX-i;
        int addr = (i * mapx + i2) * 4 + 3;
        l->h [x] [y] = map [addr];
        if (map [addr] < minh) minh = map [addr];
        if (map [addr] > maxh) maxh = map [addr];
      }

    float hscale = 65534.0F / (maxh - minh) * scaleheight;
    float hadd = -hscale * minh + 0.5F + 32768.0F * (1.0F - scaleheight);
    for (i = 0; i < mapcy; i ++)
      for (i2 = 0; i2 < mapcx; i2 ++)
      {
        int x = i2, y = MAXX-i;
        float test = hadd + hscale * l->h [x] [y];
        l->h [x] [y] = (int) test;
        l->hw [x] [y] = l->h [x] [y];
      }

    for (i = 0; i < mapcy; i ++)
      for (i2 = 0; i2 < mapcx; i2 ++)
      {
        int x = i2, y = MAXX-i;
        int addr = (i * mapx + i2) * 4;
        int r = map [addr];
        int g = map [addr + 1];
        int b = map [addr + 2];

        if (save_map_type < LANDSCAPE_MOON) // alpine
        {
          l->f [x] [y] = GRASS;
          if (r <= 50 && g <= 50 && b >= 200)
            l->f [x] [y] = WATER;
          else if (r <= 50 && g >= 50 && g <= 200 && b <= 50)
            l->f [x] [y] = DECIDUOUSWOODS0;
          else if (r <= 120 && g <= 120 && b <= 120)
            l->f [x] [y] = ROCKS;
          else if (r <= 210 && g <= 210 && b <= 210)
            l->f [x] [y] = GRAVEL;
          else if (r > 210 && g > 210 && b > 210)
            l->f [x] [y] = GLACIER;
          else if (r > 210 && g < 50 && b < 50)
            l->f [x] [y] = TOWN;
        }
        else if (save_map_type < LANDSCAPE_CANYON) // moon
        {
          l->f [x] [y] = MOONSAND;
        }
        else if (save_map_type < LANDSCAPE_DESERT) // canyon
        {
          l->f [x] [y] = REDSTONE;
          if (r <= 50 && g <= 50 && b >= 200)
            l->f [x] [y] = WATER;
          else if (r > 210 && g > 210 && b > 210)
            l->f [x] [y] = REDSAND;
          else if (r > 150 && g > 150 && b < 50)
            l->f [x] [y] = REDTREE0;
        }
        else if (save_map_type < LANDSCAPE_ARCTIC) // desert
        {
          l->f [x] [y] = DESERTSAND;
          if (r <= 50 && g <= 50 && b >= 200)
            l->f [x] [y] = WATER;
          else if (r <= 50 && g >= 50 && b <= 50)
            l->f [x] [y] = CACTUS0;
        }
      }

    for (i = 1; i < mapcy - 1; i ++)
      for (i2 = 1; i2 < mapcx - 1; i2 ++)
      {
        int x = i2, y = MAXX-i;
        if (l->f [x] [y] == WATER)
        {
          this->minh = 0xFFFF;
          getLakeMin (x, y);
          setLakeMin (x, y);
        }
      }

    int htreemin = 65535, htreemax = 0;
    for (i = 0; i < mapcy; i ++)
      for (i2 = 0; i2 < mapcx; i2 ++)
      {
        int x = i2, y = MAXX-i;
        if (l->f [x] [y] == DECIDUOUSWOODS0)
        {
          if (l->h [x] [y] < htreemin) htreemin = l->h [x] [y];
          if (l->h [x] [y] > htreemax) htreemax = l->h [x] [y];
        }
      }

    int htreediff = htreemax - htreemin;
    int treeh1 = htreemin + htreediff * tree [0] / 100;
    int treeh2 = htreemin + htreediff * (tree [0] + tree [1]) / 100;
    int treeh3 = htreemin + htreediff * (tree [0] + tree [1] + tree [2]) / 100;
    for (i = 1; i < mapcx - 1; i ++)
      for (i2 = 1; i2 < mapcy - 1; i2 ++)
      {
        int x = i2, y = MAXX-i;
        if (l->f [x] [y] == DECIDUOUSWOODS0)
        {
          int density = 0;
          if (l->isWoods (l->h [x-1] [y])) density ++;
          if (l->isWoods (l->h [x+1] [y])) density ++;
          if (l->isWoods (l->h [x] [y+1])) density ++;
          if (l->isWoods (l->h [x] [y-1])) density ++;
          density --;
          if (density < 0) density = 0;
          if (l->h [x] [y] > treeh3)
            l->f [x] [y] = DWARFPINES0 + density;
          else if (l->h [x] [y] > treeh2)
            l->f [x] [y] = DECIDUOUSWOODS0 + density;
          else if (l->h [x] [y] > treeh1)
            l->f [x] [y] = MIXEDWOODS0 + density;
          else
            l->f [x] [y] = CONIFEROUSWOODS0 + density;
        }
      }

    l->precalculate ();
  }
  ::clouds = clouds;
  ::weather = weather;
  camera = 0;

  for (i = 1; i < numobjects; i ++)
  {
    int nextidle = obj [i - 1].idle;
    if (obj [i].idle == 0)
      obj [i].idle = nextidle;
  }

  if (numobjects > 0)
  {
    playerInit ();
    fplayer = fighter [0];
    if (obj [0].idle == 100)
    {
      l->searchPlain (-1, -1, &relx, &rely);
    }
    else if (obj [0].idle > 100 && obj [0].idle <= 116)
    {
      int addr = obj [0].idle - 101;
      l->searchPlain (addr % 4, addr / 4, &relx, &rely);
    }
    fplayer->trafo.translation.x = obj [0].trafo.translation.x + relx;
    fplayer->trafo.translation.z = obj [0].trafo.translation.z + rely;
  }

  int alliedz = 0;
  for (i = 1; i < numobjects; i ++)
  {
//    fighter [i]->o = getModel (obj [i].id);
    if (obj [i].id < FighterBeginDescriptor) obj [i].id = FalconDescriptor;
    if (obj [i].id >= FighterBeginDescriptor && obj [i].id <= FighterEndDescriptor && fplayer->party == obj [i].party)
    {
      alliedInit (obj [i].id, alliedpilot [alliedz]);
      if (alliedz < maxpilots - 1)
        alliedz ++;
    }
    else
    {
      if (obj [i].id >= FighterBeginDescriptor && obj [i].id <= FighterEndDescriptor)
        objectInit (new Fighter (obj [i].id), obj [i].party, obj [i].intelligence);
      else if (obj [i].id >= TankBeginDescriptor && obj [i].id <= TankEndDescriptor)
        objectInit (new Tank (obj [i].id), obj [i].party, obj [i].intelligence);
      else if (obj [i].id >= ShipBeginDescriptor && obj [i].id <= ShipEndDescriptor)
        objectInit (new Ship (obj [i].id), obj [i].party, obj [i].intelligence);
      else if (obj [i].id >= StaticAaBeginDescriptor && obj [i].id <= StaticAaEndDescriptor)
        objectInit (new StaticAa (obj [i].id), obj [i].party, obj [i].intelligence);
      else if (obj [i].id >= StaticPassiveBeginDescriptor)
        objectInit (new StaticPassive (obj [i].id), obj [i].party, obj [i].intelligence);
      else
        objectInit (new AiObj (obj [i].id), obj [i].party, obj [i].intelligence);
    }
    if (!obj [i].explode)
    {
      if (fighter [i]->party != fplayer->party) obj [i].explode = 1;
      else obj [i].explode = -1;
    }
    if (obj [i].idle == 1)
    {
      relx = (int) fighter [i - 1]->trafo.translation.x;
      rely = (int) fighter [i - 1]->trafo.translation.y;
    }
    else if (obj [i].idle == 2)
    {
      relx = (int) fighter [0]->trafo.translation.x;
      rely = (int) fighter [0]->trafo.translation.y;
    }
    else if (obj [i].idle == 100)
    {
      l->searchPlain (-1, -1, &relx, &rely);
    }
    else if (obj [i].idle > 100 && obj [i].idle <= 116)
    {
      int addr = obj [i].idle - 101;
      l->searchPlain (addr % 4, addr / 4, &relx, &rely);
    }
    fighter [i]->trafo.translation.x = obj [i].trafo.translation.x + relx;
    fighter [i]->trafo.translation.z = obj [i].trafo.translation.z + rely;
    if (obj [i].ttl > 0)
    {
      fighter [i]->deactivate ();
    }
  }
}

int MissionCustom::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  timer += dt;

  for (i = 1; i < numobjects; i ++)
  {
    if (obj [i].ttl > 0 && timer > obj [i].ttl)
    {
      obj [i].ttl = -1;
      fighter [i]->activate ();
      if (obj [i].idle == 1)
      {
        fighter [i]->trafo.translation.x = obj [i].trafo.translation.x + fighter [i - 1]->trafo.translation.x;
        fighter [i]->trafo.translation.z = obj [i].trafo.translation.z + fighter [i - 1]->trafo.translation.z;
      }
      else if (obj [i].idle == 2)
      {
        fighter [i]->trafo.translation.x = obj [i].trafo.translation.x + fighter [0]->trafo.translation.x;
        fighter [i]->trafo.translation.z = obj [i].trafo.translation.z + fighter [0]->trafo.translation.z;
      }
    }
  }

  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }

  for (i = 1; i < numobjects; i ++)
  {
    if (!fighter [i]->active && fighter [i]->explode >= 35 * timestep && fighter [i]->party == fplayer->party && obj [i].explode == 1)
    {
      return 2;
    }
  }

  for (i = 1; i < numobjects; i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party != fplayer->party && fighter [i]->explode < 35 * timestep && obj [i].explode == 1)
        b = true;
  }
  if (b) return 0;
  return 1;
}

void MissionCustom::draw ()
{
  int i;

  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }

  for (i = 0; i < numtext; i ++)
  {
    if (textstart [i] <= timer && textstart [i] + textdelay [i] >= timer)
      font1->drawTextCentered (0, 7, -3, text [i], textcolor);
  }
}

#endif
