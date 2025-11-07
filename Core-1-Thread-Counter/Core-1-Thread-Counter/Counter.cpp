#include <Windows.h>
#include "resource.h"

// 视觉样式
// https://learn.microsoft.com/zh-cn/windows/win32/controls/cookbook-overview

#include <CommCtrl.h>
#pragma comment(lib, "Comctl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// 常量定义
#define F_START 1	// 开始
#define F_STOP	2 // 停止
#define F_PAUSE 3 // 暂停
#define F_CONTINUE 4 // 继续

HWND g_hwndDlg;
int g_nOption;	// 标志

// 对话框窗口过程函数声明
INT_PTR CALLBACK DialogProc(HWND hDiaWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// 计数函数声明
DWORD WINAPI Counter(LPVOID lpParameter);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR pCmdline, int nCmdShow) {
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc, 0);
	return 0;
}

INT_PTR CALLBACK DialogProc(HWND hDiaWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static HWND hwndBtnStart, hwndBtnPause, hwndBtnStop, hwndBtnContinue;
	HANDLE hThread = NULL;

	switch (uMsg) {
		case WM_INITDIALOG: {
			g_hwndDlg = hDiaWnd;

			hwndBtnStart = GetDlgItem(hDiaWnd, IDC_START);
			hwndBtnContinue = GetDlgItem(hDiaWnd, IDC_CONTINUE);
			hwndBtnPause = GetDlgItem(hDiaWnd, IDC_PAUSE);
			hwndBtnStop = GetDlgItem(hDiaWnd, IDC_STOP);

			EnableWindow(hwndBtnContinue, FALSE);
			EnableWindow(hwndBtnPause, FALSE);
			EnableWindow(hwndBtnStop, FALSE);
			return TRUE;
		}
		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDOK:
				case IDCANCEL:
					EndDialog(hDiaWnd, 0);
				break;
				case IDC_START: { // 开始按钮
					g_nOption = F_START;
					EnableWindow(hwndBtnStart, FALSE);
					EnableWindow(hwndBtnContinue, FALSE);
					EnableWindow(hwndBtnPause, TRUE);
					EnableWindow(hwndBtnStop, TRUE);
					hThread = CreateThread(NULL, 0, Counter, NULL, 0, NULL);
					// 关闭线程句柄避免句柄泄漏
					if (hThread != NULL) {
						CloseHandle(hThread);
						hThread = NULL;
					}
					break;
				}
				case IDC_PAUSE: { // 暂停按钮
					g_nOption = F_PAUSE;
					EnableWindow(hwndBtnStart, FALSE);
					EnableWindow(hwndBtnContinue, TRUE);
					EnableWindow(hwndBtnPause, FALSE);
					EnableWindow(hwndBtnStop, TRUE);
					break;
				}
				case IDC_STOP: { // 停止按钮
					g_nOption = F_STOP;
					EnableWindow(hwndBtnStart, TRUE);
					EnableWindow(hwndBtnContinue, FALSE);
					EnableWindow(hwndBtnPause, FALSE);
					EnableWindow(hwndBtnStop, FALSE);
					SetDlgItemInt(g_hwndDlg, IDC_COUNTER, 0, FALSE);
					break;
				}
				case IDC_CONTINUE: { // 继续
					g_nOption = F_CONTINUE;
					EnableWindow(hwndBtnStart, FALSE);
					EnableWindow(hwndBtnContinue, FALSE);
					EnableWindow(hwndBtnPause, TRUE);
					EnableWindow(hwndBtnStop, TRUE);
					break;
				}
			}
		}
	}
	return FALSE;
}

/*
* 
* 在主线程中执行循环，会导致程序无法响应。
* 因此，将计数功能放在单独的线程中执行，以保持界面的响应性。
* 
*/
DWORD WINAPI Counter(LPVOID lpParameter) {
	int count = 0;
	while (g_nOption != F_STOP) {
		if (g_nOption == F_START || g_nOption == F_CONTINUE) {
			SetDlgItemInt(g_hwndDlg, IDC_COUNTER, count++, FALSE);
			Sleep(1000);
		}
	}
	return 0;
}