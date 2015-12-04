#include "stdafx.h"
#include "CMap.h"


CMap::CMap() : sizeX( DEFAULT_SIZE_X ), sizeY( DEFAULT_SIZE_Y ), colorsNumber( DEFAULT_COLORS_NUMBER )
{
	finishFirstCoord = DEFAULT_FINISH_POINT;
	finishSecondCoord = DEFAULT_FINISH_POINT;
	prevTexture1 = 0;
	prevTexture2 = 0;
	lButtonWasPressed = false;
}


CMap::~CMap()
{}


const std::vector< std::vector<int> >& CMap::GetNumbers()
{
    return numbers;
}


int CMap::GetX()
{
    return sizeX;
}


int CMap::GetY()
{
    return sizeY;
}


void CMap::ClickCell( int i, int j )
{
    numbers[i][j] = ( numbers[i][j] + 1 ) % colorsNumber;
}


void CMap::ClickCell( int i, int j, BType bType, bool lButtonPressed )
{
	
	switch (bType) {
        case BNone:  ClickCell( i, j ); 
			//«акрашивание кнопок финиша при клике на текстуру

			if( finishFirstCoord.first == i && finishFirstCoord.second == j ) {
				finishFirstCoord.first = -1;
				finishFirstCoord.second = -1;
			}
			if( finishSecondCoord.first == i && finishSecondCoord.second == j ) {
				finishSecondCoord.first = -1;
				finishSecondCoord.second = -1;
			}
			//
			break;
        case BTree:  
			numbers[i][j] = 0; 
			//«акрашивание кнопок финиша при клике на текстуру

			if( finishFirstCoord.first == i && finishFirstCoord.second == j ) {
				finishFirstCoord.first = -1;
				finishFirstCoord.second = -1;
			}
			if( finishSecondCoord.first == i && finishSecondCoord.second == j ) {
				finishSecondCoord.first = -1;
				finishSecondCoord.second = -1;
			}
			//
			break;
        case BRoad:  
			numbers[i][j] = 1; 
			//«акрашивание кнопок финиша при клике на текстуру

			if( finishFirstCoord.first == i && finishFirstCoord.second == j ) {
				finishFirstCoord.first = -1;
				finishFirstCoord.second = -1;
			}
			if( finishSecondCoord.first == i && finishSecondCoord.second == j ) {
				finishSecondCoord.first = -1;
				finishSecondCoord.second = -1;
			}
			//
			break;
        case BStart: numbers[i][j] = 2; 
			//«акрашивание кнопок финиша при клике на текстуру

			if( finishFirstCoord.first == i && finishFirstCoord.second == j ) {
				finishFirstCoord.first = -1;
				finishFirstCoord.second = -1;
			}
			if( finishSecondCoord.first == i && finishSecondCoord.second == j ) {
				finishSecondCoord.first = -1;
				finishSecondCoord.second = -1;
			}
			//
			break;
        case BWall: 
			numbers[i][j] = 3; 
			//«акрашивание кнопок финиша при клике на текстуру

			if( finishFirstCoord.first == i && finishFirstCoord.second == j ) {
				finishFirstCoord.first = -1;
				finishFirstCoord.second = -1;
			}
			if( finishSecondCoord.first == i && finishSecondCoord.second == j ) {
				finishSecondCoord.first = -1;
				finishSecondCoord.second = -1;
			}
			//
			break;
		case BFinish1: 
			if( finishFirstCoord.first != -1 ) {
				numbers[finishFirstCoord.first][finishFirstCoord.second] = prevTexture1;
				if( prevTexture1 == 5 ) {
					finishSecondCoord.first = finishFirstCoord.first;
					finishSecondCoord.second = finishFirstCoord.second;
				}
				if( finishFirstCoord.first == i && finishFirstCoord.second == j && !lButtonPressed && !lButtonWasPressed ) {
					finishFirstCoord.first = -1;
					finishFirstCoord.second = -1;
					numbers[i][j] = prevTexture1;
				}
				else {
					lButtonWasPressed = lButtonPressed;
					prevTexture1 = numbers[i][j];
					finishFirstCoord.first = i;
					finishFirstCoord.second = j;
					numbers[i][j] = 4;
				}
			}
			else {
				prevTexture1 = numbers[i][j];
				finishFirstCoord.first = i;
				finishFirstCoord.second = j;
				numbers[i][j] = 4;
			}

			if( finishSecondCoord.first == i && finishSecondCoord.second == j && finishFirstCoord.first != -1 ) {
				finishSecondCoord.first = -1;
				finishSecondCoord.second = -1;
			}
			break;
		case BFinish2:
			if( finishSecondCoord.first != -1 ) {
				numbers[finishSecondCoord.first][finishSecondCoord.second] = prevTexture2;
				if( prevTexture2 == 4 ) {
					finishFirstCoord.first = finishSecondCoord.first;
					finishFirstCoord.second = finishSecondCoord.second;
				}
				if( finishSecondCoord.first == i && finishSecondCoord.second == j && !lButtonPressed && !lButtonWasPressed ) {
					finishSecondCoord.first = -1;
					finishSecondCoord.second = -1;
					numbers[i][j] = prevTexture2;
				}
				else {
					lButtonWasPressed = lButtonPressed;
					prevTexture2 = numbers[i][j];
					finishSecondCoord.first = i;
					finishSecondCoord.second = j;
					numbers[i][j] = 5;
				}
			}
			else {
				prevTexture2 = numbers[i][j];
				finishSecondCoord.first = i;
				finishSecondCoord.second = j;
				numbers[i][j] = 5;
			}

			if( finishFirstCoord.first == i && finishFirstCoord.second == j && finishSecondCoord.first != -1 ) {
				finishFirstCoord.first = -1;
				finishFirstCoord.second = -1;
			}
			break;


    }
}


void CMap::LoadMapFromFile( std::ifstream& fin )
{
    fin >> sizeY >> sizeX;
    numbers.resize( sizeY );
    for( int i = 0; i < sizeY; i++ ) {
        numbers[i].resize( sizeX );
        for( int j = 0; j < sizeX; j++ ) {
            fin >> numbers[i][j];
        }
    }
}


void CMap::SaveMapToFile( std::ofstream & fout )
{
    fout << sizeY << " " << sizeX;
    fout << std::endl;

    for( int i = 0; i < sizeY; i++ ) {
        for( int j = 0; j < sizeX; j++ ) {
            switch( numbers[i][j] ) {
                case 0: fout << 1 << " "; break;
                case 1: fout << 0 << " "; break;
                case 2: fout << 2 << " "; break;
                case 3: fout << 3 << " "; break;
				case 4: fout << 4 << " "; break;
				case 5: fout << 5 << " "; break;
				case 6: fout << 6 << " "; break;
            }
        }
        fout << std::endl;
    }
}


void CMap::RestartMap()
{
	numbers = std::vector< std::vector<int> >( sizeY );

	int border = 1; // forest border
	int roadWidth = 3;

	if ( sizeX <= 10 || sizeY <= 10 ) {
		border = 0;
		roadWidth = 2;
	}

	const int ROAD = 1;
	const int FOREST = 0;
	const int WALL = 3;

	for ( int i = 0; i < sizeY; i++ ) {
		numbers[i].resize( sizeX ); // now forest is everywhere

		if ( i == border || i == sizeY - 1 - border ) { // FOREST-WALL-FOREST
			for ( int j = border; j < sizeX - border; j++ ) {
				numbers[i][j] = WALL;
			}
		}
		else if ( ( i > border && i < border + roadWidth + 1 ) ||
			( i > sizeY - 2 - border - roadWidth && i < sizeY - 1 - border ) ) // FOREST-WALL-ROAD-WALL-FOREST
		{
			// set walls
			numbers[i][border] = numbers[i][sizeX - 1 - border] = WALL;

			// set road
			for ( int j = border + 1; j < sizeX - 1 - border; j++ ) {
				numbers[i][j] = ROAD;
			}

		}
		else if ( i == border + roadWidth + 1 || i == sizeY - 2 - border - roadWidth ) { // FOREST-WALL-ROAD-WALL-ROAD-WALL-FOREST
																						 // set walls
			numbers[i][border] = numbers[i][sizeX - 1 - border] = WALL;

			for ( int j = border + roadWidth + 1; j < sizeX - 1 - border - roadWidth; j++ ) {
				numbers[i][j] = WALL;
			}

			// set road
			for ( int j = border + 1; j < border + roadWidth + 1; j++ ) {
				numbers[i][j] = ROAD;
			}

			for ( int j = sizeX - 1 - border - roadWidth; j < sizeX - 1 - border; j++ ) {
				numbers[i][j] = ROAD;
			}
		}
		else if ( i > border + roadWidth + 1 && i < sizeY - 2 - border - roadWidth ) { // FOREST-WALL-ROAD-WALL-FOREST-WALL-ROAD-WALL
			// set walls
			numbers[i][border] = numbers[i][sizeX - 1 - border] = WALL;
			numbers[i][border + roadWidth + 1] = numbers[i][sizeX - 2 - border - roadWidth] = WALL;

			// set road
			for ( int j = border + 1; j < border + roadWidth + 1; j++ ) {
				numbers[i][j] = ROAD;
			}

			for ( int j = sizeX - 1 - border - roadWidth; j < sizeX - 1 - border; j++ ) {
				numbers[i][j] = ROAD;
			}
		}
	}
}

void CMap::SetFinishCoord( int pointNum, int x, int y, int prevTexture ) {
	if( pointNum == 1 ) {
		finishFirstCoord.first = x;
		finishFirstCoord.second = y;
		prevTexture1 = prevTexture;
	}
	else {
		finishSecondCoord.first = x;
		finishSecondCoord.second = y;
		prevTexture2 = prevTexture;
	}
};

std::pair<int,int> CMap::GetFinishCoord( int pointNum ) {
	if( pointNum == 1 ) {
		return std::pair<int,int>(finishFirstCoord.first, finishFirstCoord.second);
	}
	else {
		return std::pair<int, int>( finishSecondCoord.first, finishSecondCoord.second );
	}
}

int CMap::GetPrevTexture( int num ) {
	if( num == 1 ) {
		return prevTexture1;
	}
	else {
		return prevTexture2;
	}
}

void CMap::SetPrevTexture( int num ) {
	if( num == 1 ) {
		prevTexture1 = num;
	}
	else {
		prevTexture2 = num;
	}
}

void CMap::SetSize( int x, int y )
{
    sizeX = x;
    sizeY = y;
}