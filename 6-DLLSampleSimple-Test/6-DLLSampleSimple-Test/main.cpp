#include <Windows.h>
#include <strsafe.h>
#include "dllSample.h"

#pragma comment(lib, "6-DllSample.lib")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nShowCmd) {

	TCHAR szBuffer[256] = { 0 };
	TCHAR szBuffer2[512] = { 0 };

	// 测试导出变量
	StringCchPrintf(szBuffer, _countof(szBuffer), TEXT("nValue = %d\nps.x = %d, ps.y = %d\n"), nValue, ps.x, ps.y);
	StringCchCopy(szBuffer2, _countof(szBuffer2), szBuffer);

	// 测试导出类
	CStudent *student = new CStudent((LPTSTR)TEXT("老王"), 40);


	delete student;
	return 0;
}