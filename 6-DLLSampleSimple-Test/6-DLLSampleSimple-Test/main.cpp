#include <Windows.h>
#include <strsafe.h>


// ��ʽ���� DLL �ļ�
/*
* �ڱ����������ɿ�ִ���ļ�ʱ��ϵͳ
* ���DLL�ļ����Լ��õ��ĺ���д���ִ���ļ��ĵ������
* ��һ��������һ����ִ���ļ�ʱ��PE �������������ִ���ļ�
* �ĵ�����ѵ�������г���ÿ�� DLL ӳ�䵽���̵ĵ�ַ�ռ�
* �У������ݺ�������ÿ��DLL�в��ҵ�������
*/

// ��ʾ���� DLL �ļ�
#include "dllSample.h"
#pragma comment(lib, "6-DllSample.lib")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nShowCmd) {

	TCHAR szBuffer[256] = { 0 };
	TCHAR szBuffer2[512] = { 0 };

	// ���Ե�������
	StringCchPrintf(szBuffer, _countof(szBuffer), TEXT("nValue = %d\nps.x = %d, ps.y = %d\n"), nValue, ps.x, ps.y);
	StringCchCopy(szBuffer2, _countof(szBuffer2), szBuffer);

	// ���Ե�����
	CStudent *student = new CStudent((LPTSTR)TEXT("����"), 40);
	StringCchPrintf(szBuffer, _countof(szBuffer), TEXT("����:%s, ����:%d\n"), student->GetName(), student->GetAge());
	StringCchCat(szBuffer2, _countof(szBuffer2), szBuffer);

	// ���Ե�������
	StringCchPrintf(szBuffer, _countof(szBuffer), TEXT("funAdd(5, 3) = %d\nfunMul(5, 3) = %d\n"), funAdd(5, 3), funMul(5, 3));
	StringCchCat(szBuffer2, _countof(szBuffer2), szBuffer);

	MessageBox(NULL, szBuffer2, TEXT("DLL��������"), MB_OK);
	delete student;
	return 0;
}