#pragma once

#include "..\gamelib\trackable.h"

struct ALLEGRO_EVENT_QUEUE;
class Game;

class InputSystem :	public Trackable
{
public:
	InputSystem();
	inline ~InputSystem(){ cleanup(); };

	void init();
	void cleanup();

	void update();

private:
	ALLEGRO_EVENT_QUEUE* mpEventQueue;


};

