#include <Windows.h>
#include "resource.h"

// 对话框过程函数声明
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc, NULL);
	return 0;
}

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HKEY hKey;
	LPCTSTR lpSubKey = TEXT("SOFTWARE\\REGDemo");
	LONG lRet;
	LPCTSTR lpValueNameX = TEXT("X");
	LPCTSTR lpValueNameY = TEXT("Y");
	LPCTSTR lpValueNameWidth = TEXT("Width");
	LPCTSTR lpValueNameHeight = TEXT("Height");

	DWORD dwcbData;
	DWORD dwX, dwY, dwWidth, dwHeight;

	RECT rect;

	switch (uMsg) {
	case WM_INITDIALOG: {
		// 打开子键,获取子键句柄
		lRet = RegOpenKeyEx(HKEY_CURRENT_USER, lpSubKey, 0, KEY_READ, &hKey);
		if (lRet != ERROR_SUCCESS) { 
			// 子键不存在
			return TRUE;
		}

		// 获取指定子键中指定键名的数据
		dwcbData = sizeof(DWORD);
		RegQueryValueEx(hKey, lpValueNameX, NULL, NULL, (LPBYTE)&dwX, &dwcbData);
		dwcbData = sizeof(DWORD);
		RegQueryValueEx(hKey, lpValueNameY, NULL, NULL, (LPBYTE)&dwY, &dwcbData);
		dwcbData = sizeof(DWORD);
		RegQueryValueEx(hKey, lpValueNameWidth, NULL, NULL, (LPBYTE)&dwWidth, &dwcbData);
		dwcbData = sizeof(DWORD);
		RegQueryValueEx(hKey, lpValueNameHeight, NULL, NULL, (LPBYTE)&dwHeight, &dwcbData);
		
		RegCloseKey(hKey);

		// 设置对话框位置和大小
		if (dwWidth && dwHeight) {
			SetWindowPos(hwndDlg, HWND_TOP, dwX, dwY, dwWidth, dwHeight, SWP_SHOWWINDOW);
		}
		return TRUE;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			GetWindowRect(hwndDlg, &rect);
			// 创建子键, 如果指定的子键已存在, 则该函数会打开子键并返回子键句柄
			// 注意，程序无法在HKEY_USERS或HKEY_LOCAL_MACHINE根键下面创建子键
			DWORD dwDisposition;
			lRet = RegCreateKeyEx(HKEY_CURRENT_USER, lpSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDisposition);
			
			if(lRet == ERROR_SUCCESS) {
				dwX = rect.left;
				dwY = rect.top;
				dwWidth = rect.right - rect.left;
				dwHeight = rect.bottom - rect.top;

				// 创建或者设置键值项
				RegSetValueEx(hKey, lpValueNameX, 0, REG_DWORD, (LPBYTE)&dwX, sizeof(DWORD));
				RegSetValueEx(hKey, lpValueNameY, 0, REG_DWORD, (LPBYTE)&dwY, sizeof(DWORD));
				RegSetValueEx(hKey, lpValueNameWidth, 0, REG_DWORD, (LPBYTE)&dwWidth, sizeof(DWORD));
				RegSetValueEx(hKey, lpValueNameHeight, 0, REG_DWORD, (LPBYTE)&dwHeight, sizeof(DWORD));

				RegCloseKey(hKey);
			}
			EndDialog(hwndDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}