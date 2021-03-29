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

#ifndef IS_BITMAP_H
#define IS_BITMAP_H

#ifdef _MSC_VER
  #pragma warning(disable:4786) // workaround for MSVC6 bug, needs service pack
#endif

typedef unsigned int Uint32;
typedef unsigned char Uint8;

#include <map>
#include <string>



/**
* A bitmap consists of width, height and pixel data.
* This data is normally stored as RGBA linearized in rows:
* a11, a12, ..., a1n, a21, a22, ..., a2n, a31, etc.
*/
class Bitmap
{
  public:

    Uint8 *data;
    int width;
    int height;

    Bitmap ();

//    unsigned char *getPixel (int x, int y) const;
//    void setPixel (int x, int y, unsigned char *c)
};



/**
* Load up a TGA file.
*
* As written the function load() only recognizes the following types of Targas:
*		Type 02 - Uncompressed RGB, 24 or 32 bits
*		Type 03 - Uncompressed grayscale, 8 bits
*		Type 10 - RLE-compressed RGB, 24 or 32 bits
*		Type 11 - RLE-compressed grayscale, 8 bits
* There are several other (uncommon) Targa formats which it can't currently handle.
*/
class LoadTga
{
  public:

    static unsigned char* load (const char* im_file, int* w, int* h);

  protected:

    static Uint32* flip (Uint32* in, int w, int h);
    static inline void writeRGBA (Uint32 *p, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
};



typedef std::map<std::string, Bitmap *> BitmapList;

/**
* Use the factory to load bitmaps from arbitrary files.
* The factory decides according to the filename, if the bitmap has to be loaded from disk
* or if the bitmap is already in memory.
*
* @example
* BitmapFactory bf;
* Bitmap *bitmap = bf.getBitmap ("myfile.tga");
*/
class BitmapFactory
{
  public:

    static BitmapList bitmapList;

    static Bitmap *getBitmap (const std::string &name);
};

#endif
