#pragma once 

class IStrategy {
public:
	virtual ~IStrategy() {}

	virtual int GetNextStep() = 0;
};