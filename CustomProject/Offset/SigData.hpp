#pragma once

#include "Netvar.hpp"

using namespace std;
using namespace signatures;
using namespace offsets;

extern HANDLE hProcess;
//Pattern ptr;
//Offsets ofst;

class SigData{
	private: 
		PatternScan ptrScan;
	public:
		//void Scan(ProcEx proc)
		uintptr_t scan(string ptr, MODULEENTRY32 module, int offset, int extra, bool isRelative)
		{

			intptr_t result = (intptr_t)ptrScan.ScanMod((char*)ptr.c_str(), module);
			if (offset == -1) {
				result += 1;
				ReadProcessMemory(hProcess, (BYTE*)result, &result, sizeof(offset), NULL);
				offset++;
			}
			result = result + offset;
			ReadProcessMemory(hProcess, (BYTE*)result, &result, sizeof(offset), NULL);

			if (extra)
			{
				result = result + extra;
			}

			if (isRelative)
			{
				result = result - (uintptr_t)module.modBaseAddr;
			}
			//std::cout << "\n ptr: 0x"<< result;
			return result;
		}

		void ScanAll(MODULEENTRY32 client, MODULEENTRY32 engine) {

			/*map<string, Pattern> sig = ptr.setPatterns();
			map<string, uintptr_t> offset;
			
			map<string, Pattern>::iterator itr;
			for (itr = sig.begin(); itr != sig.end(); ++itr) {
				offset.insert(make_pair(itr->first, scan(itr->second.pattern, itr->second.mod,
					itr->second.offset, itr->second.extra, itr->second.isRelative)));
				
				//uintptr_t tmp = offset.find("dwClientState");
				std::cout << "\n\t" << itr->first<<": 0x"
					<< std::uppercase << std::hex << offset.find(itr->first)->second << '\n';
			}*/

			//---------------------------------------------------------------------------------------------
			//Signature = scan(pattern, dll, offset, extra, isRelative);
			dwGetAllClasses = scan(dwGetAllClasses_pattern, client, -1, 0, true);
			dwEntityList = scan(dwEntityList_pattern, client, 1, 0, true);
			dwLocalPlayer = scan(dwLocalPlayer_pattern, client, 3, 4, true);
			dwClientState = scan(dwClientState_pattern, engine, 1, 0, true);
			dwClientState_State = scan(dwClientState_State_pattern, engine, 2, 0, false);
			dwClientState_ViewAngles = scan(dwClientState_ViewAngles_pattern, engine, 4, 0, false);
			dwViewMatrix = scan(dwViewMatrix_pattern, client, 3, 176, true);
			m_bDormant = scan(m_bDormant_pattern, client, 2, 8, false);
			/*
			dwClientState_MapDirectory = scan(dwClientState_MapDirectory_pattern, engine, 7, 0, false);
			dwForceAttack = scan(dwForceAttack_pattern, client, 2, 0, true);
			dwGameDir = scan(dwGameDir_pattern, engine, 1, 0, true);
			dwGlowObjectManager = scan(dwGlowObjectManager_pattern, client, 1, 4, true);
			model_ambient_min = scan(model_ambient_min_pattern, engine, 4, 0, true);
			*/

			//Netvars-
			Netvar nt;
			m_vecOrigin = nt.getNetvar("m_vecOrigin", "DT_BasePlayer", 0);
			m_iTeamNum = nt.getNetvar("m_iTeamNum", "DT_BasePlayer", 0);
			m_iHealth = nt.getNetvar("m_iHealth", "DT_BasePlayer", 0);
			m_lifeState = nt.getNetvar("m_lifeState", "DT_CSPlayer", 0);
			
			m_iState = nt.getNetvar("m_iState", "DT_BaseCombatWeapon", 0);
			//m_bHasDefuser = nt.getNetvar("m_bHasDefuser", "DT_CSPlayer", 0);
			//m_bHasHelmet = nt.getNetvar("m_bHasHelmet", "DT_CSPlayer", 0);
			//m_bInReload = nt.getNetvar("m_flNextPrimaryAttack", "DT_BaseCombatWeapon", 109);
			m_bIsDefusing = nt.getNetvar("m_bIsDefusing", "DT_CSPlayer", 0);
			m_bIsScoped = nt.getNetvar("m_bIsScoped", "DT_CSPlayer", 0);
			//m_bSpotted = nt.getNetvar("m_bSpotted", "DT_BaseEntity", 0);
			//m_bSpottedByMask = nt.getNetvar("m_bSpottedByMask", "DT_BaseEntity", 0);
			
			m_dwBoneMatrix = nt.getNetvar("m_nForceBone", "DT_BaseAnimating", 28);
			m_iShotsFired = nt.getNetvar("m_iShotsFired", "DT_CSPlayer", 0);
			m_clrRender = nt.getNetvar("m_clrRender", "DT_BaseEntity", 0);
			m_aimPunchAngle = nt.getNetvar("m_aimPunchAngle", "DT_BasePlayer", 0);
			m_vecVelocity = nt.getNetvar("m_vecVelocity[0]", "DT_CSPlayer", 0);
			m_vecViewOffset = nt.getNetvar("m_vecViewOffset[0]", "DT_CSPlayer", 0);
			//m_viewPunchAngle = nt.getNetvar("m_viewPunchAngle", "DT_BasePlayer", 0);
			//m_flFlashDuration = nt.getNetvar("m_flFlashDuration", "DT_CSPlayer", 0);
			
			//m_flFlashMaxAlpha = nt.getNetvar("m_flFlashMaxAlpha", "DT_CSPlayer", 0);
			//m_iGlowIndex = nt.getNetvar("m_flFlashDuration", "DT_CSPlayer", 24);
			
			m_iItemDefinitionIndex = nt.getNetvar("m_iItemDefinitionIndex", "DT_BaseCombatWeapon", 0);
			m_iCrosshairId = nt.getNetvar("m_bHasDefuser", "DT_CSPlayer", 92);
			m_iClip1 = nt.getNetvar("m_iClip1", "DT_BaseCombatWeapon", 0);
			m_hActiveWeapon = nt.getNetvar("m_hActiveWeapon", "DT_BasePlayer", 0);
			m_szLastPlaceName = nt.getNetvar("m_szLastPlaceName", "DT_CSPlayer", 0);

			//std::cout << "\n\n";			
			//std::cout << "\n dwClientState: 0x" << std::uppercase << std::hex << dwClientState << "\n";
		}
};