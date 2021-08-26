#include "Win32App.h"


using namespace std;
using namespace DirectX;

// Static variable is used to put the proc function into the class.
Win32App* Win32App::_instanceForProc = nullptr;
Win32App* Win32App::getinstanceForProc()
{
	return _instanceForProc;
}
LRESULT CALLBACK directXWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return Win32App::getinstanceForProc()->mainWndProc(hwnd, msg, wParam, lParam);
}
LRESULT CALLBACK controllWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return Win32App::getinstanceForProc()->subWndProc(hwnd, msg, wParam, lParam);
}
//

Win32App::Win32App(const int kWidth, const int KHeight)
	:_kWidth(kWidth), _kHeight(KHeight)
{
	_instanceForProc = this;
}

Win32App::~Win32App()
{
	delete _dxApp;
}

bool Win32App::initialize(HINSTANCE hInstance)
{
	WNDCLASS wc[2];
	wc[0].style = CS_HREDRAW | CS_VREDRAW;
	wc[0].lpfnWndProc = directXWndProc;
	wc[0].cbClsExtra = 0;
	wc[0].cbWndExtra = 0;
	wc[0].hInstance = hInstance;
	wc[0].hIcon = LoadIcon(0, IDI_APPLICATION);
	wc[0].hCursor = LoadCursor(0, IDC_ARROW);
	wc[0].hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc[0].lpszMenuName = 0;
	wc[0].lpszClassName = L"DirectXWnd";

	wc[1] = wc[0]; // Duplicate settings
	wc[1].lpfnWndProc = controllWndProc;
	wc[1].lpszClassName = L"ControllWnd";
	wc[1].hbrBackground = (HBRUSH)CreateSolidBrush(RGB(225, 225, 225));

	RegisterClass(&wc[0]);
	RegisterClass(&wc[1]);

	int offsetX = 200;
	int offsetY = 100;

	_mhWnd[0] = CreateWindow(L"DirectXWnd", L"d3d App",
		(WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX)), // disable resizing and maximzing 
		offsetX, offsetY, _kWidth, _kHeight,
		0, 0, hInstance, 0);

	_mhWnd[1] = CreateWindow(L"ControllWnd", L"Controller",
		(WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX)), // disable resizing and maximzing 
		offsetX + _kWidth, offsetY, 300, _kHeight,
		0, 0, hInstance, 0);

	_hInstance = hInstance;

	for (int i = 0; i < 2; i++)
	{
		ShowWindow(_mhWnd[i], SW_SHOW);
		UpdateWindow(_mhWnd[i]);
	}
	
	return true;
}                 

LRESULT Win32App::mainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
			break;
		case VK_LEFT:
			break;
		}
		return 0;

	case WM_MOVING:
		_synchronizeWinPos(_WINDOW::SUB);
		return 0;

	case WM_SIZE:
		_switchWinState(_WINDOW::SUB);
		return 0;
	}
	

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT Win32App::subWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:

	{	
		CreateWindow(L"button", L"Grid : ON ", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			90, 30, 100, 25, hwnd, (HMENU)0, _hInstance, NULL);
		CreateWindow(L"button", L"Particle : ON ", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			30, 60, 100, 25, hwnd, (HMENU)1, _hInstance, NULL);
		CreateWindow(L"button", L"Velcoity : OFF ", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			150, 60, 100, 25, hwnd, (HMENU)2, _hInstance, NULL);

		CreateWindow(L"button", L"бл", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			65, 100, 50, 25, hwnd, (HMENU)3, _hInstance, NULL);
		CreateWindow(L"button", L"бс", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			115, 100, 50, 25, hwnd, (HMENU)4, _hInstance, NULL);
		CreateWindow(L"button", L"в║l", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			175, 100, 50, 25, hwnd, (HMENU)5, _hInstance, NULL);
	}
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 0:
		{
			bool flag = !_dxApp->getDrawFlag(FLAG::GRID);
			SetDlgItemText(hwnd, 0, flag ? L"Grid : ON " : L"Grid : OFF");
			_dxApp->setDrawFlag(FLAG::GRID, flag);
			_draw();
		}
			break;
		case 1:
		{
			bool flag = !_dxApp->getDrawFlag(FLAG::PARTICLE);
			SetDlgItemText(hwnd, 1, flag ? L"Particle : ON " : L"Particle : OFF");
			_dxApp->setDrawFlag(FLAG::PARTICLE, flag);
			_draw();
		}
			break;
		case 2:
		{
			bool flag = !_dxApp->getDrawFlag(FLAG::VELOCITY);
			SetDlgItemText(hwnd, 2, flag ? L"Velocity : ON " : L"Velocity : OFF");
			_dxApp->setDrawFlag(FLAG::VELOCITY, flag);
			_draw();
		}
			break;
		case 3:
		{
			updateFlag = !updateFlag;
			SetDlgItemText(hwnd, 3, updateFlag ? L"бл" : L"в║");
		}
			break;
		case 4:
		{
			_dxApp->resetSimulationState();
			_update();
			_draw();
		}
			break;
		case 5:
		{
			if (!updateFlag)
			{
				_update();
				_draw();
			}
		}
			break;
		}
		return 0;

		// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_MOVING:
		_synchronizeWinPos(_WINDOW::MAIN);
		return 0;

	case WM_SIZE:
		_switchWinState(_WINDOW::MAIN);
		return 0;
	
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int Win32App::run()
{
	assert(_mhWnd[0] != nullptr);

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
			if (updateFlag)
			{
				_update();
				_draw();
			}
        }
    }

	return (int)msg.wParam;
}


void Win32App::initDirectX(DX12App* dxapp)
{
	// Call after window init.
	assert(_mhWnd[0] != nullptr);

	_dxApp = dxapp;

	_dxApp->setWindow(_kWidth, _kHeight, _mhWnd[0]);
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

	SetCapture(_mhWnd[0]);
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


void Win32App::_synchronizeWinPos(_WINDOW state)
{
	int i = static_cast<int>(state);
	int i_1 = (i + 1) % 2;
	WINDOWINFO winfo;

	GetWindowInfo(_mhWnd[i_1], &winfo);
	SetWindowPos(_mhWnd[i], NULL,
		// If _WINDOW::SUB,  (-1 + i * 2) == -1 + 2 == 1 
		//			thus, winfo.rcWindow.left + _kWidth
		// If _WINDOW::MAIN, (-1 + i * 2) == -1 + 0 == -1
		//			thus, winfo.rcWindow.left - _kWidth
		winfo.rcWindow.left + (-1 + i * 2) * _kWidth,
		winfo.rcWindow.top,
		0, 0, SWP_NOSIZE);
}

void Win32App::_switchWinState(_WINDOW state)
{
	int i = static_cast<int>(state);
	ShowWindow(_mhWnd[i], _swState[i]); // SW_NORMAL = 1, SW_MINIMIZE = 6
	_swState[i] = (_swState[i] * 6) % 35; //Repeat 0,6
}
