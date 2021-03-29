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

#ifndef IS_SOUNDSYSTEM_H
#define IS_SOUNDSYSTEM_H

#include "opengl/includegl.h"
#include "configuration/Configuration.h"

/**
* This class loads and plays a Wavefile with SDL or SDL_mixer if available.
*/
class WaveFile
{
  public:

#ifdef HAVE_SDL_MIXER
    Mix_Chunk *chunk;
    int channel;
#else
    SDL_AudioSpec spec;
    Uint8 *sound;
    Uint32 soundlen;
    int soundpos;
#endif
    int volume;

    WaveFile ();
    WaveFile (const char *filename);
    virtual ~WaveFile ();

    void load (const char *filename);
    void play (int channel, bool loop);
    void stop ();
    void setVolume (int level);
};

enum SoundId
{
  SOUND_EXPLOSION1 = 1000,
  SOUND_CLICK1 = 1,
  SOUND_CANNON1 = 2,
  SOUND_MISSILE1 = 3,
  SOUND_THUNDER1 = 4,
  SOUND_PLANE1 = 5,
  SOUND_HIT1 = 6,
  SOUND_BEEP1 = 7,
  SOUND_CHAFF1 = 8,
  SOUND_BEEP2 = 9
};

enum MusicId
{
  MUSIC_DARK1,
  MUSIC_WINNER1,
  MUSIC_STANDBY1,
  MUSIC_ELECTRO1,
  MUSIC_STARS1,
  MUSIC_LOSER1,
  MUSIC_SOFTTEC1,
  MUSIC_AMBIENT1,
  MUSIC_COMPUTA2,
  MUSIC_STRIFE,
  MUSIC_SUNARMADA
};

/**
* This class manages all wave and tracker files for GL-117.
*/
class SoundSystem
{
  public:

    bool audio;
    bool sound, music;
    bool musicplaying;
    int volumesound, volumemusic;
    int engine;
#ifdef HAVE_SDL_MIXER
    int playtime;
    Mix_Music *music1;
#endif
    WaveFile *waveexplosion1, *waveclick1, *wavecannon1, *wavemissile1;
    WaveFile *wavethunder1, *waveplane [10], *wavehit1, *wavebeep1, *wavechaff1, *wavebeep2;

    SoundSystem ();
    virtual ~SoundSystem ();

    void play (int sample, bool loop);
    void playLoop (int sample);
    void setPosition (int sample, int angle, int dist);
    void stop (int sample);
    void stopAll ();
    void playMusic ();
    void playMusic (int times);
    void loadMusic (MusicId music);
    void haltMusic ();
    void setVolume (int sample, int level);
    void setVolume ();
    void setVolumeMusic ();
};

#endif
