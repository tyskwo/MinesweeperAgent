#pragma once

#include <string>
#include <vector>
#include <windows.h>
#include "..\GameLib\Trackable.h"

class GameState;
class TransactionHandler;
class Transaction;
class AIHolder;
class GraphicsGrid;
class Player;
class Event;
class TaskCollection;
class InputSystem;
class GameSequencer;

class Game: public Trackable
{
public:
	Game();
	~Game();

	void init();
	void cleanup();

	void doLoop();

	inline GameState* getGameState() const { return mpState; };
	
	void processTransactions();

	AIHolder* getAIHolder( UINT index ) const;
	inline size_t getNumAIHolders() const { return mpAIHolders.size(); };

private:
	GameState* mpState;
	std::vector<AIHolder*> mpAIHolders;
	Player* mpPlayer;
	GraphicsGrid* mpGraphicsGrid;
	TaskCollection* mpTasks;
	InputSystem* mpInputSystem;
	GameSequencer* mpSequencer;
	bool mInitialized;
	bool mKeepRunning;

	void loadAIHolders( const std::string& controllerFileName );
	void loadPlayers();
	static void mineHitCallback(const Event& theEvent, TransactionHandler* pHandler);
	static void cellRevealedCallback(const Event& theEvent, TransactionHandler* pHandler);
	static void gameWonCallback(const Event& theEvent, TransactionHandler* pHandler);
	static void gameQuitCallback(const Event& theEvent, TransactionHandler* pHandler);
	static void mouseClickedCallback(const Event& theEvent, TransactionHandler* pHandler);
	static void gameResetCallback(const Event& theEvent, TransactionHandler* pHandler);

	void registerCallbacks();
	void unregisterCallbacks();
	void resetGameState( UINT width, UINT height, UINT numMines );
};

extern Game* gpGame;