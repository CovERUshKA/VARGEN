#include "HackTools.h"
#include "Config.h"
#include "SDK/UserCMD.h"

/*Calc angle to target bone*/
Vector HackTools::AngelToTarget(Entity* localPlayer, Entity* target, int boneIndex)
{
	const Vector myView = localPlayer->getEyePosition();
	const Vector aimView = target->getBonePosition(boneIndex);

	Vector dst = CalcAngle(myView, aimView).ToVector();

	NormalizeAngles(dst);

	return dst;
}

/*get the best target*/
void HackTools::getTarget(float fovpx, Entity*& bufTarget, int& bone)
{
	float minDist = 360.f;
	float maxDist = 0.f;
	Vector _viewAngels;

	interfaces.engine->getViewAngles(_viewAngels);

	const int playerTeam = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer())->getProperty<int>("m_iTeamNum");

	for (int i = 0; i <= interfaces.engine->getMaxClients(); i++)
	{
		auto target = interfaces.entityList->getEntity(i);
		if (!target || target < 0 || !target->isAlive() || target == interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer()))
			continue;

		const int targetTeam = target->getProperty<int>("m_iTeamNum");

		if (!targetTeam || (!target->isEnemy() && config.aimbot.target == 1) || (target->isEnemy() && config.aimbot.target == 2))
			continue;

		if (target->isDormant() || target->getProperty<bool>("m_bGunGameImmunity"))
			continue;

		if (config.aimbot.head)
		{
			Vector2D w2s = hacktools.WorldToScreen(target->getBonePosition(8));

			float fov = GetFov(Vector2D(interfaces.surface->getScreenSize().first / 2, interfaces.surface->getScreenSize().second / 2), w2s);
			if (fov < minDist && fov <= fovpx && target->isVisible(target->getBonePosition(8)))
			{
				minDist = fov;
				bufTarget = target;
				bone = 8;
			}
		}
		else if (config.aimbot.chest)
		{
			Vector2D w2s = hacktools.WorldToScreen(target->getBonePosition(5));

			float fov = GetFov(Vector2D(interfaces.surface->getScreenSize().first / 2, interfaces.surface->getScreenSize().second / 2), w2s);
			if (fov < minDist && fov <= fovpx && target->isVisible(target->getBonePosition(5)))
			{
				minDist = fov;
				bufTarget = target;
				bone = 5;
			}
		}
		else if (config.aimbot.legs)
		{
			if (targetTeam == TeamNums::Counter_Terrorist)
			{
				Vector2D w2s1 = hacktools.WorldToScreen(target->getBonePosition(73));

				Vector2D w2s2 = hacktools.WorldToScreen(target->getBonePosition(82));

				float fov1 = GetFov(Vector2D(interfaces.surface->getScreenSize().first / 2, interfaces.surface->getScreenSize().second / 2), w2s1);
				float fov2 = GetFov(Vector2D(interfaces.surface->getScreenSize().first / 2, interfaces.surface->getScreenSize().second / 2), w2s2);
				if (fov1 < fov2)
				{
					if (fov1 < minDist && fov1 <= fovpx && target->isVisible(target->getBonePosition(73)))
					{
						minDist = fov1;
						bufTarget = target;
						bone = 73;
					}
				}
				else
				{
					if (fov2 < minDist && fov2 <= fovpx && target->isVisible(target->getBonePosition(82)))
					{
						minDist = fov2;
						bufTarget = target;
						bone = 82;
					}
				}
			}
			else if (targetTeam == TeamNums::Terrorist)
			{
				Vector2D w2s1 = hacktools.WorldToScreen(target->getBonePosition(74));

				Vector2D w2s2 = hacktools.WorldToScreen(target->getBonePosition(67));

				float fov1 = GetFov(Vector2D(interfaces.surface->getScreenSize().first / 2, interfaces.surface->getScreenSize().second / 2), w2s1);
				float fov2 = GetFov(Vector2D(interfaces.surface->getScreenSize().first / 2, interfaces.surface->getScreenSize().second / 2), w2s2);
				if (fov1 < fov2)
				{
					if (fov1 < minDist && fov1 <= fovpx && target->isVisible(target->getBonePosition(74)))
					{
						minDist = fov1;
						bufTarget = target;
						bone = 74;
					}
				}
				else
				{
					if (fov2 < minDist && fov2 <= fovpx && target->isVisible(target->getBonePosition(67)))
					{
						minDist = fov2;
						bufTarget = target;
						bone = 67;
					}
				}
			}
		}
	}
}

Vector2D HackTools::WorldToScreen(const Vector world)
{
	auto flMatrix = interfaces.engine->worldToScreenMatrix();
	float w = 0.0f;
	HWND cswindow = FindWindowA(0, "Counter-Strike: Global Offensive");
	RECT m_Rect;
	GetWindowRect(cswindow, &m_Rect);
	float screen_x = flMatrix[0][0] * world.x + flMatrix[0][1] * world.y + flMatrix[0][2] * world.z + flMatrix[0][3];
	float screen_y = flMatrix[1][0] * world.x + flMatrix[1][1] * world.y + flMatrix[1][2] * world.z + flMatrix[1][3];
	w = flMatrix[3][0] * world.x + flMatrix[3][1] * world.y + flMatrix[3][2] * world.z + flMatrix[3][3];

	if (w < 0.01f)
	{
		return Vector2D(NULL, NULL);
	}

	const float invw = 1.0f / w;
	screen_x *= invw;
	screen_y *= invw;

	int width = static_cast<int>(m_Rect.right - m_Rect.left);
	int height = static_cast<int>(m_Rect.bottom - m_Rect.top);

	float x = float(width / 2);
	float y = float(height / 2);

	x += float(screen_x * x);
	y -= float(screen_y * y);

	screen_x = x + m_Rect.left;
	screen_y = y + m_Rect.top;

	return Vector2D(screen_x, screen_y);
}

Entity* HackTools::GetVisibleHurtablePlayer(int sortby, int filter)
{
	if (!interfaces.engine->isInGame() || !interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer())->isAlive())
		return NULL;

	int curBest{ 0 };
	float minDist{ 99999.0f };
	float maxDist{ 0.0f };
	Entity* player{ NULL };

	auto localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
	auto localPlayerPos = localPlayer->getAbsOrigin();

	for (int i = 0; i < interfaces.engine->getMaxClients(); i++)
	{
		auto entity = interfaces.entityList->getEntity(i);
		if (!entity || entity->isDormant() || entity == localPlayer || entity->getProperty<bool>("m_bGunGameImmunity") || !entity->isAlive() || (!entity->isEnemy() && filter == 1) || (entity->isEnemy() && filter == 2)) continue;

		float dist{ entity->getAbsOrigin().DistTo(localPlayerPos) };
		if (dist < minDist && entity->isVisible(entity->getBonePosition(8)) && sortby == 0)
		{
			minDist = dist;
			player = entity;
		}
		else if (dist > maxDist && entity->isVisible(entity->getBonePosition(8)) && sortby == 1)
		{
			maxDist = dist;
			player = entity;
		}
	}
	return player;
}
