#include <iostream>
#include <assert.h>
#include "GameState.h"
#include "Transaction.h"
#include "CellClickTransaction.h"
#include "DLLInittedTransaction.h"
#include "Grid.h"
#include "Cell.h"
#include "..\GameLib\EventSystem.h"
#include "Event.h"

using namespace std;

GameState::GameState()
	:mpGrid(NULL)
	,mInitialized(false)
	, mIsFirstClick(true)
	,mNumMines(0)
{

}

GameState::GameState( const GameState& rhs )
	:mInitialized(rhs.mInitialized)
	, mIsFirstClick(rhs.mIsFirstClick)
	,mpGrid(NULL)
	,mNumMines(0)
{
	mpGrid = new Grid( *(rhs.mpGrid) );
}

GameState::~GameState()
{
	cleanup();
}

//static
void GameState::handleTransaction( Transaction* pTransaction, GameState* pGameState )
{
	pGameState->handleTransaction( pTransaction );
}

void GameState::handleTransaction( Transaction* pTransaction )
{
	if( pTransaction->getType() == CELL_CLICK_TRANSACTION )
	{
		CellClickTransaction* pCellClick = dynamic_cast<CellClickTransaction*>(pTransaction);
		assert( pCellClick != NULL );
		if( pCellClick != NULL )
		{
			Cell* pCell = mpGrid->getCellAtIndex(pCellClick->getCellIndex());
			assert(pCell != NULL);

			if (mIsFirstClick)
			{
				if (pCell != NULL && pCell->hasMine())//first click on a mine?
				{
					//find the first cell without a mine
					UINT switchIndex = mpGrid->getNumCells();
					for (UINT i = 0; i < mpGrid->getNumCells(); i++)
					{
						Cell* pCheckCell = mpGrid->getCellAtIndex(i);
						if (pCheckCell != NULL && !pCheckCell->hasMine())
						{
							switchIndex = i;
							break;
						}
					}

					if (switchIndex != mpGrid->getNumCells())
					{
						Cell* pSwitchCell = mpGrid->getCellAtIndex(switchIndex);
						pSwitchCell->setFlags(MINE_CELL);
						pCell->clearFlags(MINE_CELL);
					}
				}
				mIsFirstClick = false;
			}

			if( pCell != NULL )
			{
				if( pCell->hasMine() )//kaboom!
				{
					EventSystem::getEventSystem()->fireEvent( Event(MINE_HIT_EVENT) );
				}
				else if( !pCell->isRevealed() )
				{
					mpGrid->revealZeroMinesAdjacentCells( pCellClick->getCellIndex() );

					//if there are still non-mines to reveal - fire cell revealed event
					if( mpGrid->hasHiddenNonMinesLeft() )
					{
						EventSystem::getEventSystem()->fireEvent( Event(CELL_REVEALED_EVENT) );
					}
					else
					{
						EventSystem::getEventSystem()->fireEvent( Event(GAME_WON_EVENT) );
					}
				}
				else
				{
					cout << "Cell was already revealed!\n";
				}
			}
		}
	}
	else if (pTransaction->getType() == INIT_TRANSACTION)
	{
		DLLInittedTransaction* pInit = dynamic_cast<DLLInittedTransaction*>(pTransaction);
		assert(pInit != NULL);
		cout << "DLL Initted: message=" << pInit->getMessage() << endl;
	}

	//cleanup transaction
	delete pTransaction;
}

void GameState::init( UINT width, UINT height, UINT numMines )
{
	if( !mInitialized )
	{
		createGrid( width, height );
		initGrid( numMines );
		mInitialized = true;
		mIsFirstClick = true;
	}
}

void GameState::initGrid( UINT numMines )
{
	mpGrid->clearAllCells();
	mpGrid->placeRandomMines( numMines );
	mNumMines = numMines;
}

void GameState::createGrid( UINT width, UINT height )
{
	mpGrid = new Grid( width, height );
}

void GameState::cleanup()
{
	delete mpGrid;
	mInitialized = false;
	mNumMines = 0;
}

int GameState::calcScore(bool won) const
{
	int score = 0;
	int numRevealed = mpGrid->getNumCells() - (mpGrid->getNumMines() + mpGrid->getNumHiddenNonMines());
	score = numRevealed;
	if (won)
	{
		score *= 2;
	}
	return score;
}


