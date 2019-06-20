#include "framework.h"
#include <string>
#include <Windows.h>
#include "GUI.h"
#include "Config.h"
#include "Hooks.h"
#include "Hacks/Hacks.h"

constexpr auto windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

void GUI::render() noexcept
{
	renderMainWindow();
	renderAimbotWindow();
	renderRageWindow();
	renderTriggerbotWindow();
	renderAntiAimWindow();
	renderESPWindow();
	renderChamsWindow();
	renderVisualsWindow();
	renderMiscWindow();
	renderColorsWindow();
	ImGui::End();
}

/*void GUI::checkboxedColorPicker(const std::string& name, bool* enable, float* color) noexcept
{
	ImGui::Checkbox(("##" + name).c_str(), enable);
	ImGui::SameLine(0.0f, 5.0f);
	ImGui::PushID(0);
	bool openPopup = ImGui::ColorButton(("##" + name).c_str(), ImColor{ color[0], color[1], color[2] }, ImGuiColorEditFlags_NoTooltip);
	ImGui::PopID();
	ImGui::SameLine(0.0f, 5.0f);
	ImGui::Text(name.c_str());
	ImGui::PushID(1);
	if (openPopup)
		ImGui::OpenPopup(("##" + name).c_str());
	if (ImGui::BeginPopup(("##" + name).c_str())) {
		ImGui::PushID(2);
		ImGui::ColorPicker3(("##" + name).c_str(), color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview);
		ImGui::PopID();
		ImGui::EndPopup();
	}
	ImGui::PopID();
}*/

void GUI::Button(const char* label, bool* p_open, const ImVec2& size) noexcept
{
	if (*p_open)
	{
		ImGui::ButtonEx(label, size, 0);
		return;
	}

	if (ImGui::ButtonEx(label, size, 0) && !*p_open)
	{
		window.aimbot = false;
		window.antiaim = false;
		window.chams = false;
		window.colors = false;
		window.config = false;
		window.esp = false;
		window.misc = false;
		window.rage = false;
		window.triggerbot = false;
		window.visuals = false;
		*p_open = true;
	}
	
	return;
}

void GUI::renderMainWindow() noexcept
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImGui::SetNextWindowSize({ 750.0f, 520.0f });
	ImGui::Begin("VARGEN", NULL, windowFlags);
	float width = 750.0f / 9;
	float height = 30.0f;
	Button("Aimbot", &window.aimbot, { width, height });
	ImGui::SameLine(0, 0);
	Button("Rage", &window.rage, { width, height });
	ImGui::SameLine(0, 0);
	Button("Triggerbot", &window.triggerbot, { width, height });
	ImGui::SameLine(0, 0);
	Button("Anti-Aim", &window.antiaim, { width, height });
	ImGui::SameLine(0, 0);
	Button("ESP", &window.esp, { width, height });
	ImGui::SameLine(0, 0);
	Button("Chams", &window.chams, { width, height });
	ImGui::SameLine(0, 0);
	Button("Visuals", &window.visuals, { width, height });
	ImGui::SameLine(0, 0);
	Button("Misc", &window.misc, { width, height });
	ImGui::SameLine(0, 0);
	Button("Colors", &window.colors, { width, height });
	ImGui::NewLine();
}

void GUI::renderAimbotWindow() noexcept
{
	if (window.aimbot) {
		ImGui::Checkbox("Enabled", &config.aimbot.enabled);
		ImGui::SameLine();
		ImGui::PushID(1);
		ImGui::Combo("Target", &config.aimbot.target, "All\0Enemies\0Allies\0");
		ImGui::PopID();
		ImGui::NewLine();
		ImGui::SliderInt("FOV", &config.aimbot.FOV.px, 0, 360);
		ImGui::SameLine();
		ImGui::Checkbox("Draw FOV", &config.aimbot.FOV.draw);
		ImGui::SameLine();
		ImGui::ColorEdit4("", config.aimbot.FOV.color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::SliderFloat("Smooth", &config.aimbot.smooth, 1.0f, 250.0f);
		ImGui::Checkbox("Head", &config.aimbot.head);
		ImGui::SameLine();
		ImGui::Checkbox("Chest", &config.aimbot.chest);
		ImGui::SameLine();
		ImGui::Checkbox("Legs", &config.aimbot.legs);
		ImGui::Checkbox("Only Zoom", &config.aimbot.onlyzoom);
		ImGui::Checkbox("Ignore Smoke", &config.aimbot.ignoreSmoke);
		ImGui::SameLine();
		ImGui::Checkbox("Ignore Flash", &config.aimbot.ignoreflash);
	}
}

void GUI::renderRageWindow() noexcept
{
	if (window.rage) {
		ImGui::Checkbox("Enabled", &config.rage.enabled);
		ImGui::SameLine();
		ImGui::PushID(1);
		ImGui::Combo("Target", &config.rage.target, "All\0Enemies\0Allies\0");
		ImGui::PopID();
		ImGui::NewLine();
		ImGui::PushID(0);
		ImGui::Combo("Distance", &config.rage.distance, "Nearest\0Furthest\0");
		ImGui::PopID();
		ImGui::Checkbox("Only Zoom", &config.rage.onlyzoom);
		ImGui::Checkbox("Ignore Smoke", &config.rage.ignoreSmoke);
		ImGui::SameLine();
		ImGui::Checkbox("Ignore Flash", &config.rage.ignoreflash);
	}
}

void GUI::renderTriggerbotWindow() noexcept
{
	if (window.triggerbot) {
		ImGui::Checkbox("Enabled", &config.triggerbot.enabled);
		ImGui::SameLine();
		ImGui::PushID(1);
		ImGui::Combo("Target", &config.triggerbot.target, "All\0Enemies\0Allies\0");
		ImGui::PopID();
		ImGui::Checkbox("Only Zoom", &config.triggerbot.onlyzoom);
		ImGui::Checkbox("Ignore Smoke", &config.triggerbot.ignoreSmoke);
		ImGui::SameLine();
		ImGui::Checkbox("Ignore Flash", &config.triggerbot.ignoreflash);
	}
}

void GUI::renderAntiAimWindow() noexcept
{
	if (window.antiaim) {
		ImGui::Checkbox("Enabled", &config.antiaim.enabled);
		ImGui::NewLine();
	}
}

void GUI::renderESPWindow() noexcept
{
	if (window.esp) {
		ImGui::Checkbox("Enabled", &config.esp.enabled);
		ImGui::SameLine();
		ImGui::PushID(0);
		ImGui::Combo("", &config.esp.filter, "All\0Enemies\0Allies\0");
		ImGui::PopID();
		ImGui::NewLine();
		ImGui::Checkbox("Box", &config.esp.box.enabled);
		ImGui::SameLine();
		ImGui::Checkbox("Corners", &config.esp.box.corners);
		ImGui::SameLine();
		ImGui::Checkbox("Nickname", &config.esp.nickname.enabled);
		ImGui::SameLine(0, 0);
		ImGui::ColorEdit4("", config.esp.nickname.color, ImGuiColorEditFlags_NoInputs, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::SameLine();
		ImGui::Checkbox("HP", &config.esp.hp);
		ImGui::SameLine();
		ImGui::Checkbox("Armor", &config.esp.armor);
		ImGui::Checkbox("Line", &config.esp.line.enabled);
	}
}

void GUI::renderChamsWindow() noexcept
{
	if (window.chams) {
		ImGui::PushID(1);
		ImGui::SetNextItemWidth(100);
		ImGui::Combo("", &config.chamstype, "Visible\0Invisible\0");
		ImGui::PopID();
		if (!config.chamstype)
		{
			ImGui::Checkbox("Enabled", &config.chams[0].enabled);
			ImGui::SameLine();
			ImGui::PushID(0);
			ImGui::SetNextItemWidth(100);
			ImGui::Combo("", &config.chams[0].filter, "All\0Enemies\0Allies\0");
			ImGui::PopID();
			ImGui::NewLine();
			ImGui::Checkbox("Wireframe", &config.chams[0].wireframe);
		}
		else
		{
			ImGui::Checkbox("Enabled", &config.chams[1].enabled);
			ImGui::SameLine();
			ImGui::PushID(0);
			ImGui::SetNextItemWidth(100);
			ImGui::Combo("", &config.chams[1].filter, "All\0Enemies\0Allies\0");
			ImGui::PopID();
			ImGui::NewLine();
			ImGui::Checkbox("Wireframe", &config.chams[1].wireframe);
		}
	}
}

void GUI::renderVisualsWindow() noexcept
{
	if (window.visuals) {
		ImGui::Checkbox("Third Person", &config.visuals.thirdperson.enabled);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(250);
		ImGui::SliderInt("Camera Pos", &config.visuals.thirdperson.pos, 0, 200);
		ImGui::Checkbox("Modify Zoom", &config.visuals.hudzoom.enabled);
		ImGui::SameLine(0, 0);
		ImGui::ColorEdit4("ModifyZoom", config.visuals.hudzoom.color, ImGuiColorEditFlags_NoInputs, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(150);
		ImGui::SliderInt("width", &config.visuals.hudzoom.width, 2, 10);
		ImGui::Checkbox("Radar Hack", &config.visuals.radarhack);
		ImGui::Checkbox("No Flash", &config.visuals.noflash);
		ImGui::Checkbox("No Smoke ", &config.visuals.smoke.no);
		ImGui::SameLine();
		ImGui::Checkbox("Wireframe Smoke", &config.visuals.smoke.wireframe);
		ImGui::Checkbox("Show Impacts", &config.visuals.showimpacts);
		ImGui::SetNextItemWidth(250);
		ImGui::SliderInt("FOV", &config.visuals.fov, 30, 170);
		ImGui::SetNextItemWidth(250);
		ImGui::SliderInt("Viewmodel FOV", &config.visuals.viewmodel.fov, 1, 175);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		ImGui::SliderInt("X", &config.visuals.viewmodel.x, -20, 20);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		ImGui::SliderInt("Y", &config.visuals.viewmodel.y, -20, 20);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		ImGui::SliderInt("Z", &config.visuals.viewmodel.z, -20, 20);
	}
}

void GUI::renderMiscWindow() noexcept
{
	if (window.misc)
	{
		static char buffer[16];
		ImGui::PushItemWidth(120.0f);
		ImGui::InputText("", buffer, IM_ARRAYSIZE(buffer));
		ImGui::SameLine();
		if (ImGui::Button("Set clantag")) {
			Misc::setClanTag(buffer);
			//Misc::animateClanTag(buffer);
		}
		ImGui::Checkbox("Animated clan tag", &config.misc.clantag.animate);
		ImGui::Checkbox("Auto-Pistol", &config.misc.autopistol);
		ImGui::Checkbox("Sniper Crosshair", &config.misc.snipercrosshair);
		//ImGui::Checkbox("AntiKick", &config.Misc.AntiKick);
		ImGui::Checkbox("BunnyHop", &config.misc.bunnyhop);
		ImGui::SameLine();
		ImGui::Checkbox("Auto-Strafe", &config.misc.autostrafe);
		ImGui::Checkbox("Fast Duck", &config.misc.fastduck);
		//ImGui::Checkbox("BombTimer", &config.Misc.BombTimer.Enabled);
		//ImGui::Checkbox("Auto-Accept", &config.Misc.autoAccept);
	}
}

void GUI::renderColorsWindow() noexcept
{
	if (window.colors) {
		ImGuiColorEditFlags colorpickflags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar;
		ImGui::Text("ESP");
		ImGui::ColorEdit4("Enemy", config.esp.colors.enemies, ImGuiColorEditFlags_NoInputs, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4("Team", config.esp.colors.allies, ImGuiColorEditFlags_NoInputs, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::NewLine();
		ImGui::Text("Chams");
		ImGui::ColorEdit4("Enemy", config.chams[0].colors.enemies, ImGuiColorEditFlags_NoInputs, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4("Team", config.chams[0].colors.allies, ImGuiColorEditFlags_NoInputs, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4("Enemy", config.chams[1].colors.enemies, ImGuiColorEditFlags_NoInputs, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4("Team", config.chams[1].colors.allies, ImGuiColorEditFlags_NoInputs, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
	}
}

/*void GUI::renderTriggerbotWindow() noexcept
{
	if (window.triggerbot) {
		ImGui::SetNextWindowSize({ 250.0f, 270.0f });
		ImGui::Begin("Triggerbot", &window.triggerbot, windowFlags);
		static int currentCategory{ 0 };
		ImGui::PushItemWidth(110.0f);
		ImGui::PushID(0);
		ImGui::Combo("", &currentCategory, "All\0Pistols\0Heavy\0SMG\0Rifles\0");
		ImGui::PopID();
		ImGui::SameLine();
		static int currentWeapon{ 0 };
		ImGui::PushID(1);
		switch (currentCategory) {
		case 0:
			currentWeapon = 0;
			ImGui::NewLine();
			break;
		case 1: {
			static int currentPistol{ 0 };
			ImGui::Combo("", &currentPistol, "Glock-18\0P2000\0USP-S\0Dual Berettas\0P250\0Tec-9\0Five-Seven\0CZ-75\0Desert Eagle\0Revolver\0");
			currentWeapon = currentPistol + 1;
			break;
		}
		case 2: {
			static int currentHeavy{ 0 };
			ImGui::Combo("", &currentHeavy, "Nova\0XM1014\0Sawed-off\0MAG-7\0M249\0Negev\0");
			currentWeapon = currentHeavy + 11;
			break;
		}
		case 3: {
			static int currentSmg{ 0 };
			ImGui::Combo("", &currentSmg, "Mac-10\0MP9\0MP7\0MP5-SD\0UMP-45\0P90\0PP-Bizon\0");
			currentWeapon = currentSmg + 17;
			break;
		}
		case 4: {
			static int currentRifle{ 0 };
			ImGui::Combo("", &currentRifle, "Galil AR\0Famas\0AK-47\0M4A4\0M4A1-S\0SSG-08\0SG-553\0AUG\0AWP\0G3SG1\0SCAR-20\0");
			currentWeapon = currentRifle + 24;
			break;
		}
		}
		ImGui::PopID();
		ImGui::Checkbox("Enabled", &config.triggerbot[currentWeapon].enabled);
		ImGui::Checkbox("On key", &config.triggerbot[currentWeapon].onKey);
		ImGui::SameLine();
		hotkey(config.triggerbot[currentWeapon].key);
		ImGui::Checkbox("Friendly fire", &config.triggerbot[currentWeapon].friendlyFire);
		ImGui::Checkbox("Scoped only", &config.triggerbot[currentWeapon].scopedOnly);
		ImGui::Checkbox("Ignore flash", &config.triggerbot[currentWeapon].ignoreFlash);
		ImGui::Checkbox("Ignore smoke", &config.triggerbot[currentWeapon].ignoreSmoke);
		ImGui::PushItemWidth(85.0f);
		ImGui::Combo("Hitgroup", &config.triggerbot[currentWeapon].hitgroup, "All\0Head\0Chest\0Stomach\0Left arm\0Right arm\0Left leg\0Right leg\0");
		ImGui::PushItemWidth(220.0f);
		ImGui::SliderInt("", &config.triggerbot[currentWeapon].shotDelay, 0, 250, "Shot delay: %d ms");
		ImGui::End();
	}
}

void GUI::renderBacktrackWindow() noexcept
{
	if (window.backtrack) {
		ImGui::SetNextWindowSize({ 260.f, 125.f });
		ImGui::Begin("Backtrack", &window.backtrack, windowFlags);
		ImGui::Checkbox("Enabled", &config.backtrack.enabled);
		ImGui::Checkbox("Ignore smoke", &config.backtrack.ignoreSmoke);
		ImGui::PushItemWidth(220.0f);
		ImGui::SliderInt("", &config.backtrack.timeLimit, 1, 200, "Time limit: %d ms");
		ImGui::PopItemWidth();
		ImGui::End();
	}
}

void GUI::renderGlowWindow() noexcept
{
	if (window.glow) {
		ImGui::SetNextWindowSize({ 250.0f, 245.0f });
		ImGui::Begin("Glow", &window.glow, windowFlags);
		static int currentCategory{ 0 };
		ImGui::PushItemWidth(110.0f);
		ImGui::PushID(0);
		ImGui::Combo("", &currentCategory, "Allies\0Enemies\0Planting\0Defusing\0Local player\0Weapons\0C4\0Planted C4\0Chickens\0");
		ImGui::PopID();
		static int currentItem{ 0 };
		if (currentCategory <= 3) {
			ImGui::SameLine();
			static int currentType{ 0 };
			ImGui::PushID(1);
			ImGui::Combo("", &currentType, "Visible\0Occluded\0");
			ImGui::PopID();
			currentItem = currentCategory * 2 + currentType;
		}
		else {
			currentItem = currentCategory + 4;
		}

		ImGui::Checkbox("Enabled", &config.glow[currentItem].enabled);
		ImGui::Checkbox("Health based", &config.glow[currentItem].healthBased);
		ImGui::Checkbox("Rainbow", &config.glow[currentItem].rainbow);
		bool openPopup = ImGui::ColorButton("Color", ImVec4{ config.glow[currentItem].color }, ImGuiColorEditFlags_NoTooltip);
		ImGui::SameLine(0.0f, 5.0f);
		ImGui::Text("Color");
		ImGui::PushID(2);
		if (openPopup)
			ImGui::OpenPopup("");
		if (ImGui::BeginPopup("")) {
			ImGui::PushID(3);
			ImGui::ColorPicker3("", config.glow[currentItem].color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview);
			ImGui::PopID();
			ImGui::EndPopup();
		}
		ImGui::PopID();
		ImGui::PushItemWidth(220.0f);
		ImGui::PushID(4);
		ImGui::SliderFloat("", &config.glow[currentItem].thickness, 0.0f, 1.0f, "Thickness: %.2f");
		ImGui::PopID();
		ImGui::PushID(5);
		ImGui::SliderFloat("", &config.glow[currentItem].alpha, 0.0f, 1.0f, "Alpha: %.2f");
		ImGui::PopID();
		ImGui::PushID(6);
		ImGui::SliderInt("", &config.glow[currentItem].style, 0, 3, "Style: %d");
		ImGui::PopID();
		ImGui::End();
	}
}

void GUI::renderChamsWindow() noexcept
{
	if (window.chams) {
		ImGui::SetNextWindowSize({ 250.0f, 245.0f });
		ImGui::Begin("Chams", &window.chams, windowFlags);
		static int currentCategory{ 0 };
		ImGui::PushItemWidth(110.0f);
		ImGui::PushID(0);
		ImGui::Combo("", &currentCategory, "Allies\0Enemies\0Planting\0Defusing\0Local player\0Weapons\0Hands\0Backtrack\0");
		ImGui::PopID();
		static int currentItem{ 0 };

		if (currentCategory <= 3) {
			ImGui::SameLine();
			static int currentType{ 0 };
			ImGui::PushID(1);
			ImGui::Combo("", &currentType, "Visible\0Occluded\0");
			ImGui::PopID();
			currentItem = currentCategory * 2 + currentType;
		}
		else {
			currentItem = currentCategory + 4;
		}

		ImGui::Checkbox("Enabled", &config.chams[currentItem].enabled);
		ImGui::Checkbox("Health based", &config.chams[currentItem].healthBased);
		ImGui::Checkbox("Rainbow", &config.chams[currentItem].rainbow);
		ImGui::Combo("Material", &config.chams[currentItem].material, "Normal\0Flat\0Palm\0Platinum\0Glass\0Chrome\0Crystal\0Gold\0Plastic\0Blinking\0");
		ImGui::Checkbox("Wireframe", &config.chams[currentItem].wireframe);

		bool openPopup = ImGui::ColorButton("Color", ImVec4{ config.chams[currentItem].color }, ImGuiColorEditFlags_NoTooltip);
		ImGui::SameLine(0.0f, 5.0f);
		ImGui::Text("Color");
		ImGui::PushID(2);
		if (openPopup)
			ImGui::OpenPopup("");
		if (ImGui::BeginPopup("")) {
			ImGui::PushID(3);
			ImGui::ColorPicker3("", config.chams[currentItem].color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview);
			ImGui::PopID();
			ImGui::EndPopup();
		}
		ImGui::PopID();
		ImGui::PushItemWidth(220.0f);
		ImGui::PushID(4);
		ImGui::SliderFloat("", &config.chams[currentItem].alpha, 0.0f, 1.0f, "Alpha: %.2f");
		ImGui::PopID();
		ImGui::End();
	}
}

void GUI::renderEspWindow() noexcept
{
	if (window.esp) {
		ImGui::SetNextWindowSize({ 250.0f, 190.0f });
		ImGui::Begin("Esp", &window.esp, windowFlags);
		static int currentCategory{ 0 };
		ImGui::PushItemWidth(110.0f);
		ImGui::PushID(0);
		ImGui::Combo("", &currentCategory, "Allies\0Enemies\0");
		ImGui::PopID();
		ImGui::SameLine();
		static int currentType{ 0 };
		ImGui::PushID(1);
		ImGui::Combo("", &currentType, "Visible\0Occluded\0");
		ImGui::PopID();
		int currentItem = currentCategory * 2 + currentType;
		ImGui::Checkbox("Enabled", &config.esp[currentItem].enabled);
		checkboxedColorPicker("Snaplines", &config.esp[currentItem].snaplines, config.esp[currentItem].snaplinesColor);
		checkboxedColorPicker("Box", &config.esp[currentItem].box, config.esp[currentItem].boxColor);
		checkboxedColorPicker("Name", &config.esp[currentItem].name, config.esp[currentItem].nameColor);
		checkboxedColorPicker("Head dot", &config.esp[currentItem].headDot, config.esp[currentItem].headDotColor);
		ImGui::End();
	}
}

void GUI::renderVisualsWindow() noexcept
{
	if (window.visuals) {
		ImGui::SetNextWindowSize({ 520.0f, 365.0f });
		ImGui::Begin("Visuals", &window.visuals, windowFlags);
		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnOffset(1, 210.0f);
		ImGui::Checkbox("Disable post-processing", &config.visuals.disablePostProcessing);
		ImGui::Checkbox("Inverse ragdoll gravity", &config.visuals.inverseRagdollGravity);
		ImGui::Checkbox("No fog", &config.visuals.noFog);
		ImGui::Checkbox("No 3d sky", &config.visuals.no3dSky);
		ImGui::Checkbox("No visual recoil", &config.visuals.noVisualRecoil);
		ImGui::Checkbox("No hands", &config.visuals.noHands);
		ImGui::Checkbox("No sleeves", &config.visuals.noSleeves);
		ImGui::Checkbox("No weapons", &config.visuals.noWeapons);
		ImGui::Checkbox("No smoke", &config.visuals.noSmoke);
		ImGui::Checkbox("No blur", &config.visuals.noBlur);
		ImGui::Checkbox("No scope overlay", &config.visuals.noScopeOverlay);
		ImGui::Checkbox("No grass", &config.visuals.noGrass);
		ImGui::Checkbox("Wireframe smoke", &config.visuals.wireframeSmoke);
		ImGui::NextColumn();
		ImGui::Checkbox("Thirdperson", &config.visuals.thirdperson);
		ImGui::SameLine();
		hotkey(config.visuals.thirdpersonKey);
		ImGui::PushItemWidth(290.0f);
		ImGui::PushID(0);
		ImGui::SliderInt("", &config.visuals.thirdpersonDistance, 0, 1000, "Thirdperson distance: %d");
		ImGui::PopID();
		ImGui::PushID(1);
		ImGui::SliderInt("", &config.visuals.viewmodelFov, -60, 60, "Viewmodel FOV: %d");
		ImGui::PopID();
		ImGui::PushID(2);
		ImGui::SliderInt("", &config.visuals.flashReduction, 0, 100, "Flash reduction: %d%%");
		ImGui::PopID();
		ImGui::PushID(3);
		ImGui::SliderFloat("", &config.visuals.brightness, 0.0f, 1.0f, "Brightness: %.2f");
		ImGui::PopID();
		ImGui::PopItemWidth();
		ImGui::Combo("Skybox", &config.visuals.skybox, "cs_baggage_skybox_\0cs_tibet\0embassy\0italy\0jungle\0nukeblank\0office\0sky_cs15_daylight01_hdr\0sky_cs15_daylight02_hdr\0sky_cs15_daylight03_hdr\0sky_cs15_daylight04_hdr\0sky_csgo_cloudy01\0sky_csgo_night_flat\0sky_csgo_night02\0sky_day02_05_hdr\0sky_day02_05\0sky_dust\0sky_l4d_rural02_ldr\0sky_venice\0vertigo_hdr\0vertigo\0vertigoblue_hdr\0vietnam\0");
		ImGui::ColorEdit3("World color", config.visuals.worldColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip);
		ImGui::End();
	}
}

void GUI::renderKnifeChangerWindow() noexcept
{
	if (window.knifeChanger) {
		ImGui::SetNextWindowSize({ 180.0f, 100.0f });
		ImGui::Begin("Knife changer", &window.knifeChanger, windowFlags);
		ImGui::PushItemWidth(130.0f);
		ImGui::Checkbox("Enabled", &config.knifeChanger.enabled);
		ImGui::Combo("Knife", &config.knifeChanger.knife, "Default\0Bayonet\0Bowie\0Butterfly\0Falchion\0Flip\0Gut\0Huntsman\0Karambit\0M9 Bayonet\0Shadow Daggers\0Navaja\0Stiletto\0Talon\0Ursus\0Golden\0Ghost\0");
		ImGui::End();
	}
}

void GUI::renderSkinChangerWindow() noexcept
{
	if (window.skinChanger) {
		ImGui::SetNextWindowSize({ 260.0f, 445.0f });
		ImGui::Begin("Skin changer", &window.skinChanger, windowFlags);
		static int currentCategory{ 0 };
		ImGui::PushItemWidth(110.0f);
		ImGui::PushID(0);
		ImGui::Combo("", &currentCategory, "Pistols\0Heavy\0SMG\0Rifles\0Knives\0");
		ImGui::PopID();
		ImGui::SameLine();
		static int currentWeapon{ 0 };
		ImGui::PushID(1);

		switch (currentCategory) {
		case 0: {
			static int currentPistol{ 0 };
			ImGui::Combo("", &currentPistol, "Glock-18\0P2000\0USP-S\0Dual Berettas\0P250\0Tec-9\0Five-Seven\0CZ-75\0Desert Eagle\0Revolver\0");
			currentWeapon = currentPistol + 1;
			break;
		}
		case 1: {
			static int currentHeavy{ 0 };
			ImGui::Combo("", &currentHeavy, "Nova\0XM1014\0Sawed-off\0MAG-7\0M249\0Negev\0");
			currentWeapon = currentHeavy + 11;
			break;
		}
		case 2: {
			static int currentSmg{ 0 };
			ImGui::Combo("", &currentSmg, "Mac-10\0MP9\0MP7\0MP5-SD\0UMP-45\0P90\0PP-Bizon\0");
			currentWeapon = currentSmg + 17;
			break;
		}
		case 3: {
			static int currentRifle{ 0 };
			ImGui::Combo("", &currentRifle, "Galil AR\0Famas\0AK-47\0M4A4\0M4A1-S\0SSG-08\0SG-553\0AUG\0AWP\0G3SG1\0SCAR-20\0");
			currentWeapon = currentRifle + 24;
			break;
		}
		case 4: {
			static int currentKnife{ 0 };
			ImGui::Combo("", &currentKnife, "Bayonet\0Bowie\0Butterfly\0Falchion\0Flip\0Gut\0Huntsman\0Karambit\0M9 Bayonet\0Shadow Daggers\0Navaja\0Stiletto\0Talon\0Ursus\0");
			currentWeapon = currentKnife + 35;
			break;
		}
		}
		ImGui::PopID();
		ImGui::Checkbox("Enabled", &config.skinChanger[currentWeapon].enabled);
		ImGui::PushItemWidth(90.0f);
		ImGui::InputInt("Paint kit", &config.skinChanger[currentWeapon].paintKit, 1, 5);
		ImGui::SliderFloat("", &config.skinChanger[currentWeapon].wear, 0.0f, 1.0f, "Wear: %.2f");
		ImGui::End();
	}
}

void GUI::renderMiscWindow() noexcept
{
	if (window.misc) {
		ImGui::SetNextWindowSize({ 220.0f, 490.0f });
		ImGui::Begin("Misc", &window.misc, windowFlags);
		ImGui::Text("Menu key");
		ImGui::SameLine();
		hotkey(config.misc.menuKey);
		ImGui::Checkbox("Auto strafe", &config.misc.autoStrafe);
		ImGui::Checkbox("Bunny hop", &config.misc.bunnyHop);
		static char buffer[16];
		ImGui::PushItemWidth(120.0f);
		ImGui::InputText("", buffer, IM_ARRAYSIZE(buffer));
		ImGui::SameLine();
		if (ImGui::Button("Set clantag")) {
			Misc::setClanTag(buffer);
			Misc::animateClanTag(buffer);
		}
		ImGui::Checkbox("Animated clan tag", &config.misc.animatedClanTag);
		ImGui::Checkbox("Fast duck", &config.misc.fastDuck);
		ImGui::Checkbox("Sniper crosshair", &config.misc.sniperCrosshair);
		ImGui::Checkbox("Recoil crosshair", &config.misc.recoilCrosshair);
		ImGui::Checkbox("Auto pistol", &config.misc.autoPistol);
		ImGui::Checkbox("Auto reload", &config.misc.autoReload);
		ImGui::Checkbox("Auto accept", &config.misc.autoAccept);
		ImGui::Checkbox("Radar hack", &config.misc.radarHack);
		ImGui::Checkbox("Reveal ranks", &config.misc.revealRanks);
		ImGui::Checkbox("Spectator list", &config.misc.spectatorList);
		ImGui::Checkbox("Watermark", &config.misc.watermark);
		ImGui::Checkbox("Fix animation LOD", &config.misc.fixAnimationLOD);
		ImGui::PushItemWidth(90.0f);
		ImGui::InputInt("Choked packets", &config.misc.chokedPackets, 1, 5);
		config.misc.chokedPackets = std::clamp(config.misc.chokedPackets, 0, 64);

		if (ImGui::Button("Unhook"))
			hooks.restore();
		ImGui::End();
	}
}

void GUI::renderConfigWindow() noexcept
{
	if (window.config) {
		ImGui::SetNextWindowSize({ 290.0f, 190.0f });
		ImGui::Begin("Config", &window.config, windowFlags);

		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnOffset(1, 170.0f);

		ImGui::PushItemWidth(160.0f);

		constexpr auto& configItems = config.getConfigs();
		static int currentConfig = -1;

		if (static_cast<size_t>(currentConfig) >= configItems.size())
			currentConfig = -1;

		static char buffer[16];

		if (ImGui::ListBox("", &currentConfig, [](void* data, int idx, const char** out_text) {
			auto& vector = *static_cast<std::vector<std::string>*>(data);
			*out_text = vector[idx].c_str();
			return true;
		}, &configItems, configItems.size(), 5) && currentConfig != -1)
			strcpy(buffer, configItems[currentConfig].c_str());

		ImGui::PushID(0);
		if (ImGui::InputText("", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
			if (currentConfig != -1)
				config.rename(currentConfig, buffer);
		}
		ImGui::PopID();
		ImGui::NextColumn();

		ImGui::PushItemWidth(100.0f);

		if (ImGui::Button("Create config", { 100.0f, 25.0f }))
			config.add(buffer);

		if (ImGui::Button("Reset config", { 100.0f, 25.0f }))
			config.reset();

		if (currentConfig != -1) {
			if (ImGui::Button("Load selected", { 100.0f, 25.0f }))
				config.load(currentConfig);
			if (ImGui::Button("Save selected", { 100.0f, 25.0f }))
				config.save(currentConfig);
			if (ImGui::Button("Delete selected", { 100.0f, 25.0f }))
				config.remove(currentConfig);
		}
		ImGui::End();
	}
}*/