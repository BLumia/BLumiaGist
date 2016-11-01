#include <windows.h>

const char g_szClassName[] = "myWindowClass";
HFONT g_hfFont = NULL;
BOOL g_bOpaque = FALSE;
COLORREF g_rgbText = RGB(0, 0, 255);
COLORREF g_rgbBackground = RGB(255, 255, 0);

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg) {
    	case WM_CREATE:
    	{
    		HFONT hf;
			HDC hdc;
			long lfHeight;
    		hdc = GetDC(NULL);
		    lfHeight = -MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		    ReleaseDC(NULL, hdc);
	        hf = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, "Times New Roman");

		    if(hf) {
		        DeleteObject(g_hfFont);
		        g_hfFont = hf;
		    } else {
		        MessageBox(hwnd, "Font creation failed!", "Error", MB_OK | MB_ICONEXCLAMATION);
		    } 
		}
		break;
		case WM_SIZING:
		case WM_SIZE:
		case WM_PAINT:
		{
			HDC hdc;
			PAINTSTRUCT ps;
			RECT rcClient;
			
			hdc = BeginPaint(hwnd, &ps);//GetDC(NULL);
			
			char szSize[100];
		    char szTitle[] = "These are the dimensions of your client area:";
		    HFONT hfOld = (HFONT)SelectObject(hdc, g_hfFont);
		
		    SetBkColor(hdc, g_rgbBackground);
		    SetTextColor(hdc, g_rgbText);
		
		    if(g_bOpaque)
		    {
		        SetBkMode(hdc, OPAQUE);
		    }
		    else
		    {
		        SetBkMode(hdc, TRANSPARENT);
		    }
		
			GetClientRect(hwnd, &rcClient);
		    DrawText(hdc, szTitle, -1, &rcClient, DT_WORDBREAK);
		
		    wsprintf(szSize, "{%d, %d, %d, %d}", rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);
		    DrawText(hdc, szSize, -1, &rcClient, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		
		    SelectObject(hdc, hfOld);	
		    
		    EndPaint(hwnd, &ps);//ReleaseDC(hwnd, hdc);
		}
		break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
        	DeleteObject(g_hfFont);
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

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_VREDRAW | CS_HREDRAW;
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
        "Chris is mine",
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

    // Step 3: The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
