// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "Memory.h"
#include "Hooks.h"
#include "HackTools.h"
#include "SDK/Entity.h"
#include "GUI.h"
#include "Config.h"

using namespace std;

Config config;
Interfaces interfaces;
Memory mem;
GUI gui;
Hooks hooks;
HackTools hacktools;
Netvars netvars;