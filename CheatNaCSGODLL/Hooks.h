#pragma once

#include "Memory.h"
#include "Interfaces.h"

class Hooks final {
public:
	Hooks() noexcept;

	WNDPROC originalWndProc;
	std::add_pointer_t<HRESULT __stdcall(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*)> originalPresent;
	std::add_pointer_t<HRESULT __stdcall(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*)> originalReset;

	class Vmt {
	public:
		explicit Vmt(void* const) noexcept;

		template<typename T>
		void hookAt(size_t index, T fun) const noexcept
		{
			if (index <= length)
				newVmt[index + 1] = reinterpret_cast<uintptr_t>(fun);
		}

		template<typename T, typename ...Args>
		constexpr auto callOriginal(size_t index, Args... args) const noexcept
		{
			return reinterpret_cast<T(__thiscall*)(void*, Args...)>(oldVmt[index])(base, args...);
		}
	private:
		static uintptr_t* findFreeDataPage(void* const, size_t) noexcept;
		static auto calculateLength(uintptr_t*) noexcept;
		void* base;
		uintptr_t* oldVmt;
		uintptr_t* newVmt;
		size_t length;
	};
	Vmt panel{ interfaces.panel };
	Vmt surface{ interfaces.surface };
	Vmt modelRender{ interfaces.modelRender };
	Vmt clientMode{ mem.clientMode };
	Vmt client{ interfaces.client };
};

extern Hooks hooks;