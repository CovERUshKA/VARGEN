#include "Visuals.h"
#include "../Interfaces.h"
#include "../Config.h"
#include "../Memory.h"
#include "../SDK/ViewSetup.h"
#include "../SDK/FrameStage.h"

void Visuals::Viewmodel() noexcept
{
	if (config.visuals.viewmodel.x == -21)
	{
		config.visuals.viewmodel.cl_bob_lower_amt = interfaces.cvar->findVar("cl_bob_lower_amt")->getFloat();
		config.visuals.viewmodel.cl_bobamt_lat = interfaces.cvar->findVar("cl_bobamt_lat")->getFloat();
		config.visuals.viewmodel.cl_bobamt_vert = interfaces.cvar->findVar("cl_bobamt_vert")->getFloat();
		config.visuals.viewmodel.cl_bobcycle = interfaces.cvar->findVar("cl_bobcycle")->getFloat();
	}

	const auto viewmodel_offset_x = interfaces.cvar->findVar("viewmodel_offset_x");
	const auto viewmodel_offset_y = interfaces.cvar->findVar("viewmodel_offset_y");
	const auto viewmodel_offset_z = interfaces.cvar->findVar("viewmodel_offset_z");

	if (config.visuals.viewmodel.x == -21)
	{
		config.visuals.viewmodel.x = viewmodel_offset_x->getInt();
		config.visuals.viewmodel.y = viewmodel_offset_y->getInt();
		config.visuals.viewmodel.z = viewmodel_offset_z->getInt();
	}

	if (!interfaces.engine->isInGame() || !interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer())->isAlive())
		return;

	const auto sv_cheats = interfaces.cvar->findVar("sv_cheats");
	if (!sv_cheats->getInt())
		sv_cheats->setValue(1);

	const auto sv_competitive_minspec = interfaces.cvar->findVar("sv_competitive_minspec");
	if (sv_competitive_minspec->getInt())
		sv_competitive_minspec->setValue(0);

	interfaces.cvar->findVar("cl_bob_lower_amt")->setValue(config.visuals.viewmodel.cl_bob_lower_amt);
	interfaces.cvar->findVar("cl_bobamt_lat")->setValue(config.visuals.viewmodel.cl_bobamt_lat);
	interfaces.cvar->findVar("cl_bobamt_vert")->setValue(config.visuals.viewmodel.cl_bobamt_vert);
	interfaces.cvar->findVar("cl_bobcycle")->setValue(config.visuals.viewmodel.cl_bobcycle);

	viewmodel_offset_x->setValue(config.visuals.viewmodel.x);

	viewmodel_offset_y->setValue(config.visuals.viewmodel.y);

	viewmodel_offset_z->setValue(config.visuals.viewmodel.z);
	return;
}

void Visuals::modifySmoke() noexcept
{
	static constexpr const char* smokeMaterials[]{
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
	};

	for (const auto mat : smokeMaterials) {
		auto material = interfaces.materialSystem->findMaterial(mat);
		material->setMaterialVarFlag(MaterialVar::NO_DRAW, config.visuals.smoke.no);
		material->setMaterialVarFlag(MaterialVar::WIREFRAME, config.visuals.smoke.wireframe);
	}

	if (config.visuals.smoke.no || config.visuals.smoke.wireframe)
		*mem.smokeCount = 0;
}

void Visuals::ThirdPerson() noexcept
{
	Vector vecAngles = Vector(0, 0, 0);
	interfaces.engine->getViewAngles(vecAngles);
	if (config.visuals.thirdperson.enabled && interfaces.engine->isInGame() && interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer())->isAlive())
	{
		const auto sv_cheats = interfaces.cvar->findVar("sv_cheats");
		if (!sv_cheats->getInt())
			sv_cheats->setValue(1);

		if (!mem.input->isCameraInThirdPerson)
		{
			mem.input->isCameraInThirdPerson = true;
		}
		mem.input->cameraOffset.z = config.visuals.thirdperson.pos;
	}
	else
	{
		if (mem.input->isCameraInThirdPerson)
		{
			mem.input->isCameraInThirdPerson = false;
			mem.input->cameraOffset.z = 0;
		}
	}

	return;
}

void Visuals::ShowImpacts() noexcept
{
	if (config.visuals.showimpacts && interfaces.engine->isInGame())
	{
		const auto sv_cheats = interfaces.cvar->findVar("sv_cheats");
		if (!sv_cheats->getInt())
			sv_cheats->setValue(1);

		const auto sv_showimpacts = interfaces.cvar->findVar("sv_showimpacts");

		if (!sv_showimpacts->getInt())
		{
			sv_showimpacts->setValue(1);
		}
	}
	else
	{
		const auto sv_cheats = interfaces.cvar->findVar("sv_cheats");
		if (!sv_cheats->getInt())
			return;

		const auto sv_showimpacts = interfaces.cvar->findVar("sv_showimpacts");
		if (sv_showimpacts->getInt() > 0)
		{
			sv_showimpacts->setValue(0);
		}
	}

	return;
}

void Visuals::ModifyZoom() noexcept
{
	const auto localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
	if (localPlayer->getProperty<bool>("m_bIsScoped") && interfaces.engine->isInGame())
	{
		const auto activeWeaponNumber = localPlayer->getProperty<DWORD>("m_hActiveWeapon");
		Entity* activeWeapon = interfaces.entityList->getEntityFromHandle(activeWeaponNumber);
		if (activeWeapon->isWeapon() && activeWeapon->isSniperRifle())
		{
			interfaces.surface->setDrawColor(config.visuals.hudzoom.color);
			auto screensize = interfaces.surface->getScreenSize();
			interfaces.surface->drawFilledRect(screensize.first / 2 - (config.visuals.hudzoom.width / 2), 0, screensize.first / 2 + (config.visuals.hudzoom.width / 2), screensize.second);
			interfaces.surface->drawFilledRect(0, screensize.second / 2 - (config.visuals.hudzoom.width / 2), screensize.first, screensize.second / 2 + (config.visuals.hudzoom.width / 2));
			return;
		}
	}

	return;
}

void Visuals::RadarHack() noexcept
{
	if (!config.visuals.radarhack || !interfaces.engine->isInGame())
		return;

	auto localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
	int localTeam = localPlayer->getProperty<int>("m_iTeamNum");

	for (int i = 0; i < interfaces.engine->getMaxClients(); i++)
	{
		auto pEnt = interfaces.entityList->getEntity(i);
		if (!pEnt || localPlayer == pEnt || !pEnt->isAlive())
			continue;

		int entTeam = pEnt->getProperty<int>("m_iTeamNum");
		if (!entTeam || entTeam == localTeam)
			continue;

		pEnt->setProperty("m_bSpotted", TRUE);
	}
	return;

	return;
}

void Visuals::NoFlash() noexcept
{
	if (!config.visuals.noflash)
		return;

	auto localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
	if (!localPlayer || !localPlayer->isAlive())
		return;

	if (localPlayer->getProperty<float>("m_flFlashDuration") > 0.0f)
	{
		localPlayer->setProperty("m_flFlashDuration", 0.0f);
	}
	return;
}

void Visuals::FOV(CViewSetup* pSetup) noexcept
{
	pSetup->fov = config.visuals.fov;
	return;
}