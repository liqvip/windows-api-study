#include <Windows.h>
#include "resource.h"

#define WM_WORKPROGRESS (WM_USER + 1)
#define WM_CALCOVER (WM_USER + 2)

BOOL g_bRunning = FALSE;
HWND g_hwndDialog;

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

DWORD WINAPI ThreadCalc(LPVOID lpParam);

DWORD WINAPI ThreadShow(LPVOID lpParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nShowCmd) {
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc, 0);
	return 0;
}

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static HANDLE hThreadCalc, hThreadShow;
	static HWND hwndStart, hwndStop;
	DWORD dwThreadId;

	switch (uMsg) {
		case WM_INITDIALOG: {
			g_hwndDialog = hWnd;
			hwndStart = GetDlgItem(hWnd, IDC_START);
			hwndStop = GetDlgItem(hWnd, IDC_STOP);
			EnableWindow(hwndStop, false);
			return TRUE;
		}
		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDC_START: { // 开始计时
					g_bRunning = TRUE;
					EnableWindow(hwndStart, false);
					EnableWindow(hwndStop, true);
					hThreadShow = CreateThread(NULL, 0, ThreadShow, 0, 0, &dwThreadId);
					hThreadCalc = CreateThread(NULL, 0, ThreadCalc, (LPVOID)dwThreadId, 0, NULL);
					break;
				}
				case IDC_STOP: {
					g_bRunning = FALSE;
					EnableWindow(hwndStart, true);
					EnableWindow(hwndStop, false);
					if (hThreadCalc != NULL) {
						CloseHandle(hThreadCalc);
					}
					if (hThreadShow != NULL) {
						CloseHandle(hThreadShow);
					}
					hThreadCalc = NULL;
					hThreadShow = NULL;

					break;
				}
			}
			break;
		}
		case WM_CLOSE: {
			EndDialog(hWnd, 0);
			return TRUE;
		}
	}
	return FALSE;
}

DWORD WINAPI ThreadCalc(LPVOID lpParam) {
	DWORD dwShowThreadId = (DWORD)lpParam;
	int count = 0;
	while (g_bRunning) {
		Sleep(50);
		// 给显示线程发送消息
		PostThreadMessage(dwShowThreadId, WM_WORKPROGRESS, (WPARAM)++count, 0);
	}

	// 给显示线程发送退出消息
	PostThreadMessage(dwShowThreadId, WM_CALCOVER, 0, 0);
	return 0;
}

DWORD WINAPI ThreadShow(LPVOID lpParam) {
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) != 0) {
		switch (msg.message) {
			case WM_WORKPROGRESS: {
				// 显示计算结果
				UINT count = (UINT)msg.wParam;
				SetDlgItemInt(g_hwndDialog, IDC_EDIT1, count, FALSE);
				break;
			}
			case WM_CALCOVER: {
				// 计算结束，退出线程
				return 0;
			}
		}
	}
	return 0;
}
