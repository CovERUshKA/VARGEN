#include "Anti-Aim.h"
#include "../Interfaces.h"
#include "../Config.h"
#include "../SDK/Entity.h"
#include "../Memory.h"

void AntiAim::Execute(UserCmd* cmd) noexcept
{
	if (!config.antiaim.enabled)
		return;

	const auto localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
	const auto activeWeapon = interfaces.entityList->getEntityFromHandle(localPlayer->getProperty<int>("m_hActiveWeapon"));
	if (!interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer())->isAlive() || localPlayer->getMoveType() == MoveType::MOVETYPE_LADDER || localPlayer->getMoveType() == MoveType::MOVETYPE_NOCLIP || cmd->command_number & UserCmd::IN_USE || (cmd->command_number & UserCmd::IN_ATTACK && activeWeapon->getProperty<float>("m_flNextPrimaryAttack") <= mem.globalVars->serverTime()) || config.keys[VK_LBUTTON])
		return;
	
	cmd->viewangles.normalize();
	cmd->viewangles.x = 89;
	cmd->viewangles.y = cmd->viewangles.y - 180;
	cmd->forwardmove = -(cmd->forwardmove);
	cmd->sidemove = -(cmd->sidemove);
	cmd->upmove = -(cmd->upmove);
	return;
}