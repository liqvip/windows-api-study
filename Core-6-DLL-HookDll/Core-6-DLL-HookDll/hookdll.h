#pragma once

// 声明导出函数
#ifdef DLL_EXPORT
	#define DLL_API extern "C" __declspec(dllexport)
#else
	#define DLL_API extern "C" __declspec(dllimport)
#endif

// 导出函数
DLL_API BOOL WINAPI InstallHook(int idHook, DWORD dwThreadId, HWND hwnd);

DLL_API BOOL WINAPI UnInstallHook();

// 钩子函数
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
