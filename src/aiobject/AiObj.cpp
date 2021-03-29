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

/* This file includes all AI objects instancing models. */

#ifndef IS_AIOBJECT_H

#include "AiObj.h"
#include "logging/Logging.h"
#include "gllandscape/GlLandscape.h"
#include "game/globals.h"
#include "util/Math.h"
#include "loadmodel/Model3dRegistry.h"
#include "util/Util.h"

#include <cassert>


// id values of objects
// non-AI air objects
UnitDescriptor CannonBeginDescriptor (0);
UnitDescriptor Cannon1Descriptor (0, "Cannon1", "CANNON");
UnitDescriptor Cannon1bDescriptor (0, "Cannon1b", "CANNON");
UnitDescriptor Cannon2Descriptor (0, "Cannon2", "CANNON");
UnitDescriptor Cannon2bDescriptor (0, "Cannon2b", "CANNON");
UnitDescriptor AsteroidDescriptor (50, "Asteroid", "ASTEROID");
UnitDescriptor FlareDescriptor (80, "Flare", "FLARE");
UnitDescriptor ChaffDescriptor (85, "Chaff", "CHAFF");
UnitDescriptor CannonEndDescriptor (99);
// missiles
UnitDescriptor MissileBeginDescriptor (100);
UnitDescriptor AamHs1Descriptor (100, "AamHs1", "AAM HS MK1");
UnitDescriptor AamHs2Descriptor (101, "AamHs2", "AAM HS MK2");
UnitDescriptor AamHs3Descriptor (102, "AamHs3", "AAM HS MK3");
UnitDescriptor Agm1Descriptor (103, "Agm1", "AGM MK1");
UnitDescriptor Agm2Descriptor (104, "Agm2", "AGM MK2");
UnitDescriptor DfmDescriptor (105, "Dfm", "DFM");
UnitDescriptor AamFf1Descriptor (106, "AamFf1", "AAM FF MK1");
UnitDescriptor AamFf2Descriptor (107, "AamFf2", "AAM FF MK2");
UnitDescriptor MineDescriptor (150, "Mine", "MINE");
UnitDescriptor MissileEndDescriptor (199);
// air units
UnitDescriptor AirBeginDescriptor (200);
UnitDescriptor FighterBeginDescriptor (200);
UnitDescriptor FalconDescriptor (200, "Falcon", "FALCON");
UnitDescriptor SwallowDescriptor (201, "Swallow", "SWALLOW");
UnitDescriptor HawkDescriptor (202, "Hawk", "HAWK");
UnitDescriptor Hawk2Descriptor (203, "Hawk2", "HAWK II");
UnitDescriptor BuzzardDescriptor (204, "Buzzard", "BUZZARD");
UnitDescriptor CrowDescriptor (205, "Crow", "CROW");
UnitDescriptor PhoenixDescriptor (206, "Phoenix", "PHOENIX");
UnitDescriptor RedArrowDescriptor (207, "RedArrow", "RED ARROW");
UnitDescriptor BlackBirdDescriptor (208, "BlackBird", "BLACKBIRD");
UnitDescriptor StormDescriptor (209, "Storm", "STORM");
UnitDescriptor FighterEndDescriptor (249);
UnitDescriptor TransportDescriptor (280, "Transport", "TRANSPORT");
UnitDescriptor Transport2Descriptor (281, "Transport2", "TRANSPORT");
UnitDescriptor AirEndDescriptor (299);
// moving ground units from here
UnitDescriptor MovingGroundBeginDescriptor (500);
UnitDescriptor TankBeginDescriptor (700);
UnitDescriptor WieselDescriptor (700, "Wiesel", "WIESEL");
UnitDescriptor PantherDescriptor (710, "Panther", "PANTHER");
UnitDescriptor MobileSamDescriptor (711, "MobileSam", "MOBILE SAM");
UnitDescriptor PickupDescriptor (780, "Pickup", "PICKUP");
UnitDescriptor Pickup2Descriptor (781, "Pickup2", "PICKUP");
UnitDescriptor TruckDescriptor (790, "Truck", "TRUCK");
UnitDescriptor Truck2Descriptor (791, "Truck2", "TRUCK");
UnitDescriptor TankEndDescriptor (799);
// moving water units from here
UnitDescriptor WaterBeginDescriptor (800);
UnitDescriptor ShipBeginDescriptor (800);
UnitDescriptor CruiserDescriptor (800, "Cruiser", "CRUISER");
UnitDescriptor LightDestroyerDescriptor (810, "LightDestroyer", "LIGHT DESTROYER");
UnitDescriptor ShipEndDescriptor (899);
// static ground units from here
UnitDescriptor StaticGroundBeginDescriptor (1000);
UnitDescriptor StaticAaBeginDescriptor (1000);
UnitDescriptor SacDescriptor (1000, "Sac", "SA CANNON");
UnitDescriptor SamDescriptor (1010, "Sam", "SAM");
UnitDescriptor StaticAaEndDescriptor (1099);
// passive static units from here
UnitDescriptor StaticPassiveBeginDescriptor (10000);
UnitDescriptor TentDescriptor (10000, "Tent", "TENT");
UnitDescriptor BigTentDescriptor (10003, "BigTent", "BIG TENT");
UnitDescriptor ContainerDescriptor (10100, "Container", "CONTAINER");
UnitDescriptor HallDescriptor (10200, "Hall", "HALL");
UnitDescriptor Hall2Descriptor (10201, "Hall2", "HALL");
UnitDescriptor OilrigDescriptor (10300, "Oilrig", "OILRIG");
UnitDescriptor ComplexDescriptor (10301, "Complex", "COMPLEX");
UnitDescriptor RadarDescriptor (10302, "Radar", "RADAR");
UnitDescriptor MoonBaseDescriptor (10303, "MoonBase", "MOONBASE");
UnitDescriptor DepotDescriptor (10304, "Depot", "DEPOT");
UnitDescriptor LaserBarrierDescriptor (10400, "LaserBarrier", "LASER BARRIER");
UnitDescriptor RubbleDescriptor (11000, "Rubble", "RUBBLE");
UnitDescriptor HouseDescriptor (11100, "House", "HOUSE");
UnitDescriptor ExplosionDescriptor (20000, "Explosion", "EXPLOSION");



AiObj::AiObj ()
  : DynamicObj ()
{
  proto = NULL;
  smoke = NULL;

  init ();
  smoke = new Smoke (0);
}

AiObj::AiObj (const UnitDescriptor &desc)
  : DynamicObj () // intended!
{
  id = desc;
  proto = new AiObjPrototype (id);

  init ();
  initPrototype ();

//  trafo.scaling.set (1.0, 1.0, 1.0);
  smoke = new Smoke (0);
}

AiObj::AiObj (const UnitDescriptor &desc, Space *space2, Model3d *o2, float zoom2)
  : DynamicObj () // intended!
{
  id = desc;
  proto = new AiObjPrototype (id);
  space = space2;
  o = o2;
  trafo.scaling.set (zoom2, zoom2, zoom2);
  smoke = new Smoke (0);

  init ();
  initPrototype ();

  space->addObject (this);
}

AiObj::~AiObj ()
{
  delete smoke;
  smoke = NULL;
}

AiObjPrototype *AiObj::getPrototype ()
{
  AiObjPrototype *aiproto = dynamic_cast<AiObjPrototype *>(proto);
  assert (aiproto);
  return aiproto;
}

void AiObj::initPrototype ()
{
  DynamicObj::initPrototype ();

  ammo = getPrototype ()->maxammo;
  recthrust = getPrototype ()->maxthrust / 2.0;
  force.z = recthrust;
}

void AiObj::init ()
{
  DynamicObj::init ();

  int i;
  acttype = 0;
  intelligence = 100;
  aggressivity = 100;
  precision = 100;
  ai = true;
  target = NULL;
  dtheta = 0;
  dgamma = 0;
  manoevertheta = 0;
  manoeverheight = 0;
  manoeverthrust = 0;
  manoeverstate = 0;
  idle = 0;
  smokettl = 0;
  firecannonttl = 0;
  firemissilettl = 0;
  fireflarettl = 0;
  firechaffttl = 0;
  aw = 0;
  stat.points = 0;
  easymodel = 1;
  missiletype = 0;
  autofire = false;
  ttl = -1;
  ttf = 30 * timestep;
  score = -1;
  missiletype = 0;
  for (i = 0; i < missiletypes; i ++)
    missiles [i] = 0;
  for (i = 0; i < missileracks; i ++)
  {
    missilerack [i] = 0;
    missilerackn [i] = 0;
  }
  timer = 0;
}

void AiObj::missileCount ()
{
  if (id < FighterBeginDescriptor || id > AirEndDescriptor) return;

  int i;
  for (i = 0; i < missiletypes; i ++)
    missiles [i] = 0;
  for (i = 0; i < missileracks; i ++)
  {
    if (missilerackn [i] > 0)
    {
      if (missilerack [i] < 0 || missilerack [i] > 100)
      {
        DISPLAY_ERROR("Problem: missilerack[i] is not valid!");
      }
      else
      {
        missiles [missilerack [i]] += missilerackn [i];
      }
    }
  }
}

void AiObj::newinit (const UnitDescriptor &id, int party, int intelligence, int precision, int aggressivity)
{
/*  int i;
  float zoom = trafo.scaling.x;
  ai = true;
//  this->id = id;
  this->party = party;
  activate ();
  for (i = 0; i < missileracks; i ++)
    missilerackn [i] = 0;
  ammo = -1;
  bomber = 0;
  getPrototype ()->dualshot = false;
  float cubefac = 0.6F; // fighter
  float cubefac1 = 0.7F; // tanks and sams*/

/*  if (id == 200)
  {
    o = Model3dFactory::getModel (Directory::getModels ("gl-16.3ds"));
    o->setName ("HAWK");
  }
  else*/
//  o = getModel (id);
//  o->cube.set (trafo.scaling);

//  o = Model3dFactory::getModel (Directory::getModels ("gl-15.3ds"));
//  o = Model3dRegistry::get (id.name);

/*  if (id == FalconDescriptor)
  {
    getPrototype ()->maxthrust = 0.31;
    getPrototype ()->nimbility = 0.86;
    getPrototype ()->manoeverability = 0.48;
    getPrototype ()->maxshield = 85;
    zoom = 0.35;
    getPrototype ()->maxrot.theta = 90.0;
    getPrototype ()->maxrot.gamma = 70.0;
    missilerackn [0] = 2; missilerackn [1] = 2; missilerackn [2] = 2; missilerackn [3] = 2;
    missilerack [0] = 0; missilerack [1] = 6; missilerack [2] = 6; missilerack [3] = 0;
//    flares = 20;
//    chaffs = 20;
    statfirepower = 1;
    ammo = 1200;
  }
  else if (id == SwallowDescriptor)
  {
    getPrototype ()->maxthrust = 0.24;
    getPrototype ()->nimbility = 0.64;
    getPrototype ()->manoeverability = 0.35;
    getPrototype ()->maxshield = 110;
    zoom = 0.43;
    getPrototype ()->maxrot.theta = 90.0;
    getPrototype ()->maxrot.gamma = 70.0;
    missilerackn [0] = 2; missilerackn [1] = 3; missilerackn [2] = 3; missilerackn [3] = 2;
    missilerack [0] = 6; missilerack [1] = 3; missilerack [2] = 3; missilerack [3] = 6;
//    flares = 20;
//    chaffs = 20;
    bomber = 1;
    statfirepower = 3;
    ammo = 1200;
  }
  else if (id == HawkDescriptor)
  {
    getPrototype ()->maxthrust = 0.26;
    getPrototype ()->nimbility = 0.72;
    getPrototype ()->manoeverability = 0.42;
    getPrototype ()->maxshield = 120;
    zoom = 0.43;
    getPrototype ()->maxrot.theta = 90.0;
    getPrototype ()->maxrot.gamma = 70.0;
    missilerackn [0] = 1; missilerackn [1] = 3; missilerackn [2] = 3; missilerackn [3] = 1;
    missilerack [0] = 6; missilerack [1] = 3; missilerack [2] = 3; missilerack [3] = 6;
//    flares = 20;
//    chaffs = 20;
    bomber = 1;
    statfirepower = 2;
    ammo = 1200;
  }
  else if (id == Hawk2Descriptor)
  {
    getPrototype ()->maxthrust = 0.28;
    getPrototype ()->nimbility = 0.75;
    getPrototype ()->manoeverability = 0.44;
    getPrototype ()->maxshield = 140;
    zoom = 0.45;
    getPrototype ()->maxrot.theta = 90.0;
    getPrototype ()->maxrot.gamma = 70.0;
    missilerackn [0] = 1; missilerackn [1] = 3; missilerackn [2] = 3; missilerackn [3] = 1;
    missilerack [0] = 6; missilerack [1] = 4; missilerack [2] = 4; missilerack [3] = 6;
//    flares = 20;
//    chaffs = 20;
    bomber = 1;
    statfirepower = 3;
    ammo = 1400;
    getPrototype ()->dualshot = true;
  }
  else if (id == TransportDescriptor)
  {
    getPrototype ()->maxthrust = 0.14;
    getPrototype ()->maxshield = 45;
    missiles [0] = 0;
    getPrototype ()->nimbility = 0.15;
    getPrototype ()->manoeverability = 0.05;
    getPrototype ()->impact = 5;
    zoom = 1.5;
    getPrototype ()->maxrot.gamma = 25;
    getPrototype ()->maxrot.theta = 30;
//    flares = 0;
//    chaffs = 0;
    ammo = 0;
  }
  else if (id == Transport2Descriptor)
  {
    getPrototype ()->maxthrust = 0.16;
    getPrototype ()->maxshield = 35;
    missiles [0] = 0;
    getPrototype ()->nimbility = 0.12;
    getPrototype ()->manoeverability = 0.04;
    getPrototype ()->impact = 5;
    zoom = 1.5;
    getPrototype ()->maxrot.gamma = 25;
    getPrototype ()->maxrot.theta = 30;
//    flares = 0;
//    chaffs = 0;
    ammo = 0;
  }
  else if (id == BuzzardDescriptor)
  {
    getPrototype ()->maxthrust = 0.31;
    getPrototype ()->nimbility = 0.82;
    getPrototype ()->manoeverability = 0.46;
    getPrototype ()->maxshield = 75;
    zoom = 0.44;
    getPrototype ()->maxrot.theta = 90.0;
    getPrototype ()->maxrot.gamma = 70.0;
    missilerackn [0] = 2; missilerackn [1] = 2; missilerackn [2] = 2; missilerackn [3] = 2;
    missilerack [0] = 0; missilerack [1] = 6; missilerack [2] = 6; missilerack [3] = 0;
//    flares = 20;
//    chaffs = 20;
    statfirepower = 2;
    ammo = 1200;
  }
  else if (id == CrowDescriptor)
  {
    getPrototype ()->maxthrust = 0.25;
    getPrototype ()->nimbility = 0.72;
    getPrototype ()->manoeverability = 0.4;
    getPrototype ()->maxshield = 60;
    zoom = 0.41;
    getPrototype ()->maxrot.theta = 90.0;
    getPrototype ()->maxrot.gamma = 70.0;
    missilerackn [0] = 1; missilerackn [1] = 2; missilerackn [2] = 2; missilerackn [3] = 1;
    missilerack [0] = 6; missilerack [1] = 0; missilerack [2] = 0; missilerack [3] = 6;
//    flares = 20;
//    chaffs = 20;
    statfirepower = 1;
    ammo = 1000;
  }
  else if (id == StormDescriptor)
  {
    getPrototype ()->maxthrust = 0.25;
    getPrototype ()->nimbility = 0.52;
    getPrototype ()->manoeverability = 0.34;
    getPrototype ()->maxshield = 160;
    zoom = 0.45;
    getPrototype ()->maxrot.theta = 90.0;
    getPrototype ()->maxrot.gamma = 70.0;
    missilerackn [0] = 1; missilerackn [1] = 2; missilerackn [2] = 2; missilerackn [3] = 1;
    missilerack [0] = 6; missilerack [1] = 0; missilerack [2] = 0; missilerack [3] = 6;
//    flares = 25;
//    chaffs = 25;
    statfirepower = 4;
    ammo = 1800;
  }
  else if (id == PhoenixDescriptor)
  {
    getPrototype ()->maxthrust = 0.3;
    getPrototype ()->nimbility = 0.54;
    getPrototype ()->manoeverability = 0.34;
    getPrototype ()->maxshield = 180;
    zoom = 0.47;
    getPrototype ()->maxrot.theta = 90.0;
    getPrototype ()->maxrot.gamma = 70.0;
    missilerackn [0] = 3; missilerackn [1] = 3; missilerackn [2] = 3; missilerackn [3] = 3;
    missilerack [0] = 4; missilerack [1] = 4; missilerack [2] = 4; missilerack [3] = 4;
//    flares = 25;
//    chaffs = 25;
    bomber = 1;
    statfirepower = 5;
    ammo = 2000;
    getPrototype ()->dualshot = true;
  }
  else if (id == RedArrowDescriptor)
  {
    getPrototype ()->maxthrust = 0.33;
    getPrototype ()->nimbility = 0.95;
    getPrototype ()->manoeverability = 0.52;
    getPrototype ()->maxshield = 120;
    zoom = 0.4;
    getPrototype ()->maxrot.theta = 90.0;
    getPrototype ()->maxrot.gamma = 70.0;
    missilerackn [0] = 2; missilerackn [1] = 3; missilerackn [2] = 3; missilerackn [3] = 2;
    missilerack [0] = 7; missilerack [1] = 1; missilerack [2] = 1; missilerack [3] = 7;
//    flares = 25;
//    chaffs = 25;
    statfirepower = 2;
    ammo = 1400;
    getPrototype ()->dualshot = true;
  }
  else if (id == BlackBirdDescriptor)
  {
    getPrototype ()->maxthrust = 0.3;
    getPrototype ()->nimbility = 1.0;
    getPrototype ()->manoeverability = 0.54;
    getPrototype ()->maxshield = 85;
    zoom = 0.33;
    getPrototype ()->maxrot.theta = 90.0;
    getPrototype ()->maxrot.gamma = 70.0;
    missilerackn [0] = 2; missilerackn [1] = 3; missilerackn [2] = 3; missilerackn [3] = 2;
    missilerack [0] = 7; missilerack [1] = 1; missilerack [2] = 1; missilerack [3] = 7;
//    flares = 25;
//    chaffs = 25;
    statfirepower = 2;
    ammo = 1400;
    getPrototype ()->dualshot = true;
  }
  if (id >= FighterBeginDescriptor && id <= AirEndDescriptor)
  {
    recthrust = getPrototype ()->maxthrust / 2.0;
    shield = getPrototype ()->maxshield;
    thrust = recthrust = getPrototype ()->maxthrust / 2;
    smoke->type = 1;
    getPrototype ()->impact = 2;
    force.z = recthrust;
    o->cube.set (zoom * cubefac, zoom * cubefac, zoom * cubefac);
  }

  if (id == SacDescriptor)
  {
    getPrototype ()->maxthrust = 0;
    thrust = 0;
    getPrototype ()->maxrot.gamma = 0;
    getPrototype ()->maxrot.theta = 0.03;
    getPrototype ()->manoeverability = 12.0;
    shield = getPrototype ()->maxshield = 80;
    zoom = 0.35;
  }
  if (id == SamDescriptor)
  {
    getPrototype ()->maxthrust = 0;
    thrust = 0;
    getPrototype ()->maxrot.gamma = 0;
    getPrototype ()->maxrot.theta = 0.03;
    getPrototype ()->manoeverability = 6.0;
    shield = getPrototype ()->maxshield = 70;
    zoom = 0.3;
    missiles [6] = 100;
  }
  if (id >= StaticAaBeginDescriptor && id <= StaticAaEndDescriptor)
  {
    o->cube.set (zoom * cubefac1, zoom * cubefac1, zoom * cubefac1);
  }

  if (id == WieselDescriptor)
  {
    getPrototype ()->maxthrust = 0.04;
    thrust = 0;
    currot.gamma = 180; currot.theta = 0; currot.phi = 0;
    getPrototype ()->maxrot.gamma = 0;
    getPrototype ()->maxrot.theta = 0.03;
    getPrototype ()->manoeverability = 8.0;
    shield = getPrototype ()->maxshield = 160;
    zoom = 0.35;
    o->cube.set (zoom * 0.7, zoom * 0.45, zoom * 0.7);
  }
  else if (id == PantherDescriptor)
  {
    getPrototype ()->maxthrust = 0.04;
    thrust = 0;
    currot.gamma = 180; currot.theta = 0; currot.phi = 0;
    getPrototype ()->maxrot.gamma = 0;
    getPrototype ()->maxrot.theta = 0.03;
    getPrototype ()->manoeverability = 8.0;
    shield = getPrototype ()->maxshield = 200;
    zoom = 0.4;
    o->cube.set (zoom * 0.7, zoom * 0.5, zoom * 0.7);
  }
  else if (id == PickupDescriptor)
  {
    getPrototype ()->maxthrust = 0;
    thrust = 0.02;
    getPrototype ()->maxrot.gamma = 0;
    getPrototype ()->maxrot.theta = 0.03;
    getPrototype ()->manoeverability = 0;
    shield = getPrototype ()->maxshield = 30;
    zoom = 0.25;
    o->cube.set (zoom * 0.7, zoom * 0.55, zoom * 0.7);
  }
  else if (id == TruckDescriptor)
  {
    getPrototype ()->maxthrust = 0;
    thrust = 0.02;
    getPrototype ()->maxrot.gamma = 0;
    getPrototype ()->maxrot.theta = 0.03;
    getPrototype ()->manoeverability = 0;
    shield = getPrototype ()->maxshield = 20;
    zoom = 0.45;
    o->cube.set (zoom * 0.6, zoom * 0.35, zoom * 0.6);
  }
  else if (id == Truck2Descriptor)
  {
    getPrototype ()->maxthrust = 0;
    thrust = 0.02;
    getPrototype ()->maxrot.gamma = 0;
    getPrototype ()->maxrot.theta = 0.03;
    getPrototype ()->manoeverability = 0;
    shield = getPrototype ()->maxshield = 40;
    zoom = 0.4;
    o->cube.set (zoom * 0.6, zoom * 0.35, zoom * 0.6);
  }
  else if (id == MobileSamDescriptor)
  {
    getPrototype ()->maxthrust = 0;
    thrust = 0.02;
    getPrototype ()->maxrot.gamma = 0;
    getPrototype ()->maxrot.theta = 0.03;
    getPrototype ()->manoeverability = 0;
    shield = getPrototype ()->maxshield = 50;
    zoom = 0.35;
    missiles [6] = 200;
    o->cube.set (zoom * 0.7, zoom * 0.6, zoom * 0.7);
  }
  if (id >= TankBeginDescriptor && id <= TankEndDescriptor)
  {
  }

  if (id == CruiserDescriptor)
  {
    zoom = 5.0;
    getPrototype ()->maxthrust = 0.05;
    thrust = 0.05;
    getPrototype ()->maxrot.gamma = 0;
    getPrototype ()->maxrot.theta = 0.03;
    getPrototype ()->manoeverability = 4.0;
    getPrototype ()->impact = 20;
    shield = getPrototype ()->maxshield = 5500;
    missiles [6] = 200;
    o->cube.set (zoom * 0.35, zoom * 0.1, zoom * 0.35);
  }
  else if (id == LightDestroyerDescriptor)
  {
    zoom = 2.5;
    getPrototype ()->maxthrust = 0.05;
    thrust = 0.05;
    getPrototype ()->maxrot.gamma = 0;
    getPrototype ()->maxrot.theta = 0.03;
    getPrototype ()->manoeverability = 6.0;
    getPrototype ()->impact = 20;
    shield = getPrototype ()->maxshield = 2800;
    o->cube.set (zoom * 0.4, zoom * 0.12, zoom * 0.4);
  }

  float missilethrustbase = 1.2F;
  if (id == AamHs1Descriptor)
  {
    intelligence = 100;
    getPrototype ()->maxthrust = 0.7 * missilethrustbase;
    getPrototype ()->nimbility = 2.5; // old 2.2
    getPrototype ()->manoeverability = 1.5;
    ttl = 300 * timestep;
    getPrototype ()->impact = 35;
  }
  else if (id == AamHs2Descriptor)
  {
    intelligence = 50;
    getPrototype ()->maxthrust = 0.75 * missilethrustbase;
    getPrototype ()->nimbility = 3.5; // old 3.5
    getPrototype ()->manoeverability = 2.0;
    ttl = 320 * timestep;
    getPrototype ()->impact = 45;
  }
  else if (id == AamHs3Descriptor)
  {
    intelligence = 0;
    getPrototype ()->maxthrust = 0.8 * missilethrustbase;
    getPrototype ()->nimbility = 4.5;
    getPrototype ()->manoeverability = 2.5;
    ttl = 340 * timestep;
    getPrototype ()->impact = 55;
  }
  else if (id == Agm1Descriptor)
  {
    intelligence = 50;
    getPrototype ()->maxthrust = 0.75 * missilethrustbase;
    getPrototype ()->nimbility = 1.2;
    getPrototype ()->manoeverability = 1.0;
    ai = true;
    ttl = 300 * timestep;
    getPrototype ()->impact = 400;
  }
  else if (id == Agm2Descriptor)
  {
    intelligence = 0;
    getPrototype ()->maxthrust = 0.8 * missilethrustbase;
    getPrototype ()->nimbility = 1.5;
    getPrototype ()->manoeverability = 1.0;
    ai = true;
    ttl = 400 * timestep;
    getPrototype ()->impact = 500;
  }
  else if (id == DfmDescriptor)
  {
    intelligence = 0;
    getPrototype ()->maxthrust = 0.75 * missilethrustbase;
    getPrototype ()->nimbility = 0.0;
    getPrototype ()->manoeverability = 0.0;
    ai = true;
    ttl = 350 * timestep;
    getPrototype ()->impact = 920;
  }
  else if (id == AamFf1Descriptor)
  {
    intelligence = 0;
    getPrototype ()->maxthrust = 0.8 * missilethrustbase;
    getPrototype ()->nimbility = 2.0;
    getPrototype ()->manoeverability = 1.3;
    ttl = 300 * timestep;
    getPrototype ()->impact = 40;
  }
  else if (id == AamFf2Descriptor)
  {
    intelligence = 0;
    getPrototype ()->maxthrust = 0.85 * missilethrustbase;
    getPrototype ()->nimbility = 3.0;
    getPrototype ()->manoeverability = 2.0;
    ttl = 320 * timestep;
    getPrototype ()->impact = 50;
  }
  else if (id == MineDescriptor)
  {
    intelligence = 0;
    getPrototype ()->maxthrust = 0.1;
    if (difficulty == 1) getPrototype ()->maxthrust = 0.14;
    else if (difficulty == 2) getPrototype ()->maxthrust = 0.18;
    getPrototype ()->nimbility = 1.5;
    getPrototype ()->manoeverability = 1.0;
    ai = true;
    ttl = -1;
    getPrototype ()->impact = 500;
    zoom = 0.3;
  }
  if (id >= MissileBeginDescriptor && id <= MissileEndDescriptor)
  {
    o->cube.set (zoom, zoom, zoom);
  }

  if (id >= StaticPassiveBeginDescriptor)
  {
    intelligence = 0;
    getPrototype ()->maxthrust = 0;
    getPrototype ()->nimbility = 0;
    getPrototype ()->manoeverability = 0;
    getPrototype ()->impact = 5;
    getPrototype ()->maxrot.theta = 0;
    getPrototype ()->maxrot.gamma = 0;
  }
  if (id == TentDescriptor)
  {
    shield = getPrototype ()->maxshield = 80;
    zoom = 0.5;
    o->cube.set (zoom * 0.9, zoom, zoom * 0.9);
  }
  if (id == BigTentDescriptor)
  {
    shield = getPrototype ()->maxshield = 160;
    zoom = 1.2;
    o->cube.set (zoom * 0.7, zoom * 0.42, zoom * 0.7);
  }
  if (id == ContainerDescriptor)
  {
    shield = getPrototype ()->maxshield = 30;
    zoom = 1.0;
    getPrototype ()->impact = 20;
    o->cube.set (zoom * 0.4, zoom * 0.35, zoom * 0.9);
  }
  if (id == HallDescriptor)
  {
    shield = getPrototype ()->maxshield = 450;
    zoom = 1.8;
    getPrototype ()->impact = 20;
    o->cube.set (zoom * 0.45, zoom * 0.42, zoom);
  }
  if (id == Hall2Descriptor)
  {
    shield = getPrototype ()->maxshield = 900;
    zoom = 2.5;
    getPrototype ()->impact = 20;
    o->cube.set (zoom, zoom * 0.45, zoom);
  }
  if (id == OilrigDescriptor)
  {
    shield = getPrototype ()->maxshield = 1400;
    zoom = 3.5;
    getPrototype ()->impact = 20;
    o->cube.set (zoom * 0.95, zoom * 0.5, zoom * 0.95);
  }
  if (id == ComplexDescriptor)
  {
    shield = getPrototype ()->maxshield = 5000;
    zoom = 2.0;
    getPrototype ()->impact = 20;
    o->cube.set (zoom * 0.75, zoom * 0.6, zoom * 0.75);
  }
  if (id == RadarDescriptor)
  {
    shield = getPrototype ()->maxshield = 500;
    zoom = 1.3;
    getPrototype ()->impact = 20;
    o->cube.set (zoom * 0.5, zoom * 0.7, zoom * 0.5);
  }
  if (id == AsteroidDescriptor)
  {
    shield = getPrototype ()->maxshield = 100000;
    zoom = 0.01 * Math::random (60) + 1.0;
    getPrototype ()->impact = 5;
    thrust = 0.25;
    getPrototype ()->maxthrust = 0.25;
    force.z = 0.12;
    ai = false;
    o->cube.set (zoom * 0.7, zoom * 0.7, zoom * 0.7);
  }
  if (id == MoonBaseDescriptor)
  {
    shield = getPrototype ()->maxshield = 5500;
    zoom = 4.0;
    getPrototype ()->impact = 20;
    o->cube.set (zoom * 0.7, zoom * 0.5, zoom * 0.7);
  }
  if (id == DepotDescriptor)
  {
    shield = getPrototype ()->maxshield = 3000;
    zoom = 1.5;
    getPrototype ()->impact = 20;
    o->cube.set (zoom, zoom * 0.5, zoom);
  }
  if (id == LaserBarrierDescriptor)
  {
    shield = getPrototype ()->maxshield = 1000;
    zoom = 12.0;
    getPrototype ()->impact = 2000;
    o->cube.set (0.8, 11, 11);
  }
  if (id >= StaticPassiveBeginDescriptor)
  {
  }*/

//  missileCount ();
}

void AiObj::newinit (const UnitDescriptor &id, int party, int intelligence)
{
  newinit (id, party, intelligence, intelligence, intelligence);
}

void AiObj::initValues (DynamicObj *dobj, float phi)
{
  float fac = trafo.scaling.x / 8;
  if (dobj->id == FlareDescriptor || dobj->id == ChaffDescriptor) fac = -fac;
  // use the exact polar coordinates because of gamma and theta
  float cgamma = currot.gamma;
  dobj->trafo.translation.x = trafo.translation.x + COS(cgamma) * SIN(phi) * fac;
  dobj->trafo.translation.y = trafo.translation.y - SIN(cgamma) * fac;
  if ((id >= StaticAaBeginDescriptor && id <= StaticAaEndDescriptor) || (id >= TankBeginDescriptor && id <= TankEndDescriptor))
    dobj->trafo.translation.y += fac;
  dobj->trafo.translation.z = trafo.translation.z + COS(cgamma) * COS(phi) * fac;
  dobj->currot.phi = phi;
  dobj->recrot.theta = dobj->currot.theta;
  dobj->force.set (force);
  dobj->trafo.rotation.set ((short) (90 + dobj->currot.gamma - 180), (short) dobj->currot.theta + 180, (short) -dobj->currot.phi);
}

void AiObj::fireCannon (DynamicObj *l, float phi)
{
  if (firecannonttl > 0) return;
  if (ammo == 0) return;

  laser.add (l);

  ammo --;
//  l->thrust = 0;
//  l->recthrust = l->thrust;
  l->getPrototype ()->manoeverability = 0.0;
//  l->getPrototype ()->maxthrust = 0;
  if (target != NULL && ai)
  {
    if (target->active)
    {
      // exact calculation to hit enemy (non-static turret!)
      if (id >= FighterBeginDescriptor && id <= AirEndDescriptor)
        l->currot.gamma = currot.gamma;
      else
        l->currot.gamma = 180.0 + atan ((target->trafo.translation.y - trafo.translation.y) / distance (target)) * 180.0 / PI;
    }
  }
  else
    l->currot.gamma = currot.gamma; // + 90.0;
  l->party = party;
//  l->ttl = 80 * timestep;
//  l->shield = 1;
  l->immunity = (int) (trafo.scaling.x * 12) * timestep;
  l->source = this;
  l->currot.phi = phi;
  l->currot.theta = currot.theta;
  initValues (l, phi);
  float fac = 0.7F;
  l->force.x += COS(l->currot.gamma) * SIN(l->currot.phi) * fac;
  l->force.y -= SIN(l->currot.gamma) * fac;
  l->force.z += COS(l->currot.gamma) * COS(l->currot.phi) * fac;
  l->activate ();
  firecannonttl += 45;
  if (day)
  {
    if (getPrototype ()->dualshot)
      l->o = Model3dRegistry::get ("Cannon1b");
    else
      l->o = Model3dRegistry::get ("Cannon1");
  }
  else
  {
    if (getPrototype ()->dualshot)
      l->o = Model3dRegistry::get ("Cannon2b");
    else
      l->o = Model3dRegistry::get ("Cannon2");
  }
}

void AiObj::fireCannon (std::vector<DynamicObj *> &laser, float phi)
{
  if (firecannonttl > 0) return;
  if (ammo == 0) return;
  fireCannon (new DynamicObj (Cannon1Descriptor), phi);
}

void AiObj::fireCannon (std::vector<DynamicObj *> &laser)
{
  if (firecannonttl > 0) return;
  if (ammo == 0) return;
  fireCannon (laser, currot.phi);
}

void AiObj::fireMissile2 (AiObj *m, AiObj *target)
{
  DISPLAY_DEBUG(FormatString ("Missile: party=%d, id=%d", party, id.id));

  missile.add (m);

  ttf = 50 * timestep;
/*  missile->id = id;
  missile->init ();
  missile->initPrototype ();
  missile->newinit (id, party, 0);*/
  initValues (m, currot.phi);
//  missile->explode = 0;
  m->thrust = thrust + 0.001;
  m->recthrust = m->getPrototype ()->maxthrust;
  m->currot.gamma = currot.gamma;
  m->target = target;
  m->recrot.gamma = currot.gamma;
//  missile->shield = 1;
  m->party = party;
  m->immunity = (45 + (int) (trafo.scaling.x * 6.0)) * timestep;
  m->dtheta = 0;
  m->dgamma = 0;
  m->source = this;
//  missile->ttl = 300 * timestep;
  m->activate ();
  if (this->id >= FighterBeginDescriptor && this->id <= AirEndDescriptor)
  {
    m->manoeverheight = 30 * timestep;
    m->recheight = m->trafo.translation.y - l->getHeight (m->trafo.translation.x, m->trafo.translation.z) - 4;
  }
}

int AiObj::firstMissile ()
{
  int i = 0;
  while (!missiles [i])
  {
    i ++;
    if (i >= missiletypes) return 0;
  }
  ttf = 50 * timestep;
  return i;
}

int AiObj::nextMissile (int from)
{
  int i = from + 1;
  if (i >= missiletypes) i = 0;
  while (!missiles [i])
  {
    i ++;
    if (i >= missiletypes) i = 0;
    if (i == from) break;
  }
  ttf = 50 * timestep;
  return i;
}

bool AiObj::haveMissile (const UnitDescriptor &id)
{
  int value = id - MissileBeginDescriptor;
  if (value < 0 || value >= missiletypes)
  {
    DISPLAY_ERROR("Wrong missile ID");
  }
  if (missiles [value] > 0)
    return true;
  return false;
}

bool AiObj::haveMissile () // due to missiletype
{
  if (missiles [missiletype] > 0)
    return true;
  return false;
}

void AiObj::decreaseMissile (const UnitDescriptor &id)
{
  int i;
  int value = id - MissileBeginDescriptor;
  if (value < 0 || value >= missiletypes)
  {
    DISPLAY_ERROR("Wrong missile ID");
  }
  missiles [value] --;
  int ptrrack = 0, maxrack = 0;
  for (i = 0; i < missileracks; i ++)
    if (missilerack [i] == id.id)
      if (missilerackn [i] > maxrack)
      {
        ptrrack = i;
        maxrack = missilerackn [i];
      }
  if (maxrack > 0)
  {
    missilerackn [ptrrack] --;
    ref [ptrrack * 3 + 2 - missilerackn [ptrrack]].trafo.scaling.set (0, 0, 0);
  }
}

bool AiObj::fireMissile (const UnitDescriptor &id, std::vector<AiObj *> &missile, AiObj *target)
{
  if (!haveMissile (id)) return false;
  if (ttf > 0) return false;
  fireMissile2 (new Missile (id), target);
  decreaseMissile (id);
  firemissilettl = 20 * timestep;
  return true;
}

bool AiObj::fireMissile (std::vector<AiObj *> &missile, AiObj *target)
{
  if (ttf > 0) return false;
  UnitDescriptor desc = UnitDescriptorRegistry::get (MissileBeginDescriptor + missiletype);
  return fireMissile (desc, missile, (AiObj *) target);
}

bool AiObj::fireMissile (const UnitDescriptor &id, std::vector<AiObj *> &missile)
{
  if (ttf > 0) return false;
  return fireMissile (id, missile, (AiObj *) target);
}

bool AiObj::fireMissile (std::vector<AiObj *> &missile)
{
  if (ttf > 0) return false;
  UnitDescriptor desc = UnitDescriptorRegistry::get (MissileBeginDescriptor + missiletype);
  return fireMissile (desc, missile);
}

bool AiObj::fireMissileAir (std::vector<AiObj *> &missile, AiObj *target)
{
  if (ttf > 0) return false;
  if (target->id >= MovingGroundBeginDescriptor) return false;
  if (haveMissile (AamHs3Descriptor))
    return fireMissile (AamHs3Descriptor, missile, (AiObj *) target);
  else if (haveMissile (AamHs2Descriptor))
    return fireMissile (AamHs2Descriptor, missile, (AiObj *) target);
  else if (haveMissile (AamHs1Descriptor))
    return fireMissile (AamHs1Descriptor, missile, (AiObj *) target);
  return false;
}

bool AiObj::selectMissileAir (std::vector<AiObj *> &missile)
{
  bool sel = false;
  if (haveMissile (AamHs3Descriptor))
  { missiletype = AamHs3Descriptor - MissileBeginDescriptor; sel = true; }
  else if (haveMissile (AamHs2Descriptor))
  { missiletype = AamHs2Descriptor - MissileBeginDescriptor; sel = true; }
  else if (haveMissile (AamHs1Descriptor))
  { missiletype = AamHs1Descriptor - MissileBeginDescriptor; sel = true; }
  return sel;
}

bool AiObj::fireMissileAirFF (std::vector<AiObj *> &missile, AiObj *target)
{
  if (ttf > 0) return false;
  if (target->id >= MovingGroundBeginDescriptor) return false;
  if (haveMissile (AamFf2Descriptor))
    return fireMissile (AamFf2Descriptor, missile, (AiObj *) target);
  else if (haveMissile (AamFf1Descriptor))
    return fireMissile (AamFf1Descriptor, missile, (AiObj *) target);
  return false;
}

bool AiObj::selectMissileAirFF (std::vector<AiObj *> &missile)
{
  bool sel = false;
  if (haveMissile (AamFf2Descriptor))
  { missiletype = AamFf2Descriptor - MissileBeginDescriptor; sel = true; }
  else if (haveMissile (AamFf1Descriptor))
  { missiletype = AamFf1Descriptor - MissileBeginDescriptor; sel = true; }
  return sel;
}

bool AiObj::fireMissileGround (std::vector<AiObj *> &missile)
{
  if (ttf > 0) return false;
  if (target->id < MovingGroundBeginDescriptor) return false;
  if (haveMissile (Agm2Descriptor))
    return fireMissile (Agm2Descriptor, missile, (AiObj *) target);
  else if (haveMissile (Agm1Descriptor))
    return fireMissile (Agm1Descriptor, missile, (AiObj *) target);
  return false;
}

bool AiObj::selectMissileGround (std::vector<AiObj *> &missile)
{
  bool sel = false;
  if (haveMissile (Agm2Descriptor))
  { missiletype = Agm2Descriptor - MissileBeginDescriptor; sel = true; }
  else if (haveMissile (Agm1Descriptor))
  { missiletype = Agm1Descriptor - MissileBeginDescriptor; sel = true; }
  return sel;
}

void AiObj::targetNearestGroundEnemy (std::vector<AiObj *> &f)
{
  float d = 1E12; //10000 is too low
  ttf = 50 * timestep;
  for (unsigned i = 0; i < f.size (); i ++)
  {
    if (this != f [i] && party != f [i]->party && f [i]->active)
    {
      float phi = getAngle (f [i]);
      float d2 = distance (f [i]) * (60 + fabs (phi)); // prefer enemies in front
      if (bomber)
        if (f [i]->id < MovingGroundBeginDescriptor)
          d2 += 1E10; // only use this target if no ground targets exist
      if (d2 < d)
      {
        d = d2;
        target = f [i];
      }
    }
  }
  if (target)
    if (distance (target) > 400)
    { target = NULL; }
}

void AiObj::targetNearestEnemy (std::vector<AiObj *> &f)
{
  float d = 1E12; //10000 is too low
  ttf = 50 * timestep;
  for (unsigned i = 0; i < f.size (); i ++)
  {
    if (this != f [i] && party != f [i]->party && f [i]->active)
    {
      float phi = getAngle (f [i]);
      float d2 = distance (f [i]) * (60 + fabs (phi)); // prefer enemies in front
      if (d2 < d)
      {
        d = d2;
        target = f [i];
      }
    }
  }
  if (!ai && target)
    if (distance (target) > 400)
      target = NULL;
}

void AiObj::targetLockingEnemy (std::vector<AiObj *> &f)
{
  int i;
  ttf = 50 * timestep;
  if (target == NULL) target = f [0];
  for (i = 0; i < static_cast<int>(f.size ()); i ++)
    if (target == f [i])
      break;
  int z = 0;
  do
  {
    i ++;
    if (i >= static_cast<int>(f.size ())) { i = 0; z ++; }
  } while ((!f [i]->active || f [i]->party == party || f [i]->target != this || distance (f [i]) > 200) && z <= 1);
  target = f [i];
  if (z > 1 && !ai) target = NULL;
}

void AiObj::targetNext (std::vector<AiObj *> &f)
{
  int i;
  ttf = 50 * timestep;
  if (target == NULL) target = f [0];
  for (i = 0; i < static_cast<int>(f.size ()); i ++)
    if (target == f [i])
      break;
  int z = 0;
  do
  {
    i ++;
    if (i >= static_cast<int>(f.size ())) i = 0;
    if (f [i] == this)
    { i ++; z ++; }
    if (i >= static_cast<int>(f.size ())) i = 0;
  } while ((!f [i]->active || distance (f [i]) > 400) && z <= 1);
  target = f [i];
  if (z > 1 && !ai) target = NULL;
}

void AiObj::targetNextEnemy (std::vector<AiObj *> &f)
{
  int i;
  ttf = 50 * timestep;
  if (target == NULL) target = f [0];
  for (i = 0; i < static_cast<int>(f.size ()); i ++)
    if (target == f [i])
      break;
  int z = 0;
  do
  {
    i ++;
    if (i >= static_cast<int>(f.size ())) i = 0;
    if (f [i] == this)
    { i ++; z ++; }
    if (i >= static_cast<int>(f.size ())) i = 0;
  } while ((!f [i]->active || distance (f [i]) > 400 || party == f [i]->party) && z <= 1);
  target = f [i];
  if (z > 1 && !ai) target = NULL;
}

void AiObj::targetPrevious (std::vector<AiObj *> &f)
{
  int i;
  ttf = 50 * timestep;
  if (target == NULL) target = f [0];
  for (i = 0; i < static_cast<int>(f.size ()); i ++)
    if (target == f [i])
      break;
  int z = 0;
  do
  {
    i --;
    if (i < 0) i = static_cast<int>(f.size ()) - 1;
    if (f [i] == this)
    { i --; z ++; }
    if (i < 0) i = static_cast<int>(f.size ()) - 1;
  } while ((!f [i]->active || distance (f [i]) > 400) && z <= 1);
  target = f [i];
  if (z > 1 && !ai) target = NULL;
}

void AiObj::setSmoke (Uint32 dt)
{
  float sz = COS(currot.gamma) * COS(currot.phi) * trafo.scaling.z * 1.1; // polar (spherical) coordinates
  float sy = -SIN(currot.gamma) * trafo.scaling.y * 1.1;
  float sx = COS(currot.gamma) * SIN(currot.phi) * trafo.scaling.x * 1.1;

  // some smoke elements per discrete movement
  float fg = sqrt (force.x * force.x + force.y * force.y + force.z * force.z) * 13;
  if (fg >= MAXSMOKEELEM) fg = (float) MAXSMOKEELEM - 0.5;
  for (int i = 0; i < (int) fg; i ++)
  {
    float fac = (float) i / fg;
    smoke->setSmoke (trafo.translation.x - sx - force.x * fac, trafo.translation.y - sy - force.y * fac, trafo.translation.z - sz - force.z * fac, 39 - i);
  }
/*    smoke->setSmoke (trafo.translation.x - sx - force.x * 0.6, trafo.translation.y - sy - force.y * 0.6, trafo.translation.z - sz - force.z * 0.6, (int) phi, 36);
  smoke->setSmoke (trafo.translation.x - sx - force.x * 0.4, trafo.translation.y - sy - force.y * 0.4, trafo.translation.z - sz - force.z * 0.4, (int) phi, 37);
  smoke->setSmoke (trafo.translation.x - sx - force.x * 0.2, trafo.translation.y - sy - force.y * 0.2, trafo.translation.z - sz - force.z * 0.2, (int) phi, 38);
  smoke->setSmoke (trafo.translation.x - sx, trafo.translation.y - sy, trafo.translation.z - sz, (int) phi, 39);
  smoke->move (dt, 5);*/
  smoke->move (dt, (int) fg + 1);
  smokettl += timestep;
}

void AiObj::checkTtl (Uint32 dt)
{
  if (firecannonttl > 0) firecannonttl -= dt; // time to fire the next missile
  if (firemissilettl > 0) firemissilettl -= dt; // time to fire the next missile
  if (fireflarettl > 0) fireflarettl -= dt; // time to fire the next flare
  if (firechaffttl > 0) firechaffttl -= dt; // time to fire the next chaff
  if (smokettl > 0) smokettl -= dt; // time to fire the next chaff
}

void AiObj::selectNewTarget (std::vector<AiObj *> &f)
{
  if (bomber)
    targetNearestGroundEnemy (f);
  else
    targetNearestEnemy (f);
}

void AiObj::selectTarget (std::vector<AiObj *> &f)
{
  if (target == NULL)
  {
    selectNewTarget (f);
  }
  if (target != NULL)
  {
    if (!target->active)
      selectNewTarget (f);
  }
}

float AiObj::getMinimumHeight ()
{
  // precalculated height
  int lsdist = 15;
  float flyx = trafo.translation.x + force.x * lsdist, flyz = trafo.translation.z + force.z * lsdist;
  float flyx2 = trafo.translation.x + force.x * lsdist * 3, flyz2 = trafo.translation.z + force.z * lsdist * 3;
  float flyx3 = trafo.translation.x + force.x * lsdist * 8, flyz3 = trafo.translation.z + force.z * lsdist * 8;
  float h1 = l->getMaxHeight (flyx, flyz);
  float h2 = l->getMaxHeight (flyx2, flyz2);
  float h3 = l->getMaxHeight (flyx3, flyz3);
  h1 = h1 > h2 ? h1 : h2;
  h1 = h1 > h3 ? h1 : h3;
  return h1;
}

void AiObj::easyPiloting (Uint32 dt)
{
  float timefac = (float) dt / (float) timestep;
  float deltatheta;
  deltatheta = recrot.theta - currot.theta;
  if (fabs (dtheta) > 30)
  { dtheta = 0; }
  float mynimbility = fabs (deltatheta) / 5.0F * getPrototype ()->nimbility;
  if (mynimbility > getPrototype ()->nimbility) mynimbility = getPrototype ()->nimbility;
  float nimbility2 = mynimbility;
  if (nimbility2 >= -0.00001 && nimbility2 <= 0.00001)
    nimbility2 = 0.00001;

  if (deltatheta > 0 && dtheta < 0) dtheta += mynimbility * timefac;
  else if (deltatheta < 0 && dtheta > 0) dtheta -= mynimbility * timefac;
  else if (deltatheta > 0)
  {
    float estimatedtheta = dtheta * (dtheta + nimbility2 * 5 / timefac) / 2 / nimbility2;
    if (deltatheta > estimatedtheta) dtheta += mynimbility * timefac;
    else if (deltatheta < estimatedtheta) dtheta -= mynimbility * timefac;
  }
  else
  {
    float estimatedtheta = -dtheta * (dtheta - nimbility2 * 5 / timefac) / 2 / nimbility2;
    if (deltatheta < estimatedtheta) dtheta -= mynimbility * timefac;
    else if (deltatheta > estimatedtheta) dtheta += mynimbility * timefac;
  }
  if (dtheta > (getPrototype ()->nimbility * (1.0 + realspeed)) * timefac * 5.0F)
    dtheta = (getPrototype ()->nimbility * (1.0 + realspeed)) * timefac * 5.0F;
  currot.theta += dtheta;

  assert (easymodel == 1);
  // height changes
  float nimbility1 = getPrototype ()->nimbility / 5;
  if (nimbility1 >= -0.00001 && nimbility1 <= 0.00001)
    nimbility1 = 0.00001;
  if (currot.theta > getPrototype ()->maxrot.theta) currot.theta = getPrototype ()->maxrot.theta; // restrict roll angle
  else if (currot.theta < -getPrototype ()->maxrot.theta) currot.theta = -getPrototype ()->maxrot.theta;

  float deltagamma = recrot.gamma - currot.gamma;
  if (deltagamma > 0 && dgamma < 0) dgamma += nimbility1 * timefac;
  else if (deltagamma < 0 && dgamma > 0) dgamma -= nimbility1 * timefac;
  else if (deltagamma > 0)
  {
    float estimatedgamma = dgamma * (dgamma + nimbility1 * 2) / nimbility1;
    if (id == 200)
      id = id;
    if (deltagamma > estimatedgamma + 2) dgamma += nimbility1 * timefac;
    else if (deltagamma < estimatedgamma - 2) dgamma -= nimbility1 * timefac;
  }
  else if (deltagamma < 0)
  {
    float estimatedgamma = -dgamma * (dgamma + nimbility1 * 2) / nimbility1;
    if (id == 200)
      id = id;
    if (deltagamma < estimatedgamma - 2) dgamma -= nimbility1 * timefac;
    else if (deltagamma > estimatedgamma + 2) dgamma += nimbility1 * timefac;
  }
  if (dgamma > getPrototype ()->manoeverability * (3.33 + 15.0 * realspeed) * timefac)
    dgamma = getPrototype ()->manoeverability * (3.33 + 15.0 * realspeed) * timefac;
  currot.gamma += dgamma;
}

void AiObj::limitRotation ()
{
  if (currot.gamma > 180 + getPrototype ()->maxrot.gamma) currot.gamma = 180 + getPrototype ()->maxrot.gamma;
  else if (currot.gamma < 180 - getPrototype ()->maxrot.gamma) currot.gamma = 180 - getPrototype ()->maxrot.gamma;
}

void AiObj::estimateTargetPosition (float *dx2, float *dz2)
{
  float ex, ez;
  float t = 10.0 * disttarget; // generous time to new position
  if (t > 60) t = 60; // higher values will not make sense
  t *= (float) (400 - precision) / 400;
  int tt = (int) target->currot.theta;
  if (tt < 0) tt += 360;
  float newphi = t * SIN(tt) * 5.0 * target->getPrototype ()->manoeverability; // new angle of target after time t
  if (newphi > 90) newphi = 90;
  else if (newphi < -90) newphi = -90;
  newphi += (float) target->currot.phi;
  if (newphi >= 360) newphi -= 360;
  if (newphi < 0) newphi += 360;
  if ((id >= FighterBeginDescriptor && id <= AirEndDescriptor) || (id >= StaticAaBeginDescriptor && id <= StaticAaEndDescriptor) || (id >= TankBeginDescriptor && id <= TankEndDescriptor))
  {
    ex = target->trafo.translation.x - SIN(newphi) * t * target->realspeed * 0.25; // estimated target position x
    ez = target->trafo.translation.z - COS(newphi) * t * target->realspeed * 0.25; // estimated target position z
  }
  else
  {
    ex = target->trafo.translation.x - SIN(newphi) * t * target->realspeed * 0.05; // estimated target position x
    ez = target->trafo.translation.z - COS(newphi) * t * target->realspeed * 0.05; // estimated target position z
  }
  *dx2 = ex - trafo.translation.x; *dz2 = ez - trafo.translation.z; // estimated distances
}

void AiObj::estimateHeading (float dx2, float dz2)
{
  float a, w = currot.phi;
  if (dz2 > -0.0001 && dz2 < 0.0001) dz2 = 0.0001;

  // get heading to target
  a = atan (dx2 / dz2) * 180 / PI;
  if (dz2 > 0)
  {
    if (dx2 > 0) a -= 180.0F;
    else a += 180.0F;
  }
//    this->aw = a;
  aw = a - w; // aw=0: target in front, aw=+/-180: target at back
  if (aw < -180) aw += 360;
  if (aw > 180) aw -= 360;
}

void AiObj::estimateFighterHeading (AiObj *fi)
{
  disttarget = distance (fi); // distance to target
  float ex = fi->trafo.translation.x; // estimated target position x
  float ez = fi->trafo.translation.z; // estimated target position z
  float dx2 = ex - trafo.translation.x;
  float dz2 = ez - trafo.translation.z; // estimated distances
  int w = (int) currot.phi;
  if (dz2 > -0.0001 && dz2 < 0.0001) dz2 = 0.0001;
  int a = (atan (dx2 / dz2) * 180 / PI);
  if (dz2 > 0)
  {
    if (dx2 > 0) a -= 180;
    else a += 180;
  }
  aw = a - w;
  if (aw < -180) aw += 360;
  if (aw > 180) aw -= 360;
}

void AiObj::decreaseManoeverCounter (Uint32 dt)
{
  if (manoeverheight > 0) manoeverheight -= dt;
  if (manoevertheta > 0) manoevertheta -= dt;
  if (manoeverthrust > 0) manoeverthrust -= dt;
}

int AiObj::getFireRate ()
{
  if (difficulty == 0) return 12;
  else if (difficulty == 1) return 6;
  else return 3;
}

// core AI method
void AiObj::aiAction (Uint32 dt, std::vector<AiObj *> &f, std::vector<AiObj *> &m, std::vector<DynamicObj *> &c, std::vector<DynamicObj *> &flare, std::vector<DynamicObj *> &chaff, float camphi, float camgamma)
{
  timer += dt;

  if (!active && !draw) // not active, not drawn, then exit
  {
    return;
  }

  checkTtl (dt);

  // move object according to our physics
  move (dt, camphi, camgamma);

  if (id >= StaticPassiveBeginDescriptor) // no AI for static ground objects (buildings)
    return;

  // set smoke
  if ((id >= MissileBeginDescriptor && id < MineDescriptor) || (id >= FighterBeginDescriptor && id <= AirEndDescriptor)) // missile or fighter
  {
    setSmoke (dt);
  }

  if (!active) // not active, then exit
  {
    return;
  }
  if (explode > 0 || sink > 0) // exploding or sinking, then exit
  {
    thrust = 0;
    return;
  }

  // do expensive calculations only once
  float myheight = l->getExactHeight (trafo.translation.x, trafo.translation.z);
  float targetheight = trafo.translation.y;
  if (target != NULL)
    targetheight = l->getExactHeight (target->trafo.translation.x, target->trafo.translation.z);
  if (target != NULL)
    disttarget = distance (target); // distance to target
  else
    disttarget = 1;

  // get a new target if necessary
  if (id >= MissileBeginDescriptor && id <= MissileEndDescriptor)
  {
    if (target == NULL)
      ttl = 0;
    else if (!target->active)
      ttl = 0;
  }

  selectTarget (f);

  if (!ai) return;

  // which height???
  float recheight2; // this is the height, the object wants to achieve
  int lsdist = 15;
  float flyx = trafo.translation.x + force.x * lsdist, flyz = trafo.translation.z + force.z * lsdist;
  if (manoeverheight > 0)
  {
    // precalculated height
    recheight2 = l->getExactHeight (flyx, flyz) + recheight;
  }
  else
  {
     // missiles and non intelligent objects will not change their height due to the surface
    if ((id >= MissileBeginDescriptor && id <= MissileEndDescriptor && target != NULL) ||
        (trafo.translation.y - myheight > 8 && target != NULL && trafo.translation.y - myheight < 50/* && !manoeverheight*/))
    {
      recheight2 = target->trafo.translation.y - 8 * target->thrust * SIN(target->currot.gamma);
    }
    else
    {
      recheight2 = recheight + getMinimumHeight ();
    }
  }

  // FIGHTERs only: manoevers (may use the height information)
//  if (!performManoevers (myheight))
//    return;

  decreaseManoeverCounter (dt);

  // calculate the recommended height, recheight2 depends on it
  if (manoeverheight <= 0)
  {
    if (!(id >= FighterBeginDescriptor && id <= AirEndDescriptor) && target != NULL) // no fighter, has target (missile, mine)
    {
      recheight = target->trafo.translation.y - targetheight;
    }
    else if (id == TransportDescriptor || id == Transport2Descriptor) // transporters have to stay higher
    {
      recheight = 40; manoeverheight = 1;
    }
    else if (id >= FighterBeginDescriptor && id <= AirEndDescriptor && target != NULL) // fighter, has target
    {
      if (target->id >= FighterBeginDescriptor && target->id <= AirEndDescriptor)
        recheight = target->trafo.translation.y - targetheight;  // target is a fighter
      else
        recheight = target->trafo.translation.y - targetheight + 5; // target is no fighter

      int flyxs = GETCOORD((int) (trafo.translation.x + force.x * 15)), flyzs = GETCOORD((int) (trafo.translation.z + force.z * 15));
      if (!l->isWater (l->f [flyxs] [flyzs])) // not flying above water
      {
        if (recheight < 3.5 + 0.01 * aggressivity)
          recheight = 3.5 + 0.01 * aggressivity;
      }

      float minh = 5.5 + 0.01 * aggressivity; // minimum height
      if (l->type == LAND_CANYON) minh = 6.5 + 0.01 * aggressivity; // stay higher in canyons
      if (fabs (trafo.translation.y - myheight) < minh)
      {
        recheight = 9 + 0.015 * aggressivity;
        if (fabs (trafo.translation.y - myheight) < minh * 0.3)
        {
          manoeverstate = 10;
          DISPLAY_DEBUG("Manoever: Vertical climb");
        }
        else
        {
          manoeverheight = 5 * timestep; // fly manoever to gain height
        }
      }

      if (disttarget < 50 && fabs (trafo.translation.y - myheight) > 25)
      {
        recheight = 8 + 0.025 * aggressivity;
        manoeverheight = 12 * timestep;
      }
    }
  }

  // get recommended elevation to target
  if (ttl <= 0 && id >= MissileBeginDescriptor && id <= MissileEndDescriptor && id != MineDescriptor)
  { recheight = -100; recheight2 = -100; recrot.gamma = 90; }
  else if (ai)
  {
    if (target != NULL && ((id >= MissileBeginDescriptor && id <= MissileEndDescriptor) || (id >= FighterBeginDescriptor && id <= AirEndDescriptor && manoeverheight <= 0))) // is AGM
    {
      float dgamma = 0;
      if (disttarget <= -0.00001 || disttarget >= 0.00001) // no division by zero
        dgamma = atan ((target->trafo.translation.y - trafo.translation.y) / disttarget) * 180 / PI - (currot.gamma - 180);
      recrot.gamma = currot.gamma + dgamma;
    }
    else
    {
      recrot.gamma = (int) ((recheight2 - trafo.translation.y) * 10 - currot.gamma + 360);
    }
  }





  // do a smooth roll
  if (easymodel == 1)
  {
    easyPiloting (dt);
  }

  limitRotation ();

  if (id >= MissileBeginDescriptor && id <= MissileEndDescriptor)
  {
    if (target == NULL)
    {
      ttl = 0;
      return;
    }
    else if (target->active == false)
    {
      ttl = 0;
      return;
    }
  }

  if (target == NULL) return;

  // fighter's targeting mechanism for missiles
  if (id >= FighterBeginDescriptor && id <= AirEndDescriptor) // for fighters do the following
  {
    if (ai)
    {
      if (target->id >= FighterBeginDescriptor && target->id <= AirEndDescriptor)
      {
        if (!selectMissileAirFF (m))
          selectMissileAir (m);
      }
      else
      {
        selectMissileGround (m);
      }
    }
  }

  if (!ai || target == NULL) // no AI (player) or no target found, then exit
  {
    return;
  }

  float dx2, dz2;
  float dx = target->trafo.translation.x - trafo.translation.x, dz = target->trafo.translation.z - trafo.translation.z; // current distances
  if ((id >= FighterBeginDescriptor && id <= AirEndDescriptor) || (id >= MissileBeginDescriptor && id <= MissileEndDescriptor) || (id >= StaticAaBeginDescriptor && id <= StaticAaEndDescriptor) || (id >= TankBeginDescriptor && id <= TankEndDescriptor))
  {
    estimateTargetPosition (&dx2, &dz2);
  }
  else
  {
    dx2 = dx; dz2 = dz;
  }

  estimateHeading (dx2, dz2);

  // heading calculations
  int firerate = getFireRate ();

  if (id >= MissileBeginDescriptor && id <= MissileEndDescriptor) // for missiles do the following
  {
    if (fabs (aw) < 50 && disttarget > 50) // target in front and minimum distance, then no roll
      recrot.theta = 0;
    else // otherwise chase target
    {
      if (aw < -90 || aw > 90) recrot.theta = 0;
      else if (aw > 0)
      {
        recrot.theta = aw > 90 ? 90 : aw;
      }
      else
      {
        recrot.theta = aw < -90 ? -90 : aw;
      }
    }
  }
  else if (id >= StaticAaBeginDescriptor && id <= StaticAaEndDescriptor) // ground-air-cannon
  {
    recthrust = 0; thrust = 0;
    if (aw > 5)
    {
      recrot.theta = getPrototype ()->maxrot.theta;
    }
    else if (aw < -5)
    {
      recrot.theta = -getPrototype ()->maxrot.theta;
    }
    else
    {
      recrot.theta = 0;
    }
  }
  else if (id >= TankBeginDescriptor && id <= TankEndDescriptor) // tanks
  {
    recthrust = getPrototype ()->maxthrust; thrust = getPrototype ()->maxthrust; // always at maximum thrust
    if (aw > 5)
    {
      recrot.theta = getPrototype ()->maxrot.theta;
    }
    else if (aw < -5)
    {
      recrot.theta = -getPrototype ()->maxrot.theta;
    }
    else
    {
      recrot.theta = 0;
    }
    if (firecannonttl <= 0)
    {
      if (id == WieselDescriptor)
        if (fabs (recrot.theta - currot.theta) < 2 && fabs (aw) < 20 && disttarget < 40 && target->trafo.translation.y > trafo.translation.y + 2)
        {
          fireCannon (c);
          firecannonttl += firerate * timestep;
        }
      if (id == PantherDescriptor)
        if (fabs (recrot.theta - currot.theta) < 2 && fabs (aw) < 20 && disttarget < 35 && target->trafo.translation.y <= trafo.translation.y + 1 && target->trafo.translation.y >= trafo.translation.y - 1)
        {
          fireCannon (c);
          firecannonttl += firerate * timestep;
        }
    }
  }

  // thrust and manoever calculations
  if ((id >= StaticAaBeginDescriptor && id <= StaticAaEndDescriptor) || id == CruiserDescriptor || id == LightDestroyerDescriptor || id == MobileSamDescriptor)
  {
    if (firecannonttl <= 0)
      for (unsigned i = 0; i < f.size (); i ++)
        if (f [i]->active && party != f [i]->party)
        {
          estimateFighterHeading (f [i]);

          if (id == SacDescriptor)
            if (f [i]->trafo.translation.y > trafo.translation.y + 2)
            {
              if (fabs (aw) <= 20 && disttarget < 50) // + aggressive
                fireCannon (c, currot.phi + aw);
              firecannonttl = firerate * timestep;
            }
          if (id == LightDestroyerDescriptor)
            if (f [i]->trafo.translation.y > trafo.translation.y + 2)
            {
              if (aw >= 0 && aw < 40 && disttarget < 50) // + aggressive
                fireCannon (c, currot.phi + aw);
              if (aw >= -40 && aw < 0 && disttarget < 50) // + aggressive
                fireCannon (c, currot.phi + aw);
              if (aw >= 120 && aw < 160 && disttarget < 50) // + aggressive
                fireCannon (c, currot.phi + aw);
              if (aw >= -160 && aw < -120 && disttarget < 50) // + aggressive
                fireCannon (c, currot.phi + aw);
              firecannonttl = firerate * timestep;
            }
          if (firemissilettl <= 0)
            if (id == SamDescriptor)
              if (fabs (aw) < 25 && disttarget < 45) // + aggressive
                if (f [i]->trafo.translation.y > trafo.translation.y + 2)
                {
                  ttf = 0;
                  fireMissileAirFF (m, f [i]);
                  firemissilettl += (20 + firerate * 10) * timestep;
                }
          if (id == MobileSamDescriptor)
          {
            if (firemissilettl <= 0)
              if (aw >= -30 && aw < 30 && disttarget < 60) // + aggressive
              {
                ttf = 0;
                fireMissileAirFF (m, f [i]);
                firemissilettl += aggressivity / 5 * timestep;
                missiles [6] ++; // unlimited ammo
              }
          }
          if (id == CruiserDescriptor)
          {
            if (firemissilettl <= 0)
              if (aw >= -30 && aw < 30 && disttarget < 60) // + aggressive
              {
                ttf = 0;
                fireMissileAirFF (m, f [i]);
                firemissilettl += aggressivity / 5 * timestep;
                missiles [6] ++; // unlimited ammo
              }
            if (firecannonttl <= 0)
            {
              fireCannon (c, currot.phi + aw);
            }
          }
        }
  }
}

#endif
