#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#include "resource.h"

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInStance, LPSTR lpCmdLine, int nCmdShow) {
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc, NULL);
	return 0;
}

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	// INI 文件名称
	static TCHAR szFileName[MAX_PATH] = { 0 };
	// 小节名称
	LPCTSTR lpAppName = TEXT("INIDemoPositionSize");
	LPCTSTR lpKeyNameX = TEXT("X");
	LPCTSTR lpKeyNameY = TEXT("Y");
	LPCTSTR lpKeyNameWidth = TEXT("Width");
	LPCTSTR lpKeyNameHeight = TEXT("Height");

	UINT unX = 0, unY = 0, unWidth = 0, unHeight = 0;
	RECT rect;
	TCHAR szBuffer[20] = { 0 };

	switch (uMsg) {
	case WM_INITDIALOG:
		// 获取当前进程的可执行文件完整路径, 然后拼接出 INI 文件路径
		GetModuleFileName(NULL, szFileName, _countof(szFileName));
		StringCchCopy(_tcsrchr(szFileName, TEXT('\\')) + 1, _countof(szFileName), TEXT("INIDemo.ini"));

		// 获取X, Y , Width, Height
		unX = GetPrivateProfileInt(lpAppName, lpKeyNameX, NULL, szFileName);
		unY = GetPrivateProfileInt(lpAppName, lpKeyNameY, NULL, szFileName);
		unWidth = GetPrivateProfileInt(lpAppName, lpKeyNameWidth, NULL, szFileName);
		unHeight = GetPrivateProfileInt(lpAppName, lpKeyNameHeight, NULL, szFileName);

		// 如果读取到了有效值, 则设置窗口位置和大小
		if(unWidth && unHeight) {
			SetWindowPos(hWnd, HWND_TOP, unX, unY, unWidth, unHeight, SWP_SHOWWINDOW);
		}
		return TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			// 保存窗口位置和大小到 INI 文件
			GetWindowRect(hWnd, &rect);
			StringCchPrintf(szBuffer, _countof(szBuffer), TEXT("%d"), rect.left);
			WritePrivateProfileString(lpAppName, lpKeyNameX, szBuffer, szFileName);
			StringCchPrintf(szBuffer, _countof(szBuffer), TEXT("%d"), rect.top);
			WritePrivateProfileString(lpAppName, lpKeyNameY, szBuffer, szFileName);
			StringCchPrintf(szBuffer, _countof(szBuffer), TEXT("%d"), rect.right - rect.left);
			WritePrivateProfileString(lpAppName, lpKeyNameWidth, szBuffer, szFileName);
			StringCchPrintf(szBuffer, _countof(szBuffer), TEXT("%d"), rect.bottom - rect.top);
			WritePrivateProfileString(lpAppName, lpKeyNameHeight, szBuffer, szFileName);

			EndDialog(hWnd, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}