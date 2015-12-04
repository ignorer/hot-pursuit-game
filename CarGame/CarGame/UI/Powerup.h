#pragma once
#include <GL/glut.h>

#include "Utils.h"

namespace UI {
	class CPowerup {
	public:
		CPowerup( PowerupType type, const CCoordinates& coordinates );

		PowerupType GetType() const;
		CCoordinates GetCoordinates() const;

		void Draw( float cellSize, CWindowCoordinates indent, CSize mapSize ) const;

	private:
		PowerupType type;
		CCoordinates coordinates;
	};
}