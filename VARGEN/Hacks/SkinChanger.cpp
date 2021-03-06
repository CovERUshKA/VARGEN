#include "SkinChanger.h"
#include "../SDK/FrameStage.h"
#include "../Interfaces.h"
#include "../SDK/Entity.h"

void SkinChanger::Execute(int stage) noexcept
{
	if (stage == FrameStage::NET_UPDATE_POSTDATAUPDATE_START)
	{
		const auto localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
		if (!localPlayer || !localPlayer->isAlive())
			return;

		const auto activeWeapon = interfaces.entityList->getEntityFromHandle(localPlayer->getProperty<int>("m_hActiveWeapon"));
		if (!activeWeapon || !activeWeapon->isKnife())
			return;

		activeWeapon->setProperty("m_iItemDefinitionIndex", 508);
		activeWeapon->setProperty("m_OriginalOwnerXuidLow", 0);
		activeWeapon->setProperty("m_OriginalOwnerXuidHigh", 0);
		activeWeapon->setProperty("m_iViewModelIndex", 379);
		activeWeapon->setProperty("m_iWorldModelIndex", 380);
		activeWeapon->setProperty("m_iItemIDLow", -1);
		activeWeapon->setProperty("m_iItemIDHigh", -1);
		activeWeapon->setProperty("m_nFallbackStatTrak", 420);
		activeWeapon->setProperty("m_nFallbackPaintKit", 568);
		activeWeapon->setProperty("m_flFallbackWear", 0.000000001);
	}
	return;
}