#include "ComboBox.h"

Gdiplus::Image* UI::CComboBox::expandedBoxImage;
Gdiplus::Image* UI::CComboBox::clearBoxImage;
Gdiplus::Image* UI::CComboBox::collapsedBoxImage;

UI::CComboBox::CComboBox( const Gdiplus::Rect& _rect, const std::vector<const wchar_t*>& _names ) :
	rect( _rect ),
	names( _names ),
	isCollapsed( true ),
	pos( 0 )
{}

void UI::CComboBox::Draw( Gdiplus::Graphics& graphics )
{
	Gdiplus::SolidBrush brush( Gdiplus::Color( 255, 0, 0, 0 ) );
	Gdiplus::FontFamily fontFamily( L"Times new roman" );
	Gdiplus::Font font( &fontFamily, 16, Gdiplus::FontStyleBold, Gdiplus::UnitPixel );

	if( isCollapsed ) {
		graphics.DrawImage( collapsedBoxImage, rect );

		Gdiplus::PointF pointF( rect.X + 5, rect.Y + 8 );
		graphics.DrawString( names[pos], -1, &font, pointF, &brush );
	} else {
		Gdiplus::Rect imageRect(rect);
		for( int i = 0; i < names.size(); ++i ) {
			imageRect.Y = rect.Y + (pos - i) * (imageRect.GetTop() - imageRect.GetBottom());
			if( i != pos ) {
				graphics.DrawImage( clearBoxImage, imageRect );
			} else {
				graphics.DrawImage( expandedBoxImage, imageRect );
			}
			Gdiplus::PointF pointF( imageRect.X + 5, imageRect.Y + 8 );
			graphics.DrawString( names[i], -1, &font, pointF, &brush );
		}
	}
}

void UI::CComboBox::Collapse()
{
	isCollapsed = true;
}

void UI::CComboBox::Expand()
{
	isCollapsed = false;
}

bool UI::CComboBox::OnClick( int x, int y )
{
	if( isCollapsed ) {
		if( rect.Contains( x, y ) ) {
			isCollapsed = false;
			return true;
		}
	} else {
		Gdiplus::Rect newRect( rect );
		for( int i = 0; i < names.size(); ++i ) {
			newRect.Y = rect.Y + (pos - i) * (newRect.GetTop() - newRect.GetBottom());
			if( newRect.Contains( x, y ) ) {
				pos = i;
				isCollapsed = true;
				return true;
			}
		}
	}
	return false;
}

bool UI::CComboBox::IsCollapsed()
{
	return isCollapsed;
}
