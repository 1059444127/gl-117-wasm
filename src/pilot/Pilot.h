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

/* This file handles all pilots data. */

#ifndef IS_PILOTS_H
#define IS_PILOTS_H

#define SAVEVERSION "GL117_S1"

#include <stdio.h>
#include <string>

const int maxpilotdata = 100;

#define PILOT_PRIMETIME 0
#define PILOT_HEPTARGON 1
#define PILOT_LARA 2
#define PILOT_SHARK 3
#define PILOT_BOSS 4
#define PILOT_DRDOOM 5
#define PILOT_SHADOW 6
#define PILOT_MATRIX 7
#define PILOT_FIREBIRD 8
#define PILOT_THUNDER 9

/**
* This class represents a computer pilot which does not need to store all mission results.
*/
class TeamPilot
{
  public:

    int ranking;         ///< 0=lowest ranking
    std::string name;     ///< pilot nickname
    std::string fullname; ///< pilot full name
    int intelligence;    ///< intelligence, knowledge on piloting and manoevers
    int precision;       ///< precision, reaction
    int aggressivity;    ///< stay close behind the enemy, more firing tolerance
    int fighterkills;    ///< number of fighter kills for the pilot ranking

    TeamPilot (int ranking, const std::string &name, int intelligence, int precision, int aggressivity, int fighterkills);

    void flyMission (int averagekills);
    std::string getRank ();
    std::string getShortRank ();
    std::string getName ();
    std::string getShortName ();
    void load (FILE *in);
    void save (FILE *out);
};

/**
* This class represents a player's pilot.
*/
class Pilot
{
  public:

    std::string name;
    int mission_state [maxpilotdata];  ///< success/failure
    int mission_time [maxpilotdata];   ///< time spent on the mission
    int mission_fighterkills [maxpilotdata];
    int mission_shipkills [maxpilotdata];
    int mission_tankkills [maxpilotdata];
    int mission_otherkills [maxpilotdata];
    int mission_shield [maxpilotdata]; ///< shield left
    int mission_points [maxpilotdata]; ///< extra points for hitting a target
    int mission_score [maxpilotdata];  ///< overall score (calculated)
    int ranking;    ///< current ranking (calculated due to all scores)
    TeamPilot **tp; ///< team pilots

    Pilot (const std::string &name);
    ~Pilot ();

    void load ();
    void save ();
    std::string getRank (int lomissionid, int himissionid);
    std::string getShortRank (int lomissionid, int himissionid);
};

const int maxpilots = 5;

/**
* This class stores a list of custom pilots for the player.
*/
class PilotList
{
  public:

    int aktpilots;
    int aktpilot;
    Pilot *pilot [maxpilots];

    PilotList (const std::string &fname);
    ~PilotList ();

    void load (const std::string &fname);
    void save (const std::string &fname);
    void rm ();
    void add (const std::string &name);
};

#endif
