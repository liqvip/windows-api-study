#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	TCHAR szAppName[] = TEXT("Base-8-SubWindow-Button");
	TCHAR szClassName[] = TEXT("Base8SubWindowButton");

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
	wndClass.lpszMenuName = NULL; // Assuming menu resource ID is 1
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

	/*
	* 
	* ������԰Ѵ�������ָ��ΪϵͳԤ���������Ӵ��ڿؼ�����������
	* CreateWindow/CreateWindowEx��������һ���Ӵ��ڿؼ����Ӵ��ڿؼ�
	* �͸�����֮����Ի��෢����Ϣ������ͨ�š��Ӵ��ڿؼ��ڶԻ����
	* �����õø��ձ飬����Ҫ����CreateWindow / CreateWindowEx������
	* ֱ������Դ�ű��ļ��ж����Ӵ��ڿؼ����ɣ�Ҳ����ͨ����Դ�༭��
	* �ӹ���������ҷ�Ӵ��ڿؼ����Ի���������ĺ���λ�á�
	*/

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
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}