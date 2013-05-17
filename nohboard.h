/********************************************************************************
 Copyright (C) 2012 Eric Bataille <e.c.p.bataille@gmail.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
********************************************************************************/


#include "resource.h"
#include "nbtools.h"
#include "d3dstuff.h"
#include "kbparser.h"
#include "configparser.h"
#include "llist.h"
#include <time.h>
#include <string>
// Version 0xMMmmbb (Major.minor.build)
#define version 0x000401
#define version_string L"NohBoard v0.5b1"
#define method 1
#define keyboardVersion 2
#define configfile L"NohBoard.config"

// I changed from not sizable to sizable windows and now I need to add some magic numbers to the
// widths and heights of the windows because for some reason the current numbers aren't good enough
// anymore, and I don't want to bother forcing everyone to update their keyboard files.
#define extraX 10
#define extraY 10

// Threading
bool bStopping = false;
bool bRestart = false;
bool bRender = false;
clock_t begin_time = 0;
CRITICAL_SECTION csKB;
D3DStuff *ds;

HHOOK keyboardHook = NULL;
HHOOK mouseHook = NULL;
HWND hWnd;
HINSTANCE hInstMain;

// List of key pressed statuses
lnode *fPressed = NULL;
bool pressed[256] = { false };
bool shiftDown1 = false;
bool shiftDown2 = false;
bool shiftDown() { return shiftDown1 || shiftDown2; }

// Settings window
COLORREF custColors[16];
std::wstring initialLayout;

// Configuration stuff
ConfigParser * config;
KBInfo *kbinfo;

// Window sizing
float aspect;
float lastw;
float lasth;

enum
{
    ID_LOADSETTINGS=5000,
    ID_EXITNOHBOARD,
    ID_RESETSIZE
};
