#pragma once
#include <vector>
#include <set>
#include <GL/glut.h>

#include "Utils.h"

namespace UI
{
	class CMap {
	public:
		CMap();
		CMap( const std::vector<std::vector<int>> &mapData );
		void Calculate();
		void DrawFinishLine( std::pair<CCoordinates, CCoordinates> finishLine ) const;
		void DrawShot( const std::pair<Core::CCoordinates, Core::CCoordinates>& shot ) const;
		void Draw();
		void HighlightActiveCells() const;
		float GetCellSize() const;

		CWindowCoordinates GetIndent() const;
		CSize GetSize() const;

		bool NeedToReload();
		GLuint textureForest;
		GLuint textureRoad;
		GLuint textureWall;
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

		void saveTexture() const;
		void reload();
	};
}