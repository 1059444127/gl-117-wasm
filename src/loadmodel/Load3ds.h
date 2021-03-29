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

#ifndef IS_LOAD3DS_H
#define IS_LOAD3DS_H

#include "model3d/Model3d.h" // ok



const int debug3ds = 1; // set to 1 to get all Chunk3ds IDs to stdout

extern void ErrorOutOfMemory ();

/**
* This class is an abstract binary file representation in memory to load 3ds files.
*/
class BinaryFile3ds
{
  public:

    BinaryFile3ds (const char *filename);
    ~BinaryFile3ds ();

    int readFloat (float *f);
    int readFloat (float *f, const int n);
    int readUInt32 (Uint32 *i);
    int readUInt16 (Uint16 *i);
    int readString (char *ptr, const int ptrmax, const int n);
    int readString (char *ptr, const int n);
    int readString (char *ptr);
    int skip (const int n);
    Uint32 getSize ();

private:

    FILE *in;              ///< file struct
    Uint32 size;           ///< file size
    Uint32 filepointer;    ///< pointer to char in memory
    unsigned char *data;   ///< file data in memory
    unsigned char ret [8]; ///< big endian swapping buffer
};

// Primary Chunk3ds (19789)
#define PRIMARY           0x4D4D

// Main Chunks (15677, 2, 45056)
#define VERSION           0x0002
#define MAINSCALE         0x0100
#define OBJECTINFO        0x3D3D
#define EDITKEYFRAME      0xB000

// Sub defines of OBJECTINFO (45055, 16384)
#define MATERIAL          0xAFFF
#define OBJECT            0x4000

// Sub defines of MATERIAL
#define MAT_NAME          0xA000
#define MAT_DIFFUSE       0xA020
#define MAT_MAP           0xA200
#define MAT_MAPFILE       0xA300
#define MAT_USCALE        0xA354
#define MAT_VSCALE        0xA356
#define MAT_UOFFSET       0xA358
#define MAT_VOFFSET       0xA35A
#define MAT_ROTATION      0xA35C

// Read new object
#define OBJECT_MESH       0x4100

// Sub defines of OBJECT_MESH
#define OBJECT_VERTICES   0x4110
#define OBJECT_FACES      0x4120
#define OBJECT_MATERIAL   0x4130
#define OBJECT_UV         0x4140
#define OBJECT_MESHMATRIX 0x4160

// Chunk3ds info
struct Chunk3ds
{
  Uint16 ID;
  Uint32 length;
  Uint32 bytesRead;
};

/**
* Load3ds handles all of the loading code (see NEHE's OpenGL tutorials)
*/
class Load3ds
{
  public:

    BinaryFile3ds *file;

    Load3ds ();

    void setTextureDir (const std::string &texDir);
    bool import3ds (Model3d *model, const char *filename);

  // developer specific loading routines
  private:

    FILE *filepointer;
    Chunk3ds *currentChunk;
    Chunk3ds *tempChunk;
    std::string textureDir;

    int getString (char *);
    void readChunk (Chunk3ds *);
    void processNextChunk (Model3d *model, Chunk3ds *);
    void processNextObjectChunk (Model3d *model, Object3d *object, Chunk3ds *);
    void processNextMaterialChunk (Model3d *model, Chunk3ds *);
    void readColorChunk (Material *material, Chunk3ds *pChunk);
    void readVertices (Object3d *object, Chunk3ds *);
    void readVertexIndices (Object3d *object, Chunk3ds *);
    void readUVCoordinates (Object3d *object, Chunk3ds *);
    void readMeshMatrix (Object3d *object, Chunk3ds *);
    void readUScale (Material *material, Chunk3ds *);
    void readVScale (Material *material, Chunk3ds *);
    void readUOffset (Material *material, Chunk3ds *);
    void readVOffset (Material *material, Chunk3ds *);
    void readUVRotation (Material *material, Chunk3ds *);
    void readObjectMaterial (Model3d *model, Object3d *object, Chunk3ds *previousChunk);
    void compile (Model3d *model);
    void computeNormals (Model3d *model);
    void computeColors (Model3d *model);
    void loadTextures (Model3d *model);
    void normalize (Model3d *model);
    void cleanUp ();
};

#endif

