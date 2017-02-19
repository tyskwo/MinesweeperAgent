#include "ResetGameEvent.h"

ResetGameEvent::ResetGameEvent(UINT numMines)
	:Event(GAME_RESET_EVENT)
	, mNumMines(numMines)
{

}

