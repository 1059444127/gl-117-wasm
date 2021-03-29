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

#include <stdio.h>
#include <string.h>



void Mission::autoLFBriefing ()
{
  int i, z = 0;
  for (i = 0; i < (int) strlen (briefing); i ++)
  {
    z ++;
    if (z > 50 && briefing [i] == ' ')
    {
      briefing [i] = '\n';
      z = 0;
    }
  }
}

Mission::Mission ()
{
  int i;
  timer = 0;
  for (i = 0; i < 3; i ++)
    selfighter [i] = 0;
  selfighter [0] = FalconDescriptor;
  selfighter [1] = HawkDescriptor;
  selfighter [2] = StormDescriptor;
  selfighters = 3;
  wantfighter = 0;
  selweapons = 3;
  selweapon [0] = AamHs1Descriptor;
  selweapon [1] = Agm1Descriptor;
  selweapon [2] = DfmDescriptor;
  wantweapon = 0;
  textcolor.set (255, 255, 0, 180);
  clouds = 0;
  heading = 180;
  state = 0;
}

void setIntelligence (AiObj &obj, int intelligence, int precision, int aggressivity)
{
  if (difficulty == 0) // easy
  {
    intelligence = 400 - (400 - intelligence) * 1 / 3;
    precision = 400 - (400 - precision) * 1 / 3;
    aggressivity = 400 - (400 - aggressivity) * 1 / 3;
    if (obj.party != 1 && obj.shield > 10) // not player party
    {
      obj.shield = obj.shield * 8 / 10;
//      obj.getPrototype ()->maxshield = obj.shield;
    }
  }
  else if (difficulty == 1) // normal
  {
    intelligence = 400 - (400 - intelligence) * 2 / 3;
    precision = 400 - (400 - precision) * 2 / 3;
    aggressivity = 400 - (400 - aggressivity) * 2 / 3;
  }
  else if (difficulty == 2) // hard
  {
  }

//  trafo.scaling.set (zoom, zoom, zoom);
  obj.intelligence = intelligence;
  obj.precision = precision;
  obj.aggressivity = aggressivity;
}

void Mission::reincarnateFighter (int i)
{
  int fkills = fighter [i]->stat.fighterkills;
  int party = fighter [i]->party;
  int intelligence = fighter [i]->intelligence;
  Vector3 tl (fighter [i]->trafo.translation);
  dynamic_cast<Fighter *>(fighter [i])->init ();
  dynamic_cast<Fighter *>(fighter [i])->initPrototype ();
  fighter [i]->immunity = 50 * timestep;
  fighter [i]->party = party;
  fighter [i]->intelligence = intelligence;
  fighter [i]->aggressivity = intelligence;
  fighter [i]->precision = intelligence;
  fighter [i]->stat.fighterkills = fkills;
  fighter [i]->trafo.translation.set (tl);
  fighter [i]->trafo.translation.y += 10;
  if (i == 0)
  {
    fighter [i]->ai = false;
    fighter [i]->easymodel = 2;
  }
  camera = 0;
}

void Mission::playerInit (bool newFighter)
{
  Fighter *f = dynamic_cast<Fighter *>(fplayer);
  if (newFighter)
  {
    f = new Fighter (selfighter [wantfighter]);
    fighter.add (f);
    fplayer = f;
  }

  int i;
  if (controls != 100)
    fplayer->easymodel = 2;
  fplayer->target = NULL;
  setIntelligence (*fplayer, 0, 0, 0);
  fplayer->missileCount ();
  fplayer->party = 1;
//  fplayer->newinit (selfighter [wantfighter], 1, 0);
  fplayer->ai = false;
  for (i = 0; i < missiletypes; i ++)
    fplayer->missiles [i] = 0;
  for (i = 0; i < missileracks; i ++)
    fplayer->missilerack [i] = -1;
  if (selweapon [wantweapon] == AamHs1Descriptor)
  {
    if (selfighter [wantfighter] == FalconDescriptor)
    { fplayer->missilerackn [0] = 1; fplayer->missilerackn [1] = 2; fplayer->missilerackn [2] = 2; fplayer->missilerackn [3] = 1;
      fplayer->missilerack [0] = 6; fplayer->missilerack [1] = 0; fplayer->missilerack [2] = 0; fplayer->missilerack [3] = 6; }
    if (selfighter [wantfighter] == HawkDescriptor)
    { fplayer->missilerackn [0] = 2; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 2;
      fplayer->missilerack [0] = 6; fplayer->missilerack [1] = 0; fplayer->missilerack [2] = 0; fplayer->missilerack [3] = 6; }
    if (selfighter [wantfighter] == StormDescriptor)
    { fplayer->missilerackn [0] = 3; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 3;
      fplayer->missilerack [0] = 0; fplayer->missilerack [1] = 6; fplayer->missilerack [2] = 6; fplayer->missilerack [3] = 0; }
    if (selfighter [wantfighter] == Hawk2Descriptor)
    { fplayer->missilerackn [0] = 2; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 2;
      fplayer->missilerack [0] = 1; fplayer->missilerack [1] = 6; fplayer->missilerack [2] = 6; fplayer->missilerack [3] = 1; }
    if (selfighter [wantfighter] == PhoenixDescriptor)
    { fplayer->missilerackn [0] = 3; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 3;
      fplayer->missilerack [0] = 2; fplayer->missilerack [1] = 7; fplayer->missilerack [2] = 7; fplayer->missilerack [3] = 2; }
    if (selfighter [wantfighter] == RedArrowDescriptor)
    { fplayer->missilerackn [0] = 2; fplayer->missilerackn [1] = 2; fplayer->missilerackn [2] = 2; fplayer->missilerackn [3] = 2;
      fplayer->missilerack [0] = 7; fplayer->missilerack [1] = 1; fplayer->missilerack [2] = 1; fplayer->missilerack [3] = 7; }
  }
  if (selweapon [wantweapon] == Agm1Descriptor)
  {
    if (selfighter [wantfighter] == FalconDescriptor)
    { fplayer->missilerackn [0] = 1; fplayer->missilerackn [1] = 2; fplayer->missilerackn [2] = 2; fplayer->missilerackn [3] = 1;
      fplayer->missilerack [0] = 0; fplayer->missilerack [1] = 3; fplayer->missilerack [2] = 3; fplayer->missilerack [3] = 0; }
    if (selfighter [wantfighter] == HawkDescriptor)
    { fplayer->missilerackn [0] = 1; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 1;
      fplayer->missilerack [0] = 6; fplayer->missilerack [1] = 3; fplayer->missilerack [2] = 3; fplayer->missilerack [3] = 6; }
    if (selfighter [wantfighter] == StormDescriptor)
    { fplayer->missilerackn [0] = 2; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 2;
      fplayer->missilerack [0] = 3; fplayer->missilerack [1] = 3; fplayer->missilerack [2] = 3; fplayer->missilerack [3] = 3; }
    if (selfighter [wantfighter] == Hawk2Descriptor)
    { fplayer->missilerackn [0] = 1; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 1;
      fplayer->missilerack [0] = 6; fplayer->missilerack [1] = 4; fplayer->missilerack [2] = 4; fplayer->missilerack [3] = 6; }
    if (selfighter [wantfighter] == PhoenixDescriptor)
    { fplayer->missilerackn [0] = 3; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 3;
      fplayer->missilerack [0] = 4; fplayer->missilerack [1] = 4; fplayer->missilerack [2] = 4; fplayer->missilerack [3] = 4; }
    if (selfighter [wantfighter] == RedArrowDescriptor)
    { fplayer->missilerackn [0] = 1; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 1;
      fplayer->missilerack [0] = 1; fplayer->missilerack [1] = 4; fplayer->missilerack [2] = 4; fplayer->missilerack [3] = 1; }
  }
  if (selweapon [wantweapon] == DfmDescriptor)
  {
    if (selfighter [wantfighter] == FalconDescriptor)
    { fplayer->missilerackn [0] = 1; fplayer->missilerackn [1] = 1; fplayer->missilerackn [2] = 1; fplayer->missilerackn [3] = 1;
      fplayer->missilerack [0] = 0; fplayer->missilerack [1] = 5; fplayer->missilerack [2] = 5; fplayer->missilerack [3] = 0; }
    if (selfighter [wantfighter] == HawkDescriptor)
    { fplayer->missilerackn [0] = 1; fplayer->missilerackn [1] = 2; fplayer->missilerackn [2] = 2; fplayer->missilerackn [3] = 1;
      fplayer->missilerack [0] = 0; fplayer->missilerack [1] = 5; fplayer->missilerack [2] = 5; fplayer->missilerack [3] = 0; }
    if (selfighter [wantfighter] == StormDescriptor)
    { fplayer->missilerackn [0] = 1; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 1;
      fplayer->missilerack [0] = 3; fplayer->missilerack [1] = 5; fplayer->missilerack [2] = 5; fplayer->missilerack [3] = 3; }
    if (selfighter [wantfighter] == Hawk2Descriptor)
    { fplayer->missilerackn [0] = 1; fplayer->missilerackn [1] = 2; fplayer->missilerackn [2] = 2; fplayer->missilerackn [3] = 1;
      fplayer->missilerack [0] = 1; fplayer->missilerack [1] = 5; fplayer->missilerack [2] = 5; fplayer->missilerack [3] = 1; }
    if (selfighter [wantfighter] == PhoenixDescriptor)
    { fplayer->missilerackn [0] = 2; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 2;
      fplayer->missilerack [0] = 4; fplayer->missilerack [1] = 5; fplayer->missilerack [2] = 5; fplayer->missilerack [3] = 4; }
    if (selfighter [wantfighter] == RedArrowDescriptor)
    { fplayer->missilerackn [0] = 1; fplayer->missilerackn [1] = 1; fplayer->missilerackn [2] = 1; fplayer->missilerackn [3] = 1;
      fplayer->missilerack [0] = 1; fplayer->missilerack [1] = 5; fplayer->missilerack [2] = 5; fplayer->missilerack [3] = 1; }
  }
  fplayer->missileCount ();

  // place missiles to racks
//  Fighter *f = dynamic_cast<Fighter *>(fplayer); // TODO: warning C4541: 'dynamic_cast' fuer polymorphen Typ 'class AiObj' mit /GR- verwendet; unvorhersehbares Verhalten moeglich
//  if (f)
    f->placeMissiles ();
}

void Mission::selectMissiles (AiObj &aiobj)
{
  UnitDescriptor id = aiobj.id;
  if (id == FalconDescriptor)
  {
    aiobj.missilerackn [0] = 2; aiobj.missilerackn [1] = 2; aiobj.missilerackn [2] = 2; aiobj.missilerackn [3] = 2;
    aiobj.missilerack [0] = 0; aiobj.missilerack [1] = 6; aiobj.missilerack [2] = 6; aiobj.missilerack [3] = 0;
  }
  if (id == HawkDescriptor)
  {
    aiobj.missilerackn [0] = 1; aiobj.missilerackn [1] = 3; aiobj.missilerackn [2] = 3; aiobj.missilerackn [3] = 1;
    aiobj.missilerack [0] = 6; aiobj.missilerack [1] = 3; aiobj.missilerack [2] = 3; aiobj.missilerack [3] = 6;
  }
  if (id == StormDescriptor)
  {
    aiobj.missilerackn [0] = 1; aiobj.missilerackn [1] = 2; aiobj.missilerackn [2] = 2; aiobj.missilerackn [3] = 1;
    aiobj.missilerack [0] = 6; aiobj.missilerack [1] = 0; aiobj.missilerack [2] = 0; aiobj.missilerack [3] = 6;
  }
  if (id == Hawk2Descriptor)
  {
    aiobj.missilerackn [0] = 1; aiobj.missilerackn [1] = 3; aiobj.missilerackn [2] = 3; aiobj.missilerackn [3] = 1;
    aiobj.missilerack [0] = 6; aiobj.missilerack [1] = 4; aiobj.missilerack [2] = 4; aiobj.missilerack [3] = 6;
  }
  if (id == PhoenixDescriptor)
  {
    aiobj.missilerackn [0] = 3; aiobj.missilerackn [1] = 3; aiobj.missilerackn [2] = 3; aiobj.missilerackn [3] = 3;
    aiobj.missilerack [0] = 4; aiobj.missilerack [1] = 4; aiobj.missilerack [2] = 4; aiobj.missilerack [3] = 4;
  }
  if (id == RedArrowDescriptor)
  {
    aiobj.missilerackn [0] = 2; aiobj.missilerackn [1] = 3; aiobj.missilerackn [2] = 3; aiobj.missilerackn [3] = 2;
    aiobj.missilerack [0] = 7; aiobj.missilerack [1] = 1; aiobj.missilerack [2] = 1; aiobj.missilerack [3] = 7;
  }
  if (id == CrowDescriptor)
  {
    aiobj.missilerackn [0] = 1; aiobj.missilerackn [1] = 2; aiobj.missilerackn [2] = 2; aiobj.missilerackn [3] = 1;
    aiobj.missilerack [0] = 6; aiobj.missilerack [1] = 0; aiobj.missilerack [2] = 0; aiobj.missilerack [3] = 6;
  }
  if (id == BuzzardDescriptor)
  {
    aiobj.missilerackn [0] = 2; aiobj.missilerackn [1] = 2; aiobj.missilerackn [2] = 2; aiobj.missilerackn [3] = 2;
    aiobj.missilerack [0] = 0; aiobj.missilerack [1] = 6; aiobj.missilerack [2] = 6; aiobj.missilerack [3] = 0;
  }
  if (id == SwallowDescriptor)
  {
    aiobj.missilerackn [0] = 2; aiobj.missilerackn [1] = 3; aiobj.missilerackn [2] = 3; aiobj.missilerackn [3] = 2;
    aiobj.missilerack [0] = 6; aiobj.missilerack [1] = 3; aiobj.missilerack [2] = 3; aiobj.missilerack [3] = 6;
  }
  if (id == BlackBirdDescriptor)
  {
    aiobj.missilerackn [0] = 2; aiobj.missilerackn [1] = 3; aiobj.missilerackn [2] = 3; aiobj.missilerackn [3] = 2;
    aiobj.missilerack [0] = 7; aiobj.missilerack [1] = 1; aiobj.missilerack [2] = 1; aiobj.missilerack [3] = 7;
  }
  if (id == CruiserDescriptor)
  {
    aiobj.missiles [6] = 200;
//    aiobj.missilerackn [0] = 200; aiobj.missilerack [0] = 6;
  }
  if (id == MobileSamDescriptor)
  {
    aiobj.missiles [6] = 200;
//    aiobj.missilerackn [0] = 200; aiobj.missilerack [0] = 6;
  }
  if (id == SamDescriptor)
  {
    aiobj.missiles [6] = 200;
//    aiobj.missilerackn [0] = 200; aiobj.missilerack [0] = 6;
  }
}

void Mission::alliedInit (const UnitDescriptor &id, int pilotid)
{
  Fighter *aiobj = new Fighter (id);
  fighter.add (aiobj);

  Pilot *p = pilots->pilot [pilots->aktpilot];
  aiobj->easymodel = 1;
  aiobj->target = NULL;
  int intelligence = p->tp [pilotid]->intelligence;
  int precision = p->tp [pilotid]->precision;
  int aggressivity = p->tp [pilotid]->aggressivity;
  setIntelligence (*aiobj, intelligence, precision, aggressivity);
  aiobj->missileCount ();
  aiobj->party = 1;
//  aiobj->newinit (fighterid, 1, intelligence, precision, aggressivity);
  aiobj->ai = true;

  selectMissiles (*aiobj);

  // place missiles to racks
  if (aiobj->id >= FighterBeginDescriptor && aiobj->id <= AirEndDescriptor)
  {
//    Fighter *f = dynamic_cast<Fighter *>(aiobj); // TODO: warning C4541: 'dynamic_cast' fuer polymorphen Typ 'class AiObj' mit /GR- verwendet; unvorhersehbares Verhalten moeglich
//    if (f)
      aiobj->placeMissiles ();
  }
}

void Mission::objectInit (AiObj *aiobj, int party, int ailevel)
{
  fighter.add (aiobj);

  setIntelligence (*aiobj, ailevel, ailevel, ailevel);
  aiobj->missileCount ();
  aiobj->party = party;
//  aiobj->newinit (aiobj->id, party, ailevel);

  selectMissiles (*aiobj);

  // place missiles to racks
  if (aiobj->id >= FighterBeginDescriptor && aiobj->id <= AirEndDescriptor)
  {
    Fighter *f = dynamic_cast<Fighter *>(aiobj); // TODO: warning C4541: 'dynamic_cast' fuer polymorphen Typ 'class AiObj' mit /GR- verwendet; unvorhersehbares Verhalten moeglich
    if (f)
      f->placeMissiles ();
  }
}

int Mission::processtimer (Uint32 dt)
{
  bool b = false;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
  for (unsigned i = 0; i < fighter.size (); i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party == 0)
        b = true;
  }
  if (b) return 0;
  return 1;
}

/*void Mission::init ()
{
}

void Mission::start ()
{
}

void Mission::draw ()
{
}*/

// invert y values of all fighters for this mission, only possible if no static buildings are placed on flat ground
// missions were designed to fly towards the sun but since v1.0 the rendering coords have been mapped to the OpenGL coords
// therefore a bug was introduced in v1.0 showing the sun from the wrong side
// however this function should let the player fly into the sun in a couple of missions
void Mission::invertZ ()
{
  for (unsigned i = 0; i < fighter.size (); i ++)
  {
    fighter [i]->trafo.translation.z = -fighter [i]->trafo.translation.z;
    fighter [i]->currot.phi += 180;
  }
}

void Mission::checkScore (int missionstate, int timebonus, int fighterkills, int shipkills, int tankkills, int otherkills, int shieldbonus, int points)
{
  Pilot *p = pilots->pilot [pilots->aktpilot];
  int i, i2;
  int z = 1;
  int oldscore = p->mission_score [id]; // getScore (p->mission_state [id], p->mission_time [id], p->mission_fighterkills [id], p->mission_shipkills [id], p->mission_tankkills [id], p->mission_otherkills [id], p->mission_shield [id], p->mission_points [id]);
  int newscore = getScore (missionstate, timebonus, fighterkills, shipkills, tankkills, otherkills, shieldbonus, points);
  if (newscore > oldscore)
  {
    if (missionstate != p->mission_state [id])
    {
      for (i = 0; i < 10; i ++)
      {
        bool b = false;
        for (i2 = 0; i2 < alliedfighters - 1; i2 ++)
        {
          if (i == alliedpilot [i2])
          { b = true; break; }
        }
        if (b)
        {
          p->tp [i]->fighterkills += fighter [z ++]->stat.fighterkills;
        }
        else
        {
          if (id >= MISSION_CAMPAIGN1 && id <= MISSION_CAMPAIGN2)
            p->tp [i]->flyMission (Math::random (4));
        }
      }
    }
    p->mission_state [id] = missionstate;
    p->mission_time [id] = timebonus;
    p->mission_fighterkills [id] = fighterkills;
    p->mission_shipkills [id] = shipkills;
    p->mission_tankkills [id] = tankkills;
    p->mission_otherkills [id] = otherkills;
    p->mission_shield [id] = shieldbonus;
    p->mission_points [id] = points;
    p->mission_score [id] = newscore;
    p->save ();
  }
}

int Mission::getScore (int missionstate, int timebonus, int fighterkills, int shipkills, int tankkills, int otherkills, int shieldbonus, int points)
{
  if (points > 2000) points = 2000;
  int score = timebonus + shieldbonus + fighterkills * 20 + shipkills * 12 + tankkills * 10 + otherkills * 5/* + points / 20*/;
  if (difficulty == 0) score -= 25;
  else if (difficulty == 1) score += 25;
  else if (difficulty == 2) score += 75;
  if (!fplayer->realism) score -= 25;
  else score += 25;
  if (missionstate == 2) // if failed
  {
    score = 0;
  }
  return score;
}

int Mission::getScore (int missionstate)
{
  int shieldbonus = (int) (fplayer->shield * 100 / fplayer->getPrototype ()->maxshield);
  int timebonus = 0;
  if (timer < maxtime)
    timebonus = (maxtime - timer) * 100 / maxtime;
  checkScore (missionstate, timebonus, fplayer->stat.fighterkills, fplayer->stat.shipkills, fplayer->stat.tankkills, fplayer->stat.otherkills, shieldbonus, fplayer->stat.points);
  return getScore (missionstate, timebonus, fplayer->stat.fighterkills, fplayer->stat.shipkills, fplayer->stat.tankkills, fplayer->stat.otherkills, shieldbonus, fplayer->stat.points);
}

#endif
