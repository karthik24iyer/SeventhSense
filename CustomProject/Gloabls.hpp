#pragma once

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <ctime>
#include <conio.h>
#include <vector>
#include <stdlib.h>
#include <map>
#include <string>
#include <filesystem>
#include <d3dx9math.h>
#include "Offset/MyOffsets.hpp"
#include "Memory/Polymorph.hpp"
#include "Memory/MemoryRW.hpp"
#include "Util/FeatureUtil.hpp"
#include "Feature/Trigger.hpp"
#include "Feature/Aim.hpp"
#include "Feature/Radar.hpp"
#include "Feature/Visible.hpp"
//#include "Feature/Cham.hpp"
#include "Toggles.hpp"
#include "Offset/Patterns.hpp"
//#include "stdafx.hpp"
#include "Offset/PatternScan.hpp"
#include "Offset/SigData.hpp"
#include "Util/CoOrd.hpp"

bool hideMenu;
bool toggleMain;
bool toggleVisible;
bool toggleRadar;
//bool toggleFlash;
//bool toggleChams;
//bool resetChams;
bool toggleTrigger;
//bool toggleDelay;
//bool toggleNoRecoil;
bool toggleAim;
bool toggleFriendlyFire;

//float cursorPosX;
//float cursorPosY;

uintptr_t moduleBase;
uintptr_t engineBase;
DWORD procId;
HWND hwnd;
uintptr_t engineaddr;
HANDLE hConsole;
MODULEENTRY32 client;
MODULEENTRY32 engine;

Polymorph pm;
MemoryRW mem;
FeatureUtil util;
Trigger trgr;
Aim aim;
Radar rdr;
Visible vis;
//Cham cham;
//Stable nr;
Toggles menu;
SigData sig;

using namespace std;
