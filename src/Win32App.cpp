#include "Win32App.h"


using namespace std;
using namespace DirectX;

// Static variable is used to put the proc function into the class.
Win32App* Win32App::_instanceForProc[2] = { nullptr, nullptr };
Win32App* Win32App::getinstanceForProc(int i)
{
	return _instanceForProc[i];
}
LRESULT CALLBACK mainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return Win32App::getinstanceForProc(0)->wndProc(hwnd, msg, wParam, lParam);
}
//

Win32App::Win32App(const int kWidth, const int KHeight)
	:_kWidth(kWidth), _kHeight(KHeight)
{
	_instanceForProc[0] = this;
}

Win32App::~Win32App()
{
	delete _dxApp;
}

bool Win32App::initialize(HINSTANCE hInstance)
{
	// Just call it once.
	assert(_mhMainWnd[0] == nullptr);

	WNDCLASS wc[2];
	wc[0].style = CS_HREDRAW | CS_VREDRAW;
	wc[0].lpfnWndProc = mainWndProc;
	wc[0].cbClsExtra = 0;
	wc[0].cbWndExtra = 0;
	wc[0].hInstance = hInstance;
	wc[0].hIcon = LoadIcon(0, IDI_APPLICATION);
	wc[0].hCursor = LoadCursor(0, IDC_ARROW);
	wc[0].hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc[0].lpszMenuName = 0;
	wc[0].lpszClassName = L"MainWnd";

	wc[1] = wc[0]; // Duplicate settings

	if (!RegisterClass(&wc[0]) && !RegisterClass(&wc[1])) return 0;

	_mhMainWnd[0] = CreateWindow(L"MainWnd", L"d3d App",
		(WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX)), // disable resizing and maximzing 
		0, 0, _kWidth, _kHeight,
		0, 0, hInstance, 0);

	_mhMainWnd[1] = CreateWindow(L"MainWnd", L"d3d App",
		(WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX)), // disable resizing and maximzing 
		100, 100, _kWidth, _kHeight,
		0, 0, hInstance, 0);


	ShowWindow(_mhMainWnd[0], SW_SHOW);
	ShowWindow(_mhMainWnd[1], SW_SHOW);
	UpdateWindow(_mhMainWnd[0]);
	UpdateWindow(_mhMainWnd[1]);


	return true;
}                 

LRESULT Win32App::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{
		// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		_onMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		_onMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEMOVE:
		_onMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_KEYDOWN:
		switch (wParam) 
		{
			// v
		case 0x56:
			_dxApp->dvel = !_dxApp->dvel;
			break;
		case VK_LEFT:
			_dxApp->pvel = !_dxApp->pvel;
			break;
		}
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}


int Win32App::run()
{
	assert(_mhMainWnd[0] != nullptr);

	MSG msg = {0};
	while(msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if(PeekMessage( &msg, 0, 0, 0, PM_REMOVE ))
		{
            TranslateMessage( &msg );
            DispatchMessage( &msg );
		}
		// Otherwise, do animation/game stuff.
		else
        {	
			_update();
			_draw();
        }
    }

	return (int)msg.wParam;
}


void Win32App::initDirectX(DX12App* dxapp)
{
	// Call after window init.
	assert(_mhMainWnd[0] != nullptr);

	_dxApp = dxapp;

	_dxApp->setWindow(_kWidth, _kHeight, _mhMainWnd[0]);
	_dxApp->initialize();
}

void Win32App::_update()
{
	if (_dxApp)
	{
		_dxApp->update();
	}
}

void Win32App::_draw()
{
	if (_dxApp)
	{
		_dxApp->draw();
	}
}




void Win32App::_onMouseDown(WPARAM btnState, int x, int y)
{
	_mLastMousePos.x = x;
	_mLastMousePos.y = y;

	if (_dxApp)
	{
		if ((btnState & MK_MBUTTON) != 0)
		{
			_dxApp->resetVirtualSphereAnglesRadius();
		}
	}

	SetCapture(_mhMainWnd[0]);
}

void Win32App::_onMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void Win32App::_onMouseMove(WPARAM btnState, int x, int y)
{
	if (_dxApp)
	{
		if ((btnState & MK_LBUTTON) != 0)
		{
			_dxApp->updateVirtualSphereAngles(_mLastMousePos, x, y);
		}
		else if ((btnState & MK_RBUTTON) != 0)
		{
			_dxApp->updateVirtualSphereRadius(_mLastMousePos, x, y);
		}

		_mLastMousePos.x = x;
		_mLastMousePos.y = y;
	}
}