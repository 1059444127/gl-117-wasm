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

#ifndef IS_COCKPIT_H
#define IS_COCKPIT_H

#include "configuration/Configuration.h" // ok
#include "model3d/Model3d.h" // ok

/**
* This class includes all methods to render a standard cockpit.
*/
class Cockpit
{
  public:

    int flarewarning;
    int chaffwarning;
    Uint32 dt;

    Cockpit ();

    void drawBlip (int shape, float x, float y, float z, unsigned char r, unsigned char g, unsigned char b);

    void setColor (int alpha);
    void setColor (Color *color, int alpha);
    void drawCounter (bool beep);
    void drawTargeter ();
    void drawCross ();
    void drawHeading ();
    void drawTargetedElement ();
    void drawWeapon ();
    void drawRadar ();
    void drawRelativeHeightBar();
    void drawThrustBar();

  private:

    void cockpitvertex (float phi, float gamma);
};

#endif

