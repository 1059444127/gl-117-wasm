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

/* This file includes multiplayer support. Copyright Norbert Drexl. */

#ifndef IS_NET_H

#include "net.h"

int port = 4567;
char *message;



#ifdef HAVE_SDL_NET
int getMsg (TCPsocket sock, char *buf)
{
  Uint32 len, result;

  // receive the length of the string message
  result = SDLNet_TCP_Recv (sock, &len, sizeof (len));
  if (result < sizeof (len))
  {
    if (SDLNet_GetError () && strlen (SDLNet_GetError ())) // sometimes blank!
      printf ("SDLNet_TCP_Recv: %s\n", SDLNet_GetError ());
    else
      printf ("unknown error"); fflush (stdout);
    return 0;
  }

  // swap byte order to our local order
  len = SDL_SwapBE32 (len);

  // check if anything is strange, like a zero length buffer
  if (!len)
    return 0;

  // get the string buffer over the socket
  result = SDLNet_TCP_Recv (sock, buf, len);
  if (result<len)
  {
    if (SDLNet_GetError () && strlen (SDLNet_GetError ())) // sometimes blank!
      printf ("SDLNet_TCP_Recv: %s\n", SDLNet_GetError ());
    return 0;
  }

  return 1;
}
#endif

// send a string buffer over a TCP socket with error checking
// returns 0 on any errors, length sent on success
#ifdef HAVE_SDL_NET
int putMsg (TCPsocket sock, const char *buf, int len)
{
  Uint32 result;

  // change endianness to network order
  len = SDL_SwapBE32 (len);

  // send the length of the string
  result = SDLNet_TCP_Send (sock, &len, sizeof(len));
  if (result < sizeof (len)) {
    if (SDLNet_GetError () && strlen (SDLNet_GetError ())) // sometimes blank!
      printf ("SDLNet_TCP_Send: %s\n", SDLNet_GetError ());
    return(0);
  }

  // revert to our local byte order
  len = SDL_SwapBE32 (len);

  // send the buffer, with the NULL as well
  result = SDLNet_TCP_Send (sock, buf, len);
  if (result < len)
  {
    if (SDLNet_GetError () && strlen (SDLNet_GetError ())) // sometimes blank!
      printf ("SDLNet_TCP_Send: %s\n", SDLNet_GetError ());
    return 0;
  }

  // return the length sent
  return result;
}
#endif

#endif
