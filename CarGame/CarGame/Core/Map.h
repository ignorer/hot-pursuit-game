#pragma once

#include <vector>

#include "Utils.h"

namespace Core {
	class CMap {
	public:
		CMap( const CField& newMap, CLine finish, const std::vector<CCoordinates>& startPoints );

		bool IsEmpty( int x, int y ) const;
		
		CSize GetSize() const;
		CField GetField() const;
		CLine GetFinishLine() const;
		const std::vector<CCoordinates>& GetStartPoints() const;
		
	private:
		CField map;
		CLine finishLine;
		std::vector<CCoordinates> startPoints;
	};
}