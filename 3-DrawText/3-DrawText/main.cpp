#include <Windows.h>
#include <strsafe.h>
#include "Metrics.h"
#include "resource.h"

const int NUMLINES = sizeof(METRICS) / sizeof(METRICS[0]);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nShowCmd) {

	HWND hwnd;
	TCHAR szAppName[] = TEXT("DrawText");
	TCHAR szClassName[] = TEXT("DrawText");

	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	WNDCLASSEX wndclass;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLE));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szClassName;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLE));

	// 注册窗口类
	RegisterClassEx(&wndclass);

	// 创建窗口
	hwnd = CreateWindowEx(
		0,
		szClassName,
		szAppName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	int y;
	TCHAR szBuf[10] = {0};

	switch (iMsg) {
	case WM_CREATE:
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SetTextColor(hdc, 0x00FF00);
		SetBkMode(hdc, TRANSPARENT);

		for (int i = 0; i < NUMLINES; i++) {
			y = 18 * i;
			TextOut(hdc, 0, y, METRICS[i].m_pLabel, lstrlen(METRICS[i].m_pLabel));
			TextOut(hdc, 240, y, METRICS[i].m_pDesc, lstrlen(METRICS[i].m_pDesc));
			StringCchPrintf(szBuf, _countof(szBuf), TEXT("%d"), GetSystemMetrics(METRICS[i].m_nIndex));
			TextOut(hdc, 760, y, szBuf, lstrlen(szBuf));
		}

		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}