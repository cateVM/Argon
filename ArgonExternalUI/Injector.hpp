#pragma once

// For windows
#if defined(_WIN32)
#include <Windows.h>
#include <iostream>
#include <string>
#include <io.h> 
#endif

namespace Injector {
// For windows
#if defined(_WIN32)
	std::string Inject(HWND Window, std::string Path);
	std::string GetArgonDllPath();
#endif
}