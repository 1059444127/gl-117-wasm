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

#ifndef IS_MODEL3D_H
#define IS_MODEL3D_H

#include "opengl/includegl.h"
#include "opengl/GlPrimitives.h"
#include "util/Math.h"

#include <iostream>

extern int antialiasing;
extern int timestep;

/* Currently models are normalized to the (-1,-1,-1)-(1,1,1) cube and static!
   The model represents the "class" description of a static model's geometry and colors.
   It is "instanciated" using a DynamicObj which has a reference to the model,
   scaling, rotation, translation, physical attributes...
   A model consists of several objects.
   Objects consist of vertices, quads, triangles, and one material. */

/* Coding:
   - do not overload operators, it's crucial to differ from pointer assignments and fast ops
   - do not write destructors, as arrays of these classes should be treatable as float* array
     thus never make a destructor virtual as the virtptr is also somewhere in memory */

/**
* Color stores color information for red, green, blue, alpha, each 0-255.
* Access is granted directly through array c with c[0]=red, c[1]=green, etc.
*/
class Color
{
  public:

    /// color information as vector, 32 bpp (floats would be faster for the vertex arrays)
    unsigned char c [4];

    Color ();
    Color (const Color &color);
    Color (int red, int green, int blue, int alpha = 255);

    void set (const Color &color);
    void set (int red, int green, int blue, int alpha = 255);
    bool isEqual (const Color &color) const; // compare colors
};

/**
* Texture loads and stores a texture to memory.
* To use a texture, it must be loaded and added to the OpenGL texture list using
*   gl->genTextureTGA ();
* instead of this class!
*/
class Texture
{
  public:

    unsigned char *data; ///< texture color data as array, 32bpp, RGBA
    std::string name;    ///< texture file name
    bool mipmap;         ///< mipmapping on/off depending on gluBuildMipmaps
    int textureID;       ///< texture ID of native GL code
    int width;           ///< texture width
    int height;          ///< texture height
    // average values to substitute textured quads by colors for LOD
    float texlight;      ///< average of texture's overall brightness
    float texred;        ///< average of texture's red
    float texgreen;      ///< average of texture's green
    float texblue;       ///< average of texture's blue
    bool alpha;          ///< alpha blending on/off

    Texture ();
    Texture (const std::string &filename, int alphaprogram = -1,
              bool mipmap = true, bool alpha = false); // TODO: alpha really necessary?
    ~Texture (); // necessary

    /// loadFromTGA is called via gl->genTextureTGA() to not load the same texture twice
    bool loadFromTGA (const std::string &filename, int alphaprogram = -1,
                      bool mipmap = true, bool alpha = false);
    /// get color of a special pixel
//    void getColor (Color *c, int x, int y) const;
    /// set to linear shading between texels (expensive esp. on old graphic cards)
//    void shadeLinear () const;
    /// set to const shading between texels (fast)
//    void shadeConst () const;
};

/**
* Vector3 stores the components of a 3D vector (x,y,z).
* Access x,y,z directly.
*/
class Vector3
{
  public:

    float x; ///< x coordinate
    float y; ///< y coordinate
    float z; ///< z coordinate

    Vector3 ();
    Vector3 (float x, float y, float z);
    Vector3 (float xyz);
    Vector3 (const Vector3 &v);

    void set (const Vector3 &v);
    void set (float x, float y, float z);
    void neg ();
    void add (const Vector3 &v);
    void sub (const Vector3 &v);
    void mul (float fac);
    void crossproduct (const Vector3 &v);
    float dotproduct (const Vector3 &v) const;
    float length () const;
    void norm ();
    /// exactly equal in memory? (e.g. after loading from file)
    bool isEqual (const Vector3 &v) const;
    /// numerically equal, use a tolerance like 1E-8
    bool isEqual (const Vector3 &v, float tolerance) const;
};

/**
* Vector2 stores the components of a 2D vector (x,y).
* Access x,y directly.
*/
class Vector2
{
  public:

    float x; ///< x coordinate
    float y; ///< y coordinate

    Vector2 ();
    Vector2 (float x, float y);
    Vector2 (const Vector2 &v);

    void set (const Vector2 &v);
    void set (float x, float y);
    /// exactly equal in memory (no sense for comparisons)
    bool isEqual (const Vector2 &v) const;
    /// numerically equal, use a tolerance like 1E-8
    bool isEqual (const Vector2 &v, float tolerance) const;
};

/**
* Vertex represents a vertex which may take information about
* color, location, normal, texture, number of surrounding triangles.
*/
class Vertex
{
  public:

    int triangles;   ///< number of triangles to which this vertex belongs to
    Color color;     ///< color, used when displayed without textures
    Vector3 vector;  ///< position
    Vector3 normal;  ///< normal vector, interpolation of all surrounding triangles
    Vector2 tex;     ///< 2D texture coordinates

    Vertex ();
    Vertex (const Vertex &v);

    /// copy data from v
    void set (const Vertex &v);
    /// the normal vector of a vertex can be calculated as average of all adjacent plane normals
    void addNormal (const Vector3 &n);
    /// the color of a vertex can be calculated as average of all adjacent plane colors
    void addColor (const Color &c);
};

/**
* Rotation stores one (x,y,z)-rotation.
*/
class Rotation
{
  public:

    float gamma; ///< rotation angle for a plane of the coordinate system
    float theta; ///< rotation angle for a plane of the coordinate system
    float phi;   ///< rotation angle for a plane of the coordinate system

    Rotation ();
    Rotation (float gamma, float theta, float phi);

    void set (const Rotation &r);
    void set (float gamma, float theta, float phi);
    void add (const Rotation &r);
    void add (float gamma, float theta, float phi);
/*    void calculate (); // calculate the rotation matrix
    float rotateX (const Vector3 &v) const;
    float rotateY (const Vector3 &v) const;
    float rotateZ (const Vector3 &v) const;*/

  protected:

    // the rotation matrix of this rotation
//    float rot [3] [3];
};

/**
* Transformation stores a complete 3D transformation (position, rotation, scaling).
*/
class Transformation
{
  public:

    Vector3 translation;
    Rotation rotation;
    Vector3 scaling;

    Transformation ();
    Transformation (const Transformation &trafo);
    Transformation (const Vector3 &tl, const Rotation &rot, const Vector3 &scaling);

    void set (const Transformation &trafo);
    void set (const Vector3 &tl, const Rotation &rot, const Vector3 &scaling);
};

/**
* Triangle stores references to the vertices of the triangle/face.
*/
class Triangle
{
  public:

    Vertex *v [3]; ///< references to the three vertices

    Triangle ();

    void calcNormal (Vector3 *n); ///< return normal n
    void setVertices (Vertex *a, Vertex *b, Vertex *c); // not const as a,b,c may be altered
};

/**
* Quad stores references to the vertices of the quad/square/face.
*/
class Quad
{
  public:

    Vertex *v [4]; ///< references to the four vertices

    Quad ();

    void calcNormal (Vector3 *n); ///< return normal n
    void setVertices (Vertex *a, Vertex *b, Vertex *c, Vertex *d); // not const as a,b,c,d may be altered
};

/**
* Material stores the name, filename, color, and texture of a material.
*/
class Material
{
  public:

    std::string name;     ///< unique name
    std::string filename; ///< unique file name
    Color color;          ///< uniform color
    Texture *texture;     ///< reference to a texture (or NULL if there is no texture)
    float utile;          ///< tiling coordinates
    float vtile;          ///< tiling coordinates
    float uoffset;        ///< texture offsets (the importer must calculate u/v due to offsets)
    float voffset;        ///< texture offsets (the importer must calculate u/v due to offsets)
    float uscale;         ///< texture scaling (the importer must calculate u/v due to scaling)
    float vscale;         ///< texture scaling (the importer must calculate u/v due to scaling)
    float wrot;           ///< rotation in degree (the importer must calculate u/v due to wrot)

    Material ();
    Material (const Material &material);
};

/**
* Object3d stores the material, vertices, and faces (triangles, quads) of an object.
*/
class Object3d
{
  public:

    Uint16 numVertices;
    Uint16 numTriangles;
    Uint16 numQuads;
    Uint16 numTexVertex;
    std::string name;    ///< unique object name
    Material *material;  ///< an object has one unique material
    bool hasTexture;     ///< an object can have one unique texture
    Vertex *vertex;      ///< vertex list (of exact length)
    Triangle *triangle;  ///< triangle list (of exact length)
    Quad *quad;          ///< quad list (of exact length)

    Object3d ();
//    Object3d (const Object3d &object); // still to implement!

    ///< vertex list must be allocated, add vertex only if not near another vertex
    int addVertex (const Vertex &w);
    void setColor (const Color &col);
    void clear(void);
};

/**
* Model3d stores the materials and objects of a model.
* Its data structure is optimized esp. for 3DS files.
* The drawing methods take the following parameters:
*   tl+tl2=translation,
*   rot=rotation,
*   lum=luminance (default 1.0),
*   explode=radial translation (default 0)
*/
class Model3d
{
  public:

//    std::string name;    ///< unique model name like "GL-117"
    int shading;         ///< shading can be set to FLAT (0) or SMOOTH/GOURAUD (1)
    Uint16 numObjects;   ///< number of objects (which have a unique material)
    Uint16 numMaterials; ///< number of materials (should be the same as the number of objects)
//    bool displaylist;    ///< enable using a display list
    std::vector<Material *> material;  ///< pointers to materials
    std::vector<Object3d *> object;    ///< pointers to objects
    bool nolight;        ///< do not use light?
    bool alpha;          ///< use alpha blending?
    int numRefpoints;    ///< number of reference points for missiles
    Vector3 *refpoint;   ///< multiple references to missile rack positions (at most 10)
    float scale;         ///< overall scaling of original 3DS coords
    Vector3 vscale;      ///< scaling of the original 3DS coords in x,y,z
    // Note: the model is stored to (-1,-1,-1)-(1,1,1) in RAM, the SpaceObj tells about the scaling of a model
    Vector3 cube;        ///< surrounding cube (or radius of sphere) for simplified collision detection
//    int list1, list2, list3;   ///< display lists already generated for each type of draw() method

    Model3d ();
    ~Model3d ();

    void calcMissiles ();
//    void setName (const std::string &name);
    void addMaterial (const Material &material);
    void addObject (const Object3d &object);
    void addRefPoint (const Vector3 &tl);
    void setColor (const Color &col);
    void drawVertexNormals (const Object3d &cm, float zoom);
    int rotateColor (int n);
    void scaleTexture (float fx, float fy);
    /// draw everything
/*    void draw (const Vector3 &tl, const Vector3 &tl2, const Rotation &rot,
		       float zoom = 1.0F, float lum = 1.0F, int explode = 0);
    /// draw without GL lighting, no luminance => VERY FAST (uses display list)
    void drawNoLight (const Vector3 &tl, const Vector3 &tl2, const Rotation &rot,
		              float zoom = 1.0F, int explode = 0);
    /// draw without textures, different luminance
    void drawNoTexture (const Vector3 &tl, const Vector3 &tl2, const Rotation &rot,
		                float zoom = 1.0F, float lum = 1.0F, int explode = 0);*/

  private:

    // all private members are only used "temporarily" at runtime!
    int rotcol; ///< very special for flickering light sources, e.g. the engine's bright yellow color is rotated
//    VertexArray *va; ///< using a vertex array means more memory, but better performance
};

/**
* Sphere represents an ellipsoid according to the Model3d data structure.
*/
class Sphere : public Model3d
{
  public:

    int segments;     ///< segments on lateral angle, doubled for longitudinal angle
    float radius;
    float dx, dy, dz; ///< ellipsoid measures in the carthesian cosy

    Sphere ();
    Sphere (float radius, int segments, float dx, float dy, float dz);
    ~Sphere ();

    void init (float radius, int segments, float dx = 1.0F, float dy = 1.0F, float dz = 1.0F, int randomized = 0);
    void invertNormals (); ///< point outside/inside
    void setNorthPoleColor (const Color &c, float w); ///< see setPoleColor, phi=0, theta=90
    void setSouthPoleColor (const Color &c, float w);
    void setPoleColor (int phi, int theta, const Color &c, float w); ///< shade color over any pole

  private:

    int random (int n);
};

/**
* Part of an ellipsoid.
*/
class SpherePart : public Model3d
{
  public:

    int segments; ///< segments on lateral angle, doubled for longitudinal angle
    float radius;

    SpherePart ();
    SpherePart (float radius, int segments, float phi);
    ~SpherePart ();

    void init (float radius, int segments, float phi = 10.0F);
    void setNorthPoleColor (const Color &c, float w); ///< see setPoleColor, phi=0, theta=90
    void setSouthPoleColor (const Color &c, float w);
    void setPoleColor (int phi, int theta, const Color &c, float w); ///< shade color over any pole
};

// More models could be added here, esp. for importing complex non-polygonal formats (obj, wrl)

class Space; // forward reference

/**
* SpaceObj represents an abstract object (like a cannon shot or a complete fighter).
*/
class SpaceObj
{
  public:

    bool valid;     ///< if not valid, the model is free to be removed
    int draw;       ///< draw/hide
    int explode;    ///< explosion stadium (0=no explosion)
    bool drawLight; ///< draw with/without light
//    float alpha;    ///< overall alpha value (should be 1.0)
    float lum;      ///< luminance (default 1.0)
    Model3d *o;     ///< pointer to a model
    Transformation trafo; ///< transformation
    Space *space;

    /// reference models (e.g. missiles for fighters)
    std::vector<SpaceObj> ref;

    SpaceObj ();
    SpaceObj (const SpaceObj &spaceobj);
    SpaceObj (Model3d *o, const Transformation &trafo);
    virtual ~SpaceObj ();

    void init ();
    void addRefModel (const SpaceObj &ref2);
    void translate (Vector3 &v);
    void translate (float x, float y, float z);
    void rotate (short a, short b, short c);
    void rotateOn (short a, short b, short c);
    ///< z1,z2=easy preculling space/cube, tl=translation
//    virtual void drawGL (Vector3 &tl, float alpha2, float lum2, bool drawLight2, bool isTextured2);
};

/**
* Space represents a collection of SpaceObj
*/
class Space
{
  public:

//    int no;         ///< number of objects in space
//    bool drawLight; ///< draw with/without light
//    float alpha;
//    float lum;      ///< luminance
    std::vector<SpaceObj *> o; ///< reference to all objects in space
//    Vector3 tl;    ///< translation

    Space ();
    virtual ~Space ();

//    void init ();
    void addObject (SpaceObj *o);
    bool removeObject (SpaceObj *o2);
    void removeAllObjects ();
//    void translate (Vector3 &v);
//    virtual void drawGL ();
};

#endif
