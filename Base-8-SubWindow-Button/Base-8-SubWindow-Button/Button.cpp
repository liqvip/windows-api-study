#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	TCHAR szAppName[] = TEXT("Base-8-SubWindow-Button");
	TCHAR szClassName[] = TEXT("Base8SubWindowButton");

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
	wndClass.lpszMenuName = NULL; // Assuming menu resource ID is 1
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

	/*
	* 
	* 程序可以把窗口类名指定为系统预定义的相关子窗口控件类名来调用
	* CreateWindow/CreateWindowEx函数创建一个子窗口控件，子窗口控件
	* 和父窗口之间可以互相发送消息来进行通信。子窗口控件在对话框程
	* 序中用得更普遍，不需要调用CreateWindow / CreateWindowEx函数，
	* 直接在资源脚本文件中定义子窗口控件即可，也可以通过资源编辑器
	* 从工具箱中拖曳子窗口控件到对话框程序界面的合适位置。
	*/

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
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}