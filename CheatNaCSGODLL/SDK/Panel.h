#pragma once
#include <string>
#include "Utils.h"

class Panel {
public:
	const char* getName(unsigned int panel) noexcept
	{
		return std::string{ callVirtualMethod<const char*, unsigned int>(this, 36, panel) }.c_str();
	}
};