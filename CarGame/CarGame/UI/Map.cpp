#include <memory>

#include "UI/Map.h"

namespace UI {
	CMap::CMap() :
		textureBoard( 0 ),
		textureRoad( 0 ),
		textureMap( 0 ),
		textureActiveCell( 0 ),
		textureFinish( 0 ),
		cellSize( 0 ),
		indent( 0, 0 ),
		needReload( 0 )
	{}

	CMap::CMap( const std::vector<std::vector<int>>& map_data ) :
		textureBoard( 0 ),
		textureRoad( 0 ),
		textureActiveCell( 0 ),
		textureFinish( 0 ),
		textureMap( 0 ),
		map( map_data ),
		cellSize( 0 ),
		indent( 0, 0 ),
		needReload( true )
	{
		glGenTextures( 1, &textureMap );
		glGenTextures( 1, &textureBoard );
		glGenTextures( 1, &textureRoad );
		glGenTextures( 1, &textureActiveCell );
		glGenTextures( 1, &textureFinish );
	}

	void CMap::Calculate()
	{
		int n = map.size(), m = map[0].size();
		float height = glutGet( GLUT_WINDOW_HEIGHT ),
			width = glutGet( GLUT_WINDOW_WIDTH );
		cellSize = fmin( height / n, width / m );
		indent.x = (width - cellSize * m) / 2;
		indent.y = (height - cellSize * n) / 2;
		needReload = true;
	}

	void CMap::saveTexture() const
	{
		// write window to array of pixels
		unsigned long imageSize;
		GLint viewPort[4];
		glGetIntegerv( GL_VIEWPORT, viewPort );
		GLint width = viewPort[2];
		GLint height = viewPort[3];
		imageSize = ((width + ((4 - (width % 4)) % 4)) * height * 3);
		std::shared_ptr<GLbyte> data = std::shared_ptr<GLbyte>( new GLbyte[imageSize] );
		glReadBuffer( GL_BACK );
		glReadPixels( 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data.get() );

		// write pixels to texture of map
		glBindTexture( GL_TEXTURE_2D, textureMap );
		gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data.get() );
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, float( GL_MODULATE) );
	}

	void CMap::reload()
	{
		int n = map.size(), m = map[0].size();
		glEnable( GL_TEXTURE_2D );

		glEnable( GL_BLEND );
		glColor3f( 1, 1, 1 );
		for( int i = 0; i < n; i++ ) {
			for( int j = 0; j < m; j++ ) {
				switch( map[i][j] ) {
					case 1:
						glBindTexture( GL_TEXTURE_2D, textureBoard ); // load a texture of board (forest)
						break;
					case 0:
						glBindTexture( GL_TEXTURE_2D, textureRoad ); // load a texture of road
						break;
					default:
						throw std::runtime_error( "Wrong nuber of texture" );
				}
				//calculate coordinates
				float left = j * cellSize + indent.x;
				float right = (j + 1) * cellSize + indent.x;
				float bottom = (n - i) * cellSize + indent.y;
				float top = (n - (i + 1)) * cellSize + indent.y;
				//Draw a cell with texture (board or road)
				glBegin( GL_POLYGON );
				{
					glTexCoord2f( 0, 0 ); glVertex2f( left, bottom );
					glTexCoord2f( 1, 0 ); glVertex2f( right, bottom );
					glTexCoord2f( 1, 1 ); glVertex2f( right, top );
					glTexCoord2f( 0, 1 ); glVertex2f( left, top );
				}
				glEnd();
			}
		}
		glDisable( GL_BLEND );
		glDisable( GL_TEXTURE_2D );
		saveTexture(); // save the whole window with map to texture
		needReload = false;
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
		glBindTexture( GL_TEXTURE_2D, textureFinish );
		glColor3f( 1, 1, 1 );
		glBegin( GL_POLYGON );
		{
			auto point1 = transateToWcoord( finishLine.first.x + 0.5, finishLine.first.y + 0.5, cellSize, indent, GetSize() );
			auto point2 = transateToWcoord( finishLine.second.x + 0.5, finishLine.second.y + 0.5, cellSize, indent, GetSize() );
			double distance = std::hypot( finishLine.first.x - finishLine.second.x, finishLine.second.y - finishLine.second.y );
			double distanceWindow = std::hypot( point1.x - point2.x, point1.y - point2.y );
			std::pair<double, double> direction( (point2.x - point1.x) /  distanceWindow, (point2.y - point1.y) / distanceWindow );
			direction = std::make_pair( direction.second, -direction.first );
			glTexCoord2f( 0.0f, 0.0f ); glVertex2f( point1.x - 5 * direction.first, point1.y - 5 * direction.second );
			glTexCoord2f( distance, 0.0f ); glVertex2f( point2.x - 5 * direction.first, point2.y - 5 * direction.second );
			glTexCoord2f( distance, 1.0f ); glVertex2f( point2.x + 5 * direction.first, point2.y + 5 * direction.second );
			glTexCoord2f( 0.0f, 1.0f ); glVertex2f( point1.x + 5 * direction.first, point1.y + 5 * direction.second );
		}
		glEnd();
		glDisable( GL_REPEAT );
		glDisable( GL_BLEND );
		glDisable( GL_TEXTURE_2D );
	}

	void CMap::Draw()
	{
		if( needReload ) {
			reload();
			return;
		}
		// load window from texture
		glEnable( GL_TEXTURE_2D );
		int height = glutGet( GLUT_WINDOW_HEIGHT ),
			width = glutGet( GLUT_WINDOW_WIDTH );
		// choose texture
		glBindTexture( GL_TEXTURE_2D, textureMap );
		// Draw a polygon of window size with texture
		glBegin( GL_POLYGON );
		{
			glTexCoord2f( 0, 0 ); glVertex2f( 0, 0 );
			glTexCoord2f( 1, 0 ); glVertex2f( width, 0 );
			glTexCoord2f( 1, 1 ); glVertex2f( width, height );
			glTexCoord2f( 0, 1 ); glVertex2f( 0, height );
		}
		glEnd();
		glDisable( GL_TEXTURE_2D );
	}

	void CMap::HighlightActiveCells() const
	{
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, textureActiveCell );
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

	bool CMap::NeedToReload()
	{
		return needReload;
	}
}