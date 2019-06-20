#include "Rage.h"
#include "../Interfaces.h"
#include "../Config.h"
#include "../SDK/Entity.h"
#include "Memory.h"
#include "../HackTools.h"

void Rage::Execute(UserCmd* cmd) noexcept
{
	if (!config.rage.enabled)
		return;

	const auto localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
	if (!localPlayer || !localPlayer->isAlive() || localPlayer->getProperty<float>("m_flNextAttack") > mem.globalVars->serverTime())
		return;

	const auto activeWeapon = interfaces.entityList->getEntityFromHandle(localPlayer->getProperty<int>("m_hActiveWeapon"));
	if (!activeWeapon || !activeWeapon->isWeapon() || !activeWeapon->getProperty<int>("m_iClip1") || !(activeWeapon->getProperty<float>("m_flNextPrimaryAttack") <= mem.globalVars->serverTime()) || activeWeapon->isMeleeAttackWeapon() || activeWeapon->isGrenade() || activeWeapon->isCommonWeapon())
		return;

	if (activeWeapon->isSniperRifle() && config.rage.onlyzoom && !localPlayer->getProperty<bool>("m_bIsScoped"))
		return;

	auto pTarget = hacktools.GetVisibleHurtablePlayer(config.rage.distance, config.rage.target);
	if (!pTarget)
		return;

	if (!config.rage.ignoreSmoke && mem.lineGoesThroughSmoke(localPlayer->getEyePosition(), pTarget->getBonePosition(8), 1)
		|| (!config.rage.ignoreflash && localPlayer->getProperty<float>("m_flFlashDuration")))
		return;

	Vector headPosition = pTarget->getBonePosition(8);
	if (headPosition.IsZero() || !headPosition.IsValid() || !pTarget->isVisible(headPosition))
		return;

	Vector targetPos = pTarget->getProperty<Vector>("m_vecOrigin");

	cmd->viewangles = hacktools.AngelToTarget(localPlayer, pTarget, 8);//Set new angle
	static auto weaponRecoilScale = interfaces.cvar->findVar("weapon_recoil_scale");
	auto aimPunch = localPlayer->getProperty<Vector>("m_aimPunchAngle");
	cmd->viewangles.x -= (aimPunch.x * weaponRecoilScale->getFloat());//subtract x
	cmd->viewangles.y -= (aimPunch.y * weaponRecoilScale->getFloat());//subtract y
	cmd->buttons |= UserCmd::IN_ATTACK;

	return;
}