#pragma once

#include <GL/glut.h>

#include "UI/Coordinates.h"
#include "GlobalDefinitions.h"

namespace UI {
	enum Color { RED, GREEN, BLUE, ORANGE };

#define PI 3.14159265

	class CCar {
	public:
		CCar( CCoordinates startCoordinates, Color carColor );

		Color GetColor() const;
		CCoordinates GetCoordinates() const;

		void Draw( float cellSize, CWindowCoordinates indent, CSize mapSize );

		// переносит машину на новые координаты, НЕ изменяя угол
		void Move( CCoordinates coordinates );
		// помечает машину как сломанную. пока она сломана, она не будет рисоваться. методов для "починки" машины у нас нет
		void Crash();
		// устанавливает машине прозрачность от 0.0 до 1.0
		void SetOpacity( float opacity );
		// поворачивает машину для движения из координат (x1,y1) в (x2,y2)
		void Rotate( float x1, float y1, float x2, float y2 );
		GLuint texture;
		GLuint explosion;
		GLuint shield;
		int explosionFrameNumber;


	private:
		void rotate( float &x, float &y, float angle ) const;
		void rotateCar( float &Ax, float &Ay, float &Bx, float &By, float &Cx, float &Cy, float &Dx, float &Dy, float centerX, float centerY, float angle ) const;

		CCoordinates coords;
		Color color;
		float alpha;
		bool crashed;
		bool shielded;
	};
}