#include <Windows.h>
#include <strsafe.h>

#pragma comment(lib, "Winmm.lib")   // ����������PlaySound������ҪWinmm�����

// �������������ڹ���
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;                        // RegisterClassEx�����õ�WNDCLASSEX�ṹ
    TCHAR szClassName[] = TEXT("MyWindow");     // RegisterClassEx����ע��Ĵ����������
    TCHAR szAppName[] = TEXT("HelloWindows");   // ���ڱ���
    HWND hwnd;                                  // CreateWindowEx���������Ĵ��ڵľ��
    MSG msg;                                    // ��Ϣѭ�����õ���Ϣ�ṹ��

    wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS; // ��������
	wndclass.lpfnWndProc = WindowProc; // ָ�򴰿ڹ��̵�ָ��
	wndclass.cbClsExtra = 0; // ��������ڴ�
	wndclass.cbWndExtra = 0; // ����Ĵ����ڴ�
	wndclass.hInstance = hInstance; // ��Ӧ�ó���ʵ���ľ��
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // Ĭ��ͼ��
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW); // Ĭ�ϼ�ͷ���
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // ��ɫ������ˢ
	wndclass.lpszMenuName = NULL; // û�в˵�
	wndclass.lpszClassName = szClassName; // ��������
	wndclass.hIconSm = NULL; // Сͼ��
	RegisterClassEx(&wndclass); // ע�ᴰ����

	// ��ȡ��Ļ���
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int windowWidth = 800;
	int windowHeight = 800;
	int x = (screenWidth - windowWidth) / 2;
	int y = (screenHeight - windowHeight) / 2;

    hwnd = CreateWindowEx(
		0, // ��չ���ͨ��Ϊ0
		szClassName, // ��������
		szAppName, // ���ڱ���
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, // ���ڷ��
		x, // x����
		y, // y����
		windowWidth, // ���
		windowHeight, // �߶�
		NULL, // �����ھ��
		NULL, // �˵����
		hInstance, // ��Ӧ�ó���ʵ���ľ��
		NULL
    );

	ShowWindow(hwnd, nCmdShow); // ��ʾ����
	UpdateWindow(hwnd); // ���´���

    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	// HDC : �豸�����ľ�������ڻ�ͼ
    HDC hdc;
	// PAINTSTRUCT : ��ͼ�ṹ�壬����BeginPaint��EndPaint����
    PAINTSTRUCT ps;
    TCHAR szStr[] = TEXT("��ã�Windows�������");

    switch (uMsg){
        case WM_CREATE:
            PlaySound(TEXT("�ɶ�(�����).wav"), NULL, SND_FILENAME | SND_ASYNC/* | SND_LOOP*/);
            return 0;
        case WM_PAINT:
		    hdc = BeginPaint(hwnd, &ps);
            TextOut(hdc, 50, 50, szStr, lstrlen(szStr));
            EndPaint(hwnd, &ps);
		    return 0;
		case WM_LBUTTONDBLCLK:
			// ��ʾ˫��
			MessageBox(hwnd, TEXT("��˫����������"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);

			return 0;
		case WM_CHAR: {
			// ��ʾ�㰴�����ĸ���
			TCHAR szBuff[16] = { 0 };
			// _countof(szBuff) ��ȡ����Ԫ�ظ���
			// sizeof(szBuff) ��ȡ�����ֽ���
			StringCchPrintf(szBuff, _countof(szBuff), TEXT("�㰴���� %c ��, %d"), wParam, sizeof(szBuff));
			MessageBox(hwnd, szBuff, TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);
			return 0;
		}
		case WM_CLOSE:
			// ��ʾ�Ƿ�رմ���
            if (MessageBox(hwnd, TEXT("���Ҫ�رմ�����"), TEXT("�رմ���"), MB_YESNO | MB_ICONQUESTION) == IDYES) {
				// �رմ���
                DestroyWindow(hwnd);
            }
			return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
		default:
			// ������Ϣ����Ĭ�ϵĴ��ڹ��̴���
			// DefWindowProc ���ᴦ��WM_DESTROY��Ϣ
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}