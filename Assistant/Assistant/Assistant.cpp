#include <Windows.h>
#include <Windowsx.h>
#include "resource.h"

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow) {
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc, 0);
	return 0;
}

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static HKEY hKey1, hKey2;
	static HWND hWndStartUp;

	LPCTSTR lpSubKey1 = TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	LPCTSTR lpValueName1 = TEXT("Assitant");

	LPCTSTR lpSubKey2 = TEXT("Assitant");
	LPCTSTR lpValueName2 = TEXT("StartUp");

	switch (uMsg) {
		case WM_INITDIALOG: {	// 从注册表获取开机启动信息
			hWndStartUp = GetDlgItem(hWnd, IDC_STARTUP);
			LONG lResult = RegOpenKeyEx(HKEY_CURRENT_USER, lpSubKey2, 0, KEY_READ, &hKey2);

			if (lResult == ERROR_SUCCESS) {
				DWORD dwType;
				BYTE bData;
				DWORD dwCb = sizeof(BYTE);
				LONG lResult = RegQueryValueEx(hKey2, lpValueName2, NULL, &dwType, &bData, &dwCb);
				if (lResult == ERROR_SUCCESS) {
					Button_SetCheck(hWndStartUp, bData == 1 ? TRUE : FALSE);
				}
			}
			return TRUE;
		}
		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDC_OPEN_BOOKMARK: {
					LPCTSTR lpDirectory1 = TEXT("D:\\vs-project\\windows-api-study");
					LPCTSTR lpDirectory2 = TEXT("D:\\电子书");
					INT_PTR result = (INT_PTR)ShellExecute(hWnd, TEXT("explore"), lpDirectory1, NULL, NULL, SW_SHOWMINNOACTIVE);
					ShellExecute(hWnd, TEXT("explore"), lpDirectory2, NULL, NULL, SW_SHOWMINNOACTIVE);
					break;
				}
				case IDC_OPEN_VS: {
					LPCTSTR lpDirectory1 = TEXT("D:\\VisualStudio\\2022\\Community\\Common7\\IDE\\devenv.exe");
					INT_PTR result = (INT_PTR)ShellExecute(hWnd, TEXT("open"), lpDirectory1, NULL, NULL, SW_SHOWMINNOACTIVE);
					if (result <= 32) {
						MessageBox(hWnd, TEXT("打开VS失败"), TEXT("错误"), MB_OK | MB_ICONERROR);
					}
					break;
				}
				case IDC_OPEN_CALC: {
					TCHAR szLpCmdLine[MAX_PATH] = TEXT("calc");
					STARTUPINFO si = {sizeof(STARTUPINFO)};
					PROCESS_INFORMATION  pi;
					GetStartupInfo(&si);
					if (CreateProcess(NULL, szLpCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
						CloseHandle(pi.hProcess);
						CloseHandle(pi.hThread);
					}
					break;
				}
				case IDOK:
				case IDCANCEL: {	// 退出时设置(创建)键值项
					// 拿到用户开机设置
					BYTE bStartUpChecked = 0;
					LONG res = Button_GetCheck(hWndStartUp);// 使用 IsDlgButtonChecked 也可以
					bStartUpChecked = res == BST_CHECKED ? 1 : 0;

					DWORD dwPosition; // 返回函数处理结果
					LONG lResult = RegCreateKeyEx(HKEY_CURRENT_USER, lpSubKey2, NULL, NULL, 
						REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey2, &dwPosition);
					if (lResult == ERROR_SUCCESS) {	// 将自启动设置项写入注册表
						RegSetValueEx(hKey2, lpValueName2, NULL, REG_BINARY, &bStartUpChecked, sizeof(BYTE));
					} else {
						return TRUE;
					}

					// 设置自启动
					TCHAR szFileName[MAX_PATH] = { 0 };
					// 获取当前进程中已加载模块的完整路径
					GetModuleFileName(NULL, szFileName, _countof(szFileName));	// hModule = NULL, 获取当前进程可执行文件路径
					
					LPCTSTR lpData = szFileName;
					DWORD cbData = (lstrlen(lpData) + 1) * sizeof(TCHAR);
					DWORD dwPosition2; // 返回函数处理结果
					LONG lResult2 = RegCreateKeyEx(HKEY_CURRENT_USER, lpSubKey1, NULL, NULL, 
						REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey1, &dwPosition2);

					if (lResult2 == ERROR_SUCCESS) {
						if (bStartUpChecked) {
							// _countof 只能用于数组，不能传递指针
							// sizeof 用于计算变量、数组、类型的字节数
							RegSetValueEx(hKey1, lpValueName1, NULL, REG_SZ, (LPBYTE)lpData, cbData);
						} else {
							RegDeleteValue(hKey1, lpValueName1);
						}

					}
					//EndDialog(hWnd, 0);
					return TRUE;
				}
			}
			break;
		}
	}
	return FALSE;
}