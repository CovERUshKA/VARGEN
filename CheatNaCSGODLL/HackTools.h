#pragma once
#include "../../MathTools.h"
#include <Windows.h>
#include "Memory.h"
#include "SDK/UserCMD.h"

class HackTools final {
public:
	Vector getBonePos(int pTargetIn, int bone);

	/*Set new ViewAngle*/
	void setViewAngle(DWORD EngineBase, Vector angle);

	/*Set new ViewAngle*/
	Vector getViewAngle(DWORD EngineBase);

	/*Calc angle to target bone*/
	Vector AngelToTarget(Entity* localPlayer, Entity* target, int boneIndex);

	//fovpx - Field Of View in pixels from the center of the screen
	//bufTarget - buffer for the best target by fovpx
	//bone - the best bone in the target by fovpx
	void getTarget(float fovpx, Entity*& bufTarget, int& bone);

	Vector2D WorldToScreen(const Vector world);
	//Sort by distance - sortby: 0 - nearest player, 1 - furthest player
	//Filter by Team - filter: 0 - All, 1 - Enemies, 2 - Allies
	Entity* GetVisibleHurtablePlayer(int sortby, int filter);
};

extern HackTools hacktools;
