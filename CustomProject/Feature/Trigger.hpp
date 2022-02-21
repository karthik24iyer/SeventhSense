#pragma once

#include "../Util/FeatureUtil.hpp"
#include "../Util/FireUtil.hpp"
//#include "Stable.hpp"

extern bool toggleFriendlyFire;

class Trigger {
	
	private:
		FeatureUtil util;
		FireUtil fireUtil;

	public:

		void callTrigger(uintptr_t localPlayer) {
			uintptr_t enemyOnCH = util.entityOnCrosshair(localPlayer);
			//uintptr_t enemyOnCH = mem.RPM<uintptr_t>(moduleBase + dwEntityList + ((util.getLocalCrossID(localPlayer) - 1) * 0x10));
			if(!util.isValidEntity(enemyOnCH)) return;
			boolean isFriendlyFire = toggleFriendlyFire && (util.getEntityTeam(localPlayer) == util.getEntityTeam(enemyOnCH));
			if ((util.getEntityTeam(localPlayer) != util.getEntityTeam(enemyOnCH)) || (isFriendlyFire)) {
				forceAttack(localPlayer, enemyOnCH);
			}
		}

		void forceAttack(uintptr_t localPlayer, uintptr_t enemyOnCH) {
			srand((unsigned int)time(0));
			int delay = 100 + rand() % 100 + 1;

			fireUtil.fireWeapon(localPlayer, enemyOnCH, delay);
		}	
};