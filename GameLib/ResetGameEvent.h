#pragma once
#include "event.h"
#include <windows.h>


class ResetGameEvent : public Event
{
public:
	ResetGameEvent(UINT numMines);
	~ResetGameEvent(){};

	inline UINT getNumMines() const { return mNumMines; };

private:
	UINT mNumMines;
};

