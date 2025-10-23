#pragma once

#ifdef DLL_EXPORT
	#define DLL_API extern "C"	__declspec(dllexport)
#else
	#define DLL_API extern "C"	__declspec(dllimport)
#endif

// 导出函数声明
DLL_API BOOL WINAPI GetMd5(LPCTSTR lpFileName, LPTSTR lpMd5);
