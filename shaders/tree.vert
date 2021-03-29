/* -*- mode: c;   -*- ********************************************************
Path:  shaders
File:  tree.cg

GL-117

    Copyright 2004 Jean-Marc Le Peuvedic

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

******************************************************************************/

#version 110

//----------------------------------------------------------------------
// Define inputs from application
// The application draws all the trees in a quad in a single batch.
// A single display list draws a tree.
// A single GL_QUADS primitive draws a group of trees.

// Per vertex varying information :
// Position can *ONLY* be used for constant information (in display lists)
// Position[0]  vertex index from 0 to 7, 0 to 3 being the vertical quad
// Position[1]  vertical position of horizontal quad for hq trees
#define  INDEX gl_Vertex.x
#define  HQUAD gl_Vertex.y
#define  TREE  gl_Vertex.z

//----------------------------------------------------------------------
// Per terrain quad varying information

// Random is taken from glTexCoord4f
// Random[0]  transparency
// Random[1]  shade (lighting)
// Random[2]  quad scaled x position
// Random[3]  quad scaled y position
#define TRANSP gl_MultiTexCoord0.x
#define SHADE  gl_MultiTexCoord0.y
#define QUADXY gl_MultiTexCoord0.zw
#define QUADX  gl_MultiTexCoord0.z
#define QUADY  gl_MultiTexCoord0.w

// Data is taken from glColor4f
// Slope[0]   elevation of corner (0,0) of quad
// Slope[1]   elevation of corner (0,1) of quad
// Slope[2]   elevation of corner (1,0) of quad
// Slope[3]   elevation of corner (1,1) of quad
#define SLOPE00_01 gl_Color.xy
#define SLOPE10_11 gl_Color.zw

//----------------------------------------------------------------------
// Per batch (same kind of tree) uniform information
// variation[0]  average tree height
// variation[1]  tree height variation unit
// variation[2]  tree width to height ratio
#define AVG_HT    variation.x
#define HT_STEP   variation.y
#define WHRATIO   variation.z

//----------------------------------------------------------------------
// Per frame uniform information
// RotPhi matrix mat3
// Fog data :
//   Draws fog as in GL mode GL_LINEAR, with START=1.
//   END and COLOR are variable, but change rarely.
#define FOGEND   Fog.w
#define FOGCOLOR Fog.xyz

//----------------------------------------------------------------------
// Compile time constants :

// posoffsetxy  array of offsets from quad position (implicitly stored in MVP)
// vxtex        array of position and texture coordinates of vertices
// hh2          a constant which might change, scenery horizontal scaling
#define C_HH2    c_constants.x
#define C_ZOOMZ  c_constants.y
#define C_ZOOMZ2 c_constants.z

float
modulo(float x, float y)
{
  float r = mod(x,y);
  return (x>=0.0) ? r : y + r;
}

uniform vec4   variation;
uniform vec4   c_posoffsetxy[67];
uniform vec4   c_vxtex[8];
uniform vec4   c_constants;
uniform mat3   RotPhi;
uniform vec4   Fog;

void main(void)
{
  int  index = int(INDEX);
  gl_TexCoord[0]  = vec4(c_vxtex[index].zw, vec2(0.0,1.0));

  // compute pseudo random tree position
  float rand = modulo(QUADX + 2.0 * QUADY, 128.0);
  if(rand < 0.0) rand += 128.0;
  float rt = (rand + TREE)/2.0;
  int rotval = int(floor(rt));
  float fraction = fract(rt);
  vec4 packedoffsets = c_posoffsetxy[rotval];
  vec2 offsets = ((fraction<0.5) ? packedoffsets.xy : packedoffsets.zw);

  // Based on offsets in current quad, interpolate terrain elevation
  vec2 vpos2 = mix(SLOPE00_01, SLOPE10_11, offsets.x);
  float vpos = mix(vpos2.x, vpos2.y, offsets.y) * C_ZOOMZ - C_ZOOMZ2;

  // depending on whether the quad drawn is vertical or horizontal,
  // the packed vertex coordinates are read into xy or xz.
  vec3 tmppos2 = vec3( c_vxtex[index].xy, HQUAD );
  vec3 tmppos3 = ((index >= 4) ? tmppos2.xzy : tmppos2.xyz);

  // Compute eyepos, later used for fog (color) computations
  vec4 eyepos = gl_ModelViewMatrix * vec4(QUADX*C_HH2, vpos, QUADY*C_HH2, 1.0);
  // Find the projected position of tree on the horizon
  vec2 sincosh = normalize(vec2(gl_ModelViewMatrix[1][1],-gl_ModelViewMatrix[0][1]));
  float proj = dot(eyepos.xy,sincosh);
  // Find the extra angle to rotate the tree to make it face the eye
  // Trees do not all just face direction indicated by phi : they are
  // arranged in an arc in front of the eye.
  vec2 sincosv = normalize(vec2(proj,eyepos.z-0.5));
  mat2 rotAlpha = mat2(-sincosv.yx, sincosv.x, -sincosv.y);
  tmppos3.xz  = rotAlpha * tmppos3.xz;

  // Rotate vertices of the model around the vertical axis going
  // through the origin of the model. Scale
  tmppos3.xz  = (RotPhi * tmppos3).xz;

  // Compute height and width
  float htrand = (3.0*QUADX + 5.0*QUADY);
  float height = HT_STEP*modulo(htrand - TREE, 8.0)+AVG_HT;
  float width  = height * WHRATIO;
  // Compute storm induced slant
  // depends on varying position and uniform time
  float windsl = 0.0; // TODO : weather routine

  // Apply storm effect (westerly wind), which is just a slant
  tmppos3.x += tmppos3.y*windsl;

  //tmppos3.xz *= width;
  tmppos3.xz *= width;

  // move tree to current quad and position
  tmppos3.xz += (offsets + QUADXY) * C_HH2;

  // Scale height and adjust position with respect to terrain
  //tmppos3.y = tmppos3.y * height + vpos;
  tmppos3.y = tmppos3.y * height + vpos;

  // transform vertex position into homogenous clip-space
  gl_Position = gl_ModelViewProjectionMatrix * vec4(tmppos3, 1.0);

  // compute fog (from Z distance in eye coordinates)
  // fog coordinate is computed and output in case a pixel shader 
  // implements per-pixel fog later. The fixed pipeline pixel shader 
  // does not do per-pixel fog.
  float end = FOGEND;
  float f = (end - length(eyepos))/(end - 1.0);
  f = clamp(f, 0.0, 1.0);
  // gl_FogCoord = f;		// not used by fixed function pixel shader
  // We want to make trees lose contrast in the fog. Per-pixel fog
  // would do it because it would be applied after the textures.
  // But we do the same per-vertex.
  // We are going to scale down Color according to fog
  vec4 color     = vec4(f*SHADE, f*SHADE, f*SHADE, TRANSP);
  // Color gets modulated by the texture map.
  // Then we add an untextured base fog color, scaled by 1-f
  color  += vec4((1.0-f)*FOGCOLOR, 0.0);

  // various optional colorized diagnostics
  //color.r=0.5+proj/2.0;
  //color.r=0.5+(eyepos.y*-gl_ModelViewMatrix[0][1])/1024.0;
  //color.r+=(eyepos.x* gl_ModelViewMatrix[1][1])/1024.0;
  //if(dot(eyepos.xy,vec2(gl_ModelViewMatrix[1][1],-gl_ModelViewMatrix[0][1])) > 0)
    //color.r = 1.0;
  //else
    //color.b = 1.0;
  //if (gl_Color.r == 0.0)
    //color.r = rand;
  //color     = vec4(offset.x,offset.y,0.0,1.0);
  //color     = vec4(rTRANSP/32.0, 0.0, TRANSP/32.0, 1.0);
  gl_FrontColor = gl_BackColor = color;
}

//----------------------------------------------------------------------
// enhancement ideas :
//
// sinphi, cosphi are constants : precompute on CPU. The best is to
// preload a mat2x2 2D rotation matrix. All program instructions
// must involve a varying parameter, or depend on one.
// 7 insns to save.
//
// tree slant for bad weather can be computed here from a time
// information.
//
// instead of implementing per vertex lighting on the CPU, add an array
// of normal vectors and compute vertex lighting here!
