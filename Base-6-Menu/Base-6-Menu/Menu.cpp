#include <Windows.h>
#include "resource.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	TCHAR szAppName[] = TEXT("Base-6-Menu");
	TCHAR szClassName[] = TEXT("Base6MenuClass");

	WNDCLASSEX wndClass;
	HWND hwnd;
	MSG msg;

	// 窗口过程函数声明
	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1); // Assuming menu resource ID is 1
	wndClass.lpszClassName = szClassName;
	wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// 注册窗口类
	RegisterClassEx(&wndClass);

	// 创建窗口
	hwnd = CreateWindowEx(
		0,
		szClassName,
		szAppName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		500,
		400,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hwnd, nCmdShow);

	// 消息循环
	BOOL bRet;
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {			
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	// 当用户从菜单中调用命令项、控件将通知消息发送到其父窗口或转换加速键时发送。
	case WM_COMMAND:
		// 低字包含菜单项、控件或加速键的标识符。
		switch (LOWORD(wParam)) {
		case ID_FILE_NEW: 
			MessageBox(hwnd, TEXT("New File Created"), TEXT("Info"), MB_OK);
			break;
		case ID_FILE_OPEN: 			
			MessageBox(hwnd, TEXT("FILE OPEN"), TEXT("InFo"), MB_OK);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}