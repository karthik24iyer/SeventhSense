#include "ClassId.hpp"

class WeaponUtil {
	public:
		bool getIsSniper(int activeWeaponId) {
			return activeWeaponId == awp || activeWeaponId == scout
				|| activeWeaponId == awpDef || activeWeaponId == scoutDef;
		}

		bool getIsBurstFWeapon(int activeWeaponId) {
			return activeWeaponId == ak47 || activeWeaponId == ak47Def || activeWeaponId == m4a4
				|| activeWeaponId == mp9 || activeWeaponId == mp5 || activeWeaponId == mp7
				|| activeWeaponId == ump || activeWeaponId == mac10
				|| activeWeaponId == mp9Def || activeWeaponId == umpDef || activeWeaponId == mac10Def
				|| activeWeaponId == dualB || activeWeaponId == glock
				|| activeWeaponId == m4a1s || activeWeaponId == m4a1sAlt;
		}

		bool getIsSingleFWeapon(int activeWeaponId) {
			return activeWeaponId == usps || activeWeaponId == xm1014 || activeWeaponId == tec9Def
				|| activeWeaponId == tec9 || activeWeaponId == five7 || activeWeaponId == cz;
		}

		bool getIsDgle(int activeWeaponId) {
			return activeWeaponId == dgle || activeWeaponId == dgleDef;
		}

		bool getIsSemiFWeapon(int activeWeaponId) {
			return activeWeaponId == famas || activeWeaponId == gali
				|| activeWeaponId == aug || activeWeaponId == augDef || activeWeaponId == sg
				|| activeWeaponId == p250 || activeWeaponId == p250Def
				|| activeWeaponId == glock;
		}

		bool getIsGay(int activeWeaponId) {
			return activeWeaponId == g3sg || activeWeaponId == scar;
		}

		bool getIsSpam(int activeWeaponId) {
			return activeWeaponId == p90 || activeWeaponId == p90Def
				|| activeWeaponId == bison || activeWeaponId == negev;
		}

		bool isKnife(int activeWeaponId) {
			return activeWeaponId == knifeT || activeWeaponId == knifeCT
				|| activeWeaponId == myKnife;
		}
};