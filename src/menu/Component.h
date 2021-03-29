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

#ifndef IS_COMPONENT_H
#define IS_COMPONENT_H

#include "model3d/Model3d.h" // ok
#include "effects/Effects.h" // ok

extern Font *font1;

extern Color StandardTextColor;
extern Color DeactiveTextColor;
extern Color HiliteTextColor;

extern float view;

extern int menuId;
extern int menuZ;

/**
* This class represents an abstract menu component.
*/
class Component
{
  public:

    Color colback;
    int id;
    bool highlight;
    bool active;
    bool visible;
    Component *parent;

    Component ();
    virtual ~Component ();

    virtual void setActive (bool value);
    virtual void setVisible (bool value);
    virtual void setHighlight (bool value);
    bool isActive ();
    bool isVisible ();
    bool isHighlight ();
    void setBounds (float x, float y, float w, float h);
    void setFunction (void (*func) (Component *, int button));
    void execFunction (int button);
    virtual void draw ();
    virtual void eventMouse (int x, int y, int button);
    virtual void triggerMouse (int id, int button);
    virtual bool contains (int id);
    virtual void eventKey (unsigned char key);
    virtual void eventSpecial (int key);
    virtual void eventJoystick (int button);

  protected:

    float x, y, w, h;
    void (*func) (Component *, int button); ///< button=0...3 for mouse, 0...255 for one byte keys, 256... for special+256
};

/**
* This class represents an abstract container which may store up to 128 components.
*/
class Container : public Component
{
  public:

    int numcomponents;
    std::vector<Component *> components;

    Container ();
    virtual ~Container ();

    int getNumberOfComponents ();
    void add (Component *component);
    void draw ();
    void eventMouse (int x, int y, int button);
    void triggerMouse (int id, int button);
    void setVisible (bool value);
    void setActive (bool value);
    void setHighlight (bool value);
    bool contains (int id);
    void eventKey (unsigned char key);
    void eventSpecial (int key);
    void eventJoystick (int button);
};

/**
* This class is a one-line label.
*/
class Label : public Component
{
  public:

    std::string text;
    bool transparent; ///< no background

    Label ();
    Label (const std::string &text);

    void setTransparent (bool transparent);
    void setText (const std::string &text);
    void draw ();
    void triggerMouse (int id, int button);
};

/**
* This class is a one-line button.
*/
class Button : public Component
{
  public:

    std::string text;

    Button ();
    Button (const std::string &text);

    void draw ();
    void triggerMouse (int id, int button);
};

/**
* This class is a multi-line text field.
*/
class TextField : public Component
{
  public:

    std::string text;
    float fonth;

    TextField ();
    TextField (const std::string &text, float fontheight);

    void setText (const std::string &text);
    void draw ();
    void triggerMouse (int id, int button);
};

/**
* This class captures and displays a button on the keyboard.
*/
class EditKey : public Component
{
  public:

    std::string text;
    char key;
    int special;
    bool specialActive;    ///< key or special valid?

    EditKey ();
    EditKey (const std::string &text);

    void setText (const std::string &text);
    void setKey (int key); ///< please add 0xFF to two byte keys to distinguish from one byte keys
    int getKey ();         ///< returns special+256 for two byte keys ('+' is special=13, ENTER is key=13)
    void draw ();
    void triggerMouse (int id, int button);
    void eventKey (unsigned char key);
    void eventSpecial (int key);
};

/**
* This class captures and displays a button of the joystick.
*/
class EditJoystick : public Component
{
  public:

    std::string text;
    int button;

    EditJoystick ();
    EditJoystick (const std::string &text);

    void setText (const std::string &text);
    void setButton (int key); ///< button 0..99, hat 100..
    int getButton ();         ///< returns button 0..99, hat 100..
    void draw ();
    void triggerMouse (int id, int button);
    void eventJoystick (int button);
};

/**
* This class is a on-line edit field for keyboard text input.
*/
class EditField : public Component
{
  public:

    std::string text;
    int timer;
    int ptr;
    int maxlen;

    EditField ();
    EditField (int maxlen);

    void setText (const std::string &text);
    void draw ();
    void triggerMouse (int id, int button);
    void eventKey (unsigned char key);
};

#endif
