// Author: Evgeny Chernigovsky
// Description: 
#pragma once

#ifndef AI_ABBYY_EMOVEDIRECTION_H
#define AI_ABBYY_EMOVEDIRECTION_H

enum class EMovementDirection : int {
	UP_LEFT = 7, UP = 8, UP_RIGHT = 9,
	LEFT = 4, NOT_CHANGED = 5, RIGHT = 6,
	DOWN_LEFT = 1, DOWN = 2, DOWN_RIGHT = 3,
	NONE = 10
};

#endif //AI_ABBYY_EMOVEDIRECTION_H
