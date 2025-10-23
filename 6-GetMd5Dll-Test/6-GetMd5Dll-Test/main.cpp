#include <Windows.h>
#include "GetMd5.h"

#pragma comment(lib, "6-GetMd5Dll.lib")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {

	LRESULT CALLBACK wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	return 0;
}

LRESULT CALLBACK wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CLOSE: {
		int id = MessageBox(hwnd, TEXT("你确定退出吗？"), TEXT("提示"), MB_OKCANCEL);
		if (id == IDOK) {
			DestroyWindow(hwnd);
		}
		return 0;
	}
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		default:
			break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}