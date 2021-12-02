#pragma once

#include "Toggles.hpp"

extern bool aimHold;

class Thatsit {
	private:
		FeatureUtil util;
		int aimBone = 8;
		// Head - 8, Neck - 7, Chest - 6
		int aimBoneList[6] = {8, 8, 8, 7, 7, 6};
		// default values -
		float aimSmooth;  // old = 40
		float aimFov; // old = 4
		Toggles menu;
	
	public:

		void aimBot(uintptr_t localPlayer, uintptr_t entity[], int currentPlayers)
		{
			aimBone = aimBoneList[rand() % 6];
			//std::cout << "\n aimBone" << aimBone;
			if (!aimHold) {
				return;
			}
			DWORD activeWeapon = util.getActiveWeapon(localPlayer);
			long activeWeaponId = util.getActiveWeaponId(activeWeapon);

			boolean isAllowed = false;
			/*if (util.getIsSingleFWeapon(activeWeaponId)
				|| util.getIsBurstFWeapon(activeWeaponId)
				|| util.getIsSemiFWeapon(activeWeaponId)
				|| util.getIsDgle(activeWeaponId)
				|| util.getIsSpam(activeWeaponId)) {
				isAllowed = true;
			}*/
			if (!util.getIsSniper(activeWeaponId) && !util.getIsGay(activeWeaponId)) {
				isAllowed = true;
			}
			if (isAllowed) {
				aimSmooth = (float)(menu.getSmoothAimParam());
				aimFov = (float)(menu.getFovAimParam()/10);
				aimHold = util.aimAtPlayer(util.getClosestTarget(aimFov, entity, currentPlayers, aimBone), aimSmooth, aimBone);
			}			
		}
};