#include <Windows.h>
#include "resource.h"
#include "dipshook.h"

#pragma comment(lib, "Core-6-DLL-DIPSHookDll.lib")

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow) {
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc, 0);
	return 0;
}

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static HWND hwndLV;
	HWND hwndDIPSServer;

	switch (uMsg) {
		case WM_INITDIALOG: { 
			hwndLV = GetTopWindow(GetTopWindow(FindWindow(TEXT("ProgMan"), NULL)));
			// 禁用保存桌面图标、恢复桌面图标和卸载消息钩子按钮
			EnableWindow(GetDlgItem(hWnd, IDC_SAVE_ICONS), FALSE);
			EnableWindow(GetDlgItem(hWnd, IDC_RESTORE_ICONS), FALSE);
			EnableWindow(GetDlgItem(hWnd, IDC_UnInstall), FALSE);
			return TRUE;
			break;
		}
		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDC_INSTALL: {	// 安装消息钩子
					if (InstallHook(WH_GETMESSAGE, GetWindowThreadProcessId(hwndLV, NULL))) {
						// 启用保存桌面图标、恢复桌面图标和卸载消息钩子按钮
						EnableWindow(GetDlgItem(hWnd, IDC_SAVE_ICONS), TRUE);
						EnableWindow(GetDlgItem(hWnd, IDC_RESTORE_ICONS), TRUE);
						EnableWindow(GetDlgItem(hWnd, IDC_UnInstall), TRUE);
						MessageBox(hWnd, TEXT("安装消息钩子成功"), TEXT("提示"), MB_OK);
					}else {
						MessageBox(hWnd, TEXT("安装消息钩子失败"), TEXT("错误"), MB_OK | MB_ICONERROR);
					}

					break;
				}
				case IDC_UnInstall: {	// 卸载消息钩子
					// 获取服务器窗口句柄
					hwndDIPSServer = FindWindow(NULL, TEXT("DIPS_SERVER"));
					// 使用SendMessage而不是PostMessage，确保卸载钩子以前，服务器对话框已经销毁
					SendMessage(hwndDIPSServer, WM_CLOSE, 0, 0);
					if (UnInstallHook()) {
						EnableWindow(GetDlgItem(hWnd, IDC_SAVE_ICONS), FALSE);
						EnableWindow(GetDlgItem(hWnd, IDC_RESTORE_ICONS), FALSE);
						EnableWindow(GetDlgItem(hWnd, IDC_UnInstall), FALSE);
					}
					break;
				}
				case IDC_SAVE_ICONS: {	// 保存桌面图标
					hwndDIPSServer = FindWindow(NULL, TEXT("DIPS_SERVER"));
					SendMessage(hwndDIPSServer, WM_APP,(WPARAM)hwndLV, TRUE);
					break;
				}
				case IDC_RESTORE_ICONS: {	// 恢复桌面图标
					// 获取服务器窗口句柄
					hwndDIPSServer = FindWindow(NULL, TEXT("DIPS_SERVER"));
					SendMessage(hwndDIPSServer, WM_APP,(WPARAM)hwndLV, FALSE);
					break;
				}
				case IDOK:
				case IDCANCEL: {
					if (FindWindow(NULL, TEXT("DIPS_SERVER")))
						SendMessage(hWnd, WM_COMMAND, IDC_UnInstall, 0);

					EndDialog(hWnd, 0);
					break;
				}
			}
			break;
		}
	}
	return FALSE;
}