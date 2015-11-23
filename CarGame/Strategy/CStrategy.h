// Author: Evgeny Chernigovsky
// Description: Abstract strategy
#pragma once

#ifndef AI_ABBYY_CSTRATEGY_H
#define AI_ABBYY_CSTRATEGY_H


#include <utility>

class CStrategy {
public:
    virtual std::pair<int, int> GetNextPosition() = 0;
};


#endif //AI_ABBYY_CSTRATEGY_H
