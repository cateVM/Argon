/*
* Author: cate
* Minetest executor core
* Compile as an DLL (dynamic lib)
* :sunglasses:
*/

// includes
#define _CRT_SECURE_NO_WARNINGS // :troll:
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <string>




 // 00A3AD90 luaL_loadstring :troll:
 // 00A32A94 lua_gettop :sunglasses:
// 00A335D0 lua_tostring for error handleing
/*
int __cdecl luaL_loadstring(int a1, char* a2)
{

}
int __cdecl lua_pcall(int a1, int a2, int a3, int a4)
int __cdecl lua_tolstring(void *a1, int a2, int a3)

int __cdecl lj_cf_jit_off(int arg0)
{
  return setjitmode(arg0, 0); 00A2E640 lua_jit off
}
*/


/*
* Lua5.1 includes
* We dont really need these
* other than for the LUA_ERRERR, LUA_ERRMEM, LUA_ERRRUN, LUA_ERRSYNTAX, LUA_MULTRET macros.
* feel free too remove lua5.1 :)
*/
extern "C" {

#include "Lua_5.1/lapi.h"
#include "Lua_5.1/lauxlib.h"
#include "Lua_5.1/lualib.h"
#include "Lua_5.1/lua.h"
#include "Lua_5.1/lstate.h"
#include "Lua_5.1/lcode.h"
#include "Lua_5.1/lstring.h"

}
/*
* lua_loadstring
* pushes [a2] as a function onto to the stack
*/
typedef int(__cdecl* minetest_loadstring_)(int a1, const char* a2);
minetest_loadstring_ minetest_loadstring = (minetest_loadstring_)(0x00A3AD90);
/*
* lua_gettop
* we dont actually use this anywhere
* used as an (kinda) debugging tool
* feel free to remove this :)
*/
typedef int(__cdecl* lua_getTop)(int a1);
lua_getTop minetest_gettop = (lua_getTop)(0x00A32A94);

/*
* lua_tolstring
* convert a string into a C string at the given idx [a2]
* we use this for retrieving lua error messages that 
* get pushed too the stack when an error occurs
*/
typedef int(__cdecl* did_know_every_minute_60_seconds_passes_in_africa)(void* a1, int a2, int a3);
did_know_every_minute_60_seconds_passes_in_africa minetest_tolstring = (did_know_every_minute_60_seconds_passes_in_africa)(0x00A335D0);

typedef int(__cdecl* misrepresenting_is_a_cutie)(int a1, int a2, int a3, int a4);
misrepresenting_is_a_cutie minetest_pcall = (misrepresenting_is_a_cutie)(0x00A39720); //pcall

#define minetest_tostring(f,x) minetest_tolstring(f,x,NULL)
/*
* lua_gettop hook 
* for the games lua state
* rev : true = revert the hook ? : false = hook the function
*/
void* HookGettop(DWORD AddressToHook, void* FunctionToReplaceWith, bool rev = false)
{ // Uses virtualprotect too override/hook the specified address with our function
	DWORD oldprot;
	if (!rev)
	{
		void* oldmem = new void*;
		void* result = new void*;
		memcpy(oldmem, (void*)AddressToHook, sizeof(void*) * 4);
		VirtualProtect((LPVOID)AddressToHook, 1, PAGE_EXECUTE_READWRITE, &oldprot);
		*(char*)AddressToHook = 0xE9; *(DWORD*)(AddressToHook + 1) = (DWORD)FunctionToReplaceWith - AddressToHook - 5;
		memcpy(result, oldmem, sizeof(void*) * 4);
		VirtualProtect((LPVOID)AddressToHook, 1, oldprot, &oldprot);
		return result;
	}
	else
	{
		VirtualProtect((LPVOID)AddressToHook, 1, PAGE_EXECUTE_READWRITE, &oldprot);
		memcpy((void*)AddressToHook, FunctionToReplaceWith, sizeof(void*) * 4);
		VirtualProtect((LPVOID)AddressToHook, 1, oldprot, &oldprot);
		return NULL;
	}
}


DWORD m_L = 0; // minetest's lua state
DWORD hookaddr = 0x00A32A94; //gettop addr

int OurCustomGetTopFunction(DWORD wtf)
{
	m_L = wtf;
	return (*(DWORD*)(wtf + 20) - *(DWORD*)(wtf + 16)) >> 3;
}
DWORD WINAPI Argon(LPVOID lpReserved) {
	AllocConsole();
	SetConsoleTitleA("Argon");
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	::ShowWindow(ConsoleHandle, SW_NORMAL);;
	//std::cout << "Gettop hook = " << m_L << std::endl; test
	HookGettop(hookaddr, OurCustomGetTopFunction, false); // gettop hook go brrr..
	printf("hook placed\n");

	while (true) {
		/*
		* TODO: Add namepipes
		*/
		std::cout << "\nEnter Script to Execute >>" << std::endl;
		std::string s;
		std::getline(std::cin, s); // input
		/*
		* loads our code onto the stack
		*/
		int poggers = minetest_loadstring(m_L, s.c_str()); 
		/*
		* Error Handling...
		*/
		switch (poggers) { 
		case LUA_ERRERR:
			printf("\n[Argon]  -> [Execution] -> [LUA_ERRERR] -> info: %s", minetest_tostring((void*)m_L, -1));
			printf("\nTerminating compliation..");
			break;
		case LUA_ERRMEM:
			printf("\n[Argon]  -> [Execution] -> [LUA_ERRMEM] -> info: %s", minetest_tostring((void*)m_L, -1)); 
			printf("\nTerminating compliation..");

			break;
		case LUA_ERRRUN:
			printf("\n[LUA_ERRRUN]  -> info: %s ", minetest_tostring((void*)m_L, -1));
			printf("\nTerminating compliation.. ");
			break;
		case LUA_ERRSYNTAX:
			printf("\n[Argon]  -> [Execution] -> [LUA_ERRSYNTAX] -> info: %s ", minetest_tostring((void*)m_L, -1));
			printf("\nTerminating compliation..");
			break;
		default:
			minetest_pcall(m_L, 0, LUA_MULTRET, 0);
			printf("[Argon] -> [Execution] -> Successfully executed!");
		}
	}

	return 0;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(nullptr, 0, Argon, hMod, 0, nullptr); // create a thread for our DLL once it attachs..
		break;

	}

	return TRUE;
}