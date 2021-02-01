#pragma once
#include <Windows.h>
#include <iostream>
#include <string>

// Lua librarys
extern "C" {

#include "Lua_5.1/lapi.h"
#include "Lua_5.1/lauxlib.h"
#include "Lua_5.1/lualib.h"
#include "Lua_5.1/lua.h"
#include "Lua_5.1/lstate.h"
#include "Lua_5.1/lcode.h"
#include "Lua_5.1/lstring.h"

}

namespace CustomFunctions {
	int HttpRequest(DWORD LuaState);
}