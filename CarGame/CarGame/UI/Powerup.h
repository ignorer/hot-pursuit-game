#pragma once
#include <GL/glut.h>

#include "GlobalDefinitions.h"
#include "UI/Coordinates.h"

namespace UI {
	class CPowerup {
	public:
		CPowerup( PowerupType type, const CCoordinates& coordinates, GLuint texture );

		PowerupType GetType() const;
		CCoordinates GetCoordinates() const;

		void Draw( float cellSize, CWindowCoordinates indent, CSize mapSize ) const;

	private:
		PowerupType type;
		CCoordinates coordinates;
		GLuint texture;
	};
}