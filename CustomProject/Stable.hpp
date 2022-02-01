#pragma once

class Stable {
	private:
		FeatureUtil util;
		MyVector vec;
		/*boolean isSingleFWeapon = false;
		boolean isSemiFWeapon = false;
		boolean isGay = false;
		boolean isDgle = false;*/

	public:
		void noRecoil(uintptr_t localPlayer) {
			//DWORD activeWeapon = util.getActiveWeapon(util.getLocalPlayer());
			//long activeWeaponId = util.getActiveWeaponId(activeWeapon);
			//bool isAllowed = false;
			//not allowing burst fire for now
			/*if (util.getIsSingleFWeapon(activeWeaponId)
				|| util.getIsSemiFWeapon(activeWeaponId)
				|| util.getIsGay(activeWeaponId)
				|| util.getIsDgle(activeWeaponId)) {
				isAllowed = true;
			}*/
			if (util.getIsBurstFWeapon(util.getActiveWeaponId(util.getActiveWeapon(localPlayer)))) return;

			//if (isAllowed) {
			static D3DXVECTOR3 OldVectorPunch;
			//std::cout << "\nShotsFired()= "<<util.getShotsFired();
			if (util.getShotsFired() > 1) {
				Sleep(10);
				D3DXVECTOR3 MyViewAngles = util.getLocalViewAngles();
				D3DXVECTOR3 ViewAngles;
				D3DXVECTOR3 LocalPlayerPunch = util.getLocalPunchAngles();
				ViewAngles.x = MyViewAngles.x + OldVectorPunch.x;
				ViewAngles.y = MyViewAngles.y + OldVectorPunch.y;

				D3DXVECTOR3 NewAngles;
				NewAngles.x = ViewAngles.x - LocalPlayerPunch.x * 2;
				NewAngles.y = ViewAngles.y - LocalPlayerPunch.y * 2;
				NewAngles.z = 0.0f;
				util.setLocalViewAngles(vec.ClampAngles(NewAngles));

				OldVectorPunch.x = LocalPlayerPunch.x * 2;
				OldVectorPunch.y = LocalPlayerPunch.y * 2;
			}
			else
			{
				OldVectorPunch.x = 0.0f;
				OldVectorPunch.y = 0.0f;
				OldVectorPunch.z = 0.0f;
			}
			}

		//}
};