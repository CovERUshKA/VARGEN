#pragma once

#include "../SDK/ViewSetup.h"
#include "../SDK/FrameStage.h"

namespace SkinChanger {
	void Execute(int) noexcept;
	void scheduleHudUpdate() noexcept;
}