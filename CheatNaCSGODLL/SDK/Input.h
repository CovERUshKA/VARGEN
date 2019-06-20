#pragma once

#include <Windows.h>
#include "../../MathTools.h"

class Input {
public:
	byte pad[12];
	bool isTrackIRAvailable;
	bool isMouseInitialized;
	bool isMouseActive;
	byte pad1[158];
	bool isCameraInThirdPerson;
	byte pad2;
	Vector cameraOffset;
};