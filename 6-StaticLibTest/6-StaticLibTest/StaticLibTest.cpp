#include <Windows.h>
#include <strsafe.h>

#include "StaticLib1.h"

#pragma comment(lib, "6-StaticLib1.lib")

/*
 * ע�⣺��� .lib �ļ�������Ŀ�ĵ�ǰĿ¼,������ .lib �ļ����ڵ�·��
 * ��Ŀ->����->������->����->���ӿ�Ŀ¼
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

	TCHAR szBuffer[256] = { 0 };
	StringCchPrintf(szBuffer, _countof(szBuffer), TEXT("5 + 3 = %d\n5 * 3 = %d"), funAdd(5, 3), funMul(5, 3));
	MessageBox(NULL, szBuffer, TEXT("��ʾ"), MB_OK);
	return 0;
}