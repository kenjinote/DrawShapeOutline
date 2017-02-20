#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment(lib,"gdiplus.lib")

#include <windows.h>
#include <gdiplus.h>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace Gdiplus;
TCHAR szClassName[] = TEXT("Window");

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			{
				Graphics g(hdc);
				Gdiplus::GraphicsPath path;
				{
					Gdiplus::PointF pointStar[10];
					int i = 0;
					for (auto & v : pointStar)
					{
						v.X = 500 + (i % 2 ? 300 : 150) * cosf(1+2 * (float)M_PI * i / _countof(pointStar));
						v.Y = 500 + (i % 2 ? 300 : 150) * sinf(1+2 * (float)M_PI * i / _countof(pointStar));
						++i;
					}
					path.AddPolygon(pointStar, _countof(pointStar));
				}
				RectF rect = { 50, 50, 750, 250 };
				path.AddRectangle(rect);

				path.Outline();

				Pen pen(Color::Black, 1.0f);
				pen.SetLineJoin(LineJoinRound);
				g.DrawPath(&pen, &path);
			}
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	ULONG_PTR gdiToken;
	GdiplusStartupInput gdiSI;
	GdiplusStartup(&gdiToken, &gdiSI, NULL);
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("Window"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	GdiplusShutdown(gdiToken);
	return (int)msg.wParam;
}
