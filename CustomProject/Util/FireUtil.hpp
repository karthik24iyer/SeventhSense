#pragma once

#include "WeaponUtil.hpp"

class FireUtil {
	
	private:
		FeatureUtil util;
		WeaponUtil wUtil;
		int aimBoneList[6] = { 8, 8, 8, 7, 7, 6 };

	public:

		void fireWeapon(uintptr_t localPlayer, uintptr_t enemyOnCH, int delay) {
			int activeWeaponId = util.getActiveWeaponId(util.getActiveWeapon(localPlayer));

			if (wUtil.isKnife(activeWeaponId)) {
				return;
			}

			if (wUtil.getIsSniper(activeWeaponId)) {
				fireSniper(delay, localPlayer);
			}
			else if (wUtil.getIsDgle(activeWeaponId)) {
				fireDgle(enemyOnCH);
			}
			else if (wUtil.getIsGay(activeWeaponId)) {
				fireGay(delay, localPlayer);
			}
			else {
				fireGeneral();
			}
		}

		void fireSniper(int delay, uintptr_t localPlayer) {
			bool isScoped = util.isScoped(localPlayer);
			bool isMoving = util.isEntityMoving(localPlayer);
			if (isScoped && !isMoving) {
				Sleep(delay % 5);
				util.startFire();
				Sleep(10);
				util.stopFire();
			}

		}

		void fireDgle(uintptr_t enemyOnCH) {
			int clipCount = 1;
			while (clipCount > 0) {
				/*float delta = util.getEntityBoneDelta(util.getLocalEyePos(), util.getLocalViewAngles(),
					enemyOnCH, aimBoneList[rand() % 6]);
				//std::cout << "\n delta: " << delta;

				if (delta > 1.f) break;*/

				util.startFire();
				//std::cout << "\n shot ";
				clipCount--;
				Sleep(15);
			}
			util.stopFire();
		}

		void fireGay(int delay, uintptr_t localPlayer) {
			bool isScoped = util.isScoped(localPlayer);
			bool isMoving = util.isEntityMoving(localPlayer);
			if (isScoped && !isMoving) {
				int clipCount = 3;
				Sleep(delay % 7);
				while (clipCount > 0) {
					util.startFire();
					clipCount--;
					Sleep(10);
				}
				util.stopFire();
			}
		}

		void fireGeneral() {
			int clipCount = 5;
			while (clipCount > 0) {
				util.startFire();
				Sleep(5);
				util.stopFire();
				clipCount--;
			}
		}


};