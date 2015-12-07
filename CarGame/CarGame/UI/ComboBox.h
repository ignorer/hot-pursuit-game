#pragma once

#include "UI/ButtonUtils.h"
#include <vector>

namespace UI {
	class CComboBox {
	public:
		CComboBox( const Gdiplus::Rect& _rect, const std::vector<std::wstring>& _names );

		void Draw( Gdiplus::Graphics& graphics );

		void Collapse();
		void Expand();

		// Возвращает true, если обработал клик
		bool OnClick( int x, int y );

		bool IsCollapsed() const;

		const wchar_t* GetChoosedName() const;

		void SetChoosedPosition( int position );

		static Gdiplus::Image* clearBoxImage;
		static Gdiplus::Image* collapsedBoxImage;
		static Gdiplus::Image* expandedBoxImage;
	private:
		Gdiplus::Rect rect;
		std::vector<std::wstring> names;
		int pos;
		bool isCollapsed;
	};

}