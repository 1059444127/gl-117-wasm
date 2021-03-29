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

#ifndef IS_GLLANDSCAPE_H
#define IS_GLLANDSCAPE_H

#include <string.h>

#include "model3d/Model3d.h" // ok
#include "landscape/Landscape.h" // ok
#include "shader/GlShaders.h"



#define PARTS 48 ///< maximum LOD (occlusion culling) parts

// weather
#define WEATHER_SUNNY 0
#define WEATHER_THUNDERSTORM 1
#define WEATHER_CLOUDY 2

/// the exact landscape type
#define LANDSCAPE_ALPINE 0
#define LANDSCAPE_ALPINE_NOLAKE 1
#define LANDSCAPE_LOW_ALPINE 2
#define LANDSCAPE_ALPINE_EROSION 5
#define LANDSCAPE_ALPINE_SEA 6
#define LANDSCAPE_ALPINE_ROCKY 7
#define LANDSCAPE_SEA 10
#define LANDSCAPE_MOON 20
#define LANDSCAPE_FLAT_MOON 21
#define LANDSCAPE_CANYON 30
#define LANDSCAPE_CANYON_TRENCH 31
#define LANDSCAPE_DESERT 40
#define LANDSCAPE_ARCTIC 50

/// the maximum number of materials
#define MAXMATERIAL 101

/// textures need to be loaded in the application
extern Texture *texgrass, *texrocks, *texwater, *textree, *textree2, *textree3, *texcactus1, *texredstone;
extern Texture *textreeu, *textreeu2, *textreeu3, *textreeu4, *textreeu5, *texcactusu1;
extern Texture *textree4, *textree5, *texearth, *texsand, *texredsand, *texgravel1;
extern Texture *texglitter1;

extern int antialiasing;
extern int weather;
extern float view;
extern int contrast;
extern float sungamma; ///< angle of sunrays dropping on horizon
extern int day;
extern int quality;
extern int specialeffects;

/**
* This class calculates all landscape data that is related to OpenGL visualisation.
* It's very BIG (sorry) but therefore extremely fast.
* precalculate () has to be called on init to create all color/light maps and texture references.
* draw () is called each time the landscape should be rendered with sperical coords as parameters
* (to do LOD for trees and other objects).
* Briefly speaking, the terrain rendering technique includes a multilayered height grid.
* Terrain near to the camera is visualized on a finer grid than further quads.
* Besides, a very coarse grid is used to do prefrustum culling and occlusion culling.
* Occlusion culling is done by ray casting for each of the coarse quads in time O(numQuads).
*/
class GlLandscape : public Landscape
{
    VertexArray vertexarrayquadstrip;
    VertexArray vertexarrayquad [30];
    VertexArray vertexarraytriangle [30];
    VertexArray vertexarrayglitter [2];
  public:

    short nl [MAXX + 2] [MAXX + 2]; ///< lighting mask, MAXX+1 x MAXX+1 is sufficient, but VC bug or my bug?
    float lv [3];
    unsigned char r [MAXX + 1] [MAXX + 1];  ///< red
    unsigned char g [MAXX + 1] [MAXX + 1];  ///< green
    unsigned char b [MAXX + 1] [MAXX + 1];  ///< blue
    unsigned char drawrule [MAXX] [MAXX];   ///< draw triangles or quads?
    unsigned char tex1 [MAXX] [MAXX];       ///< texture for quad or triangle1
    unsigned char tex2 [MAXX] [MAXX];       ///< texture for triangle2 if needed
    unsigned char dl [MAXX + 1] [MAXX + 1]; ///< dynamic light mask (explosions)
    int lsticker;     ///< a timer increased every time draw() is called
    int neargridstep; ///< landscape near grid resolution (1=fine ... 4=coarse)
    int fargridstep;  ///< landscape far grid resolution (1=fine ... 4=coarse)
    int gridstep;     ///< landscape raster block grid resolution (1=fine ... 4=coarse)
    float glittering; ///< water glittering
    unsigned short hcmin [MAXX/4 + 1] [MAXX/4 + 1]; ///< height mask on a coarser grid: max{h[x+-2][y+-2]}
    unsigned short hcmax [MAXX/4 + 1] [MAXX/4 + 1]; ///< height mask on a coarser grid: max{h[x+-2][y+-2]}
    bool hastowns;
    int detail [PARTS] [PARTS]; ///< LOD
    float sunlight;
//    Space *space;

    GlShaders * shaders;

  public:

    friend class TreeShader;

    GlLandscape (int type, int *heightmask);
    ~GlLandscape()
    {
      destroyShaders(shaders);
      shaders = 0;
    }

    void precalculate (); ///< precalculate everything (colors, light mask)
    float getView (); ///< get current view depth depending on the weather

    float getMinHeight (float x, float z);      ///< min height of 4x4 grid at landscape point
    float getMaxHeight (float x, float z);      ///< min height of 4x4 grid at landscape point
    float getHeight (float x, float z);         ///< height at landscape point
    float getExactHeight2 (float x, float z);   ///< linear interpolation, coarse grid
    float getExactHeight3 (float x, float z);   ///< linear interpolation, coarse grid
    float getExactHeight4 (float x, float z);   ///< linear interpolation, coarse grid
    float getExactHeight (float x, float z);    ///< linear interpolation
    float getExactLSHeight2 (float x, float z); ///< linear interpolation, coarse grid
    float getExactLSHeight3 (float x, float z); ///< linear interpolation, coarse grid
    float getExactLSHeight4 (float x, float z); ///< linear interpolation, coarse grid
    float getExactLSHeight (float x, float z);  ///< linear interpolation
    float getRayHeight (float x, float z);      ///< height of lowest sun ray
    float getExactRayHeight (float x, float z); ///< linear interpolation

    void drawTree (float x, float y, float htree, float wtree, int phi); // two in one
    void drawTreeQuad (int x, int y, int phi, bool hq);
    void drawTown (int x, int y);

    void drawQuadStrip (int x1, int y1, int x2, int y2); ///< fast quad strip without textures
    void drawQuad (int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
    void drawTriangle (int x1, int y1, int x2, int y2, int x3, int y3);
    void drawTexturedQuad (int x, int y);
    void drawTexturedTriangle (int x1, int y1, int x2, int y2, int x3, int y3);
    void drawTexturedQuad (int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
    void drawWaterTexturedQuad (Vector3 &cam, int x, int y);
    void drawTexturedTriangle1 (int x, int y);
    void drawTexturedTriangle2 (int x, int y);

    void draw (Vector3 &cam, float phi, float gamma); ///< draw ALL
    /// explosions and other light sources light up the surface, calculate dynamically by CPU
//    void calcDynamicLight (Explosion **explo, SpaceObj **cannon, SpaceObj **missile, SpaceObj **flare);
    void calcDynamicLight (SpaceObj *object, float threshold, float maxintens, float intensfac);
    void setMaterial (int n, float r, float g, float b, Texture *tex);

  private:

    int texturetree1, texturetree2;
    unsigned short hray [MAXX + 1] [MAXX + 1]; ///< height of lowest sunray
    float mat [MAXMATERIAL] [4];   ///< materials=colors of landscape IDs
    Texture *texmap [MAXMATERIAL]; ///< texture for material
    Color treecolor;
    int vmin [PARTS] [PARTS]; ///< LOD
    int vmax [PARTS] [PARTS]; ///< LOD
    float vh [PARTS] [PARTS]; ///< LOD
    bool vis [PARTS] [PARTS]; ///< LOD
    float lightfac; ///< light factor to multiply
    unsigned short lg [MAXX + 1] [MAXX + 1]; ///< gaussian filter result

    void norm (float *c);
    void normalcrossproduct (float *a, float *b, float *c);
//    int selectColor (int x, int y);
};

/// The current OpenGL landscape
extern GlLandscape *l;

#endif
