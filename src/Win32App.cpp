#include "Win32App.h"


using namespace std;
using namespace DirectX;

// Static variable is used to put the proc function into the class.
Win32App* Win32App::instanceForProc = nullptr;
Win32App* Win32App::GetinstanceForProc()
{
	return instanceForProc;
}
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return Win32App::GetinstanceForProc()->WndProc(hwnd, msg, wParam, lParam);
}
//

Win32App::Win32App(const int kWidth, const int KHeight)
	:kWidth(kWidth), kHeight(KHeight)
{
	instanceForProc = this;
}

Win32App::~Win32App()
{
}

bool Win32App::Initialize(HINSTANCE hInstance)
{
	// Just call it once.
	assert(mhMainWnd == nullptr);

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"MainWnd";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}


	mhMainWnd = CreateWindow(L"MainWnd", L"d3d App",
		(WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX)), // disable resizing and maximzing 
		CW_USEDEFAULT, CW_USEDEFAULT, kWidth, kHeight,
		0, 0, hInstance, 0);

	if (!mhMainWnd)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	ShowWindow(mhMainWnd, SW_SHOW);
	UpdateWindow(mhMainWnd);


	return true;
}


LRESULT Win32App::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
		OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}



int Win32App::Run()
{
	assert(mhMainWnd != nullptr);

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
			Update();
			Draw();
        }
    }

	return (int)msg.wParam;
}


void Win32App::InitDirectX()
{
	// Call after window init
	assert(mhMainWnd != nullptr);

	// Just call it once.
	assert(dxApp == nullptr);
	//std::make_unique<DX12App> adxApp_;

	dxApp = std::make_unique<DX12App>(kWidth, kHeight, mhMainWnd);

	vector<Vertex> vertices =
	{
		Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f) }),
		Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f) }),
		Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f) }),
		Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f) }),
		Vertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f) }),
		Vertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f) }) //, XMFLOAT4(Colors::Black)
	};

	vector<uint16_t> indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	dxApp->SetVertexIndexResource(vertices, indices);
}

void Win32App::CreateObjects(const int count, const float scale)
{
	// Call after directx init
	assert(dxApp != nullptr);

	dxApp->Initialize(count, scale);
	
}

void Win32App::Update()
{
	if (dxApp)
	{
		dxApp->Update();
	}
}

void Win32App::Draw()
{
	if (dxApp)
	{
		dxApp->Draw();
	}
}




void Win32App::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}

void Win32App::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void Win32App::OnMouseMove(WPARAM btnState, int x, int y)
{
	if (dxApp)
	{
		if ((btnState & MK_LBUTTON) != 0)
		{
			dxApp->UpdateVirtualSphereAngles(mLastMousePos, x, y);
		}
		else if ((btnState & MK_RBUTTON) != 0)
		{
			dxApp->UpdateVirtualSphereRadius(mLastMousePos, x, y);
		}

		mLastMousePos.x = x;
		mLastMousePos.y = y;
	}
}


HWND Win32App::GetMhMainWnd()
{
	return mhMainWnd;
}