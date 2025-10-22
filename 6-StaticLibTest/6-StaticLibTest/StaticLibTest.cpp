#include <Windows.h>
#include <strsafe.h>

#include "StaticLib1.h"

#pragma comment(lib, "6-StaticLib1.lib")

/*
 * 注意：如果 .lib 文件不在项目的当前目录,需配置 .lib 文件所在的路径
 * 项目->属性->链接器->常规->附加库目录
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

	TCHAR szBuffer[256] = { 0 };
	StringCchPrintf(szBuffer, _countof(szBuffer), TEXT("5 + 3 = %d\n5 * 3 = %d"), funAdd(5, 3), funMul(5, 3));
	MessageBox(NULL, szBuffer, TEXT("提示"), MB_OK);
	return 0;
}