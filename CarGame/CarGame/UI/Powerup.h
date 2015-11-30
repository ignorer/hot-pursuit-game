#pragma once
#include <GL/glut.h>

#include "GlobalDefinitions.h"
#include "UI/Coordinates.h"

namespace UI {
	class CPowerup {
	public:
		CPowerup( PowerupType type, const CCoordinates& coordinates, GLuint texture )
			: type( type ),
			coordinates( coordinates ),
			texture( texture )
		{
		}

	private:
		PowerupType type;
		CCoordinates coordinates;
		GLuint texture;
	};
}