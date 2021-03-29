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


Fighter::Fighter (const UnitDescriptor &desc)
  : AiObj ()
{
  id = desc;
  proto = new FighterPrototype (id);
  initPrototype ();
}

/*Fighter::Fighter (const UnitDescriptor &desc, Space *space2, Model3d *o2, float zoom2)
  : AiObj (desc, space2, o2, zoom2)
{
}*/

Fighter::~Fighter ()
{
}

void Fighter::initPrototype ()
{
  AiObj::initPrototype ();

  flares = getPrototype ()->maxflares;
  chaffs = getPrototype ()->maxchaffs;
}

FighterPrototype *Fighter::getPrototype ()
{
  FighterPrototype *fproto = dynamic_cast<FighterPrototype *>(proto);
  assert (fproto);
  return fproto;
}

void Fighter::placeMissiles ()
{
  int i2;
  Rotation rotmissile (90.0F, 0.0F, 270.0F); // TODO: double code in Game.cpp
  Vector3 tlnull (0.0F, 0.0F, 0.0F);
  Vector3 scale (0.2F, 0.2F, 0.2F);
  ref.clear ();
  for (i2 = 0; i2 < 12; i2 ++)
    addRefModel (SpaceObj (Model3dRegistry::get (AamHs1Descriptor.name), Transformation (tlnull, rotmissile, scale)));
  for (i2 = 0; i2 < 4; i2 ++)
  {
    int type = missilerack [i2];
/*    ref [i2 * 3].o = o;
    ref [i2 * 3 + 1].o = o;
    ref [i2 * 3 + 2].o = o; */
    ref [i2 * 3 + 1].trafo.translation.z = ref [i2 * 3].trafo.translation.z - 0.04;
    ref [i2 * 3 + 2].trafo.translation.z = ref [i2 * 3].trafo.translation.z + 0.04;
    ref [i2 * 3].trafo.translation.y = ref [i2 * 3 + 1].trafo.translation.y - 0.04;
    int tmp = missilerackn [i2];
    ref [i2 * 3].trafo.scaling.set (0.25, 0.25, 0.25);
    ref [i2 * 3 + 1].trafo.scaling.set (0.25, 0.25, 0.25);
    ref [i2 * 3 + 2].trafo.scaling.set (0.25, 0.25, 0.25);
    if (tmp < 3) ref [i2 * 3].trafo.scaling.set (0, 0, 0);
    if (tmp < 2) ref [i2 * 3 + 1].trafo.scaling.set (0, 0, 0);
  }
}

void Fighter::fireFlare2 (DynamicObj *flare)
{
  DISPLAY_DEBUG(FormatString ("FlareDescriptor: party=%d", party));
  flare->init ();
  flare->thrust = 0;
  flare->realspeed = 0;
  flare->recthrust = 0;
  flare->getPrototype ()->manoeverability = 0.0;
  flare->getPrototype ()->maxthrust = 1.0;
  flare->currot.gamma = 0;
  flare->party = party;
  flare->ttl = 80 * timestep;
  flare->shield = 1;
  flare->immunity = (int) (trafo.scaling.x * 12) * timestep;
  flare->source = this;
  flare->currot.phi = currot.phi;
  flare->id = FlareDescriptor;
  initValues (flare, currot.phi);
  flare->activate ();
  flare->explode = 0;
}

void Fighter::fireChaff2 (DynamicObj *chaff)
{
  DISPLAY_DEBUG(FormatString ("ChaffDescriptor: party=%d", party));
  chaff->init ();
  chaff->thrust = 0;
  chaff->realspeed = 0;
  chaff->recthrust = 0;
  chaff->getPrototype ()->manoeverability = 0.0;
  chaff->getPrototype ()->maxthrust = 1.0;
  chaff->currot.gamma = 0;
  chaff->party = party;
  chaff->ttl = 80 * timestep;
  chaff->shield = 1;
  chaff->immunity = (int) (trafo.scaling.x * 12) * timestep;
  chaff->source = this;
  chaff->currot.phi = currot.phi;
  chaff->id = ChaffDescriptor;
  initValues (chaff, currot.phi);
  chaff->activate ();
  chaff->explode = 0;
  chaff->trafo.scaling.set (0.12F, 0.12F, 0.12F);
}

bool Fighter::fireFlare (std::vector<DynamicObj *> &flare, std::vector<AiObj *> &missile)
{
  unsigned i, i2;
  if (flares <= 0) return false;
  if (fireflarettl > 0) return false;
  for (i = 0; i < flare.size (); i ++)
  {
    if (flare [i]->ttl <= 0) break;
  }
  if (i < flare.size ())
  {
    fireFlare2 (flare [i]);
    flares --;
    fireflarettl = 8 * timestep;
    for (i2 = 0; i2 < missile.size (); i2 ++)
    {
      if (missile [i2]->ttl > 0)
      {
        if (missile [i2]->id >= AamHs1Descriptor && missile [i2]->id <= AamHs3Descriptor) // only heat seeking missiles
          if (missile [i2]->target == this) // only change target if angle is good
          {
            bool hit = false;
            if (easymodel == 1)
            {
              if (Math::random ((int) (currot.theta + 20)) > 50) hit = true;
            }
            else if (easymodel == 2)
            {
              if (Math::random ((int) (fabs (elevatoreffect) * 90 + 20)) > 50) hit = true;
            }
            if (hit)
            {
              DISPLAY_DEBUG("Missile to flare");
              missile [i2]->target = flare [i];
            }
          }
      }
    }
    return true;
  }
  return false;
}

bool Fighter::fireChaff (std::vector<DynamicObj *> &chaff, std::vector<AiObj *> &missile)
{
  unsigned i, i2;
  if (chaffs <= 0) return false;
  if (firechaffttl > 0) return false;
  for (i = 0; i < chaff.size (); i ++)
  {
    if (chaff [i]->ttl <= 0) break;
  }
  if (i < chaff.size ())
  {
    fireChaff2 (chaff [i]);
    chaffs --;
    firechaffttl = 8 * timestep;
    for (i2 = 0; i2 < missile.size (); i2 ++)
    {
      if (missile [i2]->ttl > 0)
      {
        if (missile [i2]->id > AamHs3Descriptor) // only radar seeking missiles
          if (missile [i2]->target == this) // only change target if angle is good
          {
            bool hit = false;
            if (easymodel == 1)
            {
              if (Math::random ((int) (currot.theta + 20)) > 50) hit = true;
            }
            else if (easymodel == 2)
            {
              if (Math::random ((int) (fabs (elevatoreffect) * 90 + 20)) > 50) hit = true;
            }
            if (hit)
            {
              DISPLAY_DEBUG("Missile to chaff");
              missile [i2]->target = chaff [i];
            }
          }
      }
    }
    return true;
  }
  return false;
}

bool Fighter::performManoevers (float myheight)
{
  if (manoeverstate && active && draw)
  {
    easymodel = 2;

    if (manoeverstate == 1) // Immelmann
    {
      recelevatoreffect = 0.05;
      if (fabs (currot.theta) > 10) recrolleffect = -1;
      else
      {
        recrolleffect = 0;
        manoeverstate = 2;
      }
    }
    else if (manoeverstate == 2)
    {
      if (fabs (currot.theta) <= 150)
      {
        recrolleffect = 0;
        recelevatoreffect = 1;
      }
      else
      {
        manoeverstate = 3;
      }
    }
    else if (manoeverstate == 3)
    {
      if (currot.gamma < 170 || currot.gamma > 190)
      {
        recrolleffect = 0;
        recelevatoreffect = 1;
      }
      else
      {
        manoeverstate = 4;
      }
    }
    else if (manoeverstate == 4)
    {
      if (fabs (currot.theta) > 20)
      {
        recrolleffect = 1;
        recelevatoreffect = 0.05;
      }
      else
      {
        manoeverstate = 0;
      }
    }

    if (manoeverstate == 10) // climb vertical
    {
      recrolleffect = 0;
      recelevatoreffect = 1;
      if (currot.gamma > 260 || currot.gamma < 90)
      {
        recrolleffect = 0;
        recelevatoreffect = 0;
        manoeverstate = 11;
      }
    }
    else if (manoeverstate == 11)
    {
      if (fabs (trafo.translation.y - myheight) > 3)
      {
        manoeverstate = 12;
      }
    }
    else if (manoeverstate == 12)
    {
      recelevatoreffect = -0.5;
      if (currot.gamma > 170 && currot.gamma < 190)
      {
        recelevatoreffect = 0;
        manoeverstate = 0;
      }
    }

    if (manoeverstate == 20) // Roll
    {
      recelevatoreffect = 0.55;
      recrolleffect = 1;
      if (currot.theta > 80 && currot.theta < 90)
      {
        manoeverstate = 21;
      }
    }
    else if (manoeverstate == 21)
    {
      if (currot.theta > -10 && currot.theta < 10)
      {
        manoeverstate = 0;
      }
    }

    float pulljoystick = 0.005;
    float nocorrection = 0.1;
    if (recrolleffect > rolleffect + nocorrection) rolleffect += pulljoystick * timestep;
    else if (recrolleffect < rolleffect - nocorrection) rolleffect -= pulljoystick * timestep;
    if (recelevatoreffect > elevatoreffect + nocorrection) elevatoreffect += pulljoystick * timestep;
    else if (recelevatoreffect < elevatoreffect - nocorrection) elevatoreffect -= pulljoystick * timestep;
    return 0;
  }
  else
  {
    if (ai)
      easymodel = 1;
  }
  return 1;
}

// core AI method
void Fighter::aiAction (Uint32 dt, std::vector<AiObj *> &f, std::vector<AiObj *> &m, std::vector<DynamicObj *> &c, std::vector<DynamicObj *> &flare, std::vector<DynamicObj *> &chaff, float camphi, float camgamma)
{
  timer += dt;

  if (!active && !draw) // not active, not drawn, then exit
  {
    return;
  }

  checkTtl (dt);

  // move object according to our physics
  move (dt, camphi, camgamma);

  // set smoke
  setSmoke (dt);

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

  selectTarget (f);

  if (target != NULL)
    disttarget = distance (target); // distance to target
  else
    disttarget = 1;

  if (haveMissile () && target != NULL)
  {
    float dgamma = atan ((target->trafo.translation.y - trafo.translation.y) / disttarget) * 180 / PI - (currot.gamma - 180);
    float dphi = getAngle (target);
    if (missiletype == DfmDescriptor.id - MissileBeginDescriptor.id)
    {
      ttf = 0;
    }
    else if (fabs (dphi) < 50 && fabs (dgamma) < 50 && party != target->party)
    {
      if (disttarget < 75)
      {
        if (ttf > 0)
        {
          if (missiletype >= 0 && missiletype <= 2)
          {
            if (target->id >= FighterBeginDescriptor && target->id <= AirEndDescriptor)
            {
              float dphi = fabs (currot.phi - target->currot.phi);
              if (dphi > 270) dphi = 360 - dphi;
              if (dphi < 45)
                ttf -= 2 * dt;
              else
                ttf = 50 * timestep;
            }
          }
          else if (missiletype == 6 || missiletype == 7)
          {
            if (target->id >= FighterBeginDescriptor && target->id <= AirEndDescriptor)
            {
              ttf -= 2 * dt;
            }
          }
          else
          {
            if (target->id > AirEndDescriptor)
            {
              ttf -= 2 * dt;
            }
          }
        }
      }
    }
    else
    {
      ttf = 50 * timestep;
    }
  }

  if (!ai) return;



/*
// The following model would be the REAL AI behaviour!
// However enemies are too weak!

  Vector3 targetvec;
  if (target != NULL)
  {
    targetvec.set (target->tl);
    float disttarget = distance (&targetvec);
    if (disttarget > 30) disttarget = 30;
    targetvec.x += target->force.x * disttarget / 5;
    targetvec.y += target->force.y * disttarget / 5;
    targetvec.z += target->force.z * disttarget / 5;
  }
  int lsdst = 5;
  float flyx1 = trafo.translation.x + force.x * lsdst, flyz1 = trafo.translation.z + force.z * lsdst;
  float flyx2 = trafo.translation.x + force.x * lsdst * 4, flyz2 = trafo.translation.z + force.z * lsdst * 4;
  float h1 = trafo.translation.y - l->getMaxHeight (flyx1, flyz1);
  float h2 = trafo.translation.y - l->getMaxHeight (flyx2, flyz2);
  if (h1 < 5 || h1 > 25)
  {
    targetvec.x = flyx1;
    targetvec.z = flyz1;
    targetvec.y = l->getMaxHeight (flyx1, flyz1) + 15;
  }
  disttarget = distance (&targetvec);



  float dgamma = getAngleH (&targetvec);
  float dphi = getAngle (&targetvec);
  if (fabs (dphi) < 1E-5) dphi = 1E-5;
  float delta = atan (dgamma / dphi) * 180 / PI;
  if (dphi > 0) delta -= 180;
  recrot.theta = -delta - 90;
  if (recrot.theta < -180) recrot.theta += 360;
  if (recrot.theta >= 180) recrot.theta -= 360;
  recelevatoreffect = 1;
  if (disttarget > 50 && fabs (dphi) < 25)
  {
    recrot.theta = 0;
    recelevatoreffect = 0;
    if (fabs (theta - recrot.theta) < 5) recrolleffect = 0;
    else if (theta - recrot.theta >= 5) recrolleffect = -1;
    else if (theta - recrot.theta <= -5) recrolleffect = 1;
  }
  else if (fabs (theta - recrot.theta) > 150 && fabs (dphi) < 30 && fabs (dgamma) < 30)
  {
    recrolleffect = 0;
    recelevatoreffect = -0.5;
  }
  else if (theta - recrot.theta > 50)
  {
    recrolleffect = -1;
    recelevatoreffect = 0;
  }
  else if (theta - recrot.theta < -50)
  {
    recrolleffect = 1;
    recelevatoreffect = 0;
  }
  else if (theta - recrot.theta > 20)
  {
    recrolleffect = -1;
    recelevatoreffect = 1;
  }
  else if (theta - recrot.theta < -20)
  {
    recrolleffect = 1;
    recelevatoreffect = 1;
  }
  else
  {
    recrolleffect = 0;
    recelevatoreffect = 1;
  }
  easymodel = 2;
  if (phi >= 360) phi -= 360;
  if (phi < 0) phi += 360;
  if (gamma >= 360) gamma -= 360;
  if (gamma < 0) gamma += 360;
  thrust = getPrototype ()->maxthrust * 0.7;

  float pulljoystick = 0.005;
  float nocorrection = 0.1;
  if (recrolleffect > rolleffect + nocorrection) rolleffect += pulljoystick * timestep;
  else if (recrolleffect < rolleffect - nocorrection) rolleffect -= pulljoystick * timestep;
  if (recelevatoreffect > elevatoreffect + nocorrection) elevatoreffect += pulljoystick * timestep;
  else if (recelevatoreffect < elevatoreffect - nocorrection) elevatoreffect -= pulljoystick * timestep;

  if (target)
  {
    if (disttarget < 20 && dphi < 5 && dgamma < 5)
      fireCannon (laser);
  }

  if (disttarget < 5 && dphi < 90 && dgamma < 90)
  {
    thrust = getPrototype ()->maxthrust / 2;
  }
  else
  {
    thrust = getPrototype ()->maxthrust;
  }

  return;
*/



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
    if (trafo.translation.y - myheight > 8 && target != NULL && trafo.translation.y - myheight < 50)
    {
      recheight2 = target->trafo.translation.y - 8 * target->thrust * SIN(target->currot.gamma);
    }
    else
    {
      recheight2 = recheight + getMinimumHeight ();
    }
  }

  // fire flares and chaff
  if (manoevertheta <= 0)
    for (unsigned i = 0; i < missile.size (); i ++)
      if (m [i]->ttl > 0)
        if (m [i]->target == this)
        {
          if (m [i]->id >= 0 && m [i]->id <= AamHs3Descriptor)
          {
            if ((easymodel == 1 && fabs (currot.theta) >= 30) || (easymodel == 2 && fplayer->elevatoreffect >= 0.5))
            {
              fireFlare (flare, m);
              fireflarettl += intelligence / 20 * timestep;
            }
            manoevertheta = 35 * timestep;
          }
          else
          {
            if ((easymodel == 1 && fabs (currot.theta) >= 30) || (easymodel == 2 && fplayer->elevatoreffect >= 0.5))
            {
              fireChaff (chaff, m);
              firechaffttl += intelligence / 20 * timestep;
            }
            manoevertheta = 35 * timestep;
          }
        }

  // FIGHTERs only: manoevers (may use the height information)
  if (!performManoevers (myheight))
    return;

  decreaseManoeverCounter (dt);

  // calculate the recommended height, recheight2 depends on it
  if (manoeverheight <= 0)
  {
    if (target != NULL) // fighter, has target
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
  if (ai)
  {
    if (target != NULL && manoeverheight <= 0)
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

  if (target == NULL) return;

  // fighter's targeting mechanism for missiles
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

  if (!ai || target == NULL) // no AI (player) or no target found, then exit
  {
    return;
  }

  float dx2, dz2;
  float dx = target->trafo.translation.x - trafo.translation.x, dz = target->trafo.translation.z - trafo.translation.z; // current distances

  estimateTargetPosition (&dx2, &dz2);

  estimateHeading (dx2, dz2);

  // heading calculations
  int firerate = getFireRate ();

  if (id < TransportDescriptor) // for fighters (not transproters) do the following
  {
    if (!acttype && disttarget <= 1000 && manoevertheta <= 0) // no special action, near distance, no roll manoever
    {
      if (aw > 0) // positive angle
      {
        if (aw > 140 && disttarget > 50)
        {
          if (!manoeverstate)
          {
            manoeverstate = 1;
            DISPLAY_DEBUG("Manoever: Immelmann");
          }
        }
        else if (aw > 160.0F + 0.05 * intelligence && disttarget < 4 + 0.01 * intelligence) // target very near at the back
        {
          if (!manoeverstate)
          {
            manoeverstate = 1;
            DISPLAY_DEBUG("Manoever: Immelmann");
          }
        }
        else if (aw > 160 && disttarget < 25) // target is at the back
        {
          if (fabs (trafo.translation.y - myheight) > 7 && currot.gamma >= 175 + intelligence / 100) // high enough over ground
          {
            manoeverstate = 20; // roll
            DISPLAY_DEBUG("Manoever: Roll");
          }
          else
          {
            recrot.theta = -90;
            if (manoevertheta <= 0)
            {
              manoevertheta = timestep * (100 + Math::random ((400 - intelligence) / 8)); // turn hard left or right
              DISPLAY_DEBUG("Manoever: Turn");
            }
            if (manoeverthrust <= 0)
              recthrust = getPrototype ()->maxthrust / (1.05F + (float) intelligence * 0.0015); // fly faster
            if (intelligence < 280 && manoeverheight <= 0)
            {
              recheight = 5; manoeverheight = timestep * (20 - intelligence / 50);
              DISPLAY_DEBUG("Manoever: Height change");
            } // stay low
          }
        }
        else if (aw < 40 && disttarget > 60)
        {
          recrot.theta = 0;
        }
        else if (aw < 20 && disttarget > 30)
        {
          recrot.theta = 0;
        }
        else // otherwise fly to target direction
        {
          int maw = aw > 90 ? 90 : (int) aw;
          int maw2 = 90 - maw;
          recrot.theta = 90 - maw2 * intelligence / 400;
          if (maw < 30) recrot.theta /= 2;
          if (maw < 5)
          {
            recrot.theta = 0;
            if (target->id >= FighterBeginDescriptor && target->id <= AirEndDescriptor && disttarget < 20)
              ((AiObj *) target)->manoevertheta = timestep * (50 - intelligence / 10);
          }
        }
      }
      else // same for negative angle
      {
        if (aw < -140 && disttarget > 50)
        {
          if (!manoeverstate)
          {
            manoeverstate = 1;
            DISPLAY_DEBUG("Manoever: Immelmann");
          }
        }
        else if (aw < -160.0F - 0.05 * intelligence && disttarget < 4 + 0.01 * intelligence) // target very near at the back
        {
          if (!manoeverstate)
          {
            manoeverstate = 1;
            DISPLAY_DEBUG("Manoever: Immelmann");
          }
        }
        else if (aw < -160 && disttarget < 25)
        {
          if (fabs (trafo.translation.y - myheight) > 7 && currot.gamma >= 175 + intelligence / 100) // high enough over ground
          {
            manoeverstate = 20; // roll
            DISPLAY_DEBUG("Manoever: Roll");
          }
          else
          {
            recrot.theta = 90;
            if (manoevertheta <= 0)
            {
              manoevertheta = timestep * (100 + Math::random ((400 - intelligence) / 8));
              DISPLAY_DEBUG("Manoever: Turn");
            }
            if (manoeverthrust <= 0)
              recthrust = getPrototype ()->maxthrust / (1.05F + (float) intelligence * 0.0015);
            if (intelligence < 280 && manoeverheight <= 0)
            {
              recheight = 5; manoeverheight = timestep * (20 - intelligence / 50);
              DISPLAY_DEBUG("Manoever: Height change");
            }
          }
        }
        else if (aw > -40 && disttarget > 60)
        {
          recrot.theta = 0;
        }
        else if (aw > -20 && disttarget > 30)
        {
          recrot.theta = 0;
        }
        else
        {
          int maw = aw < -90 ? -90 : (int) aw;
          int maw2 = -90 - maw;
          recrot.theta = -90 - maw2 * intelligence / 400;
          if (maw > -30) recrot.theta /= 2;
          if (maw > -5)
          {
            recrot.theta = 0;
            if (target->id >= FighterBeginDescriptor && target->id <= AirEndDescriptor && disttarget < 20)
              ((AiObj *) target)->manoevertheta = timestep * (50 - intelligence / 10);
          }
        }
      }
    }
  }

  // thrust and manoever calculations
  if (disttarget > 5 + aggressivity / 12) // 2.5 seems to be best, but fighters become far too strong
  {
    if (disttarget < 50 && fabs (aw) > 30 && manoeverthrust <= 0)
      recthrust = getPrototype ()->maxthrust / (1.0F + (float) intelligence * 0.0025);
    else thrustUp (); // otherwise fly faster
  }
  else if (manoeverthrust <= 0)
  {
    if (recthrust > target->thrust) // adopt thrust of target
    {
      thrustDown ();
    }
    else
    {
      thrustUp ();
    }
  }
  if (disttarget > 50 && fabs (aw) < 20) // high distance and target in front, then fly straight
    recrot.theta = 0;
  if (manoeverthrust <= 0)
    if (disttarget < 25 && fabs (aw) > 160 && target->id >= TankBeginDescriptor) // avoid collisions
    {
      manoeverthrust = 25 * timestep;
      recthrust = getPrototype ()->maxthrust;
      if (difficulty == 0) recthrust = getPrototype ()->maxthrust * 0.8F;
      manoevertheta = 25 * timestep;
      recrot.theta = 0;
      manoeverheight = 25 * timestep;
      recheight = 10;
    }
  // fire cannon?
  float agr = 4.0 - (float) aggressivity / 100;
  if (firecannonttl <= 0)
  {
    if (fabs (recrot.theta - currot.theta) < agr && fabs (aw) < 20 + agr * 4 && disttarget < 30)
      fireCannon (c);
    else if (disttarget < 2 + agr && fabs (aw) < 20 + agr * 4)
      fireCannon (c);
  }
  // fire missile?
  if (firemissilettl <= 0)
  {
    if (target->id >= FighterBeginDescriptor && target->id <= AirEndDescriptor)
    {
      int z1 = 0;
      if (disttarget < 15 && fabs (aw) < 20)
      {
        for (unsigned i = 0; i < f.size (); i ++)
        {
          if (target == f [i]->target && party == f [i]->party) z1 ++;
        }
        if (z1 >= 3)
        {
          manoevertheta = 15 * timestep;
          firemissilettl = 10 * timestep;
          if (Math::random (2)) recrot.theta = 90;
          else recrot.theta = -90;
          targetNextEnemy (f);
        }
      }
      if (firemissilettl <= 0)
      {
        if (fabs (recrot.theta - currot.theta) < agr / 2 && fabs (aw) < agr && disttarget < 45)
        {
          fireMissile (m, (AiObj *) target);
          firemissilettl += aggressivity * timestep;
        }
      }
    }
    else // ground target
    {
      if (fabs (recrot.theta - currot.theta) < 5 + agr * 4 && fabs (aw) < 5 + agr * 4 && disttarget < 50)
        if (!(l->lsticker & 7))
        {
          fireMissileGround (m);
          firemissilettl += aggressivity / 2 * timestep;
        }
    }
  }

  if (recrot.theta > 90 - precision / 5) recrot.theta = 90 - precision / 5;
  else if (recrot.theta < -90 + precision / 5) recrot.theta = -90 + precision / 5;
}


#endif
