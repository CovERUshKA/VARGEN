#pragma once

#include <string>

class GUI final {
public:
	void render() noexcept;
	bool isOpen{ false };
private:
	void Button(const char* label, bool* p_open, const ImVec2& size = ImVec2(0, 0)) noexcept;
	void renderMainWindow() noexcept;
	void renderAimbotWindow() noexcept;
	void renderRageWindow() noexcept;
	void renderTriggerbotWindow() noexcept;
	void renderESPWindow() noexcept;
	void renderVisualsWindow() noexcept;
	void renderMiscWindow() noexcept;
	void renderAntiAimWindow() noexcept;
	void renderColorsWindow() noexcept;
	void renderChamsWindow() noexcept;
	struct {
		bool aimbot{ true };
		bool rage{ false };
		bool antiaim{ false };
		bool triggerbot{ false };
		bool esp{ false };
		bool chams{ false };
		bool visuals{ false };
		bool misc{ false };
		bool config{ false };
		bool colors{ false };
	} window;
};

extern GUI gui;