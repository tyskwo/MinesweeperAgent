#pragma once

#include <string>
#include <windows.h>
#include "Trackable.h"
#include "Grid.h"

class Game;
class Transaction;

class GameState: public Trackable
{
public:
	friend class Game;

	//default constructor
	GameState();
	//copy constructor
	GameState( const GameState& rhs );
	//destructor
	~GameState();

	void init( UINT width, UINT height, UINT numMines );
	void cleanup();

	//to be used when creating TransactionHandler's
	static void handleTransaction( Transaction* pTransaction, GameState* pGameState );

	inline Grid* getGrid() const { return mpGrid; };

	int calcScore(bool won) const;

private:

	Grid* mpGrid;
	UINT mNumMines;
	bool mInitialized;
	bool mIsFirstClick;

	void createGrid( UINT width, UINT height );
	void initGrid( UINT numMines );

	//called from the static handleTransaction function
	void handleTransaction( Transaction* pTransaction );


};

