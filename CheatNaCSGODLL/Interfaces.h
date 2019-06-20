#pragma once
#include "SDK/GameUI.h"
#include "SDK/Panel.h"
#include "SDK/Surface.h"
#include "SDK/EntityList.h"
#include "SDK/EngineTrace.h"
#include "SDK/Engine.h"
#include "SDK/ModelRender.h"
#include "SDK/MaterialSystem.h"
#include "SDK/Client.h"
#include "SDK/Cvar.h"
#include "SDK/InputSystem.h"
#include "SDK/ModelInfo.h"

class Sound;

class Interfaces final {
public:
	Interfaces() noexcept;

	GameUI* gameUI;
	Cvar* cvar;
	Panel* panel;
	Surface* surface;
	EntityList* entityList;
	EngineTrace* engineTrace;
	Engine* engine;
	ModelRender* modelRender;
	MaterialSystem* materialSystem;
	Client* client;
	InputSystem* inputSystem;
	Sound* sound;
	ModelInfo* modelInfo;
private:
	template <typename T>
	static auto find(const char* module, const char* name)
	{
		const auto createInterface = reinterpret_cast<std::add_pointer_t<T * (const char* name, int* returnCode)>>(GetProcAddress(GetModuleHandleA(module), "CreateInterface"));

		T* foundInterface{ nullptr };

		if (createInterface)
			foundInterface = createInterface(name, nullptr);

		if (foundInterface)
			return foundInterface;
		else {
			MessageBoxA(NULL, "Error", "Error", MB_OK | MB_ICONERROR);
			exit(EXIT_FAILURE);
		}
	}
};

extern Interfaces interfaces;
