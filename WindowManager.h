#pragma once

#include <vector>
#include <functional>
#include <memory>

namespace FG
{
class Window;

class WindowManager
{
public:
	static WindowManager& GetInstance();

	void Initialize(_In_ HINSTANCE hInstance, _In_ int nCmdShow, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine);
	void Run();
	void Destroy();

	std::shared_ptr<Window> CreateWindowInstance();
	std::vector<std::shared_ptr<Window>>& GetWindowList() const;
	void CloseAllWindow();

	// 'ranged for' support
	std::vector<std::shared_ptr<Window>>::iterator begin();
	std::vector<std::shared_ptr<Window>>::iterator end();
private:
	HINSTANCE hInstance;
	int nCmdShow;
	HINSTANCE hPrevInstance;
	LPTSTR lpCmdLine;

	std::vector<std::shared_ptr<Window>> windowList;
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
}