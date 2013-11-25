#include "stdafx.h"
#include "WindowManager.h"

#include "Window.h"
#include "Input.h"

namespace FG
{
WindowManager& WindowManager::GetInstance() 
{
	static WindowManager windowManger;

	return windowManger;
}

void WindowManager::Initialize(_In_ HINSTANCE hInstance, _In_ int nCmdShow, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine)
{
	this->hInstance = hInstance;
	this->nCmdShow = nCmdShow;
	this->hPrevInstance = hPrevInstance;
	this->lpCmdLine = lpCmdLine;
}
void WindowManager::Run()
{
	for(auto window : windowList)
	{
		window->Run();
	}
}
void WindowManager::Destroy()
{
	for(auto window : windowList)
	{
		if(window != nullptr)
		{
			window->Destroy();
		}
	}

	windowList.clear();
}

std::shared_ptr<Window> WindowManager::CreateWindowInstance()
{
	auto* window = new Window;
	window->Initialize(hInstance, nCmdShow, hPrevInstance, lpCmdLine);

	std::shared_ptr<Window> sharedWindow(window);

	windowList.push_back(sharedWindow);

	return sharedWindow;
}

std::vector<std::shared_ptr<Window>>::iterator WindowManager::begin()
{
	return windowList.begin();
}
std::vector<std::shared_ptr<Window>>::iterator WindowManager::end()
{
	return windowList.end();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto& wm = WindowManager::GetInstance();

	for(auto window : wm)
	{
		auto inputList = window->GetInputList();

		for(auto input : inputList)
		{
			input->OnMsg(message, wParam, lParam);
		}
	}

	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			// TODO: 여기에 그리기 코드를 추가합니다.
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
}