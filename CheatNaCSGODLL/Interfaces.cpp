#include "Interfaces.h"

Interfaces::Interfaces() noexcept
{
	cvar = find<Cvar>("vstdlib", "VEngineCvar007");
	gameUI = find<GameUI>("client_panorama", "GameUI011");
	panel = find<Panel>("vgui2", "VGUI_Panel009");
	surface = find<Surface>("vguimatsurface", "VGUI_Surface031");
	entityList = find<EntityList>("client_panorama", "VClientEntityList003");
	engineTrace = find<EngineTrace>("engine", "EngineTraceClient004");
	engine = find<Engine>("engine", "VEngineClient014");
	modelInfo = find<ModelInfo>("engine", "VModelInfoClient004");
	modelRender = find<ModelRender>("engine", "VEngineModel016");
	materialSystem = find<MaterialSystem>("materialsystem", "VMaterialSystem080");
	client = find<Client>("client_panorama", "VClient018");
	inputSystem = find<InputSystem>("inputsystem", "InputSystemVersion001");
	sound = find<Sound>("engine", "IEngineSoundClient003");
}