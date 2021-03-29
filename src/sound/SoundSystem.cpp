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

#include <stdlib.h>

#include "SoundSystem.h"
#include "logging/Logging.h"
#include "configuration/Directory.h"
#include "util/Util.h"

#include <cassert>



SoundSystem::SoundSystem ()
{
  audio = false;
  sound = true;
  music = true;
  musicplaying = false;
  volumesound = 100;
  volumemusic = 100;
  char buf [4096];
#ifdef HAVE_SDL_MIXER
  DISPLAY_INFO("Using SDL_mixer");
  audio = false;
  return;
  if (Mix_OpenAudio (22050, AUDIO_S16, 2, 4096))
  {
    DISPLAY_ERROR("Unable to open audio device");
    audio = false;
    return;
  }
  else
  {
    audio = true;
  }
  music1 = Mix_LoadMUS (Directory::getMusic ("winner.s3m").c_str ());
  if (music1 == NULL)
  {
    assert (false);
    DISPLAY_FATAL(FormatString ("Cannot open winner.s3m: %s", Mix_GetError ()));
    exit (EXIT_LOADFILE);
  }
  Mix_FreeMusic (music1);
  music1 = Mix_LoadMUS (Directory::getMusic ("loser.s3m").c_str ());
  if (music1 == NULL)
  {
    assert (false);
    DISPLAY_FATAL(FormatString ("Cannot open loser.s3m: %s", Mix_GetError ()));
    exit (EXIT_LOADFILE);
  }
  Mix_FreeMusic (music1);
  music1 = Mix_LoadMUS (Directory::getMusic ("dark.s3m").c_str ());
  if (music1 == NULL)
  {
    assert (false);
    DISPLAY_FATAL(FormatString ("Cannot open dark.s3m: %s", Mix_GetError ()));
    exit (EXIT_LOADFILE);
  }
  Mix_FreeMusic (music1);
  music1 = Mix_LoadMUS (Directory::getMusic ("stars.s3m").c_str ());
  if (music1 == NULL)
  {
    assert (false);
    DISPLAY_FATAL(FormatString ("Cannot open stars.s3m: %s", Mix_GetError ()));
    exit (EXIT_LOADFILE);
  }
  Mix_FreeMusic (music1);
  music1 = Mix_LoadMUS (Directory::getMusic ("ambient.s3m").c_str ());
  if (music1 == NULL)
  {
    assert (false);
    DISPLAY_FATAL(FormatString ("Cannot open ambient.s3m: %s", Mix_GetError ()));
    exit (EXIT_LOADFILE);
  }
  Mix_FreeMusic (music1);
  music1 = Mix_LoadMUS (Directory::getMusic ("standby.s3m").c_str ());
  if (music1 == NULL)
  {
    assert (false);
    DISPLAY_FATAL(FormatString ("Cannot open standby.s3m: %s", Mix_GetError ()));
    exit (EXIT_LOADFILE);
  }
  Mix_FreeMusic (music1);
  music1 = Mix_LoadMUS (Directory::getMusic ("electro.s3m").c_str ());
  if (music1 == NULL)
  {
    assert (false);
    DISPLAY_FATAL(FormatString ("Cannot open electro.s3m: %s", Mix_GetError ()));
    exit (EXIT_LOADFILE);
  }
  Mix_FreeMusic (music1);
  music1 = Mix_LoadMUS (Directory::getMusic ("computa2.xm").c_str ());
  if (music1 == NULL)
  {
    assert (false);
    DISPLAY_FATAL(FormatString ("Cannot open computa2.xm: %s", Mix_GetError ()));
    exit (EXIT_LOADFILE);
  }
  Mix_FreeMusic (music1);
  music1 = Mix_LoadMUS (Directory::getMusic ("strife.it").c_str ());
  if (music1 == NULL)
  {
    assert (false);
    DISPLAY_FATAL(FormatString ("Cannot open strife.it: %s", Mix_GetError ()));
    exit (EXIT_LOADFILE);
  }
  Mix_FreeMusic (music1);
  music1 = Mix_LoadMUS (Directory::getMusic ("sunarmada.it").c_str ());
  if (music1 == NULL)
  {
    assert (false);
    DISPLAY_FATAL(FormatString ("Cannot open sunarmada.it: %s", Mix_GetError ()));
    exit (EXIT_LOADFILE);
  }
  Mix_FreeMusic (music1);
  music1 = Mix_LoadMUS (Directory::getMusic ("softtec.s3m").c_str ());
  if (music1 == NULL)
  {
    assert (false);
    DISPLAY_FATAL(FormatString ("Cannot open softtec.s3m: %s", Mix_GetError ()));
    exit (EXIT_LOADFILE);
  }
  playtime = 0;
#else
  waveclick1 = new WaveFile (Directory::getSounds ("click1.wav"));
  if (SDL_OpenAudio (&waveclick1->spec, NULL) < 0)
  {
    DISPLAY_ERROR(FormatString ("Couldn't open audio: %s, no sound available", SDL_GetError ()));
    audio = false;
    delete waveclick1;
    return;
  }
  else
  {
    audio = true;
  }
  delete waveclick1;
#endif
  int i;
  waveexplosion1 = new WaveFile (Directory::getSounds ("explode1.wav").c_str ());
  waveclick1 = new WaveFile (Directory::getSounds ("click1.wav").c_str ());
  wavecannon1 = new WaveFile (Directory::getSounds ("cannon1.wav").c_str ());
  wavemissile1 = new WaveFile (Directory::getSounds ("missile1.wav").c_str ());
  wavethunder1 = new WaveFile (Directory::getSounds ("thunder1.wav").c_str ());
  for (i = 0; i < 1; i ++)
  {
    sprintf (buf, "plane%d.wav", i + 1);
    waveplane [i] = new WaveFile (Directory::getSounds (buf).c_str ());
  }
  engine = 0;
  wavehit1 = new WaveFile (Directory::getSounds ("hit1.wav").c_str ());
  wavebeep1 = new WaveFile (Directory::getSounds ("beep1.wav").c_str ());
  wavechaff1 = new WaveFile (Directory::getSounds ("chaff1.wav").c_str ());
  wavebeep2 = new WaveFile (Directory::getSounds ("beep2.wav").c_str ());
  wavecannon1->setVolume (50);
  wavemissile1->setVolume (110);
  waveclick1->setVolume (80);
}

SoundSystem::~SoundSystem ()
{
  int i;
  if (!audio) return;
#ifdef HAVE_SDL_MIXER
  if (music1)
    Mix_FreeMusic (music1);
  Mix_CloseAudio ();
#else
  SDL_CloseAudio ();
#endif
  delete waveexplosion1;
  delete waveclick1;
  delete wavecannon1;
  delete wavemissile1;
  delete wavethunder1;
  for (i = 0; i < 1; i ++)
    delete waveplane [i];
  delete wavehit1;
  delete wavebeep1;
  delete wavechaff1;
  delete wavebeep2;
}

void SoundSystem::play (int sample, bool loop)
{
  if (!audio) return;
  if (!sound) return;
  switch (sample)
  {
    case SOUND_CLICK1:
      waveclick1->play (0, loop);
      break;
    case SOUND_CANNON1:
      wavecannon1->play (0, loop);
      break;
    case SOUND_MISSILE1:
      wavemissile1->play (1, loop);
      break;
    case SOUND_EXPLOSION1:
      waveexplosion1->play (2, loop);
      break;
    case SOUND_THUNDER1:
      wavethunder1->play (3, loop);
      break;
    case SOUND_PLANE1:
      waveplane [0]->play (4, loop); // should be [engine] instead of [0], first find better sounds
      break;
    case SOUND_HIT1:
      wavehit1->play (5, loop);
      break;
    case SOUND_BEEP1:
      wavebeep1->play (6, loop);
      break;
    case SOUND_CHAFF1:
      wavechaff1->play (7, loop);
      break;
    case SOUND_BEEP2:
      wavebeep2->play (6, loop);
      break;
  }
}

void SoundSystem::playLoop (int sample)
{
  if (!audio) return;
  if (!sound) return;
  play (sample, true);
}

void SoundSystem::playMusic ()
{
  if (!audio) return;
  if (!music) return;
  if (volumemusic == 0) return;
#ifdef HAVE_SDL_MIXER
  musicplaying = true;
  Mix_PlayMusic (music1, -1);
  setVolumeMusic ();
#endif
}

void SoundSystem::playMusic (int times)
{
  if (!audio) return;
  if (!music) return;
  if (volumemusic == 0) return;
#ifdef HAVE_SDL_MIXER
  musicplaying = true;
  Mix_PlayMusic (music1, times);
  setVolumeMusic ();
#endif
}

void SoundSystem::haltMusic ()
{
  if (!audio) return;
#ifdef HAVE_SDL_MIXER
  musicplaying = false;
  playtime = SDL_GetTicks ();
  Mix_HaltMusic ();
#endif
}

void SoundSystem::loadMusic (MusicId sample)
{
  if (!audio) return;
#ifdef HAVE_SDL_MIXER
  if (music1)
    Mix_FreeMusic (music1);
  switch (sample)
  {
    case MUSIC_DARK1:
      music1 = Mix_LoadMUS (Directory::getMusic ("dark.s3m").c_str ());
      break;
    case MUSIC_WINNER1:
      music1 = Mix_LoadMUS (Directory::getMusic ("winner.s3m").c_str ());
      break;
    case MUSIC_LOSER1:
      music1 = Mix_LoadMUS (Directory::getMusic ("loser.s3m").c_str ());
      break;
    case MUSIC_STANDBY1:
      music1 = Mix_LoadMUS (Directory::getMusic ("standby.s3m").c_str ());
      break;
    case MUSIC_ELECTRO1:
      music1 = Mix_LoadMUS (Directory::getMusic ("electro.s3m").c_str ());
      break;
    case MUSIC_STARS1:
      music1 = Mix_LoadMUS (Directory::getMusic ("stars.s3m").c_str ());
      break;
    case MUSIC_SOFTTEC1:
      music1 = Mix_LoadMUS (Directory::getMusic ("softtec.s3m").c_str ());
      break;
    case MUSIC_AMBIENT1:
      music1 = Mix_LoadMUS (Directory::getMusic ("ambient.s3m").c_str ());
      break;
    case MUSIC_COMPUTA2:
      music1 = Mix_LoadMUS (Directory::getMusic ("computa2.xm").c_str ());
      break;
    case MUSIC_STRIFE:
      music1 = Mix_LoadMUS (Directory::getMusic ("strife.it").c_str ());
      break;
    case MUSIC_SUNARMADA:
      music1 = Mix_LoadMUS (Directory::getMusic ("sunarmada.it").c_str ());
      break;
  }
#endif
}

void SoundSystem::setVolume (int sample, int level)
{
  if (!audio) return;
  level *= volumesound;
  level /= 100;
  switch (sample)
  {
    case SOUND_CLICK1:
      waveclick1->setVolume (level);
      break;
    case SOUND_CANNON1:
      wavecannon1->setVolume (level);
      break;
    case SOUND_MISSILE1:
      wavemissile1->setVolume (level);
      break;
    case SOUND_EXPLOSION1:
      waveexplosion1->setVolume (level);
      break;
    case SOUND_THUNDER1:
      wavethunder1->setVolume (level);
      break;
    case SOUND_PLANE1:
      waveplane [0]->setVolume (level); // should be [engine] instead of [0], first find better sounds
      break;
    case SOUND_HIT1:
      wavehit1->setVolume (level);
      break;
    case SOUND_BEEP1:
      wavebeep1->setVolume (level);
      break;
    case SOUND_CHAFF1:
      wavechaff1->setVolume (level);
      break;
    case SOUND_BEEP2:
      wavebeep2->setVolume (level);
      break;
  }
}

void SoundSystem::setVolume ()
{
  if (!audio) return;
  int i;
  int level = 128 * volumesound / 100;
  waveclick1->setVolume (level * 80 / 128);
  wavecannon1->setVolume (level * 50 / 128);
  wavemissile1->setVolume (level * 110 / 128);
  waveexplosion1->setVolume (level);
  wavethunder1->setVolume (level);
  for (i = 0; i < 1; i ++)
    waveplane [i]->setVolume (level);
  wavehit1->setVolume (level);
  wavebeep1->setVolume (level);
  wavechaff1->setVolume (level);
  wavebeep2->setVolume (level);
}

void SoundSystem::setVolumeMusic ()
{
  if (!audio) return;
#ifdef HAVE_SDL_MIXER
  int level = 128 * volumemusic / 100;
  Mix_VolumeMusic (level);
#endif
}

void SoundSystem::setPosition (int sample, int angle, int dist)
{
  if (!audio) return;
  switch (sample)
  {
    case SOUND_EXPLOSION1:
#ifdef HAVE_SDL_MIXER
      Mix_SetPosition (2, angle, dist);
#endif
      break;
  }
}

void SoundSystem::stop (int sample)
{
  if (!audio) return;
  switch (sample)
  {
    case SOUND_CLICK1:
      waveclick1->stop ();
      break;
    case SOUND_CANNON1:
      wavecannon1->stop ();
      break;
    case SOUND_MISSILE1:
      wavemissile1->stop ();
      break;
    case SOUND_EXPLOSION1:
      waveexplosion1->stop ();
      break;
    case SOUND_THUNDER1:
      wavethunder1->stop ();
      break;
    case SOUND_PLANE1:
      waveplane [0]->stop (); // should be [engine] instead of [0], first find better sounds
      break;
    case SOUND_HIT1:
      wavehit1->stop ();
      break;
    case SOUND_BEEP1:
      wavebeep1->stop ();
      break;
    case SOUND_CHAFF1:
      wavechaff1->stop ();
      break;
    case SOUND_BEEP2:
      wavebeep2->stop ();
      break;
  }
}

void SoundSystem::stopAll ()
{
  if (!audio) return;
#ifdef HAVE_SDL_MIXER
  Mix_HaltChannel (-1);
#endif
}

#endif
