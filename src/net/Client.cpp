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



Client::Client ()
{
#ifdef HAVE_SDL_NET
  sock = NULL;
  set=SDLNet_AllocSocketSet(1);
  if(!set)
  {
    printf("SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
    SDLNet_Quit();
    SDL_Quit();
    exit(4); //most of the time this is a major error, but do what you want.
  }
#endif
}

bool Client::getServer(const char *hostname, const char *name)
{
#ifdef HAVE_SDL_NET

  printf("try join,");

  if (sock != NULL) return true;
  IPaddress ip;
  //if(SDLNet_ResolveHost(&ip,hostname,port)==-1)
  if(SDLNet_ResolveHost(&ip,hostname,port)==-1)
  {
    printf("SDLNet_ResolveHost: %s\n",SDLNet_GetError());
    SDLNet_Quit();
    SDL_Quit();
    exit(5);
  }

  // open the server socket
  sock=SDLNet_TCP_Open(&ip);
  if(!sock)
  {
    printf("SDLNet_TCP_Open: %s\n",SDLNet_GetError());
    return false;
//    SDLNet_Quit();
//    SDL_Quit();
//    exit(6);
  }
  printf("Connecting to %s port %d\n",hostname,port);

  if(SDLNet_TCP_AddSocket(set,sock)==-1)
  {
    printf("SDLNet_TCP_AddSocket: %s\n",SDLNet_GetError());
    SDLNet_Quit();
    SDL_Quit();
    exit(7);
  }

  // login with a name
/*  if(!putMsg(sock,name))
  {
    SDLNet_TCP_Close(sock);
    SDLNet_Quit();
    SDL_Quit();
    exit(8);
  }*/
return true;
#else
return false;
#endif

}

int Client::getMessage(char *buf)
{
#ifdef HAVE_SDL_NET

  int numready=SDLNet_CheckSockets(set, 0);
  if(numready==-1)
  {
    printf("SDLNet_CheckSockets: %s\n",SDLNet_GetError());
    return 0;
  }
  if(numready)
  {
    if (SDLNet_SocketReady(sock))
    {
      if (getMsg (sock, buf))
      {
//        printf ("clientnummer: %d",n);
//        printf("Received: %.*s\n",len,message);
        if (strcmp (buf,"ready"))
        {
          sendMessage("ready", 5);
        }
        return 1;
      }
      else
      {
        printf ("Server lost, closing connection...");
        SDLNet_TCP_Close(sock);
        sock = NULL;
        return 0;
      }
    }
    else
    {
      printf ("Socket not ready");
      return 0;
    }
  }
// getch();
// printf ("numready = 0");
return 0;
#else
return 0;
#endif
}

void Client::sendMessage(const char *buf, int len)
{
#ifdef HAVE_SDL_NET
  if (!putMsg(sock, buf, len)) printf ("FEHLER");
#endif
}

#endif
