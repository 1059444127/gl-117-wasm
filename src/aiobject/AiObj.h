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
#define IS_AIOBJECT_H

#include "UnitPrototype.h"
#include "configuration/Configuration.h" // ok
#include "model3d/Model3d.h" // ok
#include "effects/Effects.h" // ok
#include "filetokenizer/FileTokenizer.h"
#include "util/Util.h"
#include "logging/Logging.h"
#include "configuration/Directory.h"

#include <map>
#include <cassert>


// id values of objects
// non-AI air objects
extern UnitDescriptor CannonBeginDescriptor;
extern UnitDescriptor Cannon1Descriptor;
extern UnitDescriptor Cannon1bDescriptor;
extern UnitDescriptor Cannon2Descriptor;
extern UnitDescriptor Cannon2bDescriptor;
extern UnitDescriptor AsteroidDescriptor;
extern UnitDescriptor FlareDescriptor;
extern UnitDescriptor ChaffDescriptor;
extern UnitDescriptor CannonEndDescriptor;
// missiles
extern UnitDescriptor MissileBeginDescriptor;
extern UnitDescriptor AamHs1Descriptor;
extern UnitDescriptor AamHs2Descriptor;
extern UnitDescriptor AamHs3Descriptor;
extern UnitDescriptor Agm1Descriptor;
extern UnitDescriptor Agm2Descriptor;
extern UnitDescriptor DfmDescriptor;
extern UnitDescriptor AamFf1Descriptor;
extern UnitDescriptor AamFf2Descriptor;
extern UnitDescriptor MineDescriptor;
extern UnitDescriptor MissileEndDescriptor;
// air units
extern UnitDescriptor AirBeginDescriptor;
extern UnitDescriptor FighterBeginDescriptor;
extern UnitDescriptor FalconDescriptor;
extern UnitDescriptor SwallowDescriptor;
extern UnitDescriptor HawkDescriptor;
extern UnitDescriptor Hawk2Descriptor;
extern UnitDescriptor BuzzardDescriptor;
extern UnitDescriptor CrowDescriptor;
extern UnitDescriptor PhoenixDescriptor;
extern UnitDescriptor RedArrowDescriptor;
extern UnitDescriptor BlackBirdDescriptor;
extern UnitDescriptor StormDescriptor;
extern UnitDescriptor FighterEndDescriptor;
extern UnitDescriptor TransportDescriptor;
extern UnitDescriptor Transport2Descriptor;
extern UnitDescriptor AirEndDescriptor;
// moving ground units from here
extern UnitDescriptor MovingGroundBeginDescriptor;
extern UnitDescriptor TankBeginDescriptor;
extern UnitDescriptor WieselDescriptor;
extern UnitDescriptor PantherDescriptor;
extern UnitDescriptor MobileSamDescriptor;
extern UnitDescriptor PickupDescriptor;
extern UnitDescriptor Pickup2Descriptor;
extern UnitDescriptor TruckDescriptor;
extern UnitDescriptor Truck2Descriptor;
extern UnitDescriptor TankEndDescriptor;
// moving water units from here
extern UnitDescriptor WaterBeginDescriptor;
extern UnitDescriptor ShipBeginDescriptor;
extern UnitDescriptor CruiserDescriptor;
extern UnitDescriptor LightDestroyerDescriptor;
extern UnitDescriptor ShipEndDescriptor;
// static ground units from here
extern UnitDescriptor StaticGroundBeginDescriptor;
extern UnitDescriptor StaticAaBeginDescriptor;
extern UnitDescriptor SacDescriptor;
extern UnitDescriptor SamDescriptor;
extern UnitDescriptor StaticAaEndDescriptor;
// passive static units from here
extern UnitDescriptor StaticPassiveBeginDescriptor;
extern UnitDescriptor TentDescriptor;
extern UnitDescriptor BigTentDescriptor;
extern UnitDescriptor ContainerDescriptor;
extern UnitDescriptor HallDescriptor;
extern UnitDescriptor Hall2Descriptor;
extern UnitDescriptor OilrigDescriptor;
extern UnitDescriptor ComplexDescriptor;
extern UnitDescriptor RadarDescriptor;
extern UnitDescriptor MoonBaseDescriptor;
extern UnitDescriptor DepotDescriptor;
extern UnitDescriptor LaserBarrierDescriptor;
extern UnitDescriptor RubbleDescriptor;
extern UnitDescriptor HouseDescriptor;
extern UnitDescriptor ExplosionDescriptor;



class ObjectStatistics
{
  public:

    int points;       ///< hitpoints damage to enemies
    int fighterkills; ///< number of fighter shots
    int shipkills;    ///< number of ships sunk
    int tankkills;    ///< number of tank shots
    int otherkills;   ///< number of other objects (buildings) shot
    bool killed;      ///< this object is already deactivated

    ObjectStatistics ()
    {
      points = 0;
      fighterkills = 0;
      shipkills = 0;
      tankkills = 0;
      otherkills = 0;
      killed = false;
    }
};



#include <time.h>

extern Space *space;
/**
* ObjectList is an extended std::vector and stores a vector of type SpaceObj*.
* It is used to have different vectors for all kinds of Objects like Missiles, Houses, etc.
* Note that TSpaceObj is intended to be of type SpaceObj* and thus a pointer itself.
* The method add() inserts a SpaceObj* at the end of the vector and adds it to the global Space.
* The method remove() sets the valid flag of the SpaceObj* to false
* (means: please remove me later on). This may also be done by the SpaceObj* itself(!).
* The method cleanUp() should be called periodically (as frequently as you like),
* as it frees the invalid SpaceObj* from Space and from RAM at most once per second.
*/
template <typename TSpaceObj>
class ObjectList : public std::vector<TSpaceObj>
{
  public:
    clock_t timer;

    void add (TSpaceObj obj)
    {
      this->push_back (obj);
      space->addObject (obj);
    }

    void remove (TSpaceObj obj)
    {
      obj->valid = false;
    }

    void cleanUp ()
    {
      unsigned i;

      // check if one second has passed since the last call to cleanUp()
      clock_t timenow = clock ();
      if (abs (timer - timenow) < 1000)
        return;
      timer = timenow;

      // realign this std::vector
      unsigned removed = 0;
      for (i = 0; i < this->size (); i ++)
      {
        if (!this->operator[](i)->valid)
        {
          removed ++;
        }
        else if (removed)
        {
          this->operator[](i - removed) = this->operator[](i);
        }
      }

      // clear invalid objects (at the end of this std::vector)
      for (i = 0; i < removed; i ++)
        this->pop_back ();
    }
};



/**
* This class represents a dynamic object in space without any AI methods.
*/
class DynamicObj : public SpaceObj
{
  public:

    UnitDescriptor id;          ///< object type: SamDescriptor, TentDescriptor, SwallowDescriptor, ...
  //  bool controls;
    bool active;  ///< deactivated means no AI, no collision control and so on
    /// easymodel==1 is the ancient core of the game ;-)
    /// 1 means that theta will directly alter phi! Computer AI uses this model!
    /// 2 means the realistic model with ailerons, elevator, rudder
    int easymodel;
    int ttl;         ///< time to live: cannon and missiles will only live a short time, missiles will sink when ttl<=0
    int immunity;    ///< immunity means the object cannot collide with others, needed for shooting missiles/cannon
    float braking;   ///< far away from reality: this factorizes the last speed vector with the current conditions (see move method)
    /// Imagine a carthesian coordinate system in the landscape, the y-axis pointing up
    Rotation currot; ///< current rotation, sets rot of SpaceObj, TODO: merge with rot
    Rotation recrot; ///< recommended rotation, recrot.phi is not used
    float thrust;    ///< current thrust, not the speed itself!
    float realspeed; ///< the current speed, we only want to calculate this once per time step
    Vector3 force;   ///< the force vector
//    float rectheta;  ///< roll angle the fighter/object wants to reach
    float recthrust; ///< throttle the fighter/object wants to reach
    float recheight; ///< height above ground the fighter wants to reach
//    float recgamma;  ///< elevation the fighter wants to reach
//    float maxtheta;  ///< a maximum roll angle the object may achieve, easymodel==1 only!
//    float maxgamma;  ///< a maximum elevation the object may achieve, easymodel==1 only!
    float elevatoreffect; ///< number between 1.0 and -0.5, as diving should be less
    float ruddereffect;
    float rolleffect;
    float recelevatoreffect; ///< number between 1.0 and -0.5, as diving should be less
    float recruddereffect;
    float recrolleffect;
    int sink;         ///< ships will not explode but sink
    int party;        ///< usually 0 for enemies, 1 for allies
    ObjectStatistics stat; ///< objects statistics like number of kills, etc.
    DynamicObj *source; ///< missiles must keep track of the object they have been fired from -> statistics
    int bomber;      ///< act as bomber and prefer ground targets
    int realism;
    Vector3 acc;     ///< acceleration
    float shield;    ///< current shield
    DynamicObjPrototype *proto;

    DynamicObj ();
    DynamicObj (const UnitDescriptor &desc);
    DynamicObj (const UnitDescriptor &desc, Space *space2, Model3d *o2, float zoom2);
    virtual ~DynamicObj ();

    void activate ();
    void deactivate ();
    DynamicObjPrototype *getPrototype ();
    virtual void initPrototype ();
    virtual void init ();
    void thrustUp ();
    void thrustDown ();
    float distance (DynamicObj *target);
    float distanceXZ (DynamicObj *target);
    /// check whether the object is exploding or sinking and deactivate if necessary
    void checkExplosion (Uint32 dt);
    /// check the objects shield value and explode/sink if necessary
    void checkShield ();
    /// check whether the object collides on the ground and alter gamma and y-translation
    void crashGround (Uint32 dt);
    /// check for collision, simplified model, each model is surrounded by a cube
    /// this works pretty well, but we must use more than one model for complex models or scenes
    void collide (DynamicObj *d, Uint32 dt); // d must be the medium (laser, missile)
    void setExplosion (float maxzoom, int len);
    void setBlackSmoke (float maxzoom, int len);
    /// return heading difference towards enemy
    int getAngle (DynamicObj *o);
    /// return elevation difference towards enemy
    int getAngleH (DynamicObj *o);
    /// check for a looping, this is tricky :-)
    bool checkLooping ();
    void move (Uint32 dt, float camphi, float camgamma);
};

/**
* This class represents an object with artificial intelligence.
* Differences between fighters, missiles... should be made be splitting this class into
* multiple subclasses. Currently everything is implemented using if-clauses.
*/
class AiObj : public DynamicObj
{
  protected:

  public:

    bool ai;            ///< AI on/off
    bool autofire;      ///< cannon fire on/off
    DynamicObj *target; ///< targeted object
    int acttype;        ///< object is doing some action (Immelmann, Loop, ... not yet implemented)
    /// three intellience characteristics which make up a pilot: 0 = best, 400 = worst
    int intelligence;   ///< valid for every AI object: manoevers, fire rate (tanks), ...
    int aggressivity;   ///< valid for fighters: fly low, stay near and behind enemy
    int precision;      ///< valid for fighters: heading calculation
    /// manoevers disable any other AI consideration
    int manoevertheta, manoeverheight, manoeverthrust;
    int idle;           ///< counter how long AI object does the same thing (to change direction)
    int firemissilettl; ///< minimum time to wait between shooting missiles
    int firecannonttl;  ///< minimum time to wait between shooting cannon
    int smokettl;       ///< minimum time to wait between setting smoke elements
    int missiletype;    ///< only relevant for the player, describes type: AAM, AGM, DF
    int missiles [missiletypes]; ///< number of missiles of each type
    int missilerack [missileracks]; ///< type of missiles per racks
    int missilerackn [missileracks]; ///< number of missiles per rack
    float aw;           ///< current heading difference to target
    int score;          ///< final score
    float dtheta, dgamma; ///< theta/gamma alteration (smooth piloting)
    float disttarget;   ///< current distance to target
    int fireflarettl;
    int firechaffttl;
    int ammo;
    int ttf;            ///< time to fire missile, targeting mechanism
    Smoke *smoke;       ///< bright smoke behind the object (fighter&missiles)
    Uint32 timer;
    int statfirepower;  ///< firepower (missiles) statistics, number of stars
    int manoeverstate;  ///< changes to realistic manoevers and turns off easymodel

    AiObj ();
    AiObj (const UnitDescriptor &desc);
    AiObj (const UnitDescriptor &desc, Space *space2, Model3d *o2, float zoom2);
    virtual ~AiObj ();

    AiObjPrototype *getPrototype ();
    virtual void initPrototype ();     ///< initialize variables from prototype
    virtual void init ();              ///< initialize variables
    void missileCount ();
    void newinit (const UnitDescriptor &id, int party, int intelligence, int precision, int aggressivity); ///< init new AI object
    void newinit (const UnitDescriptor & id, int party, int intelligence); ///< init new AI object (esp. non-fighter)
    void initValues (DynamicObj *dobj, float phi); ///< init values to shoot cannon or missile
    void fireCannon (DynamicObj *laser, float phi);
    void fireCannon (std::vector<DynamicObj *> &laser, float phi);
    void fireCannon (std::vector<DynamicObj *> &laser);
    void fireMissile2 (AiObj *m, AiObj *target);
//    void fireMissile2 (const UnitDescriptor &id, AiObj *missile, AiObj *target);
    int firstMissile ();           ///< select first missile type
    int nextMissile (int from);    ///< select next missile type (cyclic)
    bool haveMissile (const UnitDescriptor &id);     ///< missile of type id left?
    bool haveMissile ();           ///< missile of type missiletype left?
    void decreaseMissile (const UnitDescriptor &id); ///< decrease missiles by one
    bool fireMissile (const UnitDescriptor &id, std::vector<AiObj *> &missile, AiObj *target);
    bool fireMissile (std::vector<AiObj *> &missile, AiObj *target);
    bool fireMissile (const UnitDescriptor &id, std::vector<AiObj *> &missile);
    bool fireMissile (std::vector<AiObj *> &missile);
    bool fireMissileAir (std::vector<AiObj *> &missile, AiObj *target);
    bool selectMissileAir (std::vector<AiObj *> &missile);
    bool fireMissileAirFF (std::vector<AiObj *> &missile, AiObj *target);
    bool selectMissileAirFF (std::vector<AiObj *> &missile);
    bool fireMissileGround (std::vector<AiObj *> &missile);
    bool selectMissileGround (std::vector<AiObj *> &missile);
    void targetNearestGroundEnemy (std::vector<AiObj *> &f);
    void targetNearestEnemy (std::vector<AiObj *> &f);
    void targetNextEnemy (std::vector<AiObj *> &f);
    void targetLockingEnemy (std::vector<AiObj *> &f);
    void targetNext (std::vector<AiObj *> &f);
    void targetPrevious (std::vector<AiObj *> &f);
    void setSmoke (Uint32 dt);
    void selectNewTarget (std::vector<AiObj *> &f);
    void selectTarget (std::vector<AiObj *> &f);
    void checkTtl (Uint32);
    float getMinimumHeight ();
    void easyPiloting (Uint32 dt);
    void limitRotation ();
    void estimateTargetPosition (float *dx2, float *dz2);
    void estimateHeading (float dx2, float dz2);
    void estimateFighterHeading (AiObj *fi);
    void decreaseManoeverCounter (Uint32 dt);
    int getFireRate ();
    virtual void aiAction (Uint32 dt, std::vector<AiObj *> &f, std::vector<AiObj *> &m, std::vector<DynamicObj *> &c, std::vector<DynamicObj *> &flare, std::vector<DynamicObj *> &chaff, float camphi, float camgamma); // core AI method
};

class Missile : public AiObj
{
  public:
    Missile (const UnitDescriptor &desc);
    Missile (const UnitDescriptor &desc, Space *space2, Model3d *o2, float zoom2);
    virtual ~Missile ();

    virtual void aiAction (Uint32 dt, std::vector<AiObj *> &f, std::vector<AiObj *> &m, std::vector<DynamicObj *> &c, std::vector<DynamicObj *> &flare, std::vector<DynamicObj *> &chaff, float camphi, float camgamma);
};

class Tank : public AiObj
{
  public:
    Tank (const UnitDescriptor &desc);
    Tank (const UnitDescriptor &desc, Space *space2, Model3d *o2, float zoom2);
    virtual ~Tank ();

    virtual void aiAction (Uint32 dt, std::vector<AiObj *> &f, std::vector<AiObj *> &m, std::vector<DynamicObj *> &c, std::vector<DynamicObj *> &flare, std::vector<DynamicObj *> &chaff, float camphi, float camgamma);
};

class StaticAa : public AiObj
{
  public:
    StaticAa (const UnitDescriptor &desc);
    StaticAa (const UnitDescriptor &desc, Space *space2, Model3d *o2, float zoom2);
    virtual ~StaticAa ();

    virtual void aiAction (Uint32 dt, std::vector<AiObj *> &f, std::vector<AiObj *> &m, std::vector<DynamicObj *> &c, std::vector<DynamicObj *> &flare, std::vector<DynamicObj *> &chaff, float camphi, float camgamma);
};

class StaticPassive : public AiObj
{
  public:
    StaticPassive (const UnitDescriptor &desc);
    StaticPassive (const UnitDescriptor &desc, Space *space2, Model3d *o2, float zoom2);
    virtual ~StaticPassive ();

    virtual void aiAction (Uint32 dt, std::vector<AiObj *> &f, std::vector<AiObj *> &m, std::vector<DynamicObj *> &c, std::vector<DynamicObj *> &flare, std::vector<DynamicObj *> &chaff, float camphi, float camgamma);
};

class Ship : public AiObj
{
  public:
    Ship (const UnitDescriptor &desc);
    Ship (const UnitDescriptor &desc, Space *space2, Model3d *o2, float zoom2);
    virtual ~Ship ();

    virtual void aiAction (Uint32 dt, std::vector<AiObj *> &f, std::vector<AiObj *> &m, std::vector<DynamicObj *> &c, std::vector<DynamicObj *> &flare, std::vector<DynamicObj *> &chaff, float camphi, float camgamma);
};

class Fighter : public AiObj
{
  public:
    int flares;
    int chaffs;

    Fighter (const UnitDescriptor &desc);
    Fighter (const UnitDescriptor &desc, Space *space2, Model3d *o2, float zoom2);
    virtual ~Fighter ();

    FighterPrototype *getPrototype ();
    virtual void initPrototype ();
    void fireFlare2 (DynamicObj *flare);
    void fireChaff2 (DynamicObj *chaff);
    bool fireFlare (std::vector<DynamicObj *> &flare, std::vector<AiObj *> &missile);
    bool fireChaff (std::vector<DynamicObj *> &chaff, std::vector<AiObj *> &missile);
    bool performManoevers (float myheight);
    virtual void placeMissiles ();
    virtual void aiAction (Uint32 dt, std::vector<AiObj *> &f, std::vector<AiObj *> &m, std::vector<DynamicObj *> &c, std::vector<DynamicObj *> &flare, std::vector<DynamicObj *> &chaff, float camphi, float camgamma);
};

#endif
