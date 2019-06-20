#pragma once

class Config final
{
public:
	struct {
		bool enabled{ false };
		int target{ 1 };
		struct {
			int px{ 0 };
			bool draw{ false };
			float color[4]{0, 0, 0, 1};
		} FOV;
		float smooth{ 1.0f };
		bool ignoreSmoke{ false }, ignoreflash{ false };
		bool head, chest, legs;
		bool onlyzoom{ false };
	} aimbot;

	struct {
		bool enabled{ false };
		int target{ 1 };
		int distance{ 0 };
		bool onlyzoom{ false };
		bool ignoreSmoke{ false }, ignoreflash{ false };
	} rage;

	struct {
		bool enabled{ false };
		int target{ 1 };
		bool onlyzoom{ false };
		bool ignoreSmoke{ false }, ignoreflash{ false };
	} triggerbot;

	struct {
		bool enabled{ false };
	} antiaim;

	struct {
		bool enabled{ false };
		int filter{ 0 };
		struct {
			bool enabled{ false };
			bool corners{ false };
		} box;
		struct {
			bool enabled{ false };
			float color[4]{ 1, 1, 1, 1 };
		} line;
		bool hp{ false };
		bool armor{ false };
		struct {
			bool enabled{ false };
			float color[4]{ 1, 1, 1, 1 };
		} nickname;
		struct {
			float enemies[4]{ 1, 0, 0, 1 };
			float allies[4]{ 0, 1, 0, 1 };
		} colors;
	} esp;
	int chamstype{ 0 };
	struct {
		bool enabled{ false };
		int filter{ 0 };
		bool ignorez{ false };
		bool wireframe{ false };
		struct {
			float enemies[4]{ 1, 0, 0, 1 };
			float allies[4]{ 0, 1, 0, 1 };
		} colors;
	} chams[2];

	struct {
		bool enabled{ 1 };
		int knife{ 1 };
	} knifechanger;

	struct {
		struct {
			bool enabled{ false };
			int width{ 2 };
			float color[4]{0, 0, 0, 1};
		} hudzoom;
		bool radarhack{ false };
		struct {
			bool enabled{ false };
			int pos{ 0 };
		} thirdperson;
		bool noflash{ false };
		struct {
			bool no{ false };
			bool wireframe{ false };
		} smoke;
		bool showimpacts{ false };
		int fov{ 90 };
		struct {
			int fov{ 0 };
			int x{ -21 }, y{ -21 }, z{ -21 };
			float cl_bob_lower_amt, cl_bobamt_lat, cl_bobamt_vert, cl_bobcycle;
		} viewmodel;
	} visuals;

	struct {
		struct {
			bool animate{ false };
		} clantag;
		bool bunnyhop{ false };
		bool autostrafe{ false };
		bool fastduck{ false };
		bool antikick{ false };
		struct {
			bool enabled{ false };
			float time{ 0.0f };
		} bombtimer;
		bool autoaccept{ false };
		bool autopistol{ false };
		bool snipercrosshair{ false };
	} misc;
	float test1{ 0 }, test2{ 0 };

	bool keys[104];
};

extern Config config;