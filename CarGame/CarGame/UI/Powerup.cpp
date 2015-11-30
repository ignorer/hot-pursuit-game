#include "UI/Powerup.h"

namespace UI {
	CPowerup::CPowerup( PowerupType type, const CCoordinates& coordinates, GLuint texture ) :
		type( type ),
		coordinates( coordinates ),
		texture( texture )
	{
	}

	PowerupType CPowerup::GetType() const
	{
		return type;
	}

	CCoordinates CPowerup::GetCoordinates() const
	{
		return coordinates;
	}

	void CPowerup::Draw( float cellSize, CWindowCoordinates indent, CSize mapSize ) const
	{
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, texture );
		glTexEnvf( GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		CWindowCoordinates coord = transateToWcoord( coordinates.x, coordinates.y, cellSize, indent, mapSize );

		glEnable( GL_BLEND );
		glBegin( GL_POLYGON );
		{
			glColor3f( 1, 1, 1 );
			glTexCoord2f( 0.0f, 0.0f ); glVertex2f( coord.x, coord.y);
			glTexCoord2f( 1.0f, 0.0f ); glVertex2f( coord.x + cellSize, coord.y );
			glTexCoord2f( 1.0f, 1.0f ); glVertex2f( coord.x + cellSize, coord.y - cellSize );
			glTexCoord2f( 0.0f, 1.0f ); glVertex2f( coord.x, coord.y - cellSize );
		}
		glEnd( );
		glDisable( GL_BLEND );
		glDisable( GL_TEXTURE_2D );
	}
}