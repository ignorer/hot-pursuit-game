#include <memory>

#include "UI/Map.h"
#include "UI/Drawing.h"

namespace UI {
	CMap::CMap( const std::vector<std::vector<int>>& map_data ) :
		map( map_data ),
		cellSize( 0 ),
		indent( 0, 0 )
	{
	}

	void CMap::Calculate()
	{
		int n = map.size(), m = map[0].size();
		float height = glutGet( GLUT_WINDOW_HEIGHT ),
			width = glutGet( GLUT_WINDOW_WIDTH );
		cellSize = fmin( height / n, width / m );
		indent.x = (width - cellSize * m) / 2;
		indent.y = (height - cellSize * n) / 2;
	}

	void CMap::drawAllCellsOfType( int cellType ) const
	{
		switch( cellType ) {
			case 1:
				glBindTexture( GL_TEXTURE_2D, CDrawing::forestCell );
				break;
			case 0:
				glBindTexture( GL_TEXTURE_2D, CDrawing::roadCell );
				break;
			case 3:
				glBindTexture( GL_TEXTURE_2D, CDrawing::wallCell );
				break;
			default:
				throw std::invalid_argument( "Wrong nuber of texture" );
		}
		glBegin( GL_QUADS );
		{
		int n = map.size( ), m = map[0].size( );
		for( int i = 0; i < n; i++ ) {
			for( int j = 0; j < m; j++ ) {
				if( map[i][j] == cellType ) {
					float left = j * cellSize + indent.x;
					float right = (j + 1) * cellSize + indent.x;
					float bottom = (n - i) * cellSize + indent.y;
					float top = (n - (i + 1)) * cellSize + indent.y;
						glTexCoord2f( 0, 0 ); glVertex2f( left, bottom );
						glTexCoord2f( 1, 0 ); glVertex2f( right, bottom );
						glTexCoord2f( 1, 1 ); glVertex2f( right, top );
						glTexCoord2f( 0, 1 ); glVertex2f( left, top );
					}
				}
			}
		}
		glEnd();
	}

	void CMap::MarkHighlightedCells( const std::vector<Core::CCoordinates>& possibleMoves )
	{
		for( auto cell : possibleMoves ) {
			highlightedCells.insert( CCoordinates( cell.x, cell.y ) );
		}
	}

	void CMap::UnmarkHighlightedCells( const std::vector<Core::CCoordinates>& possibleMoves )
	{
		for( auto cell : possibleMoves ) {
			highlightedCells.erase( CCoordinates( cell.x, cell.y ) );
		}
	}

	void CMap::DrawFinishLine( std::pair<CCoordinates, CCoordinates> finishLine ) const
	{
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, CDrawing::finish );
		glColor3f( 1, 1, 1 );
		glBegin( GL_POLYGON );
		{
			auto point1 = transateToWcoord( finishLine.first.x + 0.5, finishLine.first.y + 0.5, cellSize, indent, GetSize() );
			auto point2 = transateToWcoord( finishLine.second.x + 0.5, finishLine.second.y + 0.5, cellSize, indent, GetSize() );
			double distance = std::hypot( finishLine.first.x - finishLine.second.x, finishLine.first.y - finishLine.second.y );
			double distanceWindow = std::hypot( point1.x - point2.x, point1.y - point2.y );
			std::pair<double, double> direction( (point2.x - point1.x) /  distanceWindow, (point2.y - point1.y) / distanceWindow );
			direction = std::make_pair( direction.second, -direction.first );
			glTexCoord2f( 0.0f, 0.0f ); glVertex2f( point1.x - 5 * direction.first, point1.y - 5 * direction.second );
			glTexCoord2f( distance, 0.0f ); glVertex2f( point2.x - 5 * direction.first, point2.y - 5 * direction.second );
			glTexCoord2f( distance, 1.0f ); glVertex2f( point2.x + 5 * direction.first, point2.y + 5 * direction.second );
			glTexCoord2f( 0.0f, 1.0f ); glVertex2f( point1.x + 5 * direction.first, point1.y + 5 * direction.second );
		}
		glEnd();
		glDisable( GL_BLEND );
		glDisable( GL_TEXTURE_2D );
	}

	void CMap::DrawShot( const std::pair<std::pair<float, float>, std::pair<float, float>>& shot ) const
	{
		glColor3f( 1, 0, 0 );
		glLineWidth( 3 );
		glBegin( GL_LINES );
		{
			auto point1 = transateToWcoord( shot.first.first + 0.5, shot.first.second + 0.5, cellSize, indent, GetSize() );
			auto point2 = transateToWcoord( shot.first.first + 0.5 + shot.second.first * 100, shot.first.second + 0.5 + shot.second.second * 100, cellSize, indent, GetSize() );
			glVertex2d( point1.x, point1.y ); glVertex2d( point2.x, point2.y );
		}
		glEnd();
	}

	void CMap::Draw() const
	{
		glEnable( GL_TEXTURE_2D );
		glEnable( GL_BLEND );
		glColor3f( 1, 1, 1 );

		drawAllCellsOfType( 0 );
		drawAllCellsOfType( 1 );
		drawAllCellsOfType( 3 );

		glDisable( GL_BLEND );
		glDisable( GL_TEXTURE_2D );
	}

	void CMap::HighlightActiveCells() const
	{
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, CDrawing::activeCell );
		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glColor4f( 1, 1, 1, 0.5 );
		for( auto cell : highlightedCells ) {
			auto center = transateToWcoord( cell.x + 0.5, cell.y + 0.5, cellSize, indent, GetSize() );
			auto top = center.y - 0.5 * cellSize;
			auto left = center.x - 0.5 * cellSize;
			auto right = center.x + 0.5 * cellSize;
			auto bottom = center.y + 0.5 * cellSize;
			glBegin( GL_POLYGON );
			{
				glTexCoord2f( 0, 1 ); glVertex2f( left, top );
				glTexCoord2f( 1, 1 ); glVertex2f( right, top );
				glTexCoord2f( 1, 0 ); glVertex2f( right, bottom );
				glTexCoord2f( 0, 0 ); glVertex2f( left, bottom );
			}
			glEnd();
		}
		glDisable( GL_BLEND );
		glDisable( GL_TEXTURE_2D );
	}

	float CMap::GetCellSize() const
	{
		return cellSize;
	}

	CWindowCoordinates CMap::GetIndent() const
	{
		return indent;
	}

	CSize CMap::GetSize() const
	{
		return CSize( map[0].size(), map.size() );
	}
}