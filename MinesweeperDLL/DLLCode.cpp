#include <iostream>
#include "DLLCode.h"
#include "DLLUtil.h"
#include "..\GameLib\GameView.h"
#include "..\GameLib\Transaction.h"
#include "..\GameLib\CellClickTransaction.h"
#include "..\GameLib\DLLInittedTransaction.h"
#include "..\GameLib\TransactionHandler.h"
#include "..\GameLib\Event.h"
#include "..\GameLib\PickCellEvent.h"
#include "..\GameLib\ResetGameEvent.h"
#include <windows.h>
#include "resource.h"





void init(TransactionHandler* pHandler)
{
	pHandler->postTransaction(new DLLInittedTransaction(getMessageFromStringTable()));
	pHandler->postTransaction(new DLLInittedTransaction(getMessageFromFileResource()));

	brain = new MineBrain(); //create our brain on startup
}






__declspec(dllexport) void makeDecision(const Event& theEvent, TransactionHandler* pHandler)
{
	typedef UINT Tile;



	if(theEvent.getType() == PICK_A_CELL_EVENT)
	{
		const PickCellEvent& pickEvent = static_cast<const PickCellEvent&>(theEvent);
		auto board = pickEvent.getGameView();


		//the tile to click
		Tile index;

		//if it's NOT the first turn of the game...
		if (!brain->IsFirstTurn())
		{
			//grab a safe tile
			index = brain->TakeSafeTile();

			//if there's none available, find the next best option
			if (index == UINT_MAX)
			{
				index = brain->FindSafeTile();
			}
		}

		//if it's the first turn...
		else
		{
			//set the reference to the board and make our first move.
			brain->SetBoard(&board);
			index = brain->MakeFirstTurn();
		}

		//pass off our move to the main program with our given tile
		pHandler->postTransaction(new CellClickTransaction(index));
	}




	else if (theEvent.getType() == GAME_RESET_EVENT)
	{
		const ResetGameEvent& resetEvent = static_cast<const ResetGameEvent&>(theEvent);

		//reset our brain for the next game and store the number of mines in the new level
		brain->Reset(resetEvent.getNumMines());
	}




	else if (theEvent.getType() == INIT_EVENT)
	{
		init(pHandler);
	}




	else if (theEvent.getType() == CLEANUP_EVENT)
	{
		delete brain;
		brain = NULL;
	}




	return;
}















string getMessageFromStringTable()
{
	char buffer[1024];
	HINSTANCE hInst = (HINSTANCE)GetCurrentModule();
	int numRead = LoadStringA(hInst, IDS_STRING103, buffer, 1023);
	return string(buffer);
}

string getMessageFromFileResource()
{
	char buffer[1024];
	HINSTANCE hInst = (HINSTANCE)GetCurrentModule();
	HRSRC hRes = FindResource(hInst, MAKEINTRESOURCE(IDR_TEXT1), RT_RCDATA);
	HGLOBAL hGlobal = LoadResource(hInst, hRes);
	DWORD dwSize = SizeofResource(hInst, hRes);
	LPVOID ptr = LockResource(hGlobal);
	strncpy_s(buffer, (char*)ptr, dwSize);
	UnlockResource(hGlobal);

	return string(buffer);
}
