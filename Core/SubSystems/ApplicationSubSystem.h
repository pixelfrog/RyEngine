#pragma once

#include <string>
#include <Windows.h>

#include "SubSystem.h"

namespace RyEngine
{
	class ApplicationSubSystem : public SubSystem
	{
	public:
		ApplicationSubSystem() {}
		virtual void OnRegister();
		virtual void OnStart();

	protected:
		HINSTANCE _hInstance;
	};
}