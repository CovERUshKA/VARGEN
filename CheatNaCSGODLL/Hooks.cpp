#include "Hooks.h"
#include "Interfaces.h"
#include "Hacks/Hacks.h"
#include "SDK/UserCMD.h"
#include "Config.h"
#include "GUI.h"
#include "SDK/ViewSetup.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#include "SDK/FrameStage.h"
#include <math.h>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static LRESULT __stdcall hookedWndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (msg == WM_LBUTTONDOWN)
		config.keys[VK_LBUTTON] = true;
	if (msg == WM_LBUTTONUP)
		config.keys[VK_LBUTTON] = false;
	if (msg == WM_KEYDOWN && LOWORD(wParam) == VK_INSERT) {
		gui.isOpen = !gui.isOpen;
		if (!gui.isOpen) {
			ImGui::GetIO().MouseDown[0] = false;
			interfaces.inputSystem->resetInputState();
		}
	}
	if (gui.isOpen && !ImGui_ImplWin32_WndProcHandler(window, msg, wParam, lParam))
	{
		return true;
	}
	return CallWindowProcA(hooks.originalWndProc, window, msg, wParam, lParam);
}

static HRESULT __stdcall hookedPresent(IDirect3DDevice9Ex* device, const RECT* src, const RECT* dest, HWND windowOverride, const RGNDATA* dirtyRegion) noexcept
{
	static bool imguiInit{ ImGui_ImplDX9_Init(device) };

	if (gui.isOpen) {
		device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
		IDirect3DVertexDeclaration9 * vertexDeclaration;
		device->GetVertexDeclaration(&vertexDeclaration);

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		gui.render();

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		device->SetVertexDeclaration(vertexDeclaration);
		vertexDeclaration->Release();
	}

	return hooks.originalPresent(device, src, dest, windowOverride, dirtyRegion);
}

static bool __stdcall DispatchUserMessage_h(int msg_type, int unk1, int nBytes, void* msg_data) noexcept
{
	switch (msg_type)
	{
	case ECstrike15UserMessages::CS_UM_VoteStart:
	{
		//interfaces.gameUI->messageBox("CS_UM_VoteStart", "CS_UM_VoteStart");
		break;
	}
	default:
		break;
	}
	return hooks.client.callOriginal<bool, int, int, int, void*>(38, msg_type, unk1, nBytes, msg_data);
}

static void __stdcall hookedFrameStageNotify(int stage) noexcept
{

	hooks.client.callOriginal<void, int>(37, stage);
}

static void __stdcall hookedOverrideView(CViewSetup* pSetup)
{
	if (pSetup && interfaces.engine->isInGame() && interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer())->isAlive() && !interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer())->getProperty<bool>("m_bIsScoped"))
	{
		Visuals::FOV(pSetup);
	}

	hooks.clientMode.callOriginal<void, CViewSetup*>(18, pSetup);
}

struct SoundData {
	byte data[12];
	const char* soundEntry;
	byte pad[56];
};

static int __stdcall hookedDoPostScreenEffects(int param) noexcept
{
	if (interfaces.engine->isInGame()) {
		Visuals::modifySmoke();
		Visuals::ThirdPerson();
		Visuals::RadarHack();
		Visuals::NoFlash();
		Visuals::ShowImpacts();
	}
	return hooks.clientMode.callOriginal<int, int>(44, param);
}

static void __stdcall hookedPaintTraverse(unsigned int panel, bool forceRepaint, bool allowForce) noexcept
{
	if (strcmp(interfaces.panel->getName(panel), "MatSystemTopPanel") == NULL)
	{
		ESP::Render();
		if (config.aimbot.FOV.draw && interfaces.engine->isInGame() && interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer())->isAlive())
		{
			interfaces.surface->setDrawColor(config.aimbot.FOV.color);
			interfaces.surface->drawOutlinedCircle(interfaces.surface->getScreenSize().first / 2, interfaces.surface->getScreenSize().second / 2, config.aimbot.FOV.px /** (M_PI * 2)*/, 100);
		}
		if (gui.isOpen)
		{
			interfaces.surface->setDrawColor(0, 0, 0, 100);
			interfaces.surface->drawFilledRect(0, 0, interfaces.surface->getScreenSize().first, interfaces.surface->getScreenSize().second);
		}
	}
	if (strcmp(interfaces.panel->getName(panel), "HudZoom") == NULL && config.visuals.hudzoom.enabled)
	{
		Visuals::ModifyZoom();
		return;
	}

	hooks.panel.callOriginal<void, unsigned int, bool, bool>(41, panel, forceRepaint, allowForce);
}

static float __stdcall hookedGetViewModelFov() noexcept
{
	if (!config.visuals.viewmodel.fov)
	{
		auto vFOV = interfaces.cvar->findVar("viewmodel_fov");
		config.visuals.viewmodel.fov = vFOV->getInt();
	}
	Visuals::Viewmodel();

	if (interfaces.engine->isInGame() && interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer())->isAlive() && !interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer())->getProperty<bool>("m_bIsScoped"))
	{
		return static_cast<float>(config.visuals.viewmodel.fov);
	}
	
	return hooks.clientMode.callOriginal<float>(35);
}

static void __stdcall hookedDrawModelExecute(void* ctx, void* state, const ModelRenderInfo& info, matrix3x4* customBoneToWorld) noexcept
{
	if (interfaces.engine->isInGame() && !interfaces.modelRender->isMaterialOverriden()) {
		Chams::Render(ctx, state, info, customBoneToWorld);
		hooks.modelRender.callOriginal<void, void*, void*, const ModelRenderInfo&, matrix3x4*>(21, ctx, state, info, customBoneToWorld);
		interfaces.modelRender->forceMaterialOverride(nullptr);
	}
	else
		hooks.modelRender.callOriginal<void, void*, void*, const ModelRenderInfo&, matrix3x4*>(21, ctx, state, info, customBoneToWorld);
}

static void __stdcall hookedLockCursor() noexcept
{
	if (gui.isOpen)
		return interfaces.surface->unlockCursor();
	return hooks.surface.callOriginal<void>(67);
}

static bool __stdcall hookedCreateMove(float inputSampleTime, UserCmd* cmd) noexcept
{
	auto result = hooks.clientMode.callOriginal<bool, float, UserCmd*>(24, inputSampleTime, cmd);

	/*if (!cmd->command_number)
		return result;*/
	
	//bool& sendPacket = *reinterpret_cast<bool*>(*(static_cast<uintptr_t*>(_AddressOfReturnAddress()) - 1) - 0x1C);

	Misc::sniperCrosshair();
	//Visuals::skybox();
	Misc::FastDuck(cmd);
	Misc::FakeDuck(cmd);
	Misc::BunnyHop(cmd);
	Misc::autoStrafe(cmd);
	Aimbot::Execute(cmd);
	Triggerbot::Execute(cmd);
	Rage::Execute(cmd);
	AntiAim::Execute(cmd);
	Misc::autoPistol(cmd);
	//Misc::animateClanTag();
	cmd->viewangles.normalize();
	ClampAngles(cmd->viewangles);

	return false;
}

Hooks::Hooks() noexcept
{
	ImGui::CreateContext();
	auto window = FindWindowA("Valve001", NULL);
	ImGui_ImplWin32_Init(window);

	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 0.0f;
	style.ChildBorderSize = 0.0f;
	style.GrabMinSize = 7.0f;
	style.GrabRounding = 0.0f;
	style.FrameRounding = 0.0f;
	style.PopupRounding = 0.0f;
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;
	char buffer[MAX_PATH];
	if (GetWindowsDirectoryA(buffer, MAX_PATH))
	{
		strcat_s(buffer, "/Fonts/Tahoma.ttf");
		io.Fonts->AddFontFromFileTTF(buffer, 16.0f);
	}

	originalWndProc = reinterpret_cast<WNDPROC>(
		SetWindowLongPtr(window, GWLP_WNDPROC, LONG_PTR(hookedWndProc))
		);

	originalPresent = **reinterpret_cast<decltype(originalPresent) * *>(mem.present);
	**reinterpret_cast<void***>(mem.present) = reinterpret_cast<void*>(hookedPresent);

	panel.hookAt(41, hookedPaintTraverse);
	modelRender.hookAt(21, hookedDrawModelExecute);
	clientMode.hookAt(24, hookedCreateMove);
	clientMode.hookAt(35, hookedGetViewModelFov);
	clientMode.hookAt(44, hookedDoPostScreenEffects);
	client.hookAt(38, DispatchUserMessage_h);
	client.hookAt(37, hookedFrameStageNotify);
	clientMode.hookAt(18, hookedOverrideView);
	surface.hookAt(67, hookedLockCursor);

	interfaces.gameUI->messageBox("VARGEN", "It's me XD");
}

uintptr_t* Hooks::Vmt::findFreeDataPage(void* const base, size_t vmtSize) noexcept
{
	MEMORY_BASIC_INFORMATION mbi;
	VirtualQuery(base, &mbi, sizeof(mbi));
	MODULEINFO moduleInfo;
	GetModuleInformation(GetCurrentProcess(), static_cast<HMODULE>(mbi.AllocationBase), &moduleInfo, sizeof(moduleInfo));

	uintptr_t* moduleEnd{ reinterpret_cast<uintptr_t*>(static_cast<byte*>(moduleInfo.lpBaseOfDll) + moduleInfo.SizeOfImage) };

	for (auto currentAddress = moduleEnd - vmtSize; currentAddress > moduleInfo.lpBaseOfDll; currentAddress -= *currentAddress ? vmtSize : 1)
		if (!*currentAddress)
			if (VirtualQuery(currentAddress, &mbi, sizeof(mbi)) && mbi.State == MEM_COMMIT
				&& mbi.Protect == PAGE_READWRITE && mbi.RegionSize >= vmtSize * sizeof(uintptr_t)
				&& std::all_of(currentAddress, currentAddress + vmtSize, [](uintptr_t a) { return !a; }))
				return currentAddress;

	return nullptr;
}

auto Hooks::Vmt::calculateLength(uintptr_t * vmt) noexcept
{
	size_t length{ 0 };
	MEMORY_BASIC_INFORMATION memoryInfo;
	while (VirtualQuery(reinterpret_cast<LPCVOID>(vmt[length]), &memoryInfo, sizeof(memoryInfo)) && memoryInfo.Protect == PAGE_EXECUTE_READ)
		length++;
	return length;
}

Hooks::Vmt::Vmt(void* const base) noexcept
{
	this->base = base;
	oldVmt = *reinterpret_cast<uintptr_t * *>(base);
	length = calculateLength(oldVmt) + 1;

	if (newVmt = findFreeDataPage(base, length)) {
		std::copy(oldVmt - 1, oldVmt - 1 + length, newVmt);
		*reinterpret_cast<uintptr_t**>(base) = newVmt + 1;
	}
}