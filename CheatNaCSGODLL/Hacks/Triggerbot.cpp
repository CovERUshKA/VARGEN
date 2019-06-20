#include "Triggerbot.h"
#include "../Interfaces.h"
#include "../Config.h"
#include "../Memory.h"
#include "../SDK/def.h"

constexpr auto degreesToRadians(float degrees) noexcept
{
	return degrees * static_cast<float>(M_PI) / 180;
}

void Triggerbot::Execute(UserCmd* cmd) noexcept
{
	if (!config.triggerbot.enabled)
		return;

	const auto localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
	if (!localPlayer || !localPlayer->isAlive() || localPlayer->getProperty<float>("m_flNextAttack") > mem.globalVars->serverTime())
		return;

	const auto localPlayerTeam = localPlayer->getProperty<int>("m_iTeamNum");

	const auto activeWeapon = interfaces.entityList->getEntityFromHandle(localPlayer->getProperty<int>("m_hActiveWeapon"));
	if (!activeWeapon || !activeWeapon->isWeapon() || !activeWeapon->getProperty<int>("m_iClip1") || !(activeWeapon->getProperty<float>("m_flNextPrimaryAttack") <= mem.globalVars->serverTime()) || activeWeapon->isMeleeAttackWeapon() || activeWeapon->isGrenade() || activeWeapon->isCommonWeapon())
		return;

	if (activeWeapon->isSniperRifle() && config.triggerbot.onlyzoom && !localPlayer->getProperty<bool>("m_bIsScoped"))
		return;
	if (localPlayer->getProperty<float>("m_flFlashDuration") && !config.triggerbot.ignoreflash)
		return;

	constexpr float maxRange{ 8192.0f };
	Vector viewAngles{ cos(degreesToRadians(cmd->viewangles.x)) * cos(degreesToRadians(cmd->viewangles.y)) * maxRange,
					   cos(degreesToRadians(cmd->viewangles.x)) * sin(degreesToRadians(cmd->viewangles.y)) * maxRange,
					  -sin(degreesToRadians(cmd->viewangles.x)) * maxRange };

	static Trace trace;
	interfaces.engineTrace->traceRay({ localPlayer->getEyePosition(), localPlayer->getEyePosition() + viewAngles }, 0x46004009, { localPlayer }, trace);

	if (trace.entity->getClientClass()->classId == ClassId::CSPlayer
		&& ((trace.entity->isEnemy() && config.triggerbot.target == 1)
			|| (!trace.entity->isEnemy() && config.triggerbot.target == 2)
			|| config.triggerbot.target == 0)
		&& !trace.entity->getProperty<bool>("m_bGunGameImmunity")
		&& (config.triggerbot.ignoreSmoke
			|| !mem.lineGoesThroughSmoke(localPlayer->getEyePosition(), localPlayer->getEyePosition() + viewAngles, 1))
		&& (config.triggerbot.ignoreflash
			|| !localPlayer->getProperty<float>("m_flFlashDuration"))
		&& (!config.triggerbot.onlyzoom
			|| !activeWeapon->isSniperRifle()
			|| activeWeapon->isSniperRifle() && localPlayer->getProperty<bool>("m_bIsScoped"))
		&& trace.hitbox == eHITBOX::HITBOX_HEAD) {
		cmd->buttons |= UserCmd::IN_ATTACK;
	}
	return;
}