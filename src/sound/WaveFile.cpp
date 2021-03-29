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
#include <cassert>

#include "SoundSystem.h"
#include "logging/Logging.h"
#include "configuration/Directory.h"
#include "util/Util.h"

WaveFile *wave = NULL;

#ifndef HAVE_SDL_MIXER
void fillrepeat (void *unused, Uint8 *stream, int len)
{
  Uint8 *waveptr;
  int waveleft;

  waveptr = wave->sound + wave->soundpos;
  waveleft = wave->soundlen - wave->soundpos;

  while (waveleft <= len)
  {
    SDL_MixAudio (stream, waveptr, waveleft, SDL_MIX_MAXVOLUME);
    stream += waveleft;
    len -= waveleft;
    waveptr = wave->sound;
    waveleft = wave->soundlen;
    wave->soundpos = 0;
    SDL_PauseAudio (1);
    return;
  }
  SDL_MixAudio (stream, waveptr, len, SDL_MIX_MAXVOLUME);
  wave->soundpos += len;
}

void fillonce (void *unused, Uint8 *stream, int len)
{
  Uint8 *waveptr;
  int waveleft;

  waveleft = wave->soundlen - wave->soundpos;

  if (waveleft <= len)
  {
    if (SDL_GetAudioStatus () == SDL_AUDIO_PLAYING)
    {
      SDL_PauseAudio (0);
    }
    return;
  }

  waveptr = wave->sound + wave->soundpos;

  SDL_MixAudio (stream, waveptr, len, SDL_MIX_MAXVOLUME);
  wave->soundpos += len;
}
#endif

void freqEffect (int channel, void *stream, int len, void *udata)
{
  /* 16 bits * 2 channels. */
  Uint32 *ptr = (Uint32 *) stream;
  int i;

  for (i = 0; i < len / 4 *7/8; i ++)
  {
    ptr [i] = ptr [i*8/7];
  }
  for (i = len/4*7/8; i < len/4; i ++)
  {
    ptr [i] = 0;
  }
}

WaveFile::WaveFile ()
{
#ifdef HAVE_SDL_MIXER
  channel = -1;
#else
  soundpos = 0;
#endif
}

WaveFile::WaveFile (const char *filename)
{
#ifdef HAVE_SDL_MIXER
  channel = -1;
#else
  soundpos = 0;
#endif
  volume = 100;
  load (filename);
}

WaveFile::~WaveFile () {}

void WaveFile::load (const char *filename)
{
#ifdef HAVE_SDL_MIXER
  chunk = Mix_LoadWAV (filename);
  if (chunk == NULL)
  {
    assert (false);
    DISPLAY_FATAL(FormatString ("SDL_Mixer: %s", Mix_GetError ()));
    exit (EXIT_LOADFILE);
  }
#else
  if (SDL_LoadWAV (filename, &spec, &sound, &soundlen) == NULL)
  {
    assert (false);
    DISPLAY_FATAL(FormatString ("Couldn't load %s: %s", filename, SDL_GetError ()));
    exit (EXIT_LOADFILE);
  }
  spec.callback = fillrepeat;
  wave = this;
#endif
}

void WaveFile::play (int chan, bool loop)
{
#ifdef HAVE_SDL_MIXER
  if (!loop) channel = Mix_PlayChannel (chan, chunk, 0);
  else channel = Mix_PlayChannel (chan, chunk, -1);
#else
  SDL_PauseAudio (1);
  if (wave != NULL)
  {
    wave->soundpos = 0;
  }
  wave = this;
  SDL_PauseAudio (0);
#endif
}

void WaveFile::stop ()
{
#ifdef HAVE_SDL_MIXER
  if (channel < 0) return;
  if (Mix_Playing (channel))
    Mix_HaltChannel (channel);
#endif
}

void WaveFile::setVolume (int level)
{
#ifdef HAVE_SDL_MIXER
  if (abs (volume - level) <= 5) return;
  volume = level;
  bool pause = false;
  if (channel >= 0)
  {
    if (Mix_Playing (channel))
    {
      Mix_Pause (channel);
      pause = true;
    }
  }
  Mix_VolumeChunk (chunk, level);
  if (pause && channel >= 0)
    Mix_Resume (channel);
#endif
}

#endif
