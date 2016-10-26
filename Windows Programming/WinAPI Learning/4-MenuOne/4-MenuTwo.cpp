// standalone cpp file.

#include <windows.h>
#include "resource.h"

const char g_szClassName[] = "myWindowClass";

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
	    case WM_CREATE:
	    {
	        HMENU hMenu, hSubMenu;
	        HICON hIcon, hIconSm;
	
	        hMenu = CreateMenu();
	
	        hSubMenu = CreatePopupMenu();
	        AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, "E&xit");
	        AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&File");
	
	        hSubMenu = CreatePopupMenu();
	        AppendMenu(hSubMenu, MF_STRING, ID_STUFF_GO, "&Go");
	        AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Stuff");
	
	        SetMenu(hwnd, hMenu);
	
	
	        hIcon = (HICON)LoadImage(NULL, "menu_one.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
	        if(hIcon)
	            SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	        else
	            MessageBox(hwnd, "Could not load large icon!", "Error", MB_OK | MB_ICONERROR);
	
	        hIconSm = (HICON)LoadImage(NULL, "menu_one.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
	        if(hIconSm)
	            SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIconSm);
	        else
	            MessageBox(hwnd, "Could not load small icon!", "Error", MB_OK | MB_ICONERROR);
	    }
	    break;
    	case WM_LBUTTONDOWN: 
        {
            char szFileName[MAX_PATH];
            HINSTANCE hInstance = GetModuleHandle(NULL);

            GetModuleFileName(hInstance, szFileName, MAX_PATH);
            MessageBox(hwnd, szFileName, "This program is:", MB_OK | MB_ICONINFORMATION);
        }
    	break;
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

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON)); 
						//LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MYMENU); 
						//NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON), IMAGE_ICON, 16, 16, 0); 
						//LoadIcon(NULL, IDI_APPLICATION);

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

    // Step 3: The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
