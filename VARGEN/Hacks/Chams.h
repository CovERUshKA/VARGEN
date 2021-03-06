#pragma once

#include "../SDK/UserCmd.h"
#include "../SDK/ModelRender.h"

namespace Chams {
	void Render(void* ctx, void* state, const ModelRenderInfo& info, matrix3x4* customBoneToWorld) noexcept;
}