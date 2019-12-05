#pragma once

#include <string>
#include <vector>

#include "SubSystems/ApplicationSubSystem.h"
#include "Window.h"

namespace RyEngine
{
	class WindowSystem : public ApplicationSubSystem
	{
	public:
		WindowSystem() : ApplicationSubSystem() {}

		void OnRegister();
		void OnStart();

		Window* get_mainWindow();

		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		Window* _mainWin = NULL;
		std::vector<Window*> _windows;

		WCHAR _winTitle[STRING_BUFFER_MAX];
		WCHAR _winClass[STRING_BUFFER_MAX];
		UINT _defaultWidth = 800;
		UINT _defaultHeight = 600;

		ATOM RegisterWindowClass();
		Window* AddWindow(LPWSTR winTitle);
		Window* AddWindow(LPWSTR winTitle, int cmdShow, UINT width, UINT height);
	};
}