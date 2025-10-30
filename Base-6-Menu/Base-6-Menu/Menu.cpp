#include <Windows.h>
#include "resource.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	TCHAR szAppName[] = TEXT("Base-6-Menu");
	TCHAR szClassName[] = TEXT("Base6MenuClass");

	WNDCLASSEX wndClass;
	HWND hwnd;
	MSG msg;

	// ���ڹ��̺�������
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

	// ע�ᴰ����
	RegisterClassEx(&wndClass);

	// ��������
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

	// ��Ϣѭ��
	BOOL bRet;
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {			
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	// ���û��Ӳ˵��е���������ؼ���֪ͨ��Ϣ���͵��丸���ڻ�ת�����ټ�ʱ���͡�
	case WM_COMMAND:
		// ���ְ����˵���ؼ�����ټ��ı�ʶ����
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