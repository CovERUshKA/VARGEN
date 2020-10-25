#pragma once

#include <cstddef>
#include "Utils.h"
#include "../MathTools.h"

struct Ray {
	Ray(const Vector& src, const Vector& dest) : start(src), delta(dest - src) { isSwept = delta.x || delta.y || delta.z; }
	Vector start{ };
	float pad{ };
	Vector delta{ };
	byte pad2[40]{ };
	bool isRay{ true };
	bool isSwept{ };
};

class Entity;

struct TraceFilter {
	TraceFilter(const Entity* entity) : skip{ entity } { }
	virtual bool shouldHitEntity(Entity* entity, int) { return entity != skip; }
	virtual int getTraceType() const { return 0; }
	const void* skip;
};

struct Trace {
	byte pad[44];
	float fraction;
	byte pad1[20];
	int hitgroup;
	int pad2;
	Entity* entity;
	int hitbox;
};

class EngineTrace {
public:
	constexpr void traceRay(const Ray& ray, unsigned int mask, const TraceFilter& filter, Trace& trace) noexcept
	{
		callVirtualMethod<void, const Ray&, unsigned int, const TraceFilter&, Trace&>(this, 5, ray, mask, filter, trace);
	}
};
