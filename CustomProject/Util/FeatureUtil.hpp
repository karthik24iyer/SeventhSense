#pragma once

#include "../Misc/MyVector.hpp"
//#include "StructUtil.hpp"
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


//extern Toggles menu;

class FeatureUtil {
	private:
		MyVector vec;
		//inline static rn::bsp_parser* bspParser = new rn::bsp_parser;
	public:

		char entityLastPlace[32];

		D3DXVECTOR3 getEntityPos(uintptr_t entity)
		{
			return mem.RPM<D3DXVECTOR3>(entity + m_vecOrigin);
		}

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

		bool isEntityAlive(uintptr_t entity)
		{
			return getEntityHealth(entity) > 0 ? 1 : 0;
		}

		int	getEntityHealth(uintptr_t entity)
		{
			return mem.RPM<int>(entity + m_iHealth);
		}

		uintptr_t getPtrOffset(uintptr_t offset) {
			return mem.RPM<uintptr_t>(offset);
		}

		int getIntOffset(uintptr_t offset) {
			return mem.RPM<int>(offset);
		}

		uintptr_t getCurrentEntity(int i) {
			return getPtrOffset(moduleBase + dwEntityList + i * 0x10);
		}

		int	getEntityTeam(uintptr_t entity)
		{
			return mem.RPM<int>(entity + m_iTeamNum);
		}

		int getIsDormant(uintptr_t entity) {
			return getIntOffset(entity + m_bDormant);
		}

		BYTE getEntityLifeState(uintptr_t entity)
		{
			return mem.RPM<BYTE>(entity + m_lifeState);
		}

		bool isValidEntity(uintptr_t entity)
		{
			if (isEntityAlive(entity) && getEntityTeam(entity) != 0 && !getIsDormant(entity))// && getEntityLifeState(entity) == 0)
				return true;
			return false;
		}

		
		DWORD getActiveWeapon(uintptr_t entity)
		{
			DWORD weaponIndex = mem.RPM<DWORD>(entity + m_hActiveWeapon) & 0xFFF;
			return mem.RPM<DWORD>((moduleBase + dwEntityList + weaponIndex * 0x10) - 0x10);
		}

		long getActiveWeaponId(DWORD activeWeapon) {
			return mem.RPM<DWORD>(activeWeapon + m_iItemDefinitionIndex);
		}

		BYTE getByteOffset(uintptr_t offset) {
			return mem.RPM<BYTE>(offset);
		}

		int isEntitySpotted(uintptr_t entity) {
			return getIntOffset(entity + m_bSpotted);
		}

		int isScoped(uintptr_t entity) {
			return getIntOffset(entity + m_bIsScoped);
		}
		
		int isEntityDefusing(uintptr_t entity) {
			return getIntOffset(entity + m_bIsDefusing);
		}

		int isEntityScoped(uintptr_t entity) {
			return getIntOffset(entity + m_bIsScoped);
		}

		void setLastPlace(uintptr_t entity) {
			int n = 0;
			char c = 'o';
			while (c != '\0') {
				c = mem.RPM<char>(entity + m_szLastPlaceName + n);
				entityLastPlace[n] = c;
				//std::cout << c;
				n++;
			}
		}
		/*
		boolean isEnityHoldingC4(uintptr_t entity) {
			if (getActiveWeaponId(getActiveWeapon(entity)) == c4) {
				return true;
			}
			return false;
		}
		
		bool isVisibleByDef(uintptr_t player1, uintptr_t player2) {
			int index = getIntOffset(player1 + 0x64);
			int byMask = getIntOffset(player2 + m_bSpottedByMask);
			return (bool)(byMask && (1 << (index - 1)));
		}
		*/
		uintptr_t entityOnCrosshair(uintptr_t localPlayer) {
			return mem.RPM<uintptr_t>(moduleBase + dwEntityList + ((getLocalCrossID(localPlayer) - 1) * 0x10));
		}

		bool isEntityOnCrosshair(uintptr_t localPlayer, uintptr_t entity) {
			return (entityOnCrosshair(localPlayer) == entity) && isValidEntity(entity);
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

		int getClipsInActiveWeapon(uintptr_t entity) {
			DWORD activeWeapon = getActiveWeapon(entity);
			//std::cout<<"\nclips= "<< mem.RPM<DWORD>(activeWeapon + m_iClip1);
			return mem.RPM<DWORD>(activeWeapon + m_iClip1);
		}

		D3DXVECTOR3 getLocalPos()
		{
			return mem.RPM<D3DXVECTOR3>(getLocalPlayer() + m_vecOrigin);
		}
		
		D3DXVECTOR3 getLocalViewAngles()
		{
			return mem.RPM<D3DXVECTOR3>(getEnginePointer() + dwClientState_ViewAngles);
		}

		D3DXVECTOR3 getLocalPunchAngles()
		{
			return mem.RPM<D3DXVECTOR3>(getLocalPlayer() + m_aimPunchAngle);
		}

		D3DXVECTOR3 getLocalEyePos()
		{
			D3DXVECTOR3 localPos = getLocalPos();
			localPos += mem.RPM<D3DXVECTOR3>(getLocalPlayer() + m_vecViewOffset);
			return localPos;
		}
		
		/*void setEntitySpotted(uintptr_t entity) {
			mem.WPM<bool>(entity + m_bSpotted, true);
		}

		void setLocalViewAngles(D3DXVECTOR3 angles)
		{
			mem.WPM<D3DXVECTOR3>(getEnginePointer() + dwClientState_ViewAngles, angles);
		}
		*/

		
		
		float getEntityBoneDelta(D3DXVECTOR3 localEyePos, D3DXVECTOR3 viewAngles, uintptr_t entity, int aimBone) 
		{
			D3DXVECTOR3 angle = vec.CalcAngle(localEyePos, getEntBonePos(entity, aimBone));
			D3DXVECTOR3 cAngles = vec.ClampAngles(angle - viewAngles);
			return sqrt(cAngles.x * cAngles.x + cAngles.y * cAngles.y);
		}
		

		D3DXVECTOR3	getEntBonePos(DWORD entity, int boneID)
		{
			Matrix3x4_t boneMatrix = mem.RPM<Matrix3x4_t>(getEntBoneMatrix(entity) + boneID * 0x30);

			return{
				boneMatrix.Matrix[0][3],
				boneMatrix.Matrix[1][3],
				boneMatrix.Matrix[2][3]
			};
		}

		DWORD getEntBoneMatrix(DWORD entity)
		{
			return mem.RPM<DWORD>(entity + m_dwBoneMatrix);
		}
		
		void startFire() {
			pressMouse();
			//mem.WPM<int>(moduleBase + dwForceAttack, 5);
		}

		void stopFire() {
			liftMouse();
			//mem.WPM<int>(moduleBase + dwForceAttack, 4);
		}

		void pressMouse() {
			mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, NULL, NULL);
		}

		void liftMouse() {
			mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, NULL, NULL);
		}
				
		int getMouseSpeed() {
			int speed;
			SystemParametersInfoA(SPI_GETMOUSESPEED, 0, &speed, 0);
			return speed;
		}

		void setMouseSpeed(int speed) {
			if (speed > 20) speed = 20;
			if (speed < 1) speed = 1;
			SystemParametersInfoA(SPI_SETMOUSESPEED, 0, (LPVOID)speed, 0);
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