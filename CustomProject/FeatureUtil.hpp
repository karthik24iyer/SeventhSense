#pragma once

#include "ClassId.hpp"

class FeatureUtil {
	
	public:
		boolean getIsSniper(int activeWeaponId) {
			return activeWeaponId == awp || activeWeaponId == scout
				|| activeWeaponId == awpDef || activeWeaponId == scoutDef;
		}

		boolean getIsBurstFWeapon(int activeWeaponId) {
			return activeWeaponId == ak47 || activeWeaponId == ak47Def || activeWeaponId == m4a4
				|| activeWeaponId == mp9 || activeWeaponId == mp5 || activeWeaponId == mp7
				|| activeWeaponId == ump || activeWeaponId == mac10
				|| activeWeaponId == mp9Def || activeWeaponId == umpDef || activeWeaponId == mac10Def
				|| activeWeaponId == dualB || activeWeaponId == glock
				|| activeWeaponId == m4a1s || activeWeaponId == m4a1sAlt;
		}

		boolean getIsSingleFWeapon(int activeWeaponId) {
			return activeWeaponId == usps || activeWeaponId == dgle || activeWeaponId == xm1014;
		}

		boolean getIsSemiFWeapon(int activeWeaponId) {
			return activeWeaponId == famas || activeWeaponId == gali
				|| activeWeaponId == aug || activeWeaponId == augDef || activeWeaponId == sg
				|| activeWeaponId == p250 || activeWeaponId == p250Def
				|| activeWeaponId == tec9 || activeWeaponId == five7 || activeWeaponId == cz || activeWeaponId == glock;
		}

		boolean getIsGay(int activeWeaponId) {
			return activeWeaponId == g3sg || activeWeaponId == scar;
		}

		boolean getIsSpam(int activeWeaponId) {
			return activeWeaponId == p90 || activeWeaponId == p90Def
				|| activeWeaponId == bison || activeWeaponId == negev;
		}

		/*
		Vec3 getBonePos(uint32_t entity, int boneID) {
			uint32_t boneMatrix = getPtrOffset(entity + m_dwBoneMatrix);
			BonePos bonepos = mem.RPM<BonePos>(boneMatrix + 0x30 * boneID + 0xC);
			return { bonepos.x, bonepos.y, bonepos.z };
		}

		Vec3 getOrigin(uintptr_t entity) {
			return mem.RPM<Vec3>(entity + m_vecOrigin);
		}

		char* getGameDirectory() {
			static array<char, 0x120> gameDirectory = mem.RPM<array<char, 0x120>>(engineaddr + dwGameDir);
			return gameDirectory.data();
		}

		char* getMapDirectory() {
			uintptr_t engineptr = mem.RPM<uintptr_t>(engineaddr + dwClientState);
			static array<char, 0x120> mapDirectory = mem.RPM<array<char, 0x120>>(engineptr + dwClientState_MapDirectory);
			return mapDirectory.data();
		}
		void loadMapRayCast() {
			if (bspParser->load_map(getGameDirectory(), getMapDirectory())) {
				//cout << "map rays loaded!\n";
			}
		}
		*/

};