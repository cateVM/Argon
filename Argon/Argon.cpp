/*
* Author: cate
* Minetest executor core
* Compile as an DLL (dynamic lib)
* x86 release
* :sunglasses:
*/

// includes
#define _CRT_SECURE_NO_WARNINGS // :troll:
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "Detours/detours.h"


// 00A2E050 int __cdecl lua_newstate(int (__cdecl *a1)(int, _DWORD, _DWORD, signed int), int a2)
// 00A43230 int (__cdecl *__cdecl luaL_openlibs(void *a1))(void *)
 // 00A3AD90 luaL_loadstring :troll:
 // 00A32A94 lua_gettop :sunglasses:
// 00A335D0 lua_tostring for error handling
// 00A35EC0 lua_createthread
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

 00A35EC0 int __cdecl lua_newthread(int a1)

  00A32AA0 unsigned int __cdecl lua_settop(int a1, int a2)
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

/*
int __cdecl lua_load(int a1, int a2, int a3, int a4)
*/
typedef int(__cdecl* minetest_load_)(int a1, int a2, int a3, const char* a4, int a5); //
minetest_load_ minetest_load = (minetest_load_)(0x00A3A450); //lua_loadx

typedef int(__cdecl* minetest_settop_)(int a1,int a2); // a1 = lua state
minetest_settop_ minetest_settop = (minetest_settop_)(0x00A32AA0); //lua_settop

typedef int(__cdecl* close_state)(int L); 
close_state minetest_close = (close_state)(0x00A39F10); 


#define minetest_tostring(f,x) minetest_tolstring(f,x,NULL)

typedef struct LoadS {
	const char* s;
	size_t size;
} LoadS;

static const char* getS(lua_State* L, void* ud, size_t* size) {
	LoadS* ls = (LoadS*)ud;
	(void)L;
	if (ls->size == 0) return NULL;
	*size = ls->size;
	ls->size = 0;
	return ls->s;
}
//int __cdecl lua_pushcclosure(int a1, int a2, int a3)
typedef int(__cdecl* minetest_pushcclosure_)(int a1, int a2, int a3, int a4);
minetest_pushcclosure_ minetest_pushcclosure = (minetest_pushcclosure_)(0x00A35620); //lua_pushcclosure

DWORD m_Lopenlibhook = 0;
DWORD m_L = 0; // minetest's lua state

int OurCustomGetTopFunction(int wtf)
{
	m_L = wtf;
	return (*(DWORD*)(wtf + 20) - *(DWORD*)(wtf + 16)) >> 3;
}

int lua_close_hook(int L) {
	return 0;
}
//int a1, int a2, int a3, int a4
DWORD WINAPI Argon(LPVOID lpReserved) {
	AllocConsole();
	SetConsoleTitleA("Argon");
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	::ShowWindow(ConsoleHandle, SW_NORMAL);;
	/*switched too detours
	* :flex:
	* :muscle:
	*/

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(LPVOID&)minetest_gettop, (PBYTE)OurCustomGetTopFunction);
	DetourAttach(&(LPVOID&)minetest_close, (PBYTE)lua_close_hook);
	DetourTransactionCommit();
	

	printf("hook placed\n");
	std::string Script = "";
	HANDLE hPipe;
	char buf[50000]; // :troll:
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\Argon"),
		PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
		PIPE_WAIT,
		1,
		999999,
		999999,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	printf("\n[Argon] -> [Pipe] -> NPCStream is now open!\n");
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		//printf("pipe open");
		if (ConnectNamedPipe(hPipe, NULL) != FALSE){
		
			while (ReadFile(hPipe, buf, sizeof(buf) - 1, &dwRead, NULL) != FALSE)
			{
				buf[dwRead] = '\0';
				try {
					try {
						Script = Script + buf;
					}
					catch (...) {
					}
				}
				catch (std::exception e) {
					std::cout << "An Exception occured 0x1"  << std::endl;
				}
				catch (...) {
					std::cout << "An Exception occured 0x2" << std::endl;
				}
			}
			LoadS ls;
			ls.s = Script.c_str();
			ls.size = strlen(Script.c_str());
			//printf("Lua State = %d", m_L);
			
			if (minetest_load(m_L,(int)getS,(int)&ls,"@Argon",0))
				printf("an error has occured!111: %s\n", minetest_tostring((void*)m_L, -1));
			else 
			{
				minetest_pcall(m_L, 0, LUA_MULTRET, 0);
				printf("\n[Argon] -> [Execution] -> Successfully Executed Script!\n");
			}
				
			Script = "";
		}
		else 
		{
			printf("[Argon] -> [Fatal] -> Could not Open the NPCStream!!");
		}
		DisconnectNamedPipe(hPipe);
		
	}


	return 0;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod); // Optimizing
		CreateThread(nullptr, 0, Argon, hMod, 0, nullptr); // create a thread for our DLL once it attaches..
		break;

	}

	return TRUE;
}
