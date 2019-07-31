#pragma once

#include "../Interfaces.h"
#include "EngineTrace.h"
#include "Utils.h"
#include "../MathTools.h"
#include "../Netvars.h"
#include "ClassId.h"
#include "ClientClass.h"
#include "def.h"

class Entity {
public:
	template <typename T>
	constexpr auto getProperty(const char* name, const std::size_t offset = 0) const noexcept
	{
		return *reinterpret_cast<const T*>(this + netvars[name] + offset);
	}

	template <typename T>
	constexpr void setProperty(const char* name, const T & value) noexcept
	{
		*reinterpret_cast<T*>(this + netvars[name]) = value;
	}

	constexpr bool isPistol() noexcept
	{
		if (!this->isWeapon()) return false;

		switch (getClientClass()->classId) {
		case ClassId::Deagle:
		case ClassId::Elite:
		case ClassId::FiveSeven:
		case ClassId::Glock:
		case ClassId::P2000:
		case ClassId::P250:
		case ClassId::Tec9:
			return true;
		default:
			return false;
		}
	}

	constexpr bool isSniperRifle() noexcept
	{
		if (!this->isWeapon()) return false;

		switch (getClientClass()->classId) {
		case ClassId::Ssg08:
		case ClassId::Awp:
		case ClassId::Scar20:
		case ClassId::G3sg1:
			return true;
		default:
			return false;
		}
	}

	constexpr bool isKnife() noexcept
	{
		if (!this->isWeapon()) return false;

		switch (getProperty<int>("m_iItemDefinitionIndex")) {
		case ItemDefinitionIndex::WEAPON_KNIFE:
		case ItemDefinitionIndex::WEAPON_KNIFE_BAYONET:
		case ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY:
		case ItemDefinitionIndex::WEAPON_KNIFE_FALCHION:
		case ItemDefinitionIndex::WEAPON_KNIFE_FLIP:
		case ItemDefinitionIndex::WEAPON_KNIFE_GUT:
		case ItemDefinitionIndex::WEAPON_KNIFE_GYPSY_JACKKNIFE:
		case ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT:
		case ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET:
		case ItemDefinitionIndex::WEAPON_KNIFE_PUSH:
		case ItemDefinitionIndex::WEAPON_KNIFE_STILETTO:
		case ItemDefinitionIndex::WEAPON_KNIFE_SURVIVAL_BOWIE:
		case ItemDefinitionIndex::WEAPON_KNIFE_T:
		case ItemDefinitionIndex::WEAPON_KNIFE_TACTICAL:
		case ItemDefinitionIndex::WEAPON_KNIFE_URSUS:
		case ItemDefinitionIndex::WEAPON_KNIFE_WIDOWMAKER:
		case ItemDefinitionIndex::WEAPON_KNIFEGG:
		case ItemDefinitionIndex::WEAPON_KNIFE_GHOST:
			return true;
		default:
			return false;
		}
	}

	constexpr bool isMeleeAttackWeapon() noexcept
	{
		if (!this->isWeapon()) return false;

		switch (getProperty<int>("m_iItemDefinitionIndex")) {
		case ItemDefinitionIndex::WEAPON_KNIFE:
		case ItemDefinitionIndex::WEAPON_KNIFE_BAYONET:
		case ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY:
		case ItemDefinitionIndex::WEAPON_KNIFE_FALCHION:
		case ItemDefinitionIndex::WEAPON_KNIFE_FLIP:
		case ItemDefinitionIndex::WEAPON_KNIFE_GUT:
		case ItemDefinitionIndex::WEAPON_KNIFE_GYPSY_JACKKNIFE:
		case ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT:
		case ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET:
		case ItemDefinitionIndex::WEAPON_KNIFE_PUSH:
		case ItemDefinitionIndex::WEAPON_KNIFE_STILETTO:
		case ItemDefinitionIndex::WEAPON_KNIFE_SURVIVAL_BOWIE:
		case ItemDefinitionIndex::WEAPON_KNIFE_T:
		case ItemDefinitionIndex::WEAPON_KNIFE_TACTICAL:
		case ItemDefinitionIndex::WEAPON_KNIFE_URSUS:
		case ItemDefinitionIndex::WEAPON_KNIFE_WIDOWMAKER:
		case ItemDefinitionIndex::WEAPON_KNIFEGG:
		case ItemDefinitionIndex::WEAPON_SHIELD:
		case ItemDefinitionIndex::WEAPON_FISTS:
		case ItemDefinitionIndex::WEAPON_MELEE:
		case ItemDefinitionIndex::WEAPON_AXE:
		case ItemDefinitionIndex::WEAPON_HAMMER:
		case ItemDefinitionIndex::WEAPON_SPANNER:
		case ItemDefinitionIndex::WEAPON_KNIFE_GHOST:
			return true;
		default:
			return false;
		}
	}

	constexpr bool isRangeAttackWeapon() noexcept
	{
		if (!this->isWeapon() || this->isMeleeAttackWeapon())
			return false;

		if (this->isPistol() || this->isSniperRifle() || this->isGrenade())
			return true;

		return false;
	}

	constexpr bool isGrenade() noexcept
	{
		if (!this->isWeapon()) return false;

		switch (getProperty<int>("m_iItemDefinitionIndex")) {
		case ItemDefinitionIndex::WEAPON_FLASHBANG:
		case ItemDefinitionIndex::WEAPON_HEGRENADE:
		case ItemDefinitionIndex::WEAPON_SMOKEGRENADE:
		case ItemDefinitionIndex::WEAPON_MOLOTOV:
		case ItemDefinitionIndex::WEAPON_DECOY:
		case ItemDefinitionIndex::WEAPON_INCGRENADE:
		case ItemDefinitionIndex::WEAPON_TAGRENADE:
		case ItemDefinitionIndex::WEAPON_FRAG_GRENADE:
			return true;
		default:
			return false;
		}
	}

	constexpr bool isCommonWeapon() noexcept
	{
		if (!this->isWeapon()) return false;

		switch (getProperty<int>("m_iItemDefinitionIndex"))
		{
			case ItemDefinitionIndex::WEAPON_C4:
			case ItemDefinitionIndex::WEAPON_HEALTHSHOT:
			case ItemDefinitionIndex::WEAPON_BUMPMINE:
				return true;
			default:
				return false;
		}
	}

	using matrix3x4 = float[3][4];

	constexpr bool setupBones(matrix3x4* out, int maxBones, int boneMask, float currentTime) noexcept
	{
		return callVirtualMethod<bool, matrix3x4*, int, int, float>(this + 4, 13, out, maxBones, boneMask, currentTime);
	}

	Vector getBonePosition(int bone) noexcept
	{
		static matrix3x4 boneMatrices[128];
		if (setupBones(boneMatrices, 128, 256, 0.0f))
			return Vector{ boneMatrices[bone][0][3], boneMatrices[bone][1][3], boneMatrices[bone][2][3] };
		else
			return Vector{ };
	}

	constexpr Vector getEyePosition() noexcept
	{
		Vector vec{ };
		callVirtualMethod<void, Vector&>(this, 281, vec);
		return vec;
	}

	bool isVisible(const Vector& position = { }) noexcept
	{
		auto localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
		Trace trace = { 0 };
		interfaces.engineTrace->traceRay({ localPlayer->getEyePosition(), position }, MASK_SHOT, { localPlayer }, trace);
		return (trace.entity == this || trace.fraction > 0.93f);
	}

	bool isEnemy() noexcept
	{
		static auto gameMode = interfaces.cvar->findVar("game_type");

		if (!isInLocalTeam() || gameMode->getInt() == 6)
			return true;
		else
			return false;
	}

	constexpr bool isDormant() noexcept
	{
		return callVirtualMethod<bool>(this + 8, 9);
	}

	constexpr bool IsPlayer() noexcept
	{
		return callVirtualMethod<bool>(this, 155);
	}

	constexpr bool isWeapon() noexcept
	{
		return callVirtualMethod<bool>(this, 163);
	}

	constexpr ClientClass* getClientClass() noexcept
	{
		return callVirtualMethod<ClientClass*>(this + 8, 2);
	}

	constexpr bool isAlive() noexcept
	{
		return callVirtualMethod<bool>(this, 153) && getProperty<int>("m_iHealth") > 0;
	}

	constexpr bool IsMoving() noexcept
	{
		return callVirtualMethod<bool>(this, 165);
	}

	constexpr bool isInLocalTeam() noexcept
	{
		return callVirtualMethod<bool>(this, 92);
	}

	constexpr float getInaccuracy() noexcept
	{
		return callVirtualMethod<float>(this, 476);
	}

	/*VarMap* getVarMap() noexcept
	{
		return reinterpret_cast<VarMap*>(this + 0x24);
	}*/

	constexpr Vector getAbsOrigin() noexcept
	{
		return callVirtualMethod<Vector&>(this, 10);
	}

	constexpr int getMoveType() noexcept
	{
		return this->getProperty<int>("m_nRenderMode", 1);
	}
};
