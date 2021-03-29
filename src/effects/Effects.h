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

/* This file includes all special effects. */

#ifndef IS_EFFECTS_H
#define IS_EFFECTS_H

#include "model3d/Model3d.h" // ok
#include "util/Math.h" // ok

extern int specialeffects;
extern int quality;

extern Texture *texsmoke, *texsmoke2, *texsmoke3;

#define MAXSMOKEELEM 40

/**
* This class adds smoke elements for fighters and missiles.
*/
class Smoke
{
  public:

    Vector3 v [MAXSMOKEELEM];
    float zoom [MAXSMOKEELEM];
    int time [MAXSMOKEELEM];
    int last;
    int type;

    Smoke (int type);
    virtual ~Smoke ();

    void setSmoke (float x, float y, float z, int mytime);
    void move (Uint32 dt, int dec);
    void drawElem (int n, float phi, float gamma);
    void drawElemHQ (int n);
    void draw (float phi, float gamma);
};

/**
* This class adds flashes for thunderstorms
*/
class Flash
{
  public:

    int time;
    int phi; // player's view angle
    Vector3 w [10];

    Flash ();
    virtual ~Flash ();

    void set (float x, float y, float z, int phi);
    void draw ();
    void drawHQ ();
};

/**
* This class adds explosion balls.
*/
class Explosion : public SpaceObj
{
  public:

    Space *space;
    int ttl;
    int maxlen;
    float maxzoom;
    Vector3 v;

    Explosion (Space *space, Model3d *sphere);
    virtual ~Explosion () {}

    void setExplosion (float x, float y, float z, float vx, float vy, float vz, float maxzoom, int len);
    void move (Uint32 dt, float camphi, float camgamma);
    // drawGL() of SpaceObj
};

/**
* This class adds dark smoke after explosions.
*/
class BlackSmoke : public SpaceObj
{
  public:

    Space *space;
    int ttl;
    int maxlen;
    float maxzoom;
    float myphi; // player's view angle, obsolete since v0.8.5
    float camphi;

    BlackSmoke (Space *space);
    virtual ~BlackSmoke () {}

    void setBlackSmoke (float x, float y, float z, float myphi, float maxzoom, int len);
    void move (Uint32 dt, float camphi, float camgamma);
    virtual void drawGL (Vector3 &tl, float alpha2, float lum2, bool drawlight2, bool istextured2);
};

/**
* This class adds stars to the night sky.
*/
class Star
{
  public:

    int phi;   ///< polar (sphere) coords
    int gamma; ///< polar (sphere) coords
    float size;

    Star (int phi, int gamma, float size);
    virtual ~Star () {}
    void draw ();
};

/**
* This class creates and displays fonts out of bitmaps.
*
* Each font is stored in one bitmap, width=height=2^n!
* Use GIMP or equiv and put the ASCII letters of your favourite font
* there, ascending order, start whereever you want. Write as many
* letters as possible per line, each separated by at least one whitespace.
* Look at the font*.tga files for examples. The max letter height is fixed.
*/
class Font
{
  public:

    Texture *texture;    ///< the font texture
    char start;          ///< starting character
    int height;          ///< fixed line height in the texture (should be maximum letter height)
    int n;               ///< number of letters
    float zoom;          ///< zoom factor for output
    short letterx [256]; ///< starting x coordinate in the texture
    short lettery [256]; ///< starting y coordinate in the texture
    short letterw [256]; ///< width in the texture
    Color stdcol;
    Color highlightcol;

    Font (const std::string &filename, int height, char start, int num); // new Font
    virtual ~Font ();

    bool isPixel (int x, int y); // is pixel set in the texture?
    void extractLetters (int height, char start, int num); // extract the letters

    // some custom OpenGL output methods
    void drawText (float x, float y, float z, const std::string &str, const Color &c, bool centered, int highlight, const Color &highlightcol);
    void drawText (float x, float y, float z, const std::string &str, const Color &c, int highlight, const Color &highlightcol);
    void drawText (float x, float y, float z, const std::string &str, const Color &c, int highlight);
    void drawText (float x, float y, float z, const std::string &str, const Color &c);
    void drawText (float x, float y, float z, const std::string &str, int highlight, const Color &highlightcol);
    void drawText (float x, float y, float z, const std::string &str, int highlight);
    void drawText (float x, float y, float z, const std::string &str);
    void drawTextCentered (float x, float y, float z, const std::string &str, const Color &c, int highlight, const Color &highlightcol);
    void drawTextCentered (float x, float y, float z, const std::string &str, const Color &c, int highlight);
    void drawTextCentered (float x, float y, float z, const std::string &str, const Color &c);
    void drawTextCentered (float x, float y, float z, const std::string &str, int highlight, const Color &highlightcol);
    void drawTextCentered (float x, float y, float z, const std::string &str, int highlight);
    void drawTextCentered (float x, float y, float z, const std::string &str);
    void drawTextRotated (float x, float y, float z, const std::string &str, const Color &color, int timer);
    void drawTextScaled (float x, float y, float z, const std::string &str, const Color &color, int timer, int highlight, const Color &highlightcol);
    void drawTextScaled (float x, float y, float z, const std::string &str, const Color &color, int timer, int highlight);
    void drawTextScaled (float x, float y, float z, const std::string &str, const Color &color, int timer);
    void drawTextScaled (float x, float y, float z, const std::string &str, int timer, int highlight, const Color &highlightcol);
    void drawTextScaled (float x, float y, float z, const std::string &str, int timer, int highlight);
    void drawTextScaled (float x, float y, float z, const std::string &str, int timer);
};

/**
* This class adds clouds to the sky.
*/
class HighClouds : public SpaceObj
{
  public:

    SpherePart *sphere;

    HighClouds (int theta);
    virtual ~HighClouds ();

    void init (int theta);
    void setTexture (Texture *texture);
    void drawGL (Vector3 &tl, Vector3 &textl);
};

#endif
