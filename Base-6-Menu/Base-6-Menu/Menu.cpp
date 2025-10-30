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

	// 加载加速键表
	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	// 消息循环
	BOOL bRet;
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {			
		/*
		* 
		* 处理菜单命令的快捷键。 该函数将 WM_KEYDOWN 或 WM_SYSKEYDOWN 
		* 消息转换为 WM_COMMAND 或 WM_SYSCOMMAND 消息（如果指定快捷键表中有项），
		* 然后将 WM_COMMAND 或 WM_SYSCOMMAND 消息直接发送到指定的窗口过程。 
		* TranslateAccelerator 在窗口过程处理消息之前不会返回。
		* 处理完快捷键后，TranslateAccelerator 返回非零值。
		*/
		int res = TranslateAccelerator(hwnd, hAccel, &msg);
		if (!res) { // 如果不是加速键消息，则进行正常的消息翻译和分发
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
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