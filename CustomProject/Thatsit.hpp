#pragma once

#include "Toggles.hpp"

class Thatsit {
	private:
		FeatureUtil util;
		int aimBone = 8; // Head;
		// default values -
		float aimSmooth;  // old = 40
		float aimFov; // old = 4
		Toggles menu;
	
	public:

		void aimBot(uintptr_t localPlayer, uintptr_t entity[], int currentPlayers)
		{
			DWORD activeWeapon = util.getActiveWeapon(localPlayer);
			long activeWeaponId = util.getActiveWeaponId(activeWeapon);

			boolean isAllowed = false;
			if (util.getIsBurstFWeapon(activeWeaponId)
				|| util.getIsSingleFWeapon(activeWeaponId)
				|| util.getIsSemiFWeapon(activeWeaponId)
				|| util.getIsDgle(activeWeaponId)) {
				isAllowed = true;
			}
			if (isAllowed) {
				aimSmooth = (float)(menu.getSmoothAimParam());
				aimFov = (float)(menu.getFovAimParam()/10);
				util.aimAtPlayer(util.getClosestTarget(aimFov, entity, currentPlayers), aimSmooth, aimBone);
			}			
		}
};