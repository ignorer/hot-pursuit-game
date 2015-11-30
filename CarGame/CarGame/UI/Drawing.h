#pragma once

#include <vector>
#include <mutex>

#include <GL/glut.h>

#include "UI/Map.h"
#include "UI/Car.h"

/* OpenGL needs static functions and command line arguments for drawing*/

namespace UI {
	class CDrawing {
	public:
		static void Init( int argc, char** argv );
		static void Finish();
		static void InitGame( const CMap &mapData, const std::vector<CCar> &carsData, const Core::CLine& finish );
		static void DropGame();
		
		static void keyboardFunction( unsigned char key, int x, int y );
		static void mouseFunction( int button, int state, int x, int y );

		static void MoveCars( const std::vector<int>& numbers, const std::vector<CCoordinates>& newCoordinates );
		static void MoveCarsToStart( const std::vector<int>& numbers, const std::vector<CCoordinates>& newCoordinates );
		static void DeleteCars( const std::vector<int>& numbers );
		static void ExplodeCars( const std::vector<int>& numbers );

		static void Start();
		static void Stop();

		static int GetWindow();

		static void DropKey();
		static int GetKey();

		static void DropMouse();
		static Core::CCoordinates GetMouse( const std::vector<Core::CCoordinates>& possibleMoves );

		static void ShowWindow();
		static void HideWindow();

		static void MarkPossibleMoves( const std::vector<Core::CCoordinates>& possibleMoves );
		static void UnMarkPossibleMoves( const std::vector<Core::CCoordinates>& possibleMoves );

		static Core::CCoordinates translateToCoord( int x, int y, float cellSize );

	private:
		static void display();
		static void reshape( int width, int height );
		static void load();
		static void loadTexture( const char*, GLuint& texture );
		static void timer( int value );

		static bool initialized;
		static bool started;
		static bool finished;
		static bool loaded;
		static bool justStartedFlag;
		static CMap map;
		static std::vector<CCar> cars;
		static std::pair<CCoordinates, CCoordinates> finishLine;
		static std::mutex mutex;
		static std::string windowName;
		static int window;
		static int key;
		static Core::CCoordinates mouse;
	};
}