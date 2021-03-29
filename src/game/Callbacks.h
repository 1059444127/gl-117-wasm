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

#ifndef IS_CALLBACKS_H
#define IS_CALLBACKS_H

#include "configuration/Configuration.h"
#include "menu/Component.h"

extern void callbackReturn (Component *comp, int key);
extern void callbackStats (Component *comp, int key);
extern void callbackQuitNow (Component *comp, int key);
extern void callbackSwitchMainMenu (Component *comp, int key);
extern void callbackQuit (Component *comp, int key);
extern void callbackCredits (Component *comp, int key);
extern void callbackPilots (Component *comp, int key);

extern void callbackJoystickAxis (Component *comp, int key);
extern void callbackSwitchStartMission (Component *comp, int key);
extern void callbackFighterPrev (Component *comp, int key);
extern void callbackFighterNext (Component *comp, int key);
extern void callbackFighter (Component *comp, int key);
extern void setpilotstext ();
extern void callbackPilotsAdd (Component *comp, int key);
extern void callbackPilotsDelete (Component *comp, int key);
extern void callbackPilotsList (Component *comp, int key);
extern void callbackJoystick (Component *comp, int key);
extern void callbackMouseDefaults (Component *comp, int key);
extern void callbackMouseAutorudder (Component *comp, int key);
extern void callbackMouseRelative (Component *comp, int key);
extern void callbackMouseReverse (Component *comp, int key);
extern void callbackMouseSensitivity (Component *comp, int key);
extern void callbackDefaultsJoystick (Component *comp, int key);
extern void callbackDefaults (Component *comp, int key);
extern void callbackKeyboard (Component *comp, int key);
extern void textControls (char *buf);
extern void callbackControls (Component *comp, int key);
extern void callbackPhysics (Component *comp, int key);
extern void callbackDifficulty (Component *comp, int key);
extern void callbackSound (Component *comp, int key);
extern void callbackMusic (Component *comp, int key);
extern void callbackBrightness (Component *comp, int key);
extern void callbackResolution (Component *comp, int key);
extern void callbackFullscreen (Component *comp, int key);
extern void callbackDynamicLighting (Component *comp, int key);
extern void callbackSpecialEffects (Component *comp, int key);
extern void callbackAntialiasing (Component *comp, int key);
extern void callbackDithering (Component *comp, int key);
extern void callbackView (Component *comp, int key);
extern void callbackQuality (Component *comp, int key);
extern void callbackTraining (Component *comp, int key);
extern void callbackCampaign (Component *comp, int key);
extern void callbackMainMenu (Component *comp, int key);

#endif

