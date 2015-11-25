#include <thread>
#include <iostream>

#include <SOIL/SOIL.h>

#include "UI/Drawing.h"
#include "GlobalDefinitions.h"

namespace UI
{
	CMap CDrawing::map; // static data members must be explicitly defined in exactly one translation unit
	std::vector<CCar> CDrawing::cars;
	bool CDrawing::initialized = false;
	bool CDrawing::started = false;
	bool CDrawing::finished = false;
	bool CDrawing::loaded = false;
	bool CDrawing::justStartedFlag = false;
	std::mutex CDrawing::mutex;
	std::string CDrawing::windowName = "Rock'n'Roll race";
	std::pair<CCoordinates, CCoordinates> CDrawing::finishLine = std::pair<CCoordinates, CCoordinates>( CCoordinates( 0, 0 ), CCoordinates( 0, 0 ) );
	int CDrawing::window;
	int CDrawing::key;
	Core::CCoordinates CDrawing::mouse;

	void CDrawing::Init( int argc, char** argv )
	{
		glutInit( &argc, argv );
		glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );
		glutInitWindowSize( 800, 600 );
		std::unique_lock<std::mutex> lock( mutex );
		window = glutCreateWindow( windowName.c_str() );
		glutHideWindow();
		lock.unlock();

		glutTimerFunc( 1, timer, 0 );
		glutReshapeFunc( reshape );
		glutDisplayFunc( display );
		glutKeyboardFunc( keyboardFunction );
		glutMouseFunc( mouseFunction );

		glutMainLoop();
	}

	void CDrawing::Finish()
	{
		std::unique_lock<std::mutex> lock( mutex );
		finished = true;
	}

	void CDrawing::InitGame( const CMap &mapData, const std::vector<CCar> &carsData, const Core::CLine& finish )
	{
		std::unique_lock<std::mutex> lock( mutex );
		if( initialized ) {
			throw std::logic_error( "Game state has been already initialized" );
		}
		map = mapData;
		cars = carsData;
		initialized = true;
		justStartedFlag = true;
		finishLine.first.x = finish.first.x;
		finishLine.first.y = finish.first.y;
		finishLine.second.x = finish.second.x;
		finishLine.second.y = finish.second.y;
	}

	void CDrawing::DropGame()
	{
		std::unique_lock<std::mutex> lock( mutex );
		initialized = false;
		started = false;
	}

	void CDrawing::reshape( int width, int height )
	{
		std::unique_lock<std::mutex> lock( mutex );
		if( !started ) {
			return;
		}

		glViewport( 0, 0, width, height ); // set view block

		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluOrtho2D( 0, width, 0, height ); // set coordinates 

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		map.Calculate(); // recalculate map
	}

	void CDrawing::timer( int value )
	{
		std::unique_lock<std::mutex> lock( mutex );
		glutPostRedisplay();
		glutTimerFunc( 1, timer, 0 );
	}

	void CDrawing::display()
	{
		std::unique_lock<std::mutex> lock( mutex );
		if( !started ) {
			return;
		}
		if( !loaded ) {
			load();
			loaded = true;
		}
		if( justStartedFlag ) {
			glViewport( 0, 0, glutGet( GLUT_WINDOW_WIDTH ), glutGet( GLUT_WINDOW_HEIGHT ) ); // set view block

			glMatrixMode( GL_PROJECTION );
			glLoadIdentity();
			gluOrtho2D( 0, glutGet( GLUT_WINDOW_WIDTH ), 0, glutGet( GLUT_WINDOW_HEIGHT ) );

			glMatrixMode( GL_MODELVIEW );
			glLoadIdentity();
			map.Calculate();
			justStartedFlag = false;
		}

		glClearColor( 1.0, 1.0, 1.0, 0.0 );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		bool mapReloaded = !map.NeedToReload();
		map.Draw();
		map.HighlightActiveCells();
		map.DrawFinishLine( finishLine );
		for( size_t i = 0; i < cars.size(); i++ ) {
			cars[i].Draw( map.GetCellSize(), map.GetIndent(), map.GetSize() );
		}
		glFlush();
		if( mapReloaded ) {
			glutSwapBuffers(); // if map wasn't reloaded (and buffers weren't swapped), swap buffers
		}
	}

	int CDrawing::GetWindow()
	{
		std::unique_lock<std::mutex> lock( mutex );
		return window;
	}

	void CDrawing::DropKey()
	{
		std::unique_lock<std::mutex> lock( mutex );
		key = -1;
	}

	void CDrawing::DropMouse()
	{
		std::unique_lock<std::mutex> lock( mutex );
		mouse.x = -1;
		mouse.y = -1;
	}

	int CDrawing::GetKey()
	{
		std::unique_lock<std::mutex> lock( mutex );
		return key;
	}

	Core::CCoordinates CDrawing::GetMouse( const std::vector<Core::CCoordinates>& possibleMoves )
	{
		std::unique_lock<std::mutex> lock( mutex );
		for( auto move : possibleMoves ) {
			if( mouse == move ) {
				return mouse;
			}
		}
		mouse.x = -1;
		mouse.y = -1;
		return mouse;
	}

	void CDrawing::ShowWindow()
	{
		glutShowWindow();
	}

	void CDrawing::HideWindow()
	{
		glutHideWindow();
	}

	void CDrawing::MarkPossibleMoves( const std::vector<Core::CCoordinates>& possibleMoves )
	{
		std::unique_lock<std::mutex> lock( mutex );
		map.MarkHighlightedCells( possibleMoves );
	}

	void CDrawing::UnMarkPossibleMoves( const std::vector<Core::CCoordinates>& possibleMoves )
	{
		std::unique_lock<std::mutex> lock( mutex );
		map.UnmarkHighlightedCells( possibleMoves );
	}

	// load image from file to texture
	void CDrawing::loadTexture( const char* filename, GLuint& texture )
	{
		// create and choose texture
		glGenTextures( 1, &texture );
		glBindTexture( GL_TEXTURE_2D, texture );
		// read from file
		int width, height;
		int channels;
		unsigned char* image = SOIL_load_image( filename, &width, &height, &channels, SOIL_LOAD_RGBA );
		// save data to texture
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image );
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, float( GL_MODULATE ) );
		SOIL_free_image_data( image );
		// unchoose texture
		glBindTexture( GL_TEXTURE_2D, 0 );
	}

	void CDrawing::MoveCars( const std::vector<int>& numbers, const std::vector<CCoordinates>& newCoordinates )
	{
		std::unique_lock<std::mutex> lock( mutex );
		std::vector<CCoordinates> oldCoordinates;
		for( int i : numbers ) {
			oldCoordinates.push_back( cars[i].GetCoordinates() );
		}
		lock.unlock();
		const int fps = 100;
		for( int i = 0; i < numbers.size(); ++i ) {
			cars[numbers[i]].Rotate( oldCoordinates[i].x, oldCoordinates[i].y, newCoordinates[i].x, newCoordinates[i].y );
		}
		for( int j = 0; j <= fps; ++j ) {
			for( int i = 0; i < numbers.size(); ++i ) {
				cars[i].Move( UI::CCoordinates(
					float( oldCoordinates[numbers[i]].x * (fps - j) + newCoordinates[i].x * j ) / fps,
					float( oldCoordinates[numbers[i]].y * (fps - j) + newCoordinates[i].y * j ) / fps ) );
			}
			std::this_thread::sleep_for( std::chrono::milliseconds( 500 / fps ) );
		}
	}

	void CDrawing::MoveCarsToStart( const std::vector<int>& numbers, const std::vector<CCoordinates>& newCoordinates )
	{
		const int framesPerStep = 100;
		for( int j = 0; j <= framesPerStep; ++j ) {
			for( int i : numbers ) {
				cars[i].SetOpacity( 1.0f - float( j ) / framesPerStep );
			}
			std::this_thread::sleep_for( std::chrono::milliseconds( 500 / framesPerStep ) );
		}
		for( int i = 0; i < numbers.size(); ++i ) {
			cars[numbers[i]].Move( newCoordinates[i] );
		}
		for( int j = 0; j <= framesPerStep; ++j ) {
			for( int i : numbers ) {
				cars[i].SetOpacity( float( j ) / framesPerStep );
			}
			std::this_thread::sleep_for( std::chrono::milliseconds( 500 / framesPerStep ) );
		}
	}

	void CDrawing::DeleteCars( const std::vector<int>& numbers )
	{
		const int framesPerStep = 100;
		for( int j = 0; j <= framesPerStep; ++j ) {
			for( int i : numbers ) {
				cars[i].SetOpacity( 1.0f - float( j ) / framesPerStep );
			}
			std::this_thread::sleep_for( std::chrono::milliseconds( 500 / framesPerStep ) );
		}
		for( int i : numbers ) {
			cars[i].Crash();
		}
	}

	void CDrawing::Start()
	{
		std::unique_lock<std::mutex> lock( mutex );
		started = true;
	}

	void CDrawing::Stop()
	{
		std::unique_lock<std::mutex> lock( mutex );
		started = false;
		glDeleteTextures( 1, &map.textureRoad );
		glDeleteTextures( 1, &map.textureBoard );
		for( auto car : cars ) {
			glDeleteTextures( 1, &car.texture );
		}
		loaded = false;
	}

	void CDrawing::load()
	{
		//load textures for map
		loadTexture( (RESOURCE_DIRECTORY + "Images\\road.png").c_str(), map.textureRoad );
		loadTexture( (RESOURCE_DIRECTORY + "Images\\forest.png").c_str(), map.textureBoard );
		loadTexture( (RESOURCE_DIRECTORY + "Images\\active.png").c_str(), map.textureActiveCell );
		loadTexture( (RESOURCE_DIRECTORY + "Images\\finish.png").c_str(), map.textureFinish );

		//load textures for cars (depends on color)
		std::string carFilename;
		for( size_t i = 0; i < cars.size(); i++ ) {
			switch( cars[i].GetColor() ) {
			case RED:
				carFilename = RESOURCE_DIRECTORY + "Images\\car_red.png";
				break;
			case BLUE:
				carFilename = RESOURCE_DIRECTORY + "Images\\car_blue.png";
				break;
			case GREEN:
				carFilename = RESOURCE_DIRECTORY + "Images\\car_green.png";
				break;
			case ORANGE:
				carFilename = RESOURCE_DIRECTORY + "Images\\car_orange.png";
				break;
			default:
				carFilename = RESOURCE_DIRECTORY + "Images\\car_red.png";
			}
			loadTexture( carFilename.c_str(), cars[i].texture );
		}
	}

	void CDrawing::keyboardFunction( unsigned char pressedKey, int x, int y )
	{
		std::unique_lock<std::mutex> lock( mutex );
		if( key >= '1' && key <= '9' ) {
			key = pressedKey - '0';
		} else {
			key = -1;
		}
	}

	Core::CCoordinates CDrawing::translateToCoord( int x, int y, float cellSize )
	{
		int newx = int( x ) / cellSize;
		int newy = int( y ) / cellSize;
		return Core::CCoordinates( newx, newy );
	}

	void CDrawing::mouseFunction( int button, int state, int x, int y )
	{
		std::unique_lock<std::mutex> lock( mutex );
		if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {
			UI::CWindowCoordinates indent = map.GetIndent();
			CSize s = map.GetSize();
			if( (x < indent.x) || (x > s.first * map.GetCellSize() + indent.x) || (y < indent.y) || (y > s.second * map.GetCellSize() + indent.y) ) {
				mouse = Core::CCoordinates( -1, -1 );
			} else {
				mouse = translateToCoord( x - indent.x, y - indent.y, map.GetCellSize() );
			}
		} else {
			mouse = Core::CCoordinates( -1, 1 );
		}
	}
}