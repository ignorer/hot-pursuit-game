#pragma once

#include "UI/ButtonUtils.h"
#include <vector>

namespace UI {
	class CComboBox {
	public:
		CComboBox( const Gdiplus::Rect& _rect, const std::vector<const wchar_t*>& _names );

		void Draw( Gdiplus::Graphics& graphics );

		void Collapse();
		void Expand();

		// Возвращает true, если обработал клик
		bool OnClick( int x, int y );

		bool IsCollapsed();

		static Gdiplus::Image* clearBoxImage;
		static Gdiplus::Image* collapsedBoxImage;
		static Gdiplus::Image* expandedBoxImage;
	private:
		Gdiplus::Rect rect;
		std::vector<const wchar_t*> names;
		int pos;
		bool isCollapsed;
	};

}