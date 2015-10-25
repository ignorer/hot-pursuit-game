#include "Graphics/Loader.h"
#include "GlobalDefinitions.h"


int main( int argc, char * argv[] )
{
	Graphics::CMap map;
	std::vector<Graphics::CCar> cars;
	Graphics::CLoader loader(RESOURCE_DIRECTORY + "Maps\\map_file.txt", RESOURCE_DIRECTORY + "Maps\\cars_file.txt", map, cars);

	Graphics::CDrawing drawing_module( map, cars );
	drawing_module.draw( argc, argv );
	return 0;
}