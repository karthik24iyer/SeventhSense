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
#include "MyOffsets.hpp"
#include "Polymorph.hpp"
#include "MemoryRW.hpp"
#include "FeatureUtil.hpp"
#include "Trigger.hpp"
#include "Visible.hpp"
#include "Cham.hpp"
#include "Thatsit.hpp"
#include "Toggles.hpp"
#include "Patterns.hpp"
#include "stdafx.hpp"
#include "PatternScan.hpp"
#include "SigData.hpp"

bool toggleMain;
bool toggleVisible;
bool toggleRadar;
bool toggleFlash;
bool toggleChams;
bool resetChams;
bool toggleTrigger;
bool toggleDelay;
bool toggleNoRecoil;
bool toggleAim;
bool toggleFriendlyFire;


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
Visible vis;
Cham cham;
Stable nr;
Thatsit aim;
Toggles menu;
SigData sig;

using namespace std;
