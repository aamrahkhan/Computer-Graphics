// Assignment2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "math.h"
#include "CS452Assign1.h"

const char g_szClassName[] = "myWindowClass";

int w = 600, h = 600;
HGLRC hRC;
HDC hDC;
HWND hwnd;
POINT mouseLoc;

int round(float f) 
{
	float t = f - floor(f);
	if (t < 0.5)
		return floor(f);
	else
		return ceil(f);
}

void SetupPixelFormat()
{
	PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),          // size of structure
            1,                                      // default version
            PFD_DRAW_TO_WINDOW |                    // window drawing support
            PFD_SUPPORT_OPENGL |                    // opengl support
            PFD_DOUBLEBUFFER,                       // double buffering support
            PFD_TYPE_RGBA,                          // RGBA color mode
            32,                                     // 32 bit color mode
            0, 0, 0, 0, 0, 0,                       // ignore color bits
            0,                                      // no alpha buffer
            0,                                      // ignore shift bit
            0,                                      // no accumulation buffer
            0, 0, 0, 0,                             // ignore accumulation bits
            16,                                     // 16 bit z-buffer size
            0,                                      // no stencil buffer
            0,                                      // no aux buffer
            PFD_MAIN_PLANE,                         // main drawing plane
            0,                                      // reserved
            0, 0, 0
	};
	
	// Choose best matching format
    int nPixelFormat = ChoosePixelFormat(hDC, &pfd);

	if (nPixelFormat == 0)
	{
		PostQuitMessage(0);
	}
	// Set the pixel format to the device context
    SetPixelFormat(hDC, nPixelFormat, &pfd);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_MOUSEMOVE)
	{
		mouseLoc.x = GET_X_LPARAM(lParam);
		mouseLoc.y = GET_Y_LPARAM(lParam);
	}
    else if (msg == WM_GETMINMAXINFO)
	{
		MINMAXINFO *info = (MINMAXINFO*)lParam;
		info->ptMaxSize.x = w;
		info->ptMaxSize.y = h;
	}
	else if (msg == WM_SIZING)
	{
		RECT *bounds = (RECT*)lParam;
		bounds->bottom = bounds->top + h;
		bounds->right = bounds->left + w;
	}
	else if (msg == WM_CLOSE)
	{
		DestroyWindow(hwnd);
	}
	else if (msg == WM_DESTROY)
	{
		wglMakeCurrent(hDC, NULL);
		wglDeleteContext(hRC);
		PostQuitMessage(0);
	}
	else if (msg == WM_CREATE)
	{
		hDC = GetDC(hwnd);
		SetupPixelFormat();
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC,hRC);
		mouseLoc.x = 0;
		mouseLoc.y = 0;
	}
	else
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    MSG Msg;

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
    wc.lpszClassName = (LPCWSTR)g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, (LPCWSTR)"Window Registration Failed!", (LPCWSTR)"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        (LPCWSTR)g_szClassName,
        (LPCWSTR)"CS 452 - Assignment 1",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
		CW_USEDEFAULT,
		w, h,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, (LPCWSTR)"Window Creation Failed!", (LPCWSTR)"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

	Initialize();
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
	
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
		Render(mouseLoc.x, mouseLoc.y);
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

	Destroy();
    return Msg.wParam;
}

void setWindowSize(int width, int height)
{
	SetWindowPos(hwnd, 0, 50, 50, width, height, 0);
	w = width;
	h = height;
}

void renderImage(FIBITMAP *bitmap, float scale)
{
	StretchDIBits(hDC, 0, 0, round(scale*FreeImage_GetWidth(bitmap)), round(scale*FreeImage_GetHeight(bitmap)),
		0, 0, FreeImage_GetWidth(bitmap), FreeImage_GetHeight(bitmap),
		FreeImage_GetBits(bitmap), FreeImage_GetInfo(bitmap), DIB_RGB_COLORS, SRCCOPY);
}
