#pragma warning(disable : 4996)
#include "Injector.hpp"
/*
  Dll injector, currently no anti-cheats to bypass so it's a simple inject (thankfully)

  This injector is for Argons External UI :3
*/

std::string Injector::Inject(HWND Window, std::string Path) {
	// Some top variables aha..
	DWORD ProcessId;
	DWORD ExitCode; // Defining the exit code for the thread
	
	long DLL_ = Path.length() + 1;

	GetWindowThreadProcessId(Window, &ProcessId); // Getting the process Id

	HANDLE ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessId); // Getting the process handle
	if (!ProcessHandle) { return "Failed to get window handle"; }
	LPVOID MemAllocation = VirtualAllocEx(ProcessHandle, NULL, DLL_, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); // Getting memory allocation
	if (!MemAllocation) { return "Internal error"; } // If didn't find the memory allocation it will return internal error, since that's what it is lool
	 /*
	  Writing to memory
	  We use virtual protect to change the protection region
	*/
	DWORD WriteMemOldProc;
	VirtualProtect(WriteProcessMemory, 1, PAGE_EXECUTE_READWRITE, &WriteMemOldProc);
	int WritingSuccessful = WriteProcessMemory(ProcessHandle, MemAllocation, Path.c_str(), DLL_, 0);
	VirtualProtect(WriteProcessMemory, 1, WriteMemOldProc, &WriteMemOldProc);
    /*
	  Creating a thread to the target handle
	*/
	LPTHREAD_START_ROUTINE Kernel32_Library = (LPTHREAD_START_ROUTINE)GetProcAddress(LoadLibrary("kernel32"), "LoadLibraryA"); // Staring the routine
	HANDLE DLLHandler_ = CreateRemoteThread(ProcessHandle, NULL, NULL, (LPTHREAD_START_ROUTINE)Kernel32_Library, MemAllocation, NULL, NULL); // Creating the thread
	WaitForSingleObject(DLLHandler_, INFINITE); // Wating for the object
	VirtualFreeEx(ProcessHandle, MemAllocation, NULL, MEM_RELEASE); // Free the dll
	GetExitCodeThread(DLLHandler_, &ExitCode); // Checking the exit code

	// Checking if the injection was successful 
	CloseHandle(ProcessHandle); // Closing the process handle
	if (!ExitCode) {
		return "Failed";
	}
	else {
		return "Injected";
	}
}

/*
 Getting the dll path
 Microsoft machine only...
*/
 
bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

std::string Injector::GetArgonDllPath()
{
	char rawPathName[MAX_PATH];
	GetModuleFileNameA(NULL, rawPathName, MAX_PATH);
	std::string executablePath = std::string(rawPathName);
	int pos = executablePath.find_last_of("/\\");
	std::string Executeble = executablePath.substr(pos + 1);
	replace(executablePath, Executeble, "");
	return executablePath + "Argon.dll";
}