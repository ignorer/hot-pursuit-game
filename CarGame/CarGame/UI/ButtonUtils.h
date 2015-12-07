#pragma once

#include <Windows.h> 
#include <gdiplus.h>


namespace UI {
	class ButtonInfo
	{
	public:
		Gdiplus::Image* curButtonImage;
		Gdiplus::Rect	buttonRect;
		const wchar_t* buttonName;
		RECT buttonNameRect;
	};
	struct ButtonImages
	{
	public:
		Gdiplus::Image* defButtonImage;
		Gdiplus::Image* hoverButtonImage;
		Gdiplus::Image* pressedButtonImage;
	};
	
}