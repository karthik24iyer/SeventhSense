#pragma once

#include <Windows.h>

extern bool toggleMain;
extern bool toggleVisible;
extern bool toggleRadar;
extern bool toggleFlash;
extern bool toggleChams;
extern bool resetChams;
extern bool toggleTrigger;
extern bool toggleDelay;
extern bool toggleNoRecoil;
extern bool toggleAim;
extern bool toggleFriendlyFire;

const int MAX_AIM_SMOOTH = 30;
const int MIN_AIM_SMOOTH = 10;
const int MAX_AIM_FOV = 28;
const int MIN_AIM_FOV = 20;
const int DEF_AIM_SMOOTH = 10;
const int DEF_AIM_FOV = 24;
const int STEP_AIM_SMOOTH = 5;
const int STEP_AIM_FOV = 2;


int aimSmooth = DEF_AIM_SMOOTH;
int aimFov = DEF_AIM_FOV;
bool aimHold = 1;

extern HANDLE hConsole;

class Toggles {

public:
	bool isToggle = 0;
	bool aim0DisplayOnce = 1;
	bool aim1DisplayOnce = 1;

	void showOTM() {
		char k;
		cin >> k;
		if (k != '0') exit(0);
	}

	void fetchInput() {
		if (GetAsyncKeyState(VK_HOME) & 1) {
			toggleMain = !toggleMain;
			showConsoleMsg();
		}

		else if (GetAsyncKeyState(VK_NUMLOCK) & 1) {
			toggleVisible = !toggleVisible;
			showConsoleMsg();
		}
		//std::cout << "\nstatus: "<< isToggle;

		else if (GetAsyncKeyState(VK_F3) & 1) {
			toggleRadar = !toggleRadar;
			showConsoleMsg();
		}

		else if (GetAsyncKeyState(VK_CAPITAL) & 1) {
			//cout << "\ntoggle";
			toggleTrigger = !toggleTrigger;
			showConsoleMsg();
			/*if (toggleDelay && toggleTrigger) {
				pressKey(VK_NUMLOCK);
			}*/

		}

		/*else if (GetAsyncKeyState(VK_F2) & 1) {
			toggleDelay = !toggleDelay;
			toggleNoRecoil = !toggleNoRecoil;
			showConsoleMsg();
			if (toggleTrigger && toggleDelay) {
				pressKey(VK_CAPITAL);
			}
		}

		else if (GetAsyncKeyState(VK_F6) & 1) {
			toggleFlash = !toggleFlash;
			showConsoleMsg();
		}

		/*else if (GetAsyncKeyState(VK_SCROLL) & 1) {
			toggleChams = !toggleChams;
			if (!toggleChams) resetChams = true;
			showConsoleMsg();
		}*/

		else if (GetAsyncKeyState(VK_RSHIFT) & 1) {
			aimSmooth = DEF_AIM_SMOOTH;
			aimFov = DEF_AIM_FOV;
			showConsoleMsg();
		}
		else if ((GetAsyncKeyState(VK_UP) & 1)) {
			if (aimFov < MAX_AIM_FOV) {
				aimFov += STEP_AIM_FOV;
				showConsoleMsg();
			}
		}

		else if ((GetAsyncKeyState(VK_DOWN) & 1)) {
			if (aimFov > MIN_AIM_FOV) {
				aimFov -= STEP_AIM_FOV;
				showConsoleMsg();
			}
		}

		else if ((GetAsyncKeyState(VK_RIGHT) & 1)) {
			if (aimSmooth < MAX_AIM_SMOOTH) {
				aimSmooth += STEP_AIM_SMOOTH;
				showConsoleMsg();
			}
		}

		else if ((GetAsyncKeyState(VK_LEFT) & 1)) {
			if (aimSmooth > MIN_AIM_SMOOTH) {
				aimSmooth -= STEP_AIM_SMOOTH;
				showConsoleMsg();
			}
		}

		else if (GetAsyncKeyState(VK_F9) & 1) {
			toggleFriendlyFire = !toggleFriendlyFire;
			showConsoleMsg();
		}

		if (GetAsyncKeyState(VK_F1)) {
			toggleAim = 1;
			//std::cout << "\ntoggleAimIf=" << toggleAim;
			if (aim1DisplayOnce) {
				aimHold = 1;
				aim1DisplayOnce = 0;
				aim0DisplayOnce = 1;
				showConsoleMsg();
			}
		}
		else {
			toggleAim = 0;
			//std::cout << "\ntoggleAimElse=" << toggleAim;
			if (aim0DisplayOnce) {
				aim0DisplayOnce = 0;
				aim1DisplayOnce = 1;
				showConsoleMsg();
			}
		}

	}

	int getFovAimParam() {
		return aimFov;
	}

	int getSmoothAimParam() {
		return aimSmooth;
	}

	void showConsoleMsg() {
		system("cls");
		SetConsoleTextAttribute(hConsole, 7);
		std::cout << "\n\tHello World :P ... Working fine as of Blast World Final 14th Dec 21 \n";
		if (toggleMain) {
			SetConsoleTextAttribute(hConsole, 10);
		}
		else {
			SetConsoleTextAttribute(hConsole, 12);
		}
		std::cout << "\n\n\t\t\tMain: " << toggleMain << " (1)";

		SetConsoleTextAttribute(hConsole, 7);
		printLine();
		printLine();

		if (toggleVisible) {
			SetConsoleTextAttribute(hConsole, 10);
		}
		else {
			SetConsoleTextAttribute(hConsole, 12);
		}
		std::cout << "\n\n\tVis: " << toggleVisible << " (0)";

		//SetConsoleTextAttribute(hConsole, 7);
		//printLine();

		if (toggleRadar) {
			SetConsoleTextAttribute(hConsole, 10);
		}
		else {
			SetConsoleTextAttribute(hConsole, 12);
		}
		std::cout << "\tRdr: " << toggleRadar << " (1)";

		/*if (toggleFlash) {
			SetConsoleTextAttribute(hConsole, 10);
		}
		else {
			SetConsoleTextAttribute(hConsole, 12);
		}
		std::cout << "\tFlh: " << toggleFlash << " (0)";

		SetConsoleTextAttribute(hConsole, 7);
		printLine();

		if (toggleChams) {
			SetConsoleTextAttribute(hConsole, 10);
		}
		else {
			SetConsoleTextAttribute(hConsole, 12);
		}
		std::cout << "\n\n\tCham: " << toggleChams << " (0)";
		*/
		SetConsoleTextAttribute(hConsole, 7);
		printLine();

		if (toggleTrigger) {
			SetConsoleTextAttribute(hConsole, 10);
		}
		else {
			SetConsoleTextAttribute(hConsole, 12);
		}
		std::cout << "\n\n\tTrgr: " << toggleTrigger << " (0)";

		/*if (toggleDelay) {
			SetConsoleTextAttribute(hConsole, 10);
		}
		else {
			SetConsoleTextAttribute(hConsole, 12);
		}
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "\tDl/Nr: " << toggleDelay << " (0)";*/

		SetConsoleTextAttribute(hConsole, 7);
		printLine();

		if (toggleAim) {
			SetConsoleTextAttribute(hConsole, 10);
		}
		else {
			SetConsoleTextAttribute(hConsole, 12);
		}
		std::cout << "\n\n\tAm: " << toggleAim << " (0)";

		setAimBars();
		/*
		if (toggleNoRecoil) {
			SetConsoleTextAttribute(hConsole, 10);
		}
		else {
			SetConsoleTextAttribute(hConsole, 12);
		}
		std::cout << "\t\tnrc: " << toggleNoRecoil << " (0)";
		*/
		SetConsoleTextAttribute(hConsole, 7);
		printLine();

		if (toggleFriendlyFire) {
			SetConsoleTextAttribute(hConsole, 10);
		}
		else {
			SetConsoleTextAttribute(hConsole, 12);
		}
		std::cout << "\n\n\tFrfr: " << toggleFriendlyFire << " (0)";

		SetConsoleTextAttribute(hConsole, 7);
		printLine();
		std::cout << "\n\n\n";
	}

	void setAimBars() {
		int bar = 0, activeBar = 0;
		SetConsoleTextAttribute(hConsole, 3);
		std::cout << "\tSmth: ";
		bar = (MAX_AIM_SMOOTH - MIN_AIM_SMOOTH) / STEP_AIM_SMOOTH;
		activeBar = (int)(1 + (aimSmooth - MIN_AIM_SMOOTH) / STEP_AIM_SMOOTH);
		SetConsoleTextAttribute(hConsole, 6);
		for (int i = 0;i < activeBar;i++) {
			std::cout << "|";
		}
		SetConsoleTextAttribute(hConsole, 7);
		for (int i = 0;i <= bar - activeBar;i++) {
			std::cout << "|";
		}

		SetConsoleTextAttribute(hConsole, 3);
		std::cout << "\tFov: ";
		bar = (MAX_AIM_FOV - MIN_AIM_FOV) / STEP_AIM_FOV;
		activeBar = (1 + (aimFov - MIN_AIM_FOV) / STEP_AIM_FOV);
		SetConsoleTextAttribute(hConsole, 5);
		for (int i = 0;i < activeBar;i++) {
			std::cout << "|";
		}
		SetConsoleTextAttribute(hConsole, 7);
		for (int i = 0;i <= bar - activeBar;i++) {
			std::cout << "|";
		}
	}

	void setToggles() {
		toggleMain = true;
		toggleVisible = false;
		toggleRadar = true;
		toggleFlash = false;
		toggleChams = false;
		toggleTrigger = false;
		toggleDelay = false;
		toggleNoRecoil = false;
		toggleAim = false;
		toggleFriendlyFire = false;

		if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0) {
			pressKey(VK_CAPITAL);
		}
		if ((GetKeyState(VK_NUMLOCK) & 0x0001) != 0) {
			pressKey(VK_NUMLOCK);
		}
	}

	void printLine() {
		std::cout << "\n\t---------------------------------------------";
	}

	void clearConsoleInputBuffer()
	{
		/* keybd_event('S', 0, 0, 0);
		keybd_event('S', 0,KEYEVENTF_KEYUP, 0);
		keybd_event(VK_BACK, 0, 0, 0);
		keybd_event(VK_BACK, 0,KEYEVENTF_KEYUP, 0); */
		PINPUT_RECORD ClearingVar1 = new INPUT_RECORD[256];
		DWORD ClearingVar2;
		ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), ClearingVar1, 256, &ClearingVar2);
		delete[] ClearingVar1;
	}

	void pressKey(WORD key) {
		INPUT input;
		WORD vkey = key;
		input.type = INPUT_KEYBOARD;
		input.ki.wScan = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
		input.ki.time = 0;
		input.ki.dwExtraInfo = 0;
		input.ki.wVk = vkey;
		input.ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN
		SendInput(1, &input, sizeof(INPUT));

		input.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &input, sizeof(INPUT));
	}

	bool isMyHwId() {

		const char* hid = "{ced62aa7-f371-11ea-b873-806e6f6e6963}";
		HW_PROFILE_INFO hwProfileInfo;
		if (GetCurrentHwProfile(&hwProfileInfo)) {
			//printf("HWID: %s\n", hwProfileInfo.szHwProfileGuid);
			const char* newHid = hwProfileInfo.szHwProfileGuid;
			if (strcmp(hid, newHid) == 0) {
				return 1;
			}
		}
		return 0;
	}
};