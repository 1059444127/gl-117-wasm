// APPLE path is different (thanks Minami)
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#ifndef __APPLE__
#include <GL/glu.h>
#endif
#include <SDL2/SDL.h>
#ifdef HAVE_SDL_MIXER
#  include <SDL2/SDL_mixer.h>
#endif
#ifdef HAVE_SDL_NET
#  include <SDL2/SDL_net.h>
#endif

#ifdef _MSC_VER
  #pragma warning(disable:4786) // workaround for MSVC6 bug, needs service pack
#endif
