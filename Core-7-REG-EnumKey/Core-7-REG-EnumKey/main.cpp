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
			LONG lRet= RegOpenKeyEx(HKEY_CURRENT_USER, lpSubKey2, 0, KEY_READ, &hKey);
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
				case IDC_GET_KEY_INFO: {	// 获取子键信息
					DWORD lpcSubKeys;	// hKey 下子键数量
					DWORD lpcMaxSubKeyLength; // hKey 下子键名称最大字符长度，不包含终止字符
					DWORD lpcValues;	// hKey 的键值项的数量
					DWORD lpcMaxValueNameLength;	// hKey 的键名最大字符长度
					DWORD lpcMaxValueLength; // hKey 的键值数据的最大长度(以字节为单位)

					LONG lRet = RegQueryInfoKey(hKey, NULL, NULL, NULL, &lpcSubKeys, &lpcMaxSubKeyLength, 
						NULL, &lpcValues, &lpcMaxValueNameLength, &lpcMaxValueLength, NULL, NULL);
					if (lRet == ERROR_SUCCESS) {
						TCHAR szInfo[1024] = { 0 };
						StringCchPrintf(szInfo, _countof(szInfo), TEXT("hKey 下子键数量:%d\r\nhKey 下子键名称最大字符长度:%d\r\nkHey的键值项数量:%d\r\nhKey的键名最大字符长度:%d\r\nhKey的键值数据最大长度:%d字节"), 
							lpcSubKeys, lpcMaxSubKeyLength, lpcValues, lpcMaxValueNameLength, lpcMaxValueLength);
						SetDlgItemText(hWnd, IDC_KEY_INFO, szInfo);
					}else {
						MessageBox(hWnd, TEXT("获取失败"), TEXT("提示"), MB_ICONERROR | MB_OK);
					}
					break;
				}
				case IDC_GET_VALUE_ITEM: {	// 枚举所有键值项
					DWORD dwIndex = 0;
					TCHAR lpValueName[MAX_PATH] = { 0 };
					DWORD lpType;
					BYTE lpData[256] = { 0 };
					TCHAR szRes[10 * 1024] = { 0 };
					while (TRUE) {
						DWORD lpcchValueName = _countof(lpValueName);
						DWORD lpcbData = _countof(lpData);
						//ZeroMemory(lpData, _countof(lpData));
						LONG lRet = RegEnumValue(hKey, dwIndex++, lpValueName, &lpcchValueName, NULL, &lpType, lpData, &lpcbData);
						TCHAR szItem[1024] = {0};
						if (lRet == ERROR_SUCCESS) {
							switch (lpType) {
								case REG_DWORD: {
									DWORD keyData = *(DWORD*)lpData;
									StringCchPrintf(szItem, _countof(szItem), TEXT("键名：%s, 键值: %d\r\n"), lpValueName, keyData);
									break;
								}
							}
						}
						StringCchCat(szRes, _countof(szRes), szItem);
						if (lRet == ERROR_NO_MORE_ITEMS) {
							break;
						}
					}
					SetDlgItemText(hWnd, IDC_EDIT1, szRes);
					break;
				}
				case IDCANCEL:
					EndDialog(hWnd, 0);
					return TRUE;
				}
			break;
		}
	return FALSE;
}