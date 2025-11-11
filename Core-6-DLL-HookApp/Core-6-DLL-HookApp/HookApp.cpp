#include <Windows.h>
#include "resource.h"
#include "hookdll.h"

#pragma comment(lib, "Core-6-DLL-HookDll.lib")

// 声明对话框窗口过程函数
INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreIntance, LPSTR lpCmdLine, int nShowCmd) {
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc, NULL);
	return 0;
}


INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_INITDIALOG: {
			break;
		}
		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDC_INSTALL: {	// 安装键盘钩子
					BOOL res = InstallHook(WH_KEYBOARD, 0, hWnd);
					if (!res) {
						MessageBox(hWnd, TEXT("安装键盘钩子失败"), TEXT("错误"), MB_OK | MB_ICONERROR);
					} else {
						MessageBox(hWnd, TEXT("安装键盘钩子成功"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
					}
					break;
				}
				case IDC_UNINSTALL: { // 卸载键盘钩子
					BOOL res = UnInstallHook();
					if (!res) {
						MessageBox(hWnd, TEXT("卸载键盘钩子失败"), TEXT("错误"), MB_OK | MB_ICONERROR);
					}
					else {
						MessageBox(hWnd, TEXT("卸载键盘钩子成功"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
						SetDlgItemText(hWnd, IDC_EDIT1, TEXT(""));
					}
					break;
				}
			}
			break;
		}
		case WM_COPYDATA: {
			SendMessage(GetDlgItem(hWnd, IDC_EDIT1), EM_SETSEL, -1, -1);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT1), EM_REPLACESEL, TRUE, (LPARAM)(LPSTR)(((PCOPYDATASTRUCT)lParam)->lpData));
			return TRUE;
		}
		case WM_CLOSE: {
			EndDialog(hWnd, 0);
			break;
		}

	}
	return FALSE;
}
