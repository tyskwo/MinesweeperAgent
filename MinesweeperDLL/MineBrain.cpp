#include "MineBrain.h"
#include <stdio.h>

typedef UINT  Tile;
typedef UINT  Number;
typedef float Probability;




//
//   public methods
//

void MineBrain::Reset(int numMines)
{
	//new game, so it's the first turn
	isFirstTurn = true;

	//clear all of our lists, we know nothing of the new board
	bombs.clear(); revealed.clear(); unrevealed.clear(); safe.clear();

	//set the number of mines
	numberOfMines = numMines;
}



void MineBrain::SetBoard(GameView* gameBoard)
{
	board = gameBoard;
}



Tile MineBrain::MakeFirstTurn()
{
	//we made our move
	isFirstTurn = false;

	//return the center tile. see here: http://www.minesweeper.info/wiki/Strategy
										http://www.techuser.net/mineclick.html
	return board->getNumCells() / 2;
}



Tile MineBrain::FindSafeTile()
{
	//update our knowledge of what's revealed/unrevealed
	revealed   = GetAllRevealedTiles();
	unrevealed = GetAllUnrevealedTiles();


	for (Tile atIndex : revealed)
	{
		//check to see the number of flagged tiles vs the number of bombs
		CheckTileForMines(atIndex);

		//if we found all of the bombs in the level, add all unrevealed, unflagged tiles to the safe list
		if (bombs.size() == numberOfMines) return AddRestToSafe();
	}

	//find the safest tile to click
	return CheckAllRevealedProbabilities();
}



Tile MineBrain::TakeSafeTile()
{
	//print out how many mines we have left to flag
	std::cout << "M: " << numberOfMines - bombs.size() << '\n';

	//if we have any safe tiles
	while (safe.size() > 0)
	{
		Tile atIndex = safe.back();
		safe.pop_back();

		//if the tile is not revealed, click it
		if (!board->getCell(atIndex).isRevealed()) return atIndex;
	}

	//we have no safe tiles :/
	return UINT_MAX;
}










//
//   private methods
//

std::vector<Tile> MineBrain::GetAllRevealedTiles()
{
	std::vector<Tile> returnVector;
	for (Tile index = 0; index < board->getNumCells(); index++)
	{
		if (board->getCell(index).isRevealed()) returnVector.push_back(index);
	}

	return returnVector;
}


std::vector<Tile> MineBrain::GetAllUnrevealedTiles()
{
	std::vector<Tile> returnVector;
	for (Tile index = 0; index < board->getNumCells(); index++)
	{
		if (!board->getCell(index).isRevealed()) returnVector.push_back(index);
	}

	return returnVector;
}


std::vector<Tile> MineBrain::GetRevealedAdjacencies(Tile index)
{
	std::vector<Tile> returnVector;

	//get all the adjacent tiles at the given index
	std::vector<Tile> adjacent = board->getAdjacentCellIndices(index);

	for (Tile index : adjacent)
	{
		//if it's revealed, add it to the list
		if (board->getCell(index).isRevealed()) returnVector.push_back(index);
	}

	return returnVector;
}


std::vector<Tile> MineBrain::GetUnrevealedAdjacencies(Tile index)
{
	std::vector<Tile> returnVector;

	//get all the adjacent tiles at the given index
	std::vector<UINT> adjacent = board->getAdjacentCellIndices(index);

	for (Tile index : adjacent)
	{
		//if it's unrevealed, add it to the list
		if (!board->getCell(index).isRevealed()) returnVector.push_back(index);
	}

	return returnVector;
}






Probability MineBrain::GetAdjacentMineWeight(Tile index)
{
	//get the adjacent cells to the index
	std::vector<Tile> adjacents = board->getAdjacentCellIndices(index);

	int numberTotalBombs = 0;	//the number of bombs for all the adjacent tiles, e.g. tile A has 4, tile B has 3 = 7
	int numberKnownBombs = 0;	//the number of bombs we have flagged
	int adjacentsRevealed = 0;	//how many of the adjacent tiles are revealed
	int adjacentsUnrevealed = 0;//how many of the adjacent tiles are NOT revealed

	//going through all the adjacents...
	for (Tile atIndex : adjacents)
	{
		//if the adjacent is revealed...
		if (board->getCell(atIndex).isRevealed())
		{
			//add its number to the total number of bombs and increment the number revealed
			numberTotalBombs += board->getNumAdjacentMines(atIndex);
			adjacentsRevealed++;
		}
		//if the adjacent is not revealed, and it is on our bomb list...
		else if (std::find(bombs.begin(), bombs.end(), atIndex) != bombs.end())
		{
			//increment the number of known bombs
			numberKnownBombs++;
		}

		if (!board->getCell(atIndex).isRevealed()) adjacentsUnrevealed++;
	}

	//if no adjacents are revealed, return 0 weight, it's a REALLY unsafe move.
	if (adjacentsRevealed == 0) return 0;
	else
	{
		//difference in bomb knowledge / number of unrevealed adjacents
		return static_cast<float>(numberTotalBombs - numberKnownBombs) / adjacentsUnrevealed;
	}
}


Number MineBrain::GetKnownMineDifference(Tile atIndex)
{
	//get the number of bombs around the tile
	Number unknownMines = board->getNumAdjacentMines(atIndex);

	//get the number of known bombs around the tile
	std::vector<Tile> foundMines;
	Number knownMines = GetExclusive(GetUnrevealedAdjacencies(atIndex), bombs, foundMines);

	//return the difference
	return unknownMines - knownMines;
}


Number MineBrain::GetExclusive(const std::vector<Tile>& a, const std::vector<Tile>& b, std::vector<Tile>& exclusive)
{
	//how can we know if they have anything shared if we haven't compared them yet?
	Number shared = 0;

	for (Tile index : a)
	{
		//if the tile in a is in b, increment number shared
		if (std::find(b.begin(), b.end(), index) != b.end()) shared++;

		//otherwise add it to the exclusive list
		else exclusive.push_back(index);
	}

	//return the number of shared items
	return shared;
}






Tile MineBrain::ChooseFromAdjacent(Tile index)
{
	//get the adjacent tiles
	std::vector<Tile> adjacent = board->getAdjacentCellIndices(index);

	//smallest weight is obviously max to start
	float smallestWeight = FLT_MAX;

	//if we can't find a safe index, we'll choose from board
	Tile returnIndex = index;



	for (Tile atIndex : adjacent)
	{
		//if the tile is unrevealed and is not flagged...
		if (!board->getCell(atIndex).isRevealed() && 
			std::find(bombs.begin(), bombs.end(), atIndex) == bombs.end())
		{
			//get the weight for the tile
			float weight = GetAdjacentMineWeight(atIndex);

			//if it's the smallest weight...
			if (weight < smallestWeight)
			{
				//update smallest weight and return Tile
				returnIndex    = atIndex;
				smallestWeight = weight;
			}
		}
	}

	//if we found a safe alternative...
	if (returnIndex != index)
	{
		//print out the weight and return it
		std::cout << smallestWeight * 100 << '\n';
		return returnIndex;
	}


	//if the weight is larger than choosing a tile at random, choose one at random
	return ChooseFromBoard();
}


Tile MineBrain::ChooseFromBoard()
{
	for (Tile index = 0; index < board->getNumCells(); index++)
	{
		//if the tile is unrevealed and not flagged, return it
		if (!board->getCell(index).isRevealed()
			&& std::find(bombs.begin(), bombs.end(), index) == bombs.end())
		{
			return index;
		}
	}

	//Somehow, somewhere, we lost our place.
	std::cout << "Brain overloaded; no non-flagged, unrevealed cell found.\n";
	return -1;
}






/* DEPRECATED */ void MineBrain::CheckOneTwo(Tile atIndex)
{
	//if the tile reduces to a 1...
	if (GetKnownMineDifference(atIndex) == 1)
	{
		//get the tiles cardinal tiles around it
		Tile above = atIndex - board->getWidth();
		Tile below = atIndex + board->getWidth();
		Tile left  = atIndex - 1;
		Tile right = atIndex + 1;



		//check for a 'two' above
		if (GetKnownMineDifference(above) == 2)
		{
			Tile bombIndex = -1;

			//left side?
			if (!board->getCell(atIndex - 1).isRevealed() && !board->getCell(above - 1).isRevealed())
			{
				bombIndex = above - board->getWidth() - 1;
			}
			//no, right side?
			else if (!board->getCell(atIndex + 1).isRevealed() && !board->getCell(above + 1).isRevealed())
			{
				bombIndex = above - board->getWidth() + 1;
			}

			//if it's not flagged, flag it
			if (bombIndex >= 0 && std::find(bombs.begin(), bombs.end(), bombIndex) == bombs.end())
			{
				bombs.push_back(bombIndex);
			}
		}

		//check for a 'two' below
		if (GetKnownMineDifference(below) == 2)
		{
			Tile bombIndex = -1;

			//left side?
			if (!board->getCell(atIndex - 1).isRevealed() && !board->getCell(below - 1).isRevealed())
			{
				bombIndex = below + board->getWidth() - 1;
			}
			//no, right side?
			else if (!board->getCell(atIndex + 1).isRevealed() && !board->getCell(below + 1).isRevealed())
			{
				bombIndex = below + board->getWidth() + 1;
			}

			//if it's not flagged, flag it

			if (bombIndex >= 0 && std::find(bombs.begin(), bombs.end(), bombIndex) == bombs.end())
			{
				bombs.push_back(bombIndex);
			}
		}
		//check for a 'two' to theleft
		if (GetKnownMineDifference(left) == 2)
		{
			Tile bombIndex = -1;

			//above?
			if (!board->getCell(atIndex - board->getWidth()).isRevealed() && !board->getCell(left - board->getWidth()).isRevealed())
			{
				bombIndex = left - 1 - board->getWidth();
			}
			//no, below?
			else if (!board->getCell(atIndex + board->getWidth()).isRevealed() && !board->getCell(left + board->getWidth()).isRevealed())
			{
				bombIndex = left - 1 + board->getWidth();
			}

			//if it's not flagged, flag it
			if (bombIndex >= 0 && std::find(bombs.begin(), bombs.end(), bombIndex) == bombs.end())
			{
				bombs.push_back(bombIndex);
			}
		}

		//two to the right
		if (GetKnownMineDifference(right) == 2)
		{
			Tile bombIndex = -1;

			//above?
			if (!board->getCell(atIndex - board->getWidth()).isRevealed() && !board->getCell(right - board->getWidth()).isRevealed())
			{
				bombIndex = right + 1 - board->getWidth();
			}
			//no, below?
			else if (!board->getCell(atIndex + board->getWidth()).isRevealed() && !board->getCell(right + board->getWidth()).isRevealed())
			{
				bombIndex = right + 1 + board->getWidth();
			}

			//if it's not flagged, flag it
			if (bombIndex >= 0 && std::find(bombs.begin(), bombs.end(), bombIndex) == bombs.end())
			{
				bombs.push_back(bombIndex);
			}
		}
	}
}


void MineBrain::CheckTileForMines(Tile index)
{
	//get the number of bombs at the give tile
	Number minesAtIndex = board->getNumAdjacentMines(index);

	//if there's no bombs, exit early
	if (minesAtIndex == 0) return;

	
	//get the adjacent tiles
	std::vector<Tile> adjacentTiles = GetUnrevealedAdjacencies(index);

	//if the number of adjacent tiles is equal to the number of mines...
	if (adjacentTiles.size() == minesAtIndex)
	{
		//flag all of the adjacent tiles (if they're not already flagged)
		for (Tile index : adjacentTiles)
		{
			if (std::find(bombs.begin(), bombs.end(), index) == bombs.end()) bombs.push_back(index); 
		}
	}
}






Tile MineBrain::CheckAllRevealedProbabilities()
{
	//reference to the index with the best probability
	Tile probabilityIndex = -1;
	
	//the highest probability found and a flag for if we found a 100% safe tile
	float highestProbability = -1.0f;
	bool foundSafeTile = false;


	//for all of the revealed tiles...
	for (Tile atIndex : revealed)
	{
		//get the probability of a safe adjacent click
		float probability = CheckAdjacentMineProbability(atIndex);

		//if it's the highest found probability...
		if (probability > highestProbability)
		{
			//set it, and store the index
			probabilityIndex = atIndex;
			highestProbability = probability;
		}

		//there's a completely safe tile if probability is 100%!
		if (probability == 1.0f) foundSafeTile = true;
	}

	//if we found a safe tile, use it this turn!
	if (foundSafeTile)
	{
		Tile index = safe.back();
		safe.pop_back();

		return index;
	}

	//otherwise, choose an adjacent index
	else
	{
		std::cout << "\t" << highestProbability * 100 << "%\t";
		return ChooseFromAdjacent(probabilityIndex);
	}
}


Probability MineBrain::CheckAdjacentMineProbability(Tile index)
{
	//get the number of adjacent bombs
	Number numberOfAdjacentMines = board->getNumAdjacentMines(index);

	//if it's 0, it's a safe tile!
	if (numberOfAdjacentMines == 0)
	{
		//if it's unrevealed
		if (!board->getCell(index).isRevealed())
		{
			safe.push_back(index);
			return 1.0f;
		}
		
		//else it's revealed, we don't want to click it!
		else return 0.0f;
	}

	//get the number of known bombs
	std::vector<Tile> adjacent = GetUnrevealedAdjacencies(index);
	std::vector<Tile> unknown;
	Number foundMines = GetExclusive(adjacent, bombs, unknown);

	//if we found all of the bombs, and there's still unrevealed adjacents, add them to the safe list
	if (foundMines == numberOfAdjacentMines && adjacent.size() > foundMines)
	{
		safe.insert(safe.end(), unknown.begin(), unknown.end());
		return 1.0f;
	}

	//unknown = unrevealed && !flagged
	//return (unknown - unknownMines) / unknown
	else return static_cast<Probability>(unknown.size() - (numberOfAdjacentMines - foundMines)) / unknown.size();
}






Tile MineBrain::AddRestToSafe()
{
	//add all unrevealed, non-flagged tiles to the safe list
	for (Tile index = 0; index < board->getNumCells(); index++)
	{
		if (!board->getCell(index).isRevealed() && 
			std::find(bombs.begin(), bombs.end(), index) == bombs.end())
		{
			safe.push_back(index);
		}
	}

	//return a safe tile
	Tile returnTile = safe.back();
	safe.pop_back();

	return returnTile;
}
