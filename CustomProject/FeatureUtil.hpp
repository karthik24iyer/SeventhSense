#pragma once

#include "ClassId.hpp"
#include "MyVector.hpp"
#include "StructUtil.hpp"
#include "Toggles.hpp"
//#include "BspParser.cpp"

extern MemoryRW mem;
extern uintptr_t moduleBase;
extern uintptr_t engineBase;
extern bool toggleFriendlyFire;
using namespace offsets;


typedef struct {
	float Matrix[3][4];
} Matrix3x4_t;


extern Toggles menu;

class FeatureUtil {
	private:
		MyVector vec;
		//inline static rn::bsp_parser* bspParser = new rn::bsp_parser;
	public:

		uintptr_t getModuleBase() {
			return moduleBase;
		}

		void setModuleBase(uintptr_t base) {
			moduleBase = base;
		}

		uintptr_t getEnginePointer() {
			return mem.RPM<uintptr_t>(engineBase + dwClientState);
		}

		bool isInGame() {
			return mem.RPM<int>(getEnginePointer() + dwClientState_State)==6?1:0;
		}

		uintptr_t getLocalPlayer() {
			return getPtrOffset(moduleBase + dwLocalPlayer);
		}

		uintptr_t getCurrentEntity(int i) {
			return getPtrOffset(moduleBase + dwEntityList + i * 0x10);
		}
		int getIntOffset(uintptr_t offset) {
			return mem.RPM<int>(offset);
		}

		uintptr_t getPtrOffset(uintptr_t offset) {
			return mem.RPM<uintptr_t>(offset);
		}

		BYTE getByteOffset(uintptr_t offset) {
			return mem.RPM<BYTE>(offset);
		}

		int isEntitySpotted(uintptr_t entity) {
			return getIntOffset(entity + m_bSpotted);
		}

		void setEntitySpotted(uintptr_t entity) {
			mem.WPM<bool>(entity + m_bSpotted, true);
		}

		int isScoped(uintptr_t entity) {
			return getIntOffset(entity + m_bIsScoped);
		}

		int getIsDormant(uintptr_t entity) {
			return getIntOffset(entity + m_bDormant);
		}

		int	getEntityTeam(uintptr_t entity)
		{
			return mem.RPM<int>(entity + m_iTeamNum);
		}

		int	getEntityHealth(uintptr_t entity)
		{
			return mem.RPM<int>(entity + m_iHealth);
		}

		int	isEntityAlive(uintptr_t entity)
		{
			return getEntityHealth(entity) > 0 ? true : false;
		}

		int getEntityLifeState(uintptr_t entity)
		{
			return mem.RPM<int>(entity + m_lifeState);
		}


		bool isValidEntity(uintptr_t entity)
		{
			if ((isEntityAlive(entity) && getEntityTeam(entity) != 0) && !getIsDormant(entity) && getEntityLifeState(entity) == 0)
				return true;
			return false;
		}

		bool isVisibleByDef(uintptr_t player1, uintptr_t player2) {
			int index = getIntOffset(player1 + 0x64);
			int byMask = getIntOffset(player2 + m_bSpottedByMask);
			return (bool)(byMask && (1 << (index - 1)));
		}

		int isEntityDefusing(uintptr_t entity) {
			return getIntOffset(entity + m_bIsDefusing);
		}

		int isEntityScoped(uintptr_t entity) {
			return getIntOffset(entity + m_bIsScoped);
		}

		boolean isEnityHoldingC4(uintptr_t entity) {
			if (getActiveWeaponId(getActiveWeapon(entity)) == c4) {
				return true;
			}
			return false;
		}

		boolean isEntityMoving(uintptr_t entity) {
			int* velocity = mem.RPM<int*>(entity + m_vecVelocity);
			if (velocity == NULL) {
				//std::cout << "\n is still";
				//std::cout << "\nmoving with vel="<<velocity;
				return false;
			}
			else {
				return true;
			}
		}

		int getShotsFired() {
			return mem.RPM<int>(getLocalPlayer() + m_iShotsFired);
		}

		int getLocalCrossID(uintptr_t localPlayer)
		{
			return mem.RPM<int>(localPlayer + m_iCrosshairId);
		}

		DWORD getActiveWeapon(uintptr_t entity)
		{
			DWORD weaponIndex = mem.RPM<DWORD>(entity + m_hActiveWeapon) & 0xFFF;
			return mem.RPM<DWORD>((moduleBase + dwEntityList + weaponIndex * 0x10) - 0x10);
		}

		long getActiveWeaponId(DWORD activeWeapon) {
			//std::cout<<"\weaponId="<< mem.RPM<DWORD>(activeWeapon + m_iItemDefinitionIndex);
			return mem.RPM<DWORD>(activeWeapon + m_iItemDefinitionIndex);
		}

		int getClipsInActiveWeapon(uintptr_t entity) {
			DWORD activeWeapon = getActiveWeapon(entity);
			//std::cout<<"\nclips= "<< mem.RPM<DWORD>(activeWeapon + m_iClip1);
			return mem.RPM<DWORD>(activeWeapon + m_iClip1);
		}

		D3DXVECTOR3 getLocalViewAngles()
		{
			return mem.RPM<D3DXVECTOR3>(getEnginePointer() + dwClientState_ViewAngles);
		}

		void setLocalViewAngles(D3DXVECTOR3 angles)
		{
			mem.WPM<D3DXVECTOR3>(getEnginePointer() + dwClientState_ViewAngles, angles);
		}

		D3DXVECTOR3 getLocalPunchAngles()
		{
			return mem.RPM<D3DXVECTOR3>(getLocalPlayer() + m_aimPunchAngle);
		}

		bool aimAtPlayer(uintptr_t localPlayer, DWORD player, float smooth, int bone)
		{
			bool isFriendlyEnabled = toggleFriendlyFire && getEntityTeam(player) == getEntityTeam(getLocalPlayer());
			if (player != NULL && (isEntitySpotted(player)|| isFriendlyEnabled))
			{
				D3DXVECTOR3 aimAngles = getEntBonePos(player, bone) - getLocalEyePos();
				vec.VectorAngles(aimAngles, aimAngles);
				D3DXVECTOR3 delta = aimAngles - getLocalViewAngles();
				float relDist = vec.getVecMagnitude(delta);
				relDist > 1.f ? 1.f : relDist;
				smooth < 1 ? 1 : smooth;
				if (relDist < 0.01f) {
					int activeWeaponId = getActiveWeaponId(getActiveWeapon(localPlayer));
					if (getIsDgle(activeWeaponId)) {
						startFire();
						Sleep(10);
						stopFire();
					}
					return 0;
				}
				//std::cout << "\nmag: " << relDist;
				D3DXVECTOR3 sAngles = vec.ClampAngles(getLocalViewAngles() + (delta / (smooth*(1+ relDist))));
				//std::cout << "\nangle: " << vec.getVecMagnitude(sAngles);
				setLocalViewAngles(sAngles);
				//return 1;
			}
			return 1;
		}

		DWORD getClosestTarget(float fov, uintptr_t entityList[], int maxPlayers, int aimBone)
		{
			D3DXVECTOR3 viewAngles = getLocalViewAngles();
			D3DXVECTOR3 localEyePos = getLocalEyePos();

			DWORD bestEntity=NULL;

			for (int i = 0; i < maxPlayers; i++)
			{
				uintptr_t localPlayer = getLocalPlayer();
				uintptr_t entity = entityList[i];
				//bool isVisible = isVisibleByParser(localPlayer, entity);// || isVisibleByDef(entity, localPlayer);
				bool isFriendlyEnabled = toggleFriendlyFire && getEntityTeam(entity) == getEntityTeam(localPlayer);
				//std::cout << "\nisFriendlyEnabled= " << isFriendlyEnabled;
				bool isEnemy = getEntityTeam(entity) != getEntityTeam(localPlayer);
				if ((isEnemy || isFriendlyEnabled))// && isVisible)
				{
					/*D3DXVECTOR3 angle = vec.CalcAngle(localEyePos, getEntBonePos(entity, aimBone));
					D3DXVECTOR3 cAngles = vec.ClampAngles(angle - viewAngles);
					float delta = sqrt(cAngles.x * cAngles.x + cAngles.y * cAngles.y);*/

					float delta = getEntityBoneDelta(localEyePos, viewAngles, entity, aimBone);
					
					if (delta < fov)
					{
						//std::cout << "\ndelta= " << delta;
						fov = delta;
						bestEntity = entity;
						break;
					}
					//if(delta<0.1)
						//Sleep(200);
				}
			}
			return bestEntity;
		}

		float getEntityBoneDelta(D3DXVECTOR3 localEyePos, D3DXVECTOR3 viewAngles, uintptr_t entity, int aimBone) {
			D3DXVECTOR3 angle = vec.CalcAngle(localEyePos, getEntBonePos(entity, aimBone));
			D3DXVECTOR3 cAngles = vec.ClampAngles(angle - viewAngles);
			return sqrt(cAngles.x * cAngles.x + cAngles.y * cAngles.y);
		}
		D3DXVECTOR3 getLocalPos()
		{
			return mem.RPM<D3DXVECTOR3>(getLocalPlayer() + m_vecOrigin);
		}

		D3DXVECTOR3 getLocalEyePos()
		{
			D3DXVECTOR3 localPos = getLocalPos();
			localPos += mem.RPM<D3DXVECTOR3>(getLocalPlayer() + m_vecViewOffset);
			return localPos;
		}

		D3DXVECTOR3	getEntBonePos(DWORD playerBase, int boneID)
		{
			Matrix3x4_t boneMatrix = mem.RPM<Matrix3x4_t>(getEntBoneMatrix(playerBase) + boneID * 0x30);

			return{
				boneMatrix.Matrix[0][3],
				boneMatrix.Matrix[1][3],
				boneMatrix.Matrix[2][3]
			};
		}

		DWORD getEntBoneMatrix(DWORD playerBase)
		{
			return mem.RPM<DWORD>(playerBase + m_dwBoneMatrix);
		}

		void startFire() {
			mem.WPM<int>(moduleBase + dwForceAttack, 5);
		}

		void stopFire() {
			mem.WPM<int>(moduleBase + dwForceAttack, 4);
		}

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
			return activeWeaponId == usps || activeWeaponId == xm1014
				|| activeWeaponId == tec9 || activeWeaponId == five7 || activeWeaponId == cz;
		}

		boolean getIsDgle(int activeWeaponId) {
			return activeWeaponId == dgle || activeWeaponId == dgleDef;
		}

		boolean getIsSemiFWeapon(int activeWeaponId) {
			return activeWeaponId == famas || activeWeaponId == gali
				|| activeWeaponId == aug || activeWeaponId == augDef || activeWeaponId == sg
				|| activeWeaponId == p250 || activeWeaponId == p250Def
				|| activeWeaponId == glock;
		}

		boolean getIsGay(int activeWeaponId) {
			return activeWeaponId == g3sg || activeWeaponId == scar;
		}

		boolean getIsSpam(int activeWeaponId) {
			return activeWeaponId == p90 || activeWeaponId == p90Def
				|| activeWeaponId == bison || activeWeaponId == negev;
		}

		/*bool isVisibleByParser(uintptr_t localPlayer, uintptr_t entity) {
			return bspParser->is_visible(getOrigin(localPlayer), getEntBonePosForParser(entity, 8)); // aimBone 8 head
			//return bspParser->is_visible(getOrigin(localPlayer), getOrigin(entity));
		}

		bool isMapLoaded() {
			return bspParser->load_map(getGameDirectory(), getMapDirectory());
		}

		int	getEntClassID(int entity)
		{
			int one = mem.RPM<int>(entity + 0x8);
			int two = mem.RPM<int>(one + 2 * 0x4);
			int three = mem.RPM<int>(two + 0x1);
			return mem.RPM<int>(three + 0x14);
		}*/

		/*
		rn::vector3 getEntBonePosForParser(uint32_t entity, int boneID) {
			uint32_t boneMatrix = getPtrOffset(entity + m_dwBoneMatrix);
			BonePos bonepos = mem.RPM<BonePos>(boneMatrix + 0x30 * boneID + 0xC);
			return { bonepos.x, bonepos.y, bonepos.z };
		}

		char* getGameDirectory() {
			static std::array<char, 0x120> gameDirectory = mem.RPM<std::array<char, 0x120>>(engineBase + dwGameDir);
			return gameDirectory.data();
		}

		char* getMapDirectory() {
			uintptr_t engineptr = mem.RPM<uintptr_t>(engineBase + dwClientState);
			static std::array<char, 0x120> mapDirectory = mem.RPM<std::array<char, 0x120>>(engineptr + dwClientState_MapDirectory);
			return mapDirectory.data();
		}

		rn::vector3 getOrigin(uintptr_t entity) {
			return mem.RPM<rn::vector3>(entity + m_vecOrigin);
		}*/
};