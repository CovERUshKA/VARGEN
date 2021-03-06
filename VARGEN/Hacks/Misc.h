#pragma once

#include "../SDK/UserCmd.h"
#include "../SDK/def.h"

namespace Misc {
	void BunnyHop(UserCmd* cmd) noexcept;
	void FastDuck(UserCmd* cmd) noexcept;
	void autoStrafe(UserCmd* cmd) noexcept;
	void FakeDuck(UserCmd* cmd) noexcept;
	void AntiKick(bf_read& msg_data) noexcept;
	void autoPistol(UserCmd* cmd) noexcept;
	void sniperCrosshair() noexcept;
	void setClanTag(const char* clanTag) noexcept;
}