#include "stdafx.h"
#include "Window.h"

#include "Input/Input.h"
#include "WindowManager/WindowManager.h"

#include "Canvas/DxCanvas.h"

#include "Graphics/graphicsclass.h"
#include "Model/modelclass.h"
#include "Texture/textureclass.h"
#include "Light/lightclass.h"

namespace FG
{

Window::Window(const Window& rhs)
{

}
Window& Window::operator=(const Window& rhs)
{
	mMainFunction = rhs.mMainFunction;
	
	for(auto input : rhs.mInputList)
	{
		RegisterInput(input);
	}

	return *this;
}

void Window::Initialize(_In_ HINSTANCE hInstance, _In_ int nCmdShow, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine)
{
	this->hInstance = hInstance;
	mWidth = 1024;
	mHeight = 768;

	WNDCLASSEX wc;

	ZeroMemory(&wc,sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "WindowClass";

	RegisterClassEx(&wc);

	RECT wr = {0, 0, mWidth, mHeight};
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(NULL,
		"WindowClass",
		"Test Title",
		WS_OVERLAPPEDWINDOW,
		300,
		300,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// Member variables initialize
	mInputList.clear();
	mMainFunction = [](Window*){ return 1;};

	InitializeDirectX();
}
void Window::RegisterMainFunction(std::function<int(Window*)> mainFunction)
{
	this->mMainFunction = mainFunction;
}
int Window::Run()
{
	MSG msg;

	while (true)
	{
		for(auto input : mInputList)
		{
			input->Start();
		}

		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			break;
		}

		int exit = mMainFunction(this);

		for(auto input : mInputList)
		{
			input->End();
		}

		if(exit == 0)
			break;
	}

	return static_cast<int>(msg.wParam);
}
void Window::Destroy()
{
	DestroyDirectX();

	mWidth = 0;
	mHeight = 0;

	for(auto input : mInputList)
	{
		input->Destroy();
	}

	mInputList.clear();
}

HWND Window::GetHwnd() const
{
	return hWnd;
}

void Window::RegisterInput(std::shared_ptr<Input> input)
{
	input->Initialize();
	mInputList.push_back(input);
}
std::vector<std::shared_ptr<Input>>& Window::GetInputList()
{
	return mInputList;
}

GraphicsClass& Window::GetGraphics() const
{
	return *mCanvas->GetGraphics();
}
DxCanvas& Window::GetCanvas() const
{
	return *mCanvas;
}

int Window::GetScreenWidth() const
{
	return mWidth;
}
int Window::GetScreenHeight() const
{
	return mHeight;
}

void Window::InitializeDirectX()
{
	mCanvas = new DxCanvas;
	mCanvas->Initialize(hWnd, mWidth, mHeight);
}
void Window::DestroyDirectX()
{
	delete mCanvas;
}

} // namespace FG