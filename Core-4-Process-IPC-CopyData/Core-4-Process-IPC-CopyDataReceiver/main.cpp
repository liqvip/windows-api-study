#include <Windows.h>
#include <tchar.h>
#include "resource.h"

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdline, int nCmdShow) {
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc, 0);
	return 0;
}

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_INITDIALOG: {
			break;
		}
		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDCANCEL:
				case IDOK:
					EndDialog(hWnd, 0);
					break;
			}
			break;
		}
	}
	return FALSE;
}