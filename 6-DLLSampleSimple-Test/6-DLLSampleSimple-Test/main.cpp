#include <Windows.h>
#include <strsafe.h>


// 隐式链接 DLL 文件
/*
* 在编译链接生成可执行文件时，系统
* 会把DLL文件名以及用到的函数写入可执行文件的导入表，这
* 样一来当运行一个可执行文件时，PE 加载器会解析可执行文件
* 的导入表，把导入表中列出的每个 DLL 映射到进程的地址空间
* 中，并根据函数名在每个DLL中查找导出函数
*/

// 显示链接 DLL 文件
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
	StringCchPrintf(szBuffer, _countof(szBuffer), TEXT("姓名:%s, 年龄:%d\n"), student->GetName(), student->GetAge());
	StringCchCat(szBuffer2, _countof(szBuffer2), szBuffer);

	// 测试导出函数
	StringCchPrintf(szBuffer, _countof(szBuffer), TEXT("funAdd(5, 3) = %d\nfunMul(5, 3) = %d\n"), funAdd(5, 3), funMul(5, 3));
	StringCchCat(szBuffer2, _countof(szBuffer2), szBuffer);

	MessageBox(NULL, szBuffer2, TEXT("DLL导出测试"), MB_OK);
	delete student;
	return 0;
}