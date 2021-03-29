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

#ifndef IS_LANDSCAPE_H
#define IS_LANDSCAPE_H

// surface extents
const int MAXX      = 512;         ///< terrain dimensions: MAXX x MAXX
const int MAXX_MASK = 0x000001ff;  ///< AND-mask to calculate modulo MAXX
const int LOG2MAXX  = 9;           ///< number of iterations
const int HEIGHT    = 120;         ///< height scaling factor

// landscapes
const int LAND_ALPINE = 0;
const int LAND_MOON   = 1;
const int LAND_CANYON = 2;
const int LAND_DESERT = 3;
const int LAND_ARCTIC = 4;

// maximum Gaussian convolution kernel extent
const int MAXCORE     = 10;

// fast coordinate cycle
#define GETCOORD(x) ((x) & MAXX_MASK)

// static ID values for the landscape type (array "f")
const int GRASS            = 0;
const int CONIFEROUSWOODS0 = 1;
const int CONIFEROUSWOODS1 = 2;
const int CONIFEROUSWOODS2 = 3;
const int CONIFEROUSWOODS3 = 4;
const int DECIDUOUSWOODS0  = 5;
const int DECIDUOUSWOODS1  = 6;
const int DECIDUOUSWOODS2  = 7;
const int DECIDUOUSWOODS3  = 8;
const int MIXEDWOODS0      = 9;
const int MIXEDWOODS1      = 10;
const int MIXEDWOODS2      = 11;
const int MIXEDWOODS3      = 12;
const int DWARFPINES0      = 13;
const int DWARFPINES1      = 14;
const int DWARFPINES2      = 15;
const int DWARFPINES3      = 16;
const int BUSHES0          = 17;
const int BUSHES1          = 18;
const int BUSHES2          = 19;
const int BUSHES3          = 20;
const int CACTUS0          = 25;
const int ROCKS            = 30;
const int ROCKS2           = 31;
const int GLACIER          = 32;
const int GREYSAND         = 33;
const int GRAVEL           = 34;
const int REDSTONE         = 35;
const int REDSAND          = 36;
const int REDTREE0         = 37;
const int WATER            = 40;
const int SHALLOWWATER     = 41;
const int DEEPWATER        = 42;
const int XSHALLOWWATER    = 43;
const int XDEEPWATER       = 44;
// Water until 49
const int MOONSAND         = 50;
const int DESERTSAND       = 60;
const int TOWN             = 90;
const int TRYLAKE          = 100;

class Landscape
{
  public:

    /// height values without fluids
    unsigned short h [MAXX + 1] [MAXX + 1];
    /// height values including fluids (water)
    unsigned short hw [MAXX + 1] [MAXX + 1];
    /// landscape type (ID values)
    unsigned char f [MAXX + 1] [MAXX + 1];
    /// landscape type (0=alpine, 1=moon, 2=canyon)
    int type;

    Landscape ();
    ~Landscape ();

    /// fast convolution function (isotropic), currently only Gauss, radius in {1,2,3}
    void convolveGauss (int radius, int hmin, int hmax);
    /// 5x5 convolution (radius=3) for a single raster point
    void gauss (int x, int y);
    /// constant height for 2*dx x 2*dy fields
    void flatten (int x, int y, int dx, int dy);
    /// special erosion function
    void smoothGlacier ();

    /// check if type=f[][] is of a specific id
    bool isType (unsigned char type, unsigned char id) const;
    /// check if type=f[][] is woods
    bool isWoods (int type) const;
    /// check if type=f[][] is water
    bool isWater (int type) const;
    /// check if type=f[][] is glacier
    bool isGlacier (int type) const;
    /// check if type=f[][] is solid ground
    bool isGround (int x, int y) const;

    /// reset data fields
    void init ();

    /// create alpine surface
    void genSurface (int elevationpc, int *heightmap);
    /// create alpine erosion surface
    void genErosionSurface (int elevationpc, int *heightmap);
    /// create arctic surface
    void genArcticSurface (int elevationpc, int *heightmap);
    /// create canyon surface
    void genCanyonSurface (int elevationpc);
    /// create moon surface
    void genMoonSurface (int height);
    /// create desert surface
    void genDesertSurface (int elevationpc);

    /// create a trench
    void genTrench (int width, int height);
    /// create percent rocks if landscape diff > diffmin
    void genRocks (int diffmin, int percent);
    /// create a lake
    void genLake (int depthpc);
    /// calculate woods on steep grass terrain
    void calcWoods (int dy);
    /// search a plain
    void searchPlain (int divx, int divy, int *x, int *y);

  protected:

    int highestPoint;   ///< maximum evaluated point of the terrain
    int lowestPoint;    ///< minimum evaluated point of the terrain

    /// calculate a lake
    int calcLake (int ys, int xs, unsigned short level, int num, int maxextent);

  private:

    unsigned char ftry [MAXX + 1] [MAXX + 1]; ///< dummy data field
    unsigned short hg [MAXX + 1] [MAXX + 1];  ///< dummy data field to apply convolution kernels
    int rockBorder;     ///< min height value for creating rocks
    int elevation;      ///< surface elevation extent (est. highest-lowest point)
};

#endif

