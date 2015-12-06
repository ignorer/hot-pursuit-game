#pragma once

class IStrategy {
public:
	virtual ~IStrategy() {}

	virtual int GetNextStep( std::vector<std::shared_ptr<IPlayerState>>& players, int playerNumber, std::vector<Powerup>& powerups ) = 0;
};