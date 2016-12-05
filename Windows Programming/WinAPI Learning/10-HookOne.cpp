// Always need -mwindows
// Need -lWinmm

#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <time.h>

const char g_szClassName[] = "myWindowClass";

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    BOOL fEatKeystroke = FALSE;

    if (nCode == HC_ACTION)
    {
        switch (wParam) {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        	PKBDLLHOOKSTRUCT pressed = (PKBDLLHOOKSTRUCT)lParam;
        	switch (pressed->vkCode) {
				case VK_CAPITAL:
					PlaySound(TEXT("key-caps.wav"), NULL, SND_FILENAME | SND_ASYNC);
					break;
				case VK_LEFT:
				case VK_UP:
				case VK_RIGHT:
				case VK_DOWN:
					PlaySound(TEXT("key-movement.wav"), NULL, SND_FILENAME | SND_ASYNC);
					break;
				case VK_BACK:
					PlaySound(TEXT("key-delete.wav"), NULL, SND_FILENAME | SND_ASYNC);
					break;
				default: {
					int u = (double)rand() / (RAND_MAX + 1) * (4 - 1) + 1; 
		        	char arr[16];
		        	strcpy(arr,"key-press-1.wav");
		        	arr[10] = u + '0';
		        	PlaySound(TEXT(arr), NULL, SND_FILENAME | SND_ASYNC);
					break;
				}
			}
            
            break;
        }
    }
    return(fEatKeystroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
}

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;
    
    HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "The title of my window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

	srand( (unsigned)time( NULL ) );

    // Step 3: The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
    	if(Msg.message == WM_KEYDOWN) ;//MessageBeep(MB_ICONHAND);
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    
    UnhookWindowsHookEx(hhkLowLevelKybd);
    
    return Msg.wParam;
}

