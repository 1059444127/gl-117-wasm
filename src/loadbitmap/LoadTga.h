/*
    GL-117
    Copyright 2001-2004 Thomas A. Drexl aka heptargon

    loader_tga.c - Loader for Truevision Targa images
    by Dan Maas <dmaas@dcine.com>   May 15, 2000
    Modified for GL-117 by Piotr Pawlow <pp@siedziba.pl>
    based on TGA specifications available at:
    http://www.wotsit.org/cgi-bin/search.cgi?TGA
    header/footer structures courtesy of the GIMP Targa plugin

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

#ifndef IS_LOADTGA_H
#define IS_LOADTGA_H

typedef unsigned int DATA32;
typedef unsigned char DATA8;

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

    static DATA32* flip (DATA32* in, int w, int h);
    static inline void writeRGBA (DATA32 *p, DATA8 r, DATA8 g, DATA8 b, DATA8 a);
};

#endif
