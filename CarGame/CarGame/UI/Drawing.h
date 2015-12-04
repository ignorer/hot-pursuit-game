#pragma once

#include <vector>
#include <mutex>
#include <map>

#include <GL/glut.h>

#include "UI/Map.h"
#include "UI/Car.h"
#include "UI/Powerup.h"

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

		static void MoveCars( const std::vector<int>& numbers, const std::vector<CCoordinates>& newCoordinates, const std::vector<bool>& shields );
		static void MoveCarsToStart( const std::vector<int>& numbers, const std::vector<CCoordinates>& newCoordinates );
		static void DeleteCars( const std::vector<int>& numbers );
		static void SetPowerups( const std::map<Core::CCoordinates, PowerupType> powerupsInfo );
		static void SetShots( const std::vector<std::pair<Core::CCoordinates, Core::CCoordinates>>& shots );

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

		static GLuint GetTextureForPowerup( PowerupType type );

		static GLuint textureOil;
		static GLuint textureSand;
		static GLuint textureWall;
		static GLuint textureBombActive;
		static GLuint textureBombInactive;
		static GLuint textureShieldToPickUp;
		static GLuint textureLazer;
	private:
		static void display();
		static void reshape( int width, int height );
		static void timer( int value );

		static void load();
		static void loadTexture( const char*, GLuint& texture );
		static Core::CCoordinates translateToCoord( int x, int y, float cellSize );

		static bool initialized;
		static bool started;
		static bool finished;
		static bool loaded;
		static bool justStartedFlag;
		static CMap map;
		static std::vector<CCar> cars;
		static std::vector<CPowerup> powerups;
		static std::pair<CCoordinates, CCoordinates> finishLine;
		static std::mutex mutex;
		static std::string windowName;
		static int window;
		static int key;
		static Core::CCoordinates mouse;
		static std::map<PowerupType, GLuint> powerupTextureMap;
		static std::vector<std::pair<Core::CCoordinates, Core::CCoordinates>> shots;
	};
}