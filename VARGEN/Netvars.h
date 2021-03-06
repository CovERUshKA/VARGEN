#pragma once

#include <unordered_map>

#include "SDK/Recv.h"

class Netvars final {
public:
	Netvars() noexcept;

	auto operator[](const char* netvar) const noexcept
	{
		return offsets[netvar];
	}

	void restore() noexcept;
private:
	void loadTable(RecvTable*, const std::size_t = 0) noexcept;
	void unloadTable(RecvTable*) noexcept;
	mutable std::unordered_map<std::string, std::size_t> offsets;
};

extern Netvars netvars;