#pragma once

namespace Core {
	class CGameMode {
	public:
		enum MovementMode { CONCURRENT, SEQUENTIAL };
		enum DeathPenalty { DESTROY, TO_START, STOP };
		enum ObjectRate { NO, FEW, NORMAL, MANY };
		enum ObjectChangeModel { NO_CHANGE, STABLE, RANDOM};

		CGameMode() = delete;

		static void Init( MovementMode movementMode = CONCURRENT, DeathPenalty deathPenalty = STOP, 
			int lapCount = 1, ObjectRate objectRate = NO, ObjectChangeModel _objectChangeModel = NO_CHANGE );

		static MovementMode GetMovementMode();
		static DeathPenalty GetDeathPenalty();
		static int GetLapCount();
		static ObjectRate GetObjectRate();
		static ObjectChangeModel GetObjectChangeModel();
		// не использовать эту функцию в ядре. это для диалогового окна с выбором режима игры
		static int GetMaxLapsCount();

	private:
		static MovementMode movementMode;
		static DeathPenalty deathPenalty;
		static int lapCount;
		static ObjectRate objectRate;
		static ObjectChangeModel objectChangeModel;
		static const int MAX_LAPS_COUNT = 10;
	};
}