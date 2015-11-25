#pragma once
#include <vector>
#include <GL/glut.h>

#include "UI/Coordinates.h"
#include "GlobalDefinitions.h"
#include <set>

namespace UI
{
	class CMap {
	public:
		CMap();
		CMap( const std::vector<std::vector<int>> &mapData );
		void Calculate();
		void DrawFinishLine( std::pair<CCoordinates, CCoordinates> finishLine ) const;
		void Draw();
		void HighlightActiveCells() const;
		float GetCellSize() const;

		CWindowCoordinates GetIndent() const;
		CSize GetSize() const;

		bool NeedToReload();
		GLuint textureBoard;
		GLuint textureRoad;
		GLuint textureActiveCell;
		GLuint textureFinish;

		GLuint textureMap;
		void MarkHighlightedCells( const std::vector<Core::CCoordinates>& possibleMoves );
		void UnmarkHighlightedCells( const std::vector<Core::CCoordinates>& possibleMoves );

	private:
		std::vector<std::vector<int>> map;
		std::set<CCoordinates> highlightedCells;
		float cellSize;
		CWindowCoordinates indent;
		bool needReload;

		void saveTexture();
		void reload();
	};
}