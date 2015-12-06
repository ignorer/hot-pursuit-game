#pragma once

#include <Windows.h> 
#include <gdiplus.h>

namespace UI {
	struct ButtonInfo
	{
		Gdiplus::Image* curButtonImage;
		Gdiplus::Rect	buttonRect;
		const wchar_t* buttonName;
		RECT buttonNameRect;
	};
}