#include "Aimbot.h"
#include "../Interfaces.h"
#include "../Config.h"
#include "../HackTools.h"

void Aimbot::Execute(UserCmd* cmd) noexcept
{
	if (!config.aimbot.enabled)
		return;

	const auto localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
	if (!localPlayer || !localPlayer->isAlive() || localPlayer->getProperty<float>("m_flNextAttack") > mem.globalVars->serverTime())
		return;

	const auto localPlayerTeam = localPlayer->getProperty<int>("m_iTeamNum");

	const auto activeWeapon = interfaces.entityList->getEntityFromHandle(localPlayer->getProperty<int>("m_hActiveWeapon"));
	if (!activeWeapon || !activeWeapon->isWeapon() || !activeWeapon->getProperty<int>("m_iClip1") || !(activeWeapon->getProperty<float>("m_flNextPrimaryAttack") <= mem.globalVars->serverTime()) || activeWeapon->isMeleeAttackWeapon() || activeWeapon->isGrenade() || activeWeapon->isCommonWeapon())
		return;

	if (activeWeapon->isSniperRifle() && config.aimbot.onlyzoom && !localPlayer->getProperty<bool>("m_bIsScoped"))
		return;

	Entity* target{ 0 };
	int targetBone{ -1 };

	hacktools.getTarget(config.aimbot.FOV.px, target, targetBone);
	if (!target || targetBone == -1)
		return;
	if (mem.lineGoesThroughSmoke(localPlayer->getEyePosition(), target->getBonePosition(targetBone), 1) && !config.aimbot.ignoreSmoke)
		return;
	if (localPlayer->getProperty<float>("m_flFlashDuration") && !config.aimbot.ignoreflash)
		return;

	Vector viewAngels;
	interfaces.engine->getViewAngles(viewAngels);
	Vector delta = viewAngels - hacktools.AngelToTarget(localPlayer, target, targetBone);
	NormalizeAngles(delta);
	delta /= config.aimbot.smooth;
	cmd->viewangles = cmd->viewangles - delta;
	interfaces.engine->setViewAngles(cmd->viewangles);
	return;
}