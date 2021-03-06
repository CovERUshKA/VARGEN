#include "ESP.h"
#include "../Config.h"
#include "../Interfaces.h"
#include "Memory.h"
#include "../HackTools.h"

void RenderBox(int i, float x, float y, float width, float height, float px, int r, int g, int b, int a)
{
	interfaces.surface->setDrawColor(r, g, b, a);
	if (config.esp.box.corners)
	{
		interfaces.surface->drawFilledRect(x, y - (px / 2), x + (width / 4), y + (px / 2));
		interfaces.surface->drawFilledRect(x + ((width / 4) * 3), y - (px / 2), x + width, y + (px / 2));
		interfaces.surface->drawFilledRect(x - (px / 2), y, x + (px / 2), y + (height / 4));
		interfaces.surface->drawFilledRect(x - (px / 2), y + ((height / 4) * 3), x + (px / 2), y + height);
		interfaces.surface->drawFilledRect(x, y + height - (px / 2), x + (width / 4), y + height + (px / 2));
		interfaces.surface->drawFilledRect(x + ((width / 4) * 3), y + height - (px / 2), x + width, y + height + (px / 2));
		interfaces.surface->drawFilledRect(x + width - (px / 2), y, x + width + (px / 2), y + (height / 4));
		interfaces.surface->drawFilledRect(x + width - (px / 2), y + ((height / 4) * 3), x + width + (px / 2), y + height);
	}
	else
	{
		interfaces.surface->drawFilledRect(x, y - (px / 2), x + width, y + (px / 2));
		interfaces.surface->drawFilledRect(x - (px / 2), y, x + (px / 2), y + height);
		interfaces.surface->drawFilledRect(x, y + height - (px / 2), x + width, y + height + (px / 2));
		interfaces.surface->drawFilledRect(x + width - (px / 2), y, x + width + (px / 2), y + height);
	}
	if (config.esp.hp)
	{
		auto entHP = interfaces.entityList->getEntity(i)->getProperty<int>("m_iHealth");
		interfaces.surface->setDrawColor(255 - (entHP * 2.55), entHP * 2.55, 0, 255);
		if (entHP)
			interfaces.surface->drawFilledRect(x + width + 3, (y + height) - ((height / 100) * entHP), x + width + 5, y + height); // Draw HP of pEntity
	}
	if (config.esp.armor)
	{
		interfaces.surface->setDrawColor(0, 0, 255, 255);
		auto entArmor = interfaces.entityList->getEntity(i)->getProperty<int>("m_ArmorValue");
		if (entArmor)
			interfaces.surface->drawFilledRect(x + width + 7, (y + height) - ((height / 100) * entArmor), x + width + 9, y + height); // Draw Armor of pEntity
	}
	if (config.esp.nickname.enabled)
	{
		Entity* entity = interfaces.entityList->getEntity(i);
		static PlayerInfo playerInfo;
		if (interfaces.engine->getPlayerInfo(i, playerInfo)) {
			static wchar_t name[128];
			if (MultiByteToWideChar(CP_UTF8, 0, playerInfo.name, -1, name, 128)) {
				static unsigned font = interfaces.surface->createFont();
				static bool init = interfaces.surface->setFontGlyphSet(font, "Tahoma", 12, 700, 0, 0, 128);
				interfaces.surface->setTextFont(font);
				interfaces.surface->setTextColor(config.esp.nickname.color);
				interfaces.surface->setTextPosition((x + width / 2) - (interfaces.surface->getTextSize(font, name).first / 2), y - 2 - interfaces.surface->getTextSize(font, name).second);
				interfaces.surface->printText(name);
				if (config.misc.bombtimer.enabled)
				{
					interfaces.surface->setTextPosition(30, 30);
					interfaces.surface->printText(std::to_wstring(config.misc.bombtimer.time).c_str());
				}
			}
		}
	}
	return;
}

void ESP::Render() noexcept
{
	if (!interfaces.engine->isInGame() || !config.esp.enabled)
		return;

	Entity* localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());

	DWORD localTeam = localPlayer->getProperty<int>("m_iTeamNum");


	for (int i = 0; i < interfaces.engine->getMaxClients(); i++)
	{
		auto target = interfaces.entityList->getEntity(i);
		if (!target || !target->isAlive() || target->isDormant() || target == localPlayer)
			continue;

		auto entTeam = target->getProperty<int>("m_iTeamNum");
		if (!entTeam)
			continue;

		Vector lowerBonePos = target->getProperty<Vector>("m_vecOrigin");
		Vector higherBonePos = target->getBonePosition(8);
		higherBonePos.z += 7;
		int EntYPos = hacktools.WorldToScreen(higherBonePos).y;
		int entHeight = hacktools.WorldToScreen(lowerBonePos).y - hacktools.WorldToScreen(higherBonePos).y;
		higherBonePos.z -= 7;
		int entHeightincorrect = hacktools.WorldToScreen(lowerBonePos).y - hacktools.WorldToScreen(higherBonePos).y;
		int EntWidth = entHeightincorrect / 2;
		int EntXPos = hacktools.WorldToScreen(lowerBonePos).x - (EntWidth / 2);
		if (EntXPos == 0 || EntXPos == interfaces.surface->getScreenSize().second || EntYPos == 0 || EntWidth == 0 || entHeight == 0)
			continue;

		if (config.esp.box.enabled)
		{
			if (entTeam == localTeam && !target->isEnemy() && (config.esp.filter == 0 || config.esp.filter == 2))
				RenderBox(i, EntXPos, EntYPos, EntWidth, entHeight, 1, config.esp.colors.allies[0] * 255, config.esp.colors.allies[1] * 255, config.esp.colors.allies[2] * 255, config.esp.colors.allies[3] * 255);
			if (entTeam != localTeam && target->isEnemy() && (config.esp.filter == 0 || config.esp.filter == 1))
				RenderBox(i, EntXPos, EntYPos, EntWidth, entHeight, 1, config.esp.colors.enemies[0] * 255, config.esp.colors.enemies[1] * 255, config.esp.colors.enemies[2] * 255, config.esp.colors.enemies[3] * 255);
		}
		if (config.esp.line.enabled)
		{
			if (entTeam == localTeam && !target->isEnemy() && (config.esp.filter == 0 || config.esp.filter == 2))
			{
				interfaces.surface->setDrawColor(config.esp.colors.allies);
				interfaces.surface->drawLine(EntXPos + (EntWidth / 2), EntYPos + entHeight, interfaces.surface->getScreenSize().first / 2, interfaces.surface->getScreenSize().second);
			}
			if (entTeam != localTeam && target->isEnemy() && (config.esp.filter == 0 || config.esp.filter == 1))
			{
				interfaces.surface->setDrawColor(config.esp.colors.enemies);
				interfaces.surface->drawLine(EntXPos + (EntWidth / 2), EntYPos + entHeight, interfaces.surface->getScreenSize().first / 2, interfaces.surface->getScreenSize().second);
			}
		}
	}
	return;
}