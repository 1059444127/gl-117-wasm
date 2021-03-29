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



DynamicObj::DynamicObj ()
{
  proto = NULL;
  init ();
}

DynamicObj::DynamicObj (const UnitDescriptor &desc)
{
  id = desc;
  proto = new DynamicObjPrototype (id);

  init ();
  initPrototype ();
}

DynamicObj::DynamicObj (const UnitDescriptor &desc, Space *space2, Model3d *o2, float zoom2)
{
  id = desc;
  proto = new DynamicObjPrototype (id);
  space = space2;
  o = o2;
  trafo.scaling.set (zoom2, zoom2, zoom2);

  init ();
  initPrototype ();

  space->addObject (this);
}

DynamicObj::~DynamicObj ()
{
  delete proto;
  proto = NULL;
}

void DynamicObj::activate ()
{
  active = true;
  draw = true;
}

void DynamicObj::deactivate ()
{
  valid = false;
  active = false;
  draw = false;
}

DynamicObjPrototype *DynamicObj::getPrototype ()
{
  return proto;
}

void DynamicObj::initPrototype ()
{
  o = Model3dRegistry::get (id.name);
  thrust = getPrototype ()->maxthrust;
  shield = getPrototype ()->maxshield;
  trafo.scaling.set (getPrototype ()->maxzoom, getPrototype ()->maxzoom, getPrototype ()->maxzoom);
  o->cube.set (getPrototype ()->cube);
  ttl = getPrototype ()->maxttl * timestep;
}

void DynamicObj::init ()
{
  SpaceObj::init ();

  acc.set (0.0, 0.0, 0.0);
  active = true;
  bomber = false;
  braking = 0.0;
  currot.set (180.0, 0.0, 0.0);
  easymodel = 1; // easy model
  elevatoreffect = 0;
  force.set (0.0, 0.0, 0.0);
  immunity = 0;
  party = 0;
  ttl = -1;
  realism = false;
  realspeed = 0;
  recelevatoreffect = 0;
  recheight = 5.0;
  recrolleffect = 0;
  recrot.set (180.0, 0.0, 0.0);
  recruddereffect = 0;
  recthrust = 0;
  rolleffect = 0;
  ruddereffect = 0;
  sink = 0;
  source = NULL;
  stat = ObjectStatistics ();

  trafo.rotation.set (90.0, 0.0, 0.0);
  trafo.translation.set (0.0, 0.0, 0.0);
}

void DynamicObj::thrustUp ()
{
  recthrust += getPrototype ()->maxthrust / 12;
  if (recthrust > getPrototype ()->maxthrust) recthrust = getPrototype ()->maxthrust;
}

void DynamicObj::thrustDown ()
{
  recthrust -= getPrototype ()->maxthrust / 12;
  if (recthrust < getPrototype ()->maxthrust / 2) recthrust = getPrototype ()->maxthrust / 2;
}

float DynamicObj::distance (DynamicObj *target)
{
  float dx = target->trafo.translation.x - trafo.translation.x;
  float dz = target->trafo.translation.z - trafo.translation.z;
  float dy = target->trafo.translation.y - trafo.translation.y;
  return sqrt (dx * dx + dz * dz + dy * dy);
}

float DynamicObj::distanceXZ (DynamicObj *target)
{
  float dx = target->trafo.translation.x - trafo.translation.x;
  float dz = target->trafo.translation.z - trafo.translation.z;
  return sqrt (dx * dx + dz * dz);
}

// check whether the object is exploding or sinking and deactivate if necessary
void DynamicObj::checkExplosion (Uint32 dt)
{
  if (explode > 0)
  {
    if (explode == 1)
    {
      ttl = -1;
      if (id == ContainerDescriptor || id == RadarDescriptor || id == ComplexDescriptor)
      {
        setExplosion (1.5, 40 * timestep);
        setBlackSmoke (3.0, 80 * timestep);
      }
      else if (id == OilrigDescriptor)
      {
        setExplosion (3.0, 40 * timestep);
        setBlackSmoke (5.5, 80 * timestep);
      }
      else if (id == TentDescriptor)
      {
      }
      else if (id == TankBeginDescriptor)
      {
      }
      else
      {
        float zoom2 = trafo.scaling.x * 2;
        if (zoom2 > 2) zoom2 = 2;
        setExplosion (zoom2, 35 * timestep);
        setBlackSmoke (1.0, 60 * timestep);
      }
    }
    if (id >= StaticGroundBeginDescriptor || (id >= MovingGroundBeginDescriptor && id <= WaterBeginDescriptor))
    {
      if (explode >= 25 * timestep && ttl == -1)
      {
        setExplosion (trafo.scaling.x * 2, 35 * timestep);
        setBlackSmoke (1.0, 60 * timestep);
        ttl = -2;
      }
      if (explode >= 30 * timestep && ttl == -2)
      {
        setExplosion (trafo.scaling.x * 2, 35 * timestep);
        setBlackSmoke (1.0, 60 * timestep);
        ttl = -3;
      }
    }
    if (explode >= 35 * timestep)
    {
      deactivate ();
      ttl = -1;
      explode += dt; // must be > 35*timestep to end mission
      if (id >= StaticGroundBeginDescriptor || (id >= MovingGroundBeginDescriptor && id <= WaterBeginDescriptor))
      {
        explode = 0;
        draw = true;
        id = StaticPassiveBeginDescriptor;
        shield = 100000;
        o = Model3dRegistry::get ("Rubble");
        trafo.scaling.x *= 0.7F;
        if (trafo.scaling.x > 1) trafo.scaling.x = 1;
        trafo.translation.y = l->getExactHeight (trafo.translation.x, trafo.translation.z) + trafo.scaling.x / 4;
      }
    }
    else
    {
      explode += dt;
    }
  }
  if (sink)
  {
    sink += dt;
    if (sink > 100 * timestep)
    {
      deactivate (); ttl = -1;
    }
  }
}

// check the objects shield value and explode/sink if necessary
void DynamicObj::checkShield ()
{
  if (shield <= 0)
  {
    shield = 0;
    if (explode <= 0)
    {
      if (id >= MissileBeginDescriptor && id <= MissileEndDescriptor)
      { explode = 1; active = false; }
      if (id >= FighterBeginDescriptor && id <= AirEndDescriptor)
      { explode = 1; active = false; }
      if (id >= TankBeginDescriptor && id <= TankEndDescriptor)
      { explode = 1; active = false; }
      if (id >= StaticAaBeginDescriptor && id <= StaticAaEndDescriptor)
      { explode = 1; active = false; }
      if (id >= StaticPassiveBeginDescriptor)
      { explode = 1; active = false; }
    }
    if (sink <= 0)
      if (id >= ShipBeginDescriptor && id <= ShipEndDescriptor)
      { sink = 1; }
  }
}

// check whether the object collides on the ground and alter gamma and y-translation
void DynamicObj::crashGround (Uint32 dt)
{
  if (immunity > 0)
    return;
  if (id >= MovingGroundBeginDescriptor)
    return;
  float height = trafo.translation.y - l->getExactHeight (trafo.translation.x, trafo.translation.z);
  if (height < trafo.scaling.x)
  {
    trafo.translation.y -= (height - trafo.scaling.x);
    currot.gamma += 10;
    if (shield > 0)
    {
      if (id >= MissileBeginDescriptor && id <= MissileEndDescriptor)
      {
        setExplosion (1.2, 30 * timestep);
        setBlackSmoke (1.2, 30 * timestep);
      }
      if (id >= FighterBeginDescriptor && id <= AirEndDescriptor)
      {
        setExplosion (0.2, 25 * timestep);
        setBlackSmoke (0.5, 25 * timestep);
      }
    }
    if (id >= CannonBeginDescriptor && id <= CannonEndDescriptor)
      deactivate ();
    float decfac = 3.0F;
    if (this == (DynamicObj *) fplayer /*&& gamestate == &stateplay*/)
    {
      if (difficulty == 1) decfac = 6.0F;
      else if (difficulty == 2) decfac = 15.0F;
    }
    if (realism && this == (DynamicObj *) fplayer /*&& gamestate == &stateplay*/)
	    shield = -1;
    else
      shield -= decfac * dt / timestep;
  }
  // restrict to a maximum height, we want an action game!!! a little bit more now 50 -> 80
  if (height > 80) trafo.translation.y = l->getHeight (trafo.translation.x, trafo.translation.z) + 80;
}

// check for collision, simplified model, each model is surrounded by a cube
// this works pretty well, but we must use more than one model for complex models or scenes
void DynamicObj::collide (DynamicObj *d, Uint32 dt) // d must be the medium (laser, missile)
{
  if (immunity > 0 || d->immunity > 0) return;
  if (explode > 0 || sink > 0) return;

  bool collide = false;
  if (trafo.translation.x + o->cube.x >= d->trafo.translation.x - d->o->cube.x && trafo.translation.x - o->cube.x <= d->trafo.translation.x + d->o->cube.x &&
      trafo.translation.y + o->cube.y >= d->trafo.translation.y - d->o->cube.y && trafo.translation.y - o->cube.y <= d->trafo.translation.y + d->o->cube.y &&
      trafo.translation.z + o->cube.z >= d->trafo.translation.z - d->o->cube.z && trafo.translation.z - o->cube.z <= d->trafo.translation.z + d->o->cube.z)
  {
    collide = true;
  }

  if (collide)
  {
    if (this == (DynamicObj *) fplayer /*&& gamestate == &stateplay*/ && realism && d->id >= AirBeginDescriptor && d->id < MovingGroundBeginDescriptor)
	  {
      shield = -1.0F; // player collision vs another plane in SIM mode, boom
	    d->shield = -1.0F;
	  }
    if (id < StaticPassiveBeginDescriptor || (id >= StaticPassiveBeginDescriptor && d->id >= MissileBeginDescriptor && d->id <= MissileEndDescriptor))
      shield -= (float) d->getPrototype ()->impact;
    else
      shield -= 2.0F;
    d->shield -= (float) getPrototype ()->impact;
    if (d->source != NULL && active) // only for missiles/cannons
    {
      if (d->source->party != party) // calculate points
      {
        if (getPrototype ()->maxshield < 2000)
          d->source->stat.points += (int) getPrototype ()->impact; // extra points for shooting an enemy object
      }
      else
      {
        d->source->stat.points -= (int) getPrototype ()->impact; // subtract points for shooting an own object
      }

      if (shield <= 0)
        if (d->source->party != party && active && draw && !stat.killed)
          if (d->source->id >= FighterBeginDescriptor && d->source->id <= AirEndDescriptor)
          {
            stat.killed = true;
            if (id >= FighterBeginDescriptor && id <= AirEndDescriptor)
              d->source->stat.fighterkills ++;
            else if (id >= ShipBeginDescriptor && id <= ShipEndDescriptor)
              d->source->stat.shipkills ++;
            else if ((id >= StaticAaBeginDescriptor && id <= StaticAaEndDescriptor) || (id >= TankBeginDescriptor && id <= TankEndDescriptor))
              d->source->stat.tankkills ++;
            else
              d->source->stat.otherkills ++;
          }
    }
    setExplosion (0.2, 20 * timestep);
    setBlackSmoke (0.5, 30 * timestep);
  }
}

void DynamicObj::setExplosion (float maxzoom, int len)
{
  Explosion *expl = new Explosion (space, Model3dRegistry::get (ExplosionDescriptor.name));
  explosion.add (expl);

/*  int i;
  for (i = 0; i < maxexplosion; i ++) // search a free explosion instance
    if (explosion [i]->ttl <= 0)
      break;
  if (i >= maxexplosion) i = 0; */
  expl->setExplosion (trafo.translation.x, trafo.translation.y, trafo.translation.z, force.x, force.y, force.z, maxzoom, len);
}

void DynamicObj::setBlackSmoke (float maxzoom, int len)
{
  BlackSmoke *bs = new BlackSmoke (space);

  blacksmoke.add (bs);
/*  int i;
  for (i = 0; i < maxblacksmoke; i ++) // search a free blacksmoke instance
    if (blacksmoke [i]->ttl <= 0)
      break;
  if (i >= maxblacksmoke) i = 0; */
  bs->setBlackSmoke (trafo.translation.x, trafo.translation.y, trafo.translation.z, currot.phi, maxzoom, len);
}

// return heading difference towards enemy
int DynamicObj::getAngle (DynamicObj *o)
{
  float dx2 = o->trafo.translation.x - trafo.translation.x, dz2 = o->trafo.translation.z - trafo.translation.z;
  int a, w = (int) currot.phi;
  if (dz2 > -0.0001 && dz2 < 0.0001) dz2 = 0.0001;

  a = (int) (atan (dx2 / dz2) * 180 / PI);
  if (dz2 > 0)
  {
    if (dx2 > 0) a -= 180;
    else a += 180;
  }
  int aw = a - w;
  if (aw < -180) aw += 360;
  if (aw > 180) aw -= 360;
  return aw;
}

// return elevation difference towards enemy
int DynamicObj::getAngleH (DynamicObj *o)
{
  float disttarget = distance (o);
  return (int) (atan ((o->trafo.translation.y - trafo.translation.y) / disttarget) * 180 / PI - (currot.gamma - 180));
}

// check for a looping, this is tricky :-)
bool DynamicObj::checkLooping ()
{
  if (currot.gamma > 270)
  {
    currot.gamma = 540 - currot.gamma;
    currot.theta += 180;
    currot.phi += 180;
    recrot.theta += 180;
    if (currot.theta >= 360) currot.theta -= 360;
    if (recrot.theta >= 360) recrot.theta -= 360;
    if (currot.phi >= 360) currot.phi -= 360;
    return true;
  }
  else if (currot.gamma < 90)
  {
    currot.gamma = 180 - currot.gamma;
    currot.theta += 180;
    currot.phi += 180;
    recrot.theta += 180;
    if (currot.theta >= 360) currot.theta -= 360;
    if (recrot.theta >= 360) recrot.theta -= 360;
    if (currot.phi >= 360) currot.phi -= 360;
    return true;
  }
  return false;
}

// discrete movement, called about timestep times per second, timer-dependant, currently without extra thread (GLUT)!
void DynamicObj::move (Uint32 dt, float camphi, float camgamma)
{
  if (dt <= 0) return;
  if (realspeed <= 0) realspeed = 1.0F;

  float brakepower = 1.0F;
  float timefac = (float) dt / (float) timestep;

  crashGround (dt); // check ground collision before moving = after all other collision tests are done!
  checkExplosion (dt); // check if this object is exploding
  if (sink > 0) // only ships (they will not explode)
  {
    trafo.translation.y -= 0.02 * timefac; // sink down
    currot.gamma = recrot.gamma = 180.0 + 0.5 * (float) sink / timestep; // change angle when sinking
    return; // and exit move()
  }
  if (!active && !draw) return; // exit if not active

  if (id >= StaticPassiveBeginDescriptor) // only buildings, static objects
  {
    if (id == TentDescriptor) currot.theta = 178;
    // set the correct angles to diplay
    trafo.rotation.set ((short) (90 + currot.gamma - 180), (short) currot.theta + 180, (short) -currot.phi);
    checkShield ();
    return; // and exit this function
  }

  if (id == FlareDescriptor) // only flares
  {
    trafo.translation.y -= 0.04 * timefac; // fall down (gravity, constant)
    trafo.scaling.x = 0.12F + 0.03F * sin ((float) ttl / (float) timestep / 15); // blink (high frequency)
    currot.phi = camphi; // angles to viewer (player)
    currot.theta = 0;
    currot.gamma = camgamma;
  }

  if (id == ChaffDescriptor) // only chaff
  {
    trafo.translation.y -= 0.04 * timefac; // fall down (gravity, constant)
    trafo.scaling.x = 0.12F + 0.01F * (80 * timestep - ttl) / timestep; // spread out
    currot.phi = camphi; // angles to viewer (player)
    currot.theta = 0;
    currot.gamma = camgamma;
  }

  // check maximum gamma
  if (easymodel == 1)
  {
    if (currot.gamma > 180 + getPrototype ()->maxrot.gamma) currot.gamma = 180 + getPrototype ()->maxrot.gamma;
    else if (currot.gamma < 180 - getPrototype ()->maxrot.gamma) currot.gamma = 180 - getPrototype ()->maxrot.gamma;
  }
  else if (easymodel == 2) // otherwise check for value overflow due to loops
  {
    (void) checkLooping ();
  }

  // the core of directional alterations and force calculations:
  // easymodel==1 means to change heading due to roll angle
  // this may seem complete nonsense for fighters, but it is just a simplification!!!
  // angle / aileron = constant, thats enough for a simple AI
  if (easymodel == 1)
  {
    if (id >= MovingGroundBeginDescriptor)
    {
      currot.phi += SIN(currot.theta) * getPrototype ()->manoeverability * 667 * timefac; //10.0 * getPrototype ()->maxthrust / div;
    }
    else
    {
      currot.phi += SIN(currot.theta) * getPrototype ()->manoeverability * (3.33 + 15.0 * realspeed) * timefac;
      currot.gamma -= fabs (SIN(currot.theta) * COS(currot.gamma) / realspeed / 20) * timefac; // realistic modification
      if (currot.gamma < 180 - getPrototype ()->maxrot.gamma) currot.gamma = 180 - getPrototype ()->maxrot.gamma;
    }
  }
  else if (easymodel == 2) // now this is much more general, however simplified:
  {
    int vz = 1;
    if (currot.gamma < 90 || currot.gamma > 270)
      vz = -1;
    // change heading and elevation due to ailerons and rudder
    if (getPrototype ()->maxthrust + thrust <= -0.00001 || getPrototype ()->maxthrust + thrust >= 0.00001)
    {
      currot.phi += vz * SIN(currot.theta) * elevatoreffect * getPrototype ()->manoeverability * (3.33 + 15.0 * realspeed) * timefac;
      currot.gamma += COS(currot.theta) * elevatoreffect * getPrototype ()->manoeverability * (3.33 + 15.0 * realspeed) * timefac;
      currot.phi += -vz * COS(currot.theta) * ruddereffect * getPrototype ()->manoeverability * (0.66 + 3.0 * realspeed) * timefac;
      currot.gamma += SIN(currot.theta) * ruddereffect * getPrototype ()->manoeverability * (0.66 + 3.0 * realspeed) * timefac;
      if (!realism)
        currot.gamma -= fabs (SIN(currot.theta) * COS(currot.gamma) / realspeed / 20) * timefac; // realistic modification
    }
    // change roll due to roll ;-)
    if (rolleffect)
    {
      currot.theta += rolleffect * (getPrototype ()->nimbility * (1.0 + realspeed)) * timefac * 5.0F;
      recrot.theta = currot.theta;
    }
  }
  if (currot.phi < 0) currot.phi += 360.0; // validate heading
  else if (currot.phi >= 360.0) currot.phi -= 360.0;

  if (easymodel == 1) // easy model restrictions
  {
    if (recrot.theta > getPrototype ()->maxrot.theta) recrot.theta = getPrototype ()->maxrot.theta;
    else if (recrot.theta < -getPrototype ()->maxrot.theta) recrot.theta = -getPrototype ()->maxrot.theta;
    if (recrot.gamma > 180 + getPrototype ()->maxrot.gamma) recrot.gamma = 180 + getPrototype ()->maxrot.gamma;
    else if (recrot.gamma < 180 - getPrototype ()->maxrot.gamma) recrot.gamma = 180 - getPrototype ()->maxrot.gamma;
  }
  else if (easymodel == 2)
  {
    if (currot.theta < -180 && recrot.theta < -180)
    { recrot.theta += 360; currot.theta += 360; }
    else if (currot.theta >= 180 && recrot.theta >= 180)
    { recrot.theta -= 360; currot.theta -= 360; }
  }

  if (recthrust > getPrototype ()->maxthrust) // check maximum throttle
    recthrust = getPrototype ()->maxthrust;

  float throttlechange = getPrototype ()->maxthrust / 200 * timefac;
  if (recthrust > thrust + throttlechange) // alter throttle effect slowly
  {
    thrust += throttlechange;
  }
  else if (recthrust < thrust - throttlechange)
  {
    thrust -= throttlechange;
  }

  // PHYSICS (simplified model)

  Vector3 vaxis, uaxis, utemp, utemp2, utemp3;
  float gammaup, phiup, thetaup;

  bool stop;
  float gravityforce;

  if (id <= CannonEndDescriptor)
  {
    trafo.translation.x += force.x * timefac; // add our vector to the translation
    trafo.translation.z += force.z * timefac;
    trafo.translation.y += force.y * timefac;
    goto cannondone; // jump down to decrease ttl and test collision
  }

  // axis pointing through the fighter's nose
  vaxis.set (COS(currot.gamma) * SIN(currot.phi), SIN(currot.gamma), COS(currot.gamma) * COS(currot.phi));

  realspeed = sqrt (force.x * force.x + force.z * force.z + force.y * force.y);

  if (realism)
  {

    // axis pointing upwards through the cockpit
    gammaup = currot.gamma + 90;
    thetaup = -currot.theta;
    phiup = currot.phi;
    uaxis.set (COS(gammaup) * SIN(phiup), SIN(gammaup), COS(gammaup) * COS(phiup)); // upward axis (theta = 0)
    // now rotate around vaxis using theta
    utemp.set (uaxis);
    utemp.mul (COS(thetaup));
    utemp2.set (vaxis);
    utemp2.mul ((1 - COS(thetaup)) * uaxis.dotproduct (vaxis));
    utemp3.set (uaxis);
    utemp3.crossproduct (vaxis);
    utemp3.mul (SIN(thetaup));
    utemp.add (utemp2);
    utemp.add (utemp3);
    uaxis.set (utemp);

    // add drag force
    braking = (fabs (ruddereffect) + fabs (elevatoreffect) * 4 + fabs (rolleffect)) * realspeed / 50;
    brakepower = pow ((float) (0.93 - braking), (float) timefac);

    acc.x *= brakepower;
    acc.y *= brakepower;
    acc.z *= brakepower;

    // add throttle force
    acc.z += thrust * vaxis.z * 0.3 * timefac;
    acc.x += thrust * vaxis.x * 0.3 * timefac;
    acc.y -= thrust * vaxis.y * 0.3 * timefac;

    // add elevation force
    acc.z += thrust * uaxis.z * 0.067 * timefac;
    acc.x += thrust * uaxis.x * 0.067 * timefac;
    acc.y -= thrust * uaxis.y * 0.067 * timefac;

    // add gravity force
    acc.y -= 0.015 * timefac;

    // calculate new forces
    float stepfac = 0.24;
    float scalef = 1.1;
    force.x = acc.x * stepfac * scalef;
    force.y = acc.y * stepfac * scalef;
    force.z = acc.z * stepfac * scalef;

    // add our vector to the translation
    trafo.translation.x += acc.x * timefac * stepfac;
    trafo.translation.z += acc.z * timefac * stepfac;
    trafo.translation.y += acc.y * timefac * stepfac;

  }
  else // action model
  {

    // and correct the speedvector

    force.z = vaxis.z * realspeed;
    force.x = vaxis.x * realspeed;
    force.y = -vaxis.y * realspeed;

    // add throttle force

    force.z += thrust * vaxis.z * 0.01 * timefac; //0.03 and braking=0.97 by try and error
    force.x += thrust * vaxis.x * 0.01 * timefac;
    force.y -= thrust * vaxis.y * 0.01 * timefac;

    gravityforce = sqrt (realspeed) * vaxis.y * 0.0012 * timefac;
    force.z += gravityforce * vaxis.z;
    force.x += gravityforce * vaxis.x;
    force.y -= gravityforce * vaxis.y;

    // drag force simulated by adjusting the vector

    if (easymodel == 2)
      braking = (fabs (ruddereffect) + fabs (elevatoreffect)) * realspeed / 50;
    else
      braking = (fabs (currot.theta / 45)) * realspeed / 50;
    brakepower = pow ((float) (0.9915 - braking), (float) timefac);
    force.x *= brakepower; force.z *= brakepower; force.y *= brakepower;

  }

  stop = false;
  if (id >= TankBeginDescriptor && id <= TankEndDescriptor) // tanks cannot climb steep faces
  {
    float newy = l->getExactHeight (trafo.translation.x + force.x, trafo.translation.z + force.z) + trafo.scaling.x / 2;
    if (fabs (newy - trafo.translation.y) > 0.05)
      stop = true;
    else if (fabs (newy - trafo.translation.y) > 2)
      stop = false;
  }

  if (!realism)
    if (!stop)
    {
      trafo.translation.x += force.x * timefac; // add our vector to the translation
      trafo.translation.z += force.z * timefac;
      trafo.translation.y += force.y * timefac;
    }

  // calculate the objects real thrust only once
  realspeed = sqrt (force.x * force.x + force.z * force.z + force.y * force.y);

  // objects moving on the ground should always change their elevation due to the surface
  if (id >= TankBeginDescriptor && id <= TankEndDescriptor && thrust > 0 && !stop)
  {
    float newy = l->getExactHeight (trafo.translation.x, trafo.translation.z) + trafo.scaling.x / 2;
    float dy = newy - trafo.translation.y + force.y;
    float dx = fabs (force.x) + fabs (force.z);
    float gamma2 = 0;
    if (fabs (dx) > 0.0001)
      gamma2 = atan (dy / dx);
    currot.gamma = 180.0 + 180.0 / PI * gamma2;
    trafo.translation.y = newy;
  }

  if (id != AsteroidDescriptor)
  {
    // set angles to correctly display the object
    trafo.rotation.set ((short) (90 + currot.gamma - 180), (short) currot.theta + 180, (short) -currot.phi);
  }
  else // asteroids should rotate around their center of weight, as we must not change theta/gamma, we do this here
  {
    ttl -= dt; // we use the ttl value as timer, for other methods ttl<0 is the same as ttl=-1
    if (ttl <= -360 * timestep) ttl = -1;
    int rot1 = (int) (sin ((trafo.scaling.x - 1.3) * 8) * 4);
    int rot2 = (int) (cos ((trafo.scaling.x - 1.3) * 8) * 4);
    trafo.rotation.set ((short) (90 + currot.gamma + ttl * rot1 / timestep - 180), (short) currot.theta + ttl * rot2 / timestep + 180, (short) -currot.phi);
  }

cannondone:;
  if (ttl > 0)
  {
    ttl -= dt; // decrease time to live
    if (ttl <= 0)
    {
      ttl = -1;
      if (id >= MissileBeginDescriptor && id <= MissileEndDescriptor) recheight = -10; // missiles drop
      else deactivate (); // cannon shots vanish
    }
  }
  checkShield (); // check shield issues
  if (immunity > 0) immunity -= dt; // decrease immunity
}

#endif
