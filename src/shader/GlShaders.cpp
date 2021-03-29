/*  -*- mode:c++; c-basic-offset:2; -*-
    GL-117
    Copyright 2001, 2004 Thomas A. Drexl aka heptargon
    This file is copyright 2004 Jean-Marc Le Peuvedic

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

#include "shader/GlShaders.h"

#include <stdexcept>
#include <list>
#include <cassert>
#include <cstring>
#define GL_GLEXT_PROTOTYPES
//#include <GL/glcorearb.h>

#include "configuration/Configuration.h"
#include "configuration/Directory.h"
#include "game/Game.h"
#include "loadmodel/LoadObj.h"

using std::list;

// display list names (same as in glland.cpp, alternate implementation)
#define DISPLIST_BASE (65536)
static const GLuint c_treelist0 = DISPLIST_BASE+20;
static const GLuint c_treelist1 = DISPLIST_BASE+21;
static const GLuint c_treelist2 = DISPLIST_BASE+22;
static const GLuint c_treelist3 = DISPLIST_BASE+23;
static const GLuint c_treelist4 = DISPLIST_BASE+24;
static const GLuint c_treelist5 = DISPLIST_BASE+25;
static const GLuint c_treelist6 = DISPLIST_BASE+26;
static const GLuint c_treelist7 = DISPLIST_BASE+27;

static const GLuint c_treelist01 = DISPLIST_BASE+0;
static const GLuint c_treelist02 = DISPLIST_BASE+2;
static const GLuint c_treelist03 = DISPLIST_BASE+4;
static const GLuint c_treelist04 = DISPLIST_BASE+6;
static const GLuint c_treelist05 = DISPLIST_BASE+8;
static const GLuint c_treelist07 = DISPLIST_BASE+10;
static const GLuint c_treelist21 = DISPLIST_BASE+12;
static const GLuint c_treelist31 = DISPLIST_BASE+14;
static const GLuint c_treelist32 = DISPLIST_BASE+16;
static const GLuint c_treelist34 = DISPLIST_BASE+18;

// identification numbers of lq display lists must be hq+1
static const GLuint c_treelist01l = DISPLIST_BASE+1;
static const GLuint c_treelist02l = DISPLIST_BASE+3;
static const GLuint c_treelist03l = DISPLIST_BASE+5;
static const GLuint c_treelist04l = DISPLIST_BASE+7;
static const GLuint c_treelist05l = DISPLIST_BASE+9;
static const GLuint c_treelist07l = DISPLIST_BASE+11;
static const GLuint c_treelist21l = DISPLIST_BASE+13;
static const GLuint c_treelist31l = DISPLIST_BASE+15;
static const GLuint c_treelist32l = DISPLIST_BASE+17;
static const GLuint c_treelist34l = DISPLIST_BASE+19;

// All list numbers between DISPLIST_BASE and
// DISPLIST_BASE+MAX_NB_TREELISTS are reserved
#define MAX_NB_TREELISTS (100)

//#define STATS 1  // display a histogram

#include "gllandscape/GlLandscape.h"  // access to textures & lsticker & getExactLSHeight
#include "logging/Logging.h"

class QuadList;

class TreeShader : public GlShaders {
private:
  GlLandscape& landscape;
  GLuint treeprog;

  GLuint c_posoffsetxy_param; // constant
  GLuint c_vxtex_param;	// constant
  GLuint c_cst_param;	// constant
  GLuint rotphi_param;	// per frame
  GLuint fog_param;	// per frame
  GLuint variation_param;	// per batch

  float fadedist;		// max dist at which solid trees are drawn
  float maxdist;		// max dist at which trees are drawn
  float maxdep;			// same as maxdist, squared

  float xytree [512];		// sequence x,y,x,y, ...
  int   treelist;		// determines current treeshape

  GLuint displist[16][16];	// display lists for all tree quads

  void renderTrees(unsigned char kind, const QuadList& qlist, bool linear);

public:
  TreeShader(GlLandscape& _land);
  virtual ~TreeShader();
  // phi in radians
  virtual void drawTrees(int ax, int ex, int ay, int ey, int step);
  virtual void loadFrameUniformParams(int phi);
};

//----------------------------------------------------------------------
// Factory function implementation
#include <cstdio>		// fprintf stderr
#include <cstdlib>		// abort()
#include "configuration/Configuration.h"  // access to weather & timestep & quality & display()

// global
static unsigned  g_shaders_count   = 0;
#ifdef STATS
static unsigned  g_statistics[18]; // 10 bins representing # vertices/primitive
#endif

static GLint gl_major;
static GLint gl_minor;

GlShaders *
createShaders(GlLandscape& _land)
{
  glGetIntegerv(GL_MAJOR_VERSION, &gl_major);
  glGetIntegerv(GL_MINOR_VERSION, &gl_minor);
  g_shaders_count ++;
  TreeShader * shader = new TreeShader(_land);
  DISPLAY_INFO("Tree shader ready");
  return shader;
}

void
destroyShaders(GlShaders * _shader)
{
  delete _shader;
  g_shaders_count --;
}

//----------------------------------------------------------------------
// TreeShader implementation
#include "util/Math.h" // access to myrandom()
#include "opengl/GlPrimitives.h"	     // Open GL

static const float hh = (float) 1 / (float) MAXX;
static const float hh2 = 2.0*hh;
static const float zoomz = 1.0/(100.0*MAXX);
static const float zoomz2 = 32768.0 * zoomz;

static GLuint makeShader(GLenum type, const char *src)
{
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);
  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE)
  {
    GLsizei length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    GLchar *log;
    if (length > 0)
    {
      log = (GLchar *) malloc(length);
      if (log)
        glGetShaderInfoLog(shader, length, NULL, log);
    }
    else
    {
      log = strdup("Failed");
    }
    if (log)
    {
      fprintf(stderr, "glCompileShader: %s\n", log);
      free(log);
    }
  }
  return shader;
}

static bool linkAndValidate(GLint program)
{
  GLint status;

  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE)
  {
    GLsizei length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    GLchar *log = (GLchar *) malloc(length);
    if (log)
    {
      glGetProgramInfoLog(program, length, NULL, log);
      fprintf(stderr, "glLinkProgram: %s\n", log);
      free(log);
    }
    return false;
  }
  glValidateProgram(program);
  glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
  if (status == GL_FALSE)
  {
    GLsizei length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    GLchar *log = (GLchar *) malloc(length);
    if (log)
    {
      glGetProgramInfoLog(program, length, NULL, log);
      fprintf(stderr, "glValidateProgram: %s\n", log);
      free(log);
    }
    return false;
  }
  return true;
}

TreeShader::TreeShader(GlLandscape& _land)
  : landscape(_land)
{
  if (gl_major < 2) {
    treeprog = 0;
    return;
  }
  treeprog = glCreateProgram();

  TextFileObj file(Directory::getShaders("tree.vert").c_str());
  GLuint vert = makeShader(GL_VERTEX_SHADER, file.data);
  glAttachShader(treeprog, vert);
  linkAndValidate(treeprog);
  glDeleteShader(vert);
  glUseProgram(treeprog);

  c_posoffsetxy_param = glGetUniformLocation(treeprog, "c_posoffsetxy");
  c_vxtex_param       = glGetUniformLocation(treeprog, "c_vxtex");
  c_cst_param         = glGetUniformLocation(treeprog, "c_constants");
  rotphi_param        = glGetUniformLocation(treeprog, "RotPhi");
  fog_param           = glGetUniformLocation(treeprog, "Fog");
  variation_param     = glGetUniformLocation(treeprog, "Variation");

  // Load random position offset table. Trees in the same quad use
  // controlled random consecutive positions from this table.
  int i = 0;
  int i2;
  while(i < 512)
  {
    bool again = false;

    xytree [ i ] = 0.001 * Math::random (940) ;// - 0.47;
    xytree [i+1] = 0.001 * Math::random (940) ;// - 0.47;
    for(i2 = i-2; i2>=0 && i2 >= i-12; i2-=2) {
      // reject positions to close to each other
      if( fabs(xytree[i]-xytree[i2]) + fabs(xytree[i+1]-xytree[i2+1]) < 0.08) {
        again = true;
        break;
      }
    }
    if(!again) i ++;
  }
  glUniform4fv(c_posoffsetxy_param, 67, xytree);

  static const float vxtexquads[8][4] = {
    { 1.0, 1.0, 0.0, 1.0},
    {-1.0, 1.0, 0.5, 1.0},
    {-1.0, 0.0, 0.5, 0.0},
    { 1.0, 0.0, 0.0, 0.0},
    { 1.4, 0.0, 0.5, 1.0},
    { 0.0,-1.4, 1.0, 1.0},
    {-1.4, 0.0, 1.0, 0.0},
    { 0.0, 1.4, 0.5, 0.0}
  };
  glUniform4fv(c_vxtex_param, 8, &vxtexquads[0][0]);

  glUniform4f(c_cst_param, hh2, zoomz, zoomz2, 0.0f);

  // define display lists
  for(i=0; i<16; i++) {		// first tree
    for(i2=0; i<16; i++) {	// number of trees
      displist[i][i2] = 0;	// means: don't draw
      if(i+i2 >= 8) continue;
      displist[i][i2] = 1;	// means: draw each tree
    }
  }
  displist[0][1] = c_treelist01;
  displist[0][2] = c_treelist02;
  displist[0][3] = c_treelist03;
  displist[0][4] = c_treelist04;
  displist[0][5] = c_treelist05;
  displist[0][7] = c_treelist07;
  displist[2][1] = c_treelist21;
  displist[3][1] = c_treelist31;
  displist[3][2] = c_treelist32;
  displist[3][4] = c_treelist34;

  // vertex number, directly interpreted by vertex program,
  // and relative position of horizontal quad.
  static const float vertices[16] = {
    // vertical eye facing area drawn in plane XY
    0.0f, 0.0f,
    1.0f, 0.0f,
    2.0f, 0.0f,
    3.0f, 0.0f,
    // second horizontal quad is more efficient.
    4.0f, 0.4f,
    5.0f, 0.4f,
    6.0f, 0.4f,
    7.0f, 0.4f
  };
  float quaddatahq[8*7][3];
  for(i=0; i<7; i++) {
    for(i2=0; i2<8; i2++) {
      unsigned ix1 = i*8+i2;
      unsigned ix2 = i2*2;
      quaddatahq[ix1][0] = vertices[ix2];
      quaddatahq[ix1][1] = vertices[ix2+1];
      quaddatahq[ix1][2] = i;
    }
  }
  glEnable(GL_VERTEX_ARRAY);

#define MAKE_LIST(name , start, count)		\
  glNewList(name, GL_COMPILE);			\
  glDrawArrays(GL_QUADS, start, count);		\
  glEndList();

  glVertexPointer(3, GL_FLOAT, 0, quaddatahq);
  // hq trees (whole quad)
  MAKE_LIST(c_treelist01,  0,  8);
  MAKE_LIST(c_treelist02,  0, 16);
  MAKE_LIST(c_treelist03,  0, 24);
  MAKE_LIST(c_treelist04,  0, 32);
  MAKE_LIST(c_treelist05,  0, 40);
  MAKE_LIST(c_treelist07,  0, 48);
  MAKE_LIST(c_treelist21, 16,  8);
  MAKE_LIST(c_treelist31, 24,  8);
  MAKE_LIST(c_treelist32, 24, 16);
  MAKE_LIST(c_treelist34, 24, 32);
  // far-away trees one by one (hq)
  MAKE_LIST(c_treelist0,  0, 8);
  MAKE_LIST(c_treelist1,  8, 8);
  MAKE_LIST(c_treelist2, 16, 8);
  MAKE_LIST(c_treelist3, 24, 8);
  MAKE_LIST(c_treelist4, 32, 8);
  MAKE_LIST(c_treelist5, 40, 8);
  MAKE_LIST(c_treelist6, 48, 8);
  MAKE_LIST(c_treelist7, 56, 8);

  float quaddatalq[4*7][3];
  for(i=0; i<7; i++) {
    for(i2=0; i2<4; i2++) {
      unsigned ix1 = i*4+i2;
      unsigned ix2 = i2*2;
      quaddatalq[ix1][0] = vertices[ix2];
      quaddatalq[ix1][1] = vertices[ix2+1];
      quaddatalq[ix1][2] = i;
    }
  }
  glVertexPointer(3, GL_FLOAT, 0, quaddatalq);

  // hq trees (whole quad)
  MAKE_LIST(c_treelist01l,  0,  4);
  MAKE_LIST(c_treelist02l,  0,  8);
  MAKE_LIST(c_treelist03l,  0, 12);
  MAKE_LIST(c_treelist04l,  0, 16);
  MAKE_LIST(c_treelist05l,  0, 20);
  MAKE_LIST(c_treelist07l,  0, 24);
  MAKE_LIST(c_treelist21l, 16,  4);
  MAKE_LIST(c_treelist31l, 24,  4);
  MAKE_LIST(c_treelist32l, 24,  8);
  MAKE_LIST(c_treelist34l, 24, 16);

#undef MAKE_LIST

  glDisable(GL_VERTEX_ARRAY);
}

TreeShader::~TreeShader(void)
{
  if (!treeprog)
    return;
  glDeleteLists(DISPLIST_BASE, MAX_NB_TREELISTS);
  glDeleteProgram(treeprog);
}

static const unsigned int c_coniferous = 0; // textree2   / tree2n.tga
static const unsigned int c_deciduous  = 1; // textree    / tree1n.tga
static const unsigned int c_bush       = 2; // textree5   / tree5n.tga
static const unsigned int c_dwarfpine  = 3; // textree3   / tree3n.tga
static const unsigned int c_redtree    = 4; // textree4   / tree4n.tga
static const unsigned int c_cactus     = 5; // texcactus1 / cactus1.tga

static const unsigned int c_max_species= 6;

#define TREE_BATCH_SIZE (400)
// an array of such structures holds the lists of quads sorted by tree specie.
class QuadList {
public:
  struct qlelement {
    int xs;
    int ys;
    unsigned char first:4;	// 0-15
    unsigned char count:4;	// 0-15
    unsigned char hq;		// draw high quality trees
  };
private:
  qlelement quads[TREE_BATCH_SIZE];
  unsigned curfree;		// index of 1st free element in quads

public:
  void clear(void) {
    curfree = 0;
  }

  QuadList(void) {
    clear();
  };

  // subroutine inserts a quad in a list
  // returns 1 when list is full after insertion
  int insert(int xs, int ys, int first, int count, int hq)
    throw(std::overflow_error)
  {
    if(count > 0) {
      assert(first+count < 8);
      if(curfree >= TREE_BATCH_SIZE) {
       throw std::overflow_error("list is full");
      }
      // <atomic> thread safety non-issue
      unsigned cur = curfree;
      curfree++;
      // </atomic>
      quads[cur].xs    = xs;
      quads[cur].ys    = ys;
      quads[cur].first = first;
      quads[cur].count = count;
      quads[cur].hq    = hq;
    }
    return curfree >= TREE_BATCH_SIZE;
  };

  // The two functions below return quad data for a kind of tree
  const qlelement* read(unsigned index) const {
    if(index >= curfree) return 0; // end marker
    return &quads[index];
  };

};

#define GET_COORD(x) ((x)&(MAXX-1))
#define APPEAR_FRAMES (20)

void
TreeShader::renderTrees(unsigned char kind, const QuadList& qlist, bool linear)
{
  glUseProgram(treeprog);

  // select texture and size coefficients
  GLuint texture;
  float average;
  float variation;
  float widthcoef;
  if(kind == c_coniferous) {
    texture = textree2->textureID;
    average   = 0.0035;
    variation = 0.0002;
    widthcoef = 70.0;
  } else if(kind == c_deciduous) {
    texture = textree->textureID;
    average   = 0.0035;
    variation = 0.0003;
    widthcoef = 60.0;
  } else if(kind == c_bush) {
    texture = textree5->textureID;
    average   = 0.0025;
    variation = 0.00015;
    widthcoef = 70.0;
  } else if(kind == c_dwarfpine) {
    texture = textree3->textureID;
    average   = 0.0015;
    variation = 0.00015;
    widthcoef = 150.0;
  } else if (kind == c_redtree) {
    texture = textree4->textureID;
    average   = 0.0040;
    variation = 0.0;
    widthcoef = 0.35/average;
  } else {
    //c_cactus
    texture = texcactus1->textureID;
    average   = 0.0040;
    variation = 0.0;
    widthcoef = 0.3/average;
  }
  widthcoef *= hh2;
  // tree height coefficients
  glUniform4f(variation_param,
		   average, variation, widthcoef, 0.0f);
  // save part of rendering state
  glPushAttrib(GL_COLOR_BUFFER_BIT|GL_ENABLE_BIT);
  // set up rendering state
  //if(linear) {
  if(1) {
    // draw closeup trees with linear interpolation
    gl.enableLinearTexture (texture, true);
    glEnable(GL_ALPHA_TEST);
  } else {
    // draw distant trees with nearest interpolation
    gl.disableLinearTexture (texture, true);
    glDisable(GL_ALPHA_TEST);
  }
  gl.enableAlphaBlending();

  glBindTexture(GL_TEXTURE_2D, texture);

  // read quad data and start loop over quads
  const QuadList::qlelement* elt;
  unsigned j;

  for(elt = qlist.read(j=0); elt != 0; elt = qlist.read(++j)) {
    // draw trees of type 'kind' in quad designated by elt
    int xs = elt->xs;
    int ys = elt->ys;
    int x = GETCOORD(xs);
    int y = GETCOORD(ys);
    float shade = landscape.g[x][y] * 0.0008
      * (landscape.nl[x][y] + (short) landscape.dl[x][y]*16)
      * landscape.sunlight / 255.0f;
    float transparency = 1.0;	// opaque

    float tdx = cam.x - xs;
    float tdy = cam.z - ys;
    float dist = sqrt(tdx * tdx + tdy * tdy) - fadedist;
    if(dist>0.0f) {
      dist = 1.0f - dist/20.0f;
      if(dist<0.0f) dist = 0.0f;
      transparency = (-2*dist+3.0f)*dist*dist; //smoothstep
      // TODO: the GPU has a smoothstep function and could do that.
      // a pixel shader would be needed to properly account for the
      // changing transparency (what is done by glAlphaFunc below)
    }

    // choose display list
    treelist = displist[elt->first][elt->count];

    // send transparency, shade (lighting), quad position
    glTexCoord4f(transparency, shade, xs, ys);

    // set alpha threshold according to transparency
    glAlphaFunc(GL_GEQUAL, 0.5 * transparency);

    // vertical position interpolation coefficients
    unsigned step = landscape.neargridstep;
    int xp1 = GETCOORD(xs+step);
    int yp1 = GETCOORD(ys+step);
    float h00 = landscape.hw[ x ][ y ];
    float h01 = landscape.hw[ x ][yp1];
    float h10 = landscape.hw[xp1][ y ];
    float h11 = landscape.hw[xp1][yp1];
    glColor4f(h00, h01, h10, h11);

    // draw trees
    if(treelist >= DISPLIST_BASE) {
      if(!elt->hq) treelist++;	// lq display list id = hq id + 1
      glCallList(treelist);
    } else if(treelist == 1) {
      // draw low quality trees tree by tree
      int i = elt->first, end = i + elt->count;
      fprintf(stderr,"TreeShader: add display list for %d to %d\n", i, end-1);
      for(; i<end; i++) glCallList(c_treelist0+i);
    }
  }			// loop over quads

  // restore rendering state
  glPopAttrib();

  glUseProgram(0);
}

void
TreeShader::drawTrees(int ax, int ex, int ay, int ey, int step)
{
  bool islinear = false;
  // The same quad can appear in
  // more than one list if more than one specie grows on it. The
  // fields "first" and "count" are used to make sure trees growing on
  // the same quad grow at different spots. first+count must be < 8.
  // There is a qlist for trees rendered with linear texture
  // interpolation, and another one for trees rendered with nearest
  // texel interpolation.
  QuadList * qlist[c_max_species*2];
  unsigned i;
  for(i=0; i<c_max_species*2; i++) qlist[i] = new QuadList();

  int xs, ys;
  static const int cutdep = 800;

  // relimit area
  if(ax < cam.x-maxdist) ax = static_cast<int>(cam.x-maxdist);
  if(ex > cam.x+maxdist) ex = static_cast<int>(cam.x+maxdist);
  if(ay < cam.z-maxdist) ay = static_cast<int>(cam.z-maxdist);
  if(ey > cam.z+maxdist) ey = static_cast<int>(cam.z+maxdist);

  // classify and render in batches
  for(xs = ax; xs < ex; xs+=step) {
    float tdx = cam.x - xs;
    float tdx2 = tdx*tdx;
    if(tdx2 >= maxdep) continue;
    int x = GETCOORD(xs);
    for(ys = ay; ys <= ey; ys+=step) {
      int y = GETCOORD(ys);
      // distance test
      float tdy = cam.z - ys;
      float dep = tdx2 + tdy * tdy;
      if(dep >= maxdep) continue;

      islinear = (dep < 100.0f); // linear texture magnification for close trees
      unsigned char landtype = landscape.f[x][y];
      int hq = (dep < cutdep);

      if(!landscape.isWoods(landtype) &&
	 !landscape.isType(landtype,REDTREE0) &&
	 !landscape.isType(landtype, CACTUS0)) continue;

      // call this expensive function
      if(!frustum.isSphereInFrustum(hh2*xs, landscape.h[x][y]*zoomz - zoomz2,
				hh2*ys, hh2*2)) continue;
      // insert xs, ys in appropriate list(s)
      if(landtype >= CONIFEROUSWOODS0 && landtype <= BUSHES3) {
	// for CONIFEROUSWOODS, DECIDUOUSWOODS, MIXEDWOODS,
	// DWARFPINEWOODS & BUSHES generates 1 to 4 trees in low quality
	int trees = 4 - ((landtype - CONIFEROUSWOODS0) % 4);
	// and 1, 3, 5 or 7 trees in hq.
	if(quality >= 2) trees += (trees - 1);

	// overflow exception in insert shall not happen

	if(landtype <= CONIFEROUSWOODS3) {
	  unsigned lix = c_coniferous + (islinear ? 0 : c_max_species);
	  if(qlist[lix]->insert(xs, ys, 0, trees, hq)) {
	    renderTrees(c_coniferous, *qlist[lix], islinear);
	    qlist[lix]->clear();
	  }
	} else if(landtype <= DECIDUOUSWOODS3) {
	  unsigned lix = c_deciduous + (islinear ? 0 : c_max_species);
	  if(qlist[lix]->insert(xs, ys, 0, trees, hq)) {
	    renderTrees(c_deciduous, *qlist[lix], islinear);
	    qlist[lix]->clear();
	  }
	}
	else if(landtype <= MIXEDWOODS3) {
	  unsigned lix = c_coniferous + (islinear ? 0 : c_max_species);
	  if(qlist[lix]->insert(xs, ys, 0, 2, hq)) {
	    renderTrees(c_coniferous, *qlist[lix], islinear);
	    qlist[lix]->clear();
	  }
	  lix = c_bush + (islinear ? 0 : c_max_species);
	  if(qlist[lix]->insert(xs, ys, 2, 1, hq)) {
	    renderTrees(c_bush, *qlist[lix], islinear);
	    qlist[lix]->clear();
	  }
	  lix = c_deciduous + (islinear ? 0 : c_max_species);
	  if(qlist[lix]->insert(xs, ys, 3, trees-3, hq)) {
	    renderTrees(c_deciduous, *qlist[lix], islinear);
	    qlist[lix]->clear();
	  }

	} else if (landtype <= DWARFPINES3) {
	  unsigned lix = c_dwarfpine + (islinear ? 0 : c_max_species);
	  if(qlist[lix]->insert(xs, ys, 0, trees, hq)) {
	    renderTrees(c_dwarfpine, *qlist[lix], islinear);
	    qlist[lix]->clear();
	  }
	} else {		// BUSHES
	  unsigned lix = c_bush + (islinear ? 0 : c_max_species);
	  if(qlist[lix]->insert(xs, ys, 0, trees, hq)) {
	    renderTrees(c_bush, *qlist[lix], islinear);
	    qlist[lix]->clear();
	  }
	}

      } else if(landtype == REDTREE0) {
	unsigned lix = c_redtree + (islinear ? 0 : c_max_species);
	if(qlist[lix]->insert(xs, ys, 0, 1, hq)) {
	  renderTrees(c_redtree, *qlist[lix], islinear);
	  qlist[lix]->clear();
	}

      } else if(landtype == CACTUS0) {
	unsigned lix = c_cactus + (islinear ? 0 : c_max_species);
	if(qlist[lix]->insert(xs, ys, 0, 1, hq)) {
	  renderTrees(c_cactus, *qlist[lix], islinear);
	  qlist[lix]->clear();
	}

      }
    } // for(ys)
  } // for(xs)

  // Outer loop over tree species.
  // The first c_max_species qlists contain linearly interpolated trees
  for(i=0; i<c_max_species; i++) {
    renderTrees(i, *qlist[i], true);
    delete qlist[i];
  }
  for(i=0; i<c_max_species; i++) {
    renderTrees(i, *qlist[i+c_max_species], false);
    delete qlist[i+c_max_species];
  }
}

void
TreeShader::loadFrameUniformParams(int phi)
{
  // set quality
  int treequal = quality;
  if(quality > 5) treequal = 5;
  if(quality < 0) treequal = 0;

  // 0 : no trees
  // 1 : fades from 30 to 50
  // 2 : fades from 35 to 55
  // 3 : fades from 40 to 60
  // 4 : fades from 45 to 65
  // 5+: fades from 50 to 70
  maxdist = 5*treequal+45;
  maxdep = maxdist * maxdist;
  fadedist = maxdist - 20.0f;

  // update phi
  // Cg shader uses a small 2x3 matrix (only xz).
  GLfloat rotationphi[9] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
  float sine, cosine, phif = phi * PI / 180.0;
  sine =   sinf(phif);
  cosine = cosf(phif);
  rotationphi[0] = cosine;
  rotationphi[2] = sine;
  rotationphi[6] = -sine;
  rotationphi[8] = cosine;
  glUniformMatrix3fv(rotphi_param, 1, GL_FALSE, rotationphi);

  static unsigned char frame=0;
  if(frame == 0) {
#if 0
    // display modelview matrix
    GLfloat mvm[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, mvm);
    fprintf(stderr,"Modelview Matrix\n"
	    "%-5.1f %-5.1f %-5.1f %-5.1f\n"
	    "%-5.1f %-5.1f %-5.1f %-5.1f\n"
	    "%-5.1f %-5.1f %-5.1f %-5.1f\n"
	    "%-5.1f %-5.1f %-5.1f %-5.1f\n\n",
	    mvm[0],mvm[4],mvm[8], mvm[12],mvm[1],mvm[5],mvm[9], mvm[13],
	    mvm[2],mvm[6],mvm[10],mvm[14],mvm[3],mvm[7],mvm[11],mvm[15]);
#endif
#ifdef STATS
    // display statistics histogram
    int i, sum=0, nb=0;

    for(i=0; i<sizeof(g_statistics)/sizeof(g_statistics[0]); i++) {
      fprintf(stderr,"%3u ", g_statistics[i]);
      sum += g_statistics[i]*i;
      nb  += g_statistics[i];
      g_statistics[i] = 0U;
    }
    fprintf(stderr,"/%3.0f\n",sum/(float)nb);

#endif
  }
  frame++;
  if(frame == 30) frame = 0;
  // update fog parameters
  struct Fog {
    GLfloat fogColor[4];
    GLfloat fogEnd;
  };

  static unsigned char msgcount = 0;
  if(glIsEnabled(GL_FOG)) {
    Fog   fog;
    GLint fogMode;

    glGetFloatv(GL_FOG_COLOR, fog.fogColor);
    glGetIntegerv(GL_FOG_MODE, &fogMode);
    switch(fogMode) {
    case GL_LINEAR:
      glGetFloatv(GL_FOG_END,   &fog.fogEnd);
      break;
    case GL_EXP:
    case GL_EXP2:
    default:
      // The shader only does GL_LINEAR, compatible with code in gl.cpp.
      if(msgcount==0)
	fprintf(stderr,"Tree Shader Warning: FOG_MODE is not GL_LINEAR.\n");
      msgcount++;
      break;
    }
    // update fog in shader
    glUniform4f(fog_param,
		     fog.fogColor[0],
		     fog.fogColor[1],
		     fog.fogColor[2],
		     fog.fogEnd);
  } else {
    // The shader cannot comply if fog is disabled.
    if(msgcount==0)
      fprintf(stderr,"Tree Shader Warning: fog disabled.\n");
    msgcount++;
  }
}
