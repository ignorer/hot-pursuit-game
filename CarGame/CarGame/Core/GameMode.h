#pragma once

namespace Core {
	class CGameMode {
	public:
		enum MovementMode { CONCURRENT, SEQUENTIAL };
		enum DeathPenalty { DESTROY, TO_START, STOP };
		enum ObjectRate { NO, FEW, NORMAL, MANY };
		enum ObjectChangeModel { NO_CHANGE, STABLE, CIRCLE_RANDOM};

		CGameMode() = delete;

		static void Init( MovementMode movementMode = CONCURRENT, DeathPenalty deathPenalty = STOP, 
			int lapCount = 1, ObjectRate objectRate = NO, ObjectChangeModel _objectChangeModel = NO_CHANGE );

		static MovementMode GetMovementMode();
		static DeathPenalty GetDeathPenalty();
		static int GetLapCount();
		static ObjectRate GetObjectRate();
		static int GetMaxLapsCount();
		static ObjectChangeModel GetObjectChangeModel();

	private:
		static MovementMode movementMode;
		static DeathPenalty deathPenalty;
		static int lapCount;
		static ObjectRate objectRate;
		static ObjectChangeModel objectChangeModel;
		static const int maxLapsCount = 10;
	};
}