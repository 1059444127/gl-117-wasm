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

#ifndef IS_GLPRIMITIVES_H
#define IS_GLPRIMITIVES_H

#ifdef _MSC_VER
  #pragma warning(disable:4786) // workaround for MSVC6 bug, needs service pack
#endif

#include <iostream>
#include <vector>

/**
* This class is a collection of OpenGL features including a CPU frustum culling
* that should be used before passing vertices to OpenGL.
*/
class GlPrimitives
{
  public:

    GlPrimitives ();
    ~GlPrimitives ();

    /// clear all buffers
    void clearBuffers () const;
    /// swap buffers to display the next buffer's content
    void swapBuffers () const;
    /// rotate using glRotatef for the 3 axes (1,0,0),(0,1,0),(0,0,1)
    void rotate (float x, float y, float z);
    /// call display list => draw content immediately
    void callList (int list) const;
    /// create a new display list and return the list number (identifier)
    int createList ();
    /// bind texture and enable linear shading between texels (slow)
    void enableLinearTexture (int texnum, bool mipmap) const;
    /// bind texture and enable const shading between texels (fast)
    void disableLinearTexture (int texnum, bool mipmap) const;
    /// test if the texture already exists, in that case return the OpenGL texture id,
    /// otherwise create an OpenGL texture and return the id
    int registerTexture (const std::string &name, const unsigned char *data,
                         int width, int height, bool mipmap);
    /// enable standard antialiasing on points, lines, etc.
    void enableAntiAliasing () const;
    void disableAntiAliasing () const;
    /// enable standard alpha blending
    void enableAlphaBlending () const;
    void disableAlphaBlending () const;
    /// enable alpha testing, which means texels less than "intensity" are NOT rendered
    void enableAlphaTesting (float intensity) const;
    void disableAlphaTesting () const;
    /// bind texture and enable texture mapping
    void enableTexture (int num) const;
    void enableLighting () const;
    void disableLighting () const;
    /// enable standard z-buffer operation
    void enableZBuffer () const;
    void disableZBuffer () const;
    void shadeFlat () const;
    void shadeSmooth () const;
    void enableFog (float view, bool fast = true) const;
    void setFogColor (int r, int g, int b);
    void setFogLuminance (float lum);

  private:

    std::vector<std::string> texList; ///< texture names
    int numLists;                     ///< display list counter
    unsigned char fogColor [3];       ///< the fog color in RGB (each 1 byte)
    float fogLuminance;               ///< the fog luminance in [0;1], multiplied with fogColor
};

/**
* This class is used to perform a pre-frustum culling on CPU which is much faster than OpenGL
* hardware implementations, as OpenGL performs this step at the end of the rendering pipeline.
*/
class Frustum
{
  public:

    /// extract the six frustum planes from Modelview and Projection matrices
    void extractFrustum ();
    /// test if point is in visible frustum
    bool isPointInFrustum (float x, float y, float z);
    /// test if sphere is in visible frustum
    bool isSphereInFrustum (float x, float y, float z, float radius);
    /// test if cube is in visible frustum (slower than sphere)
    bool isCubeInFrustum (float x, float y, float z, float size);

  private:

    float frustum [6] [4];     ///< the six frustum (cone) planes
};

extern GlPrimitives gl;
extern Frustum frustum;



#define VERTEXARRAY_V3N3C4T2 1
#define VERTEXARRAY_V3C4T2 2
#define VERTEXARRAY_V3N3T2 3
#define VERTEXARRAY_V3N3C4 4
#define VERTEXARRAY_V3C4 5

/**
* VertexArray class to emulate OpenGL primitives and pass 3D data in blocks to the hardware!
* This is much faster!
* Use one VertexArray for each #define VERTEXARRAY prototype and each primitive's type
* (GL_QUADS, GL_TRIANGLES).
*
* Usage:
*   VertexArray quads;
*   quads.glBegin (GL_QUADS);
*   quads.glVertex3f (...);
*   ... ;
*   quads.glEnd ();
*
* quads.glEnd() passes the data block to OpenGL, so make sure to call quads.glEnd()
* ONLY AFTER painting ALL your quads!
* That means, you can add vertices using quads.glVertex() at ANY time WITHOUT
* glBegin()/glEnd() calls!
* For your other primitives, use different arrays:
* - VertexArray quadstrip;
* - VertexArray triangles;
*/
class VertexArray
{
  public:

    /// construct standard vertexarray using VERTEXARRAY_V3C4T2
    VertexArray ();
    /// construct vertexarray with a custom prototype (see VERTEXARRAY_...)
    VertexArray (int pref);
    ~VertexArray ();

    void setPref (int pref);
    void _glBegin (int type);
    void _glVertex3f (float x, float y, float z);
    void _glVertex3fv (float *f);
    void _glNormal3f (float x, float y, float z);
    void _glColor4ub (int r, int g, int b, int a);
    void _glColor3ub (int r, int g, int b);
    void _glColor4f (float r, float g, float b, float a);
    void _glColor3f (float r, float g, float b);
    void _glColor4fv (float *f);
    void _glColor3fv (float *f);
    void _glTexCoord2f (float x, float y);
    void _glTexCoord2fv (float *f);
    void _glTexCoord2d (float x, float y);
    // ... add more primitives here ...
    void reallocData ();
    void _glEnd ();

  protected:

    /// primitive type in glBegin: GL_QUAD, GL_TRIANGLE...
    int type;
    /// number of vertices in this array
    int n;
    /// allocated number of vertices in this array
    int max;
    /// data block to pass to OpenGL (always float data: position, color, texture coords)
    float *data;
    /// size of data for one vertex in floats (V3N3C4T2 makes up 12 floats)
    int stride;
    /// vertex array type as given by the #define VERTEXARRAY prototypes, ONE class instance can only be of ONE type
    int pref;
};

#endif
