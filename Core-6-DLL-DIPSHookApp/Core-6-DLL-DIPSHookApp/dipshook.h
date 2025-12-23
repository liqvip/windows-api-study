#pragma once

#ifdef DLL_EXPORT
	#define DLL_API extern "C" __declspec(dllexport)
#else
	#define DLL_API extern "C" __declspec(dllimport)
#endif

DLL_API BOOL WINAPI InstallHook(int idHook, DWORD dwThreadId);

DLL_API BOOL WINAPI UnInstallHook();
