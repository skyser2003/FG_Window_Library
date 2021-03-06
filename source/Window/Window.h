#pragma once

#include <functional>
#include <vector>
#include <memory>


namespace FG
{
	class DxCanvas;
	class GraphicsClass;
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

		void RegisterInput(std::shared_ptr<Input> input);
		std::vector<std::shared_ptr<Input>>& GetInputList();

		GraphicsClass& GetGraphics() const;
		DxCanvas& GetCanvas() const;

		int GetScreenWidth() const;
		int GetScreenHeight() const;
	private:
		void InitializeDirectX();
		void DestroyDirectX();

		HWND hWnd;
		HINSTANCE hInstance;

		int mWidth, mHeight;

		std::function<int(Window*)> mMainFunction;
		std::vector<std::shared_ptr<Input>> mInputList;

		DxCanvas* mCanvas;
	};
}