#include "Chams.h"
#include "../Interfaces.h"
#include "../Config.h"
#include <fstream>
#include "../SDK/Entity.h"

void applyChams(float color[4],/*Material* chams,*/ bool ignorez/*, int health = 0*/) noexcept
{
	std::ofstream{ "csgo/materials/chamsNormal.vmt" } <<
		"VertexLitGeneric { }";

	auto material = interfaces.materialSystem->findMaterial("chamsNormal");
	material->incrementReferenceCount();
	/*if (chams.healthBased && health)
		material->colorModulate({ 1.0f - health / 100.0f,  health / 100.0f, 0.0f });
	else if (chams.rainbow)
		material->colorModulate({ sinf(0.6f * memory.globalVars->currenttime) * 0.5f + 0.5f,
								  sinf(0.6f * memory.globalVars->currenttime + 2.0f) * 0.5f + 0.5f,
								  sinf(0.6f * memory.globalVars->currenttime + 4.0f) * 0.5f + 0.5f });
	else*/
	material->colorModulate(color);
	material->alphaModulate(color[3]);
	material->setMaterialVarFlag(MaterialVar::IGNOREZ, ignorez);
	material->setMaterialVarFlag(MaterialVar::WIREFRAME, config.chams[0].wireframe);
	interfaces.modelRender->forceMaterialOverride(material);
}

void Chams::Render(void* ctx, void* state, const ModelRenderInfo& info, matrix3x4* customBoneToWorld) noexcept
{
	if (!config.chams[0].enabled && !config.chams[1].enabled)
		return;

	if (strstr(info.model->name, "models/player"))
	{
		auto entity = interfaces.entityList->getEntity(info.entityIndex);
		if (!entity || entity->isDormant())
			return;
		if (config.chams[0].enabled)
		{
			if (entity->isEnemy() && (config.chams[0].filter == 0 || config.chams[0].filter == 1))
				applyChams(config.chams[0].colors.enemies, 0);

			if (!entity->isEnemy() && (config.chams[0].filter == 0 || config.chams[0].filter == 2))
				applyChams(config.chams[0].colors.allies, 0);
		}
		if (config.chams[1].enabled)
		{
			if (entity->isEnemy() && (config.chams[1].filter == 0 || config.chams[1].filter == 1))
				applyChams(config.chams[1].colors.enemies, 1);

			if (!entity->isEnemy() && (config.chams[1].filter == 0 || config.chams[1].filter == 2))
				applyChams(config.chams[1].colors.allies, 1);
		}
	}
	return;
}