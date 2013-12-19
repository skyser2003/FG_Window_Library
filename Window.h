#pragma once

#include <functional>
#include <vector>
#include <memory>

class DxCanvas;
class GraphicsClass;

namespace FG
{
	class Input;

	class Window
	{
	public:
		Window() {}
		Window(const Window& rhs);
		Window& operator=(const Window& rhs);

		void Initialize(_In_ HINSTANCE hInstance, _In_ int nCmdShow, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine);
		void RegisterMainFunction(std::function<int(Window*)> mainFunction);
		int Run();
		void Destroy();

		HWND GetHwnd() const;

		void RegisterInput(Input* input);
		std::vector<Input*>& GetInputList();

		GraphicsClass& GetGraphics() const;
	private:
		void InitializeDirectX();
		void DestroyDirectX();

		HWND hWnd;
		HINSTANCE hInstance;

		int mWidth, mHeight;

		std::function<int(Window*)> mMainFunction;
		std::vector<Input*> mInputList;

		DxCanvas* mCanvas;
	};
}