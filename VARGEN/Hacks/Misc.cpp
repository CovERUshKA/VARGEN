#include "Misc.h"
#include "../Config.h"
#include "../Interfaces.h"
#include "../SDK/Entity.h"
#include "../SDK/WeaponID.h"
#include "../Memory.h"

void Misc::BunnyHop(UserCmd* cmd) noexcept
{
	auto localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
	if (!localPlayer || !localPlayer->isAlive())
		return;

	if (config.misc.bunnyhop && localPlayer->getMoveType() != MoveType::MOVETYPE_LADDER && localPlayer->getMoveType() != MoveType::MOVETYPE_NOCLIP && !(localPlayer->getProperty<int>("m_fFlags") & 1))
		cmd->buttons &= ~UserCmd::IN_JUMP;

	return;
}

void Misc::setClanTag(const char* clanTag) noexcept
{
	mem.setClanTag(clanTag, clanTag);
}

void Misc::FastDuck(UserCmd* cmd) noexcept
{
	if (config.misc.fastduck)
		cmd->buttons |= UserCmd::IN_BULLRUSH;

	return;
}

void Misc::autoStrafe(UserCmd* cmd) noexcept
{
	if (config.misc.autostrafe
		&& config.misc.bunnyhop && interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer())->getMoveType() != MoveType::MOVETYPE_LADDER && interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer())->getMoveType() != MoveType::MOVETYPE_NOCLIP && !(interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer())->getProperty<int>("m_fFlags") & 1)) {
		if (cmd->mousedx < 0)
			cmd->sidemove = -450.0f;
		else if (cmd->mousedx > 0)
			cmd->sidemove = 450.0f;
	}

	return;
}

void Misc::sniperCrosshair() noexcept
{
	static auto showSpread = interfaces.cvar->findVar("weapon_debug_spread_show");
	showSpread->setValue(config.misc.snipercrosshair && !interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer())->getProperty<bool>("m_bIsScoped") ? 3 : 0);
}

void Misc::autoPistol(UserCmd* cmd) noexcept
{
	if (!config.misc.autopistol)
		return;

	const auto activeWeapon = interfaces.entityList->getEntityFromHandle(interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer())->getProperty<int>("m_hActiveWeapon"));
	if (activeWeapon && activeWeapon->isPistol() && activeWeapon->getProperty<float>("m_flNextPrimaryAttack") > mem.globalVars->serverTime()) {
		if (activeWeapon->getProperty<WeaponId>("m_iItemDefinitionIndex") == WeaponId::Revolver)
			cmd->buttons &= ~UserCmd::IN_ATTACK2;
		else
			cmd->buttons &= ~UserCmd::IN_ATTACK;
	}

	return;
}

void Misc::AntiKick(bf_read& msg_data) noexcept
{
	//if (!config.misc.antikick)
		//return;

	msg_data.SetOffset(1);
	auto ent_index = msg_data.ReadByte();

	PlayerInfo pInfo;
	interfaces.engine->getPlayerInfo(ent_index, pInfo);
	return;
}

void Misc::FakeDuck(UserCmd* cmd) noexcept
{
	/*bool do_once = false, _do;
	int limit = 10 / 2;
	crouch = mem.input.choke > limit;

	if (!(GetAsyncKeyState(VK_LCONTROL) & 0x8000) && do_once)
		interfaces.engine->ExecuteClientCmd("-duck");
	if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
	{
		do_once = true;
		if (crouch)
			interfaces.engine->ExecuteClientCmd("-duck");
		else
			interfaces.engine->ExecuteClientCmd("+duck");
	}*/
	return;
}