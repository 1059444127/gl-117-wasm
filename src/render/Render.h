/* -*- mode: c++; c-basic-offset: 2; encoding: iso8859-15 -*-
    GL-117
    Copyright 2004-2004 Jean-Marc Le Peuvedic

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

#ifndef IS_RENDER_H
#define IS_RENDER_H

#include <string>

class Space;
class Landscape;
class Vector3;

/**
 * This interface exposes the API of an abstract renderer. The
 * capability of a renderer matches the internal game
 * representation stored in Landscape and Model objects. It can
 * however add effects, like water glittering, and does not
 * necessarily show all the information (optional smoke, sun flares,
 * blackout, redout, dynamic lighting, etc.).
 * The renderer shall cache geometry information internally for all
 * models. It can transform the geometry to have it at multiple
 * degrees of coarseness for LOD purposes. It can precalculate
 * information at that time too.
 * When renderering, the renderer interrogates models in order to
 * update their current position.
 */
class Renderer
{
public:
  /// Returns the number of available registered renderers
  static unsigned count(void);

  /// Returns an available renderer, maybe to offer a choice to the
  /// user. Argument varies between 0 and count()-1.
  static Renderer& get(unsigned);

  /// This static method obtains the best registered renderer
  /// available. Renderers can indicate to this function whether they
  /// are available or not. If more that one renderer is available,
  /// some performance indication can be used to help in selection.
  static Renderer& get(void);

private:
  /// private init function
  static void checkInitGlobal(void);

  // do not use this constructor
  Renderer(const Renderer&) {};

protected:
  /// Registers all renderers in a global list
  Renderer(void);

  /// Unregisters a renderer
  virtual ~Renderer();

  /// Get the performance indicator of a renderer
  virtual float performance(void) const {return 0.0f;};

  /// Determine availability of a renderer
  virtual bool isAvailable(void) const {return false;};



public:
  /// Initialize this renderer (precalculate things, cache geometry)
  virtual void init(const Space& _models, const Landscape& _landscape) {};

  /// Draw the scene
  virtual void draw(const Vector3 &cam, float phi, float gamma) const {};

  /// Renderer's name
  virtual std::string name(void) {
    return std::string("NO RENDER");
  };

};

#include "model3d/Model3d.h"
#include "Realizer.h"

class OpenGlRenderer : public Renderer
{
  public:

    OpenGlRenderer() : Renderer()
    {
    }

    virtual void init(const Space &space, const Landscape &landscape)
    {
    }

    virtual void draw(const Space &space, const Landscape &landscape) const
    {
      frustum.extractFrustum ();

      for (unsigned i = 0; i < space.o.size (); i ++)
      {
        SpaceObj *spaceobj = space.o [i];
        Model3d *model = spaceobj->o;
        if (model != NULL && spaceobj->draw)
        {
          Vector3 tl1(spaceobj->trafo.translation);
          if (frustum.isSphereInFrustum (tl1.x, tl1.y, tl1.z, space.o [i]->trafo.scaling.x * 1.5))
          {
            if (spaceobj->drawLight)
            {
              glEnable (GL_LIGHTING);
              Model3dRealizer mr;
              mr.draw (*model, spaceobj->trafo, 1.0, spaceobj->explode);
              if (!spaceobj->ref.empty ())
              {
                glPushMatrix ();
                glTranslatef (tl1.x, tl1.y - 0.001 * spaceobj->explode * spaceobj->explode / timestep / timestep, tl1.z);
                glRotatef (spaceobj->trafo.rotation.phi + 90, 0, -1, 0);
                glRotatef (-spaceobj->trafo.rotation.gamma + 90, 0, 0, 1);
                glRotatef (spaceobj->trafo.rotation.theta + 180, 1, 0, 0);
                glScalef (spaceobj->trafo.scaling.x, spaceobj->trafo.scaling.y, spaceobj->trafo.scaling.z);
                if (model->refpoint)
                  for (unsigned i2 = 0; i2 < spaceobj->ref.size (); i2 ++)
                    if (spaceobj->ref [i2].trafo.scaling.x > 0.001)
                    {
                      Vector3 tl(model->refpoint [i2 / 3]);
                      tl.add(spaceobj->ref [i2].trafo.translation);
                      mr.draw (*spaceobj->ref [i2].o, Transformation(tl, spaceobj->ref [i2].trafo.rotation, spaceobj->ref [i2].trafo.scaling), spaceobj->lum, spaceobj->explode);
//                      spaceobj->ref [i].o->draw (model->refpoint [i / 3], spaceobj->ref [i].trafo.translation, spaceobj->ref [i].trafo.rotation, ref [i].trafo.scaling.x, spaceobj->lum, spaceobj->explode);
                    }
                glPopMatrix ();
              }
            }
            else
            {
              glDisable (GL_LIGHTING);
              Model3dRealizer mr;
              mr.drawNoLight (*model, spaceobj->trafo, spaceobj->explode);
            }
          }
        }
      }
    }
};


#endif // IS_RENDER_H
