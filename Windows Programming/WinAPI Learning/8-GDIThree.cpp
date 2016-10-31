// Always need -mwindows

#include <windows.h>

const char g_szClassName[] = "myWindowClass";
HBITMAP blblb = NULL;
HBITMAP blblbMask = NULL;

const int MOVE_DELTA = 2;
const int ID_TIMER = 1;

typedef struct _BALLINFO
{
    int width;
    int height;
    int x;
    int y;

    int dx;
    int dy;
}BALLINFO;

BALLINFO g_moveInfo;

HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent)
{
    HDC hdcMem, hdcMem2;
    HBITMAP hbmMask;
    BITMAP bm;

    // Create monochrome (1 bit) mask bitmap.

    GetObject(hbmColour, sizeof(BITMAP), &bm);
    hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

    // Get some HDCs that are compatible with the display driver

    hdcMem = CreateCompatibleDC(0);
    hdcMem2 = CreateCompatibleDC(0);

    SelectObject(hdcMem, hbmColour);
    SelectObject(hdcMem2, hbmMask);

    // Set the background colour of the colour image to the colour
    // you want to be transparent.
    SetBkColor(hdcMem, crTransparent);

    // Copy the bits from the colour image to the B+W mask... everything
    // with the background colour ends up white while everythig else ends up
    // black...Just what we wanted.

    BitBlt(hdcMem2, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

    // Take our new mask and use it to turn the transparent colour in our
    // original colour image to black so the transparency effect will
    // work right.
    BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCINVERT);

    // Clean up.

    DeleteDC(hdcMem);
    DeleteDC(hdcMem2);

    return hbmMask;
}

void UpdateMove(RECT* prc)
{
    g_moveInfo.x += g_moveInfo.dx;
    g_moveInfo.y += g_moveInfo.dy;

    if(g_moveInfo.x < 0)
    {
        g_moveInfo.x = 0;
        g_moveInfo.dx = MOVE_DELTA;
    }
    else if(g_moveInfo.x + g_moveInfo.width > prc->right)
    {
        g_moveInfo.x = prc->right - g_moveInfo.width;
        g_moveInfo.dx = -MOVE_DELTA;
    }

    if(g_moveInfo.y < 0)
    {
        g_moveInfo.y = 0;
        g_moveInfo.dy = MOVE_DELTA;
    }
    else if(g_moveInfo.y + g_moveInfo.height > prc->bottom)
    {
        g_moveInfo.y = prc->bottom - g_moveInfo.height;
        g_moveInfo.dy = -MOVE_DELTA;
    }
}

void DrawMove(HDC hdc, RECT* prc)
{
	HDC hdcBuffer = CreateCompatibleDC(hdc);
	HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, prc->right, prc->bottom);
    HBITMAP hbmOldBuffer = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);

	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, blblbMask);

	FillRect(hdcBuffer, prc, (HBRUSH)GetStockObject(LTGRAY_BRUSH));

	BitBlt(hdcBuffer, g_moveInfo.x, g_moveInfo.y, g_moveInfo.width, g_moveInfo.height, hdcMem, 0, 0, SRCAND);

	SelectObject(hdcMem, blblb);
	BitBlt(hdcBuffer, g_moveInfo.x, g_moveInfo.y, g_moveInfo.width, g_moveInfo.height, hdcMem, 0, 0, SRCPAINT);

	BitBlt(hdc, 0, 0, prc->right, prc->bottom, hdcBuffer, 0, 0, SRCCOPY);

	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);

	SelectObject(hdcBuffer, hbmOldBuffer);
    DeleteDC(hdcBuffer);
    DeleteObject(hbmBuffer);
}

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
	{
	    case WM_CREATE:
	    {
	        UINT ret;
	        BITMAP bm;
			blblb = (HBITMAP)LoadImage(NULL, "blblb.bmp",
											IMAGE_BITMAP,113,109,LR_LOADFROMFILE);
			if(blblb == NULL)
	            MessageBox(hwnd, "Could not load bitmap!", "Error", MB_OK | MB_ICONERROR);

            blblbMask = CreateBitmapMask(blblb, RGB(255, 255, 255));
        	if(blblbMask == NULL)
            	MessageBox(hwnd, "Could not create mask!", "Error", MB_OK | MB_ICONEXCLAMATION);

            GetObject(blblb, sizeof(bm), &bm);

            ZeroMemory(&g_moveInfo, sizeof(g_moveInfo));
			g_moveInfo.width = bm.bmWidth;
			g_moveInfo.height = bm.bmHeight;

			g_moveInfo.dx = g_moveInfo.dy = MOVE_DELTA;

			ret = SetTimer(hwnd, ID_TIMER, 50, NULL);
			if(ret == 0)
				MessageBox(hwnd, "Could not SetTimer()!", "Error", MB_OK | MB_ICONEXCLAMATION);
	    }
	    break;
	    case WM_PAINT:
	    {
			RECT rcClient;
	        PAINTSTRUCT ps;
	        HDC hdc = BeginPaint(hwnd, &ps);
			GetClientRect(hwnd, &rcClient);
	        DrawMove(hdc, &rcClient);
	        EndPaint(hwnd, &ps);
	    }
	    break;
		case WM_TIMER:
		{
			RECT rcClient;
			HDC hdc = GetDC(hwnd);
			GetClientRect(hwnd, &rcClient);
			UpdateMove(&rcClient);
			DrawMove(hdc, &rcClient);
			ReleaseDC(hwnd, hdc);
		}
		break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
			KillTimer(hwnd, ID_TIMER);
        	DeleteObject(blblb);
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
