#pragma once

#include "../GameLib/GameView.h"
#include <vector>


class MineBrain : public Trackable
{
	typedef UINT  Tile;			//Tiles are represented by UINTs in the GameView array
	typedef UINT  Number;		//For methods that return UINTs that aren't considered Tiles
	typedef float Probability;	//For statistical checks



//
//   member variables
//

private:

	GameView*			board;			//reference to the board and all its tiles

	std::vector<Tile>	bombs,			//list of tiles we have flagged as bombs
						revealed,		//			    that are revealed
						unrevealed,		//				that are unrevealed
						safe;			//				we know are unrevealed and not bombs

	Number numberOfMines;				//the number of mines within the level

	bool   isFirstTurn;					//is this the first turn?






//
//   public methods
//

public:

//called at the beginning of each game, clears all lists, sets the number of mines
	void Reset(int numberOfMines);

//is this turn the first turn of the game?
	inline bool IsFirstTurn() { return isFirstTurn; }

//stores a reference to the GameView, the array of tiles
	void SetBoard(GameView* gameBoard);

//first turn is special - we don't know anything!
	Tile MakeFirstTurn();

//called when our safe list is empty; find the safest pick on the board
	Tile FindSafeTile();

//return the last tile in our safe list
	Tile TakeSafeTile();





	

//
//   private methods
//

private:
//returns all revealed tiles on the board
	std::vector<Tile> GetAllRevealedTiles();

//returns all unrevealed tiles on the board
	std::vector<Tile> GetAllUnrevealedTiles();	

//returns the revealed adjacent tiles given a specific index
	std::vector<Tile> GetRevealedAdjacencies(Tile atIndex);

//returns the unrevealed adjacent tiles given a specific index
	std::vector<Tile> GetUnrevealedAdjacencies(Tile atIndex);




//returns the weight of a tile; weight is calculated as (# of unknown bombs / # of unrevealed)
	Probability GetAdjacentMineWeight(Tile index);

//return the difference between the number of adjacent mines and the number of flagged mines given a specific index
	Number GetKnownMineDifference(Tile atIndex);

//returns the number of elements shared by both a and b
//http://programmers.stackexchange.com/questions/163538/efficient-way-to-find-unique-elements-in-a-vector-compared-against-multiple-vect
	Number GetExclusive(const std::vector<Tile>& a, const std::vector<Tile>& b, std::vector<Tile>& exclusive);




//returns the adjacent tile with the lowest weight
	Tile ChooseFromAdjacent(Tile atIndex);

//if none of the adjacent tiles' weights are lower than picking a tile at random, pick a tile at random
//   *this is where the 1-2 pattern would come in
	Tile ChooseFromBoard();





//DEPRECATED - not working, does not take edges into account
//use the 1-2 pattern at a given index, and add any found bombs to the bombs list
	void CheckOneTwo(Tile atIndex);

//checks the adjacent tiles for mines and adds them to the bombs list
	void CheckTileForMines(Tile index);




//returns the safest tile
	Tile CheckAllRevealedProbabilities();

//returns the probability of a mine of clicking a tile adjacent to the given tile
	Probability CheckAdjacentMineProbability(Tile index);




//adds the remaining unrevealed tiles to the safe list and returns one of those for this frame
	Tile AddRestToSafe();
};