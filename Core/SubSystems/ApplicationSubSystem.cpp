#include <Windows.h>

#include "ApplicationSubSystem.h"

namespace RyEngine
{
	void ApplicationSubSystem::OnRegister()
	{
		_hInstance = (HINSTANCE)GetModuleHandle(NULL);
	}
	
	void ApplicationSubSystem::OnStart(){}
}