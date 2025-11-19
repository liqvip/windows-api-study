#include <Windows.h>
#include "resource.h"
#include "strsafe.h"

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nShowCmd) {
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc, 0);
	return 0;
}

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static HKEY hKey;
	LPCTSTR lpSubKey = TEXT("SOFTWARE");
	LPCTSTR lpSubKey2 = TEXT("SOFTWARE\\REGDemo");
	switch (uMsg) {
		case WM_INITDIALOG: {
			// 获取子键句柄
			LONG lRet= RegOpenKeyEx(HKEY_CURRENT_USER, lpSubKey, 0, KEY_ENUMERATE_SUB_KEYS, &hKey);
			if (lRet != ERROR_SUCCESS) return TRUE;


			return TRUE;
		}
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDC_GET_SUBKEY: {	// 枚举所有子键
					// 子键索引，递增
					DWORD dwIndex = 0;
					// 接收子键名称
					TCHAR lpName[MAX_PATH] = { 0 };
					// 接收子键类名
					TCHAR lpClass[MAX_PATH] = { 0 };
					

					TCHAR szRes[10*1024] = { 0 };
					while (TRUE) {
						DWORD lpcchName = _countof(lpName);
						DWORD lpcchClass = _countof(lpClass);
						LONG lRet = RegEnumKeyEx(hKey, dwIndex++, lpName, &lpcchName, NULL, lpClass, &lpcchClass, NULL);
						if (lRet == ERROR_SUCCESS) {
							TCHAR szItem[1024] = { 0 };
							StringCchPrintf(szItem, _countof(szItem), TEXT("子键名称:%s, 子键类名:%s\r\n"), lpName, lpClass);
							StringCchCat(szRes, _countof(szRes), szItem);
						}
						if (lRet == ERROR_NO_MORE_ITEMS) {	// 没有更多子项
							break;
						}
					}

					SetDlgItemText(hWnd, IDC_EDIT1, szRes);

					break;
				}
				case IDC_GET_KEY_INFO:	// 获取子键信息
					break;
				case IDC_GET_VALUE_ITEM:	// 枚举所有键值项
					break;
				case IDCANCEL:
					EndDialog(hWnd, 0);
					return TRUE;
				}
			break;
		}
	return FALSE;
}