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

#include <vector>
#include <stdexcept>

#include "Render.h"

typedef std::vector<Renderer*>::iterator iterator;

static Renderer * g_no_render = 0;	///< default renderer
static std::vector<Renderer*> * g_render_register = 0; ///< renderers register

/// Since we don't know the order in which global constructors will be
/// called, we need this function to set up the register when it is
/// first needed.
void
Renderer::checkInitGlobal(void)
{
  if(g_render_register == 0)
    g_render_register = new std::vector<Renderer*>;
  if(g_no_render == 0)
    g_no_render = new Renderer();
}

unsigned
Renderer::count(void)
{
  checkInitGlobal();

  iterator i   = g_render_register->begin();
  iterator end = g_render_register->end();
  unsigned cnt = 0;

  for(; i<end; i++) {
    const Renderer* r = *i;
    if(r->isAvailable()) cnt++;
  }
  return cnt;
}

Renderer&
Renderer::get(unsigned _index)
{
  checkInitGlobal();

  iterator i   = g_render_register->begin();
  iterator end = g_render_register->end();
  unsigned cnt = 0;

  for(; i<end; i++) {
    Renderer* r = *i;
    if(r->isAvailable() && cnt++ == _index) return *r;
  }
  throw std::invalid_argument("_index out of bounds");

  return *g_no_render;
}

Renderer&
Renderer::get(void)
{
  checkInitGlobal();

  iterator i   = g_render_register->begin();
  iterator end = g_render_register->end();
  float best = 0.0f;
  Renderer * bestr = 0;			// the best one found

  for(; i<end; i++) {
    Renderer* r = *i;
    if(r->isAvailable() && r->performance() >= best) {
      best  = r->performance();
      bestr = r;
    }
  }
  if(bestr == 0) return *g_no_render;
  return *bestr;
}

Renderer::Renderer(void)
{
  if(g_render_register == 0)
    g_render_register = new std::vector<Renderer*>;
  g_render_register->push_back(this);
}

Renderer::~Renderer(void)
{
  iterator i   = g_render_register->begin();
  iterator end = g_render_register->end();
  for(; i<end; i++) {
    Renderer* r = *i;
    if(r == this) {
      g_render_register->erase(i);
      break;
    }
  }
}
