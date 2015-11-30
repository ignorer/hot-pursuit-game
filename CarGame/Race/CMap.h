#pragma once
#include "stdafx.h"
#include <vector>



const int DEFAULT_SIZE_X = 20;
const int DEFAULT_SIZE_Y = 15;
const int DEFAULT_COLORS_NUMBER = 4;
const std::pair<int, int> DEFAULT_FINISH_POINT = std::pair<int, int>( -1, -1 );


enum BType {
    BNone,
    BRoad,
    BTree,
    BWall,
    BStart,
	BFinish1,
	BFinish2
};


class CMap {
public:
    CMap();
    ~CMap();

    const std::vector< std::vector<int> >& GetNumbers();
    int GetX();
    int GetY();

    //меняет состояние клетки с координатами (i, j)
    void ClickCell( int i, int j );
    void ClickCell( int i, int j, BType bType );
    //обнуляет карту
    void RestartMap();
    //устанавливает размеры поля
    void SetSize( int x, int y );

    void LoadMapFromFile( std::ifstream& );
    void SaveMapToFile( std::ofstream& );
	//выставляет текущие координаты одной из точек финиша и запоминает текущую замененную текстуру 
	void SetFinishCoord( int pointNum, int x, int y, int prevTexture );
	std::pair<int, int> GetFinishCoord( int pointNum );

	int GetPrevTexture( int num );
	void SetPrevTexture( int num );
	

private:
    std::vector< std::vector<int> > numbers;
    int sizeX;
    int sizeY;
    int colorsNumber;
	std::pair<int, int> finishFirstCoord;
	int prevTexture1;
	std::pair<int, int> finishSecondCoord;
	int prevTexture2;
};
