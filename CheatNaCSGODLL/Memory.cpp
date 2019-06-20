#include "Memory.h"
#include "Interfaces.h"

Memory::Memory() noexcept
{
	present = findPattern<>("gameoverlayrenderer", "\xFF\x15????\x8B\xF8\x85\xDB", 2);
	clientMode = **reinterpret_cast<ClientMode * **>((*reinterpret_cast<uintptr_t * *>(interfaces.client))[10] + 5);
	globalVars = **reinterpret_cast<GlobalVars * **>((*reinterpret_cast<uintptr_t * *>(interfaces.client))[11] + 10);
	input = *reinterpret_cast<Input * *>((*reinterpret_cast<uintptr_t * *>(interfaces.client))[16] + 1);
	setClanTag = findPattern<decltype(setClanTag)>("engine", "\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15");
	lineGoesThroughSmoke = findPattern<decltype(lineGoesThroughSmoke)>("client_panorama", "\x55\x8B\xEC\x83\xEC\x08\x8B\x15????\x0F\x57\xC0");
	smokeCount = *(reinterpret_cast<int**>(lineGoesThroughSmoke) + 2);
	acceptMatch = findPattern<decltype(acceptMatch)>("client_panorama", "\x55\x8B\xEC\x83\xE4\xF8\x8B\x4D\x08\xBA????\xE8????\x85\xC0\x75\x12");
}