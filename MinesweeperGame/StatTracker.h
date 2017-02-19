#pragma once

#include "..\gamelib\trackable.h"
#include <string>
#include <map>
#include <windows.h>
#include <fstream>

//class only exists to hold a class level enum
class StatEnum
{
public:
	enum StatCatagory
	{
		GAMES_SIMULATED_SMALL,
		GAMES_SIMULATED_MEDIUM,
		GAMES_SIMULATED_LARGE,
		GAMES_WON_SMALL,
		GAMES_WON_MEDIUM,
		GAMES_WON_LARGE,
		GAMES_LOST_SMALL,
		GAMES_LOST_MEDIUM,
		GAMES_LOST_LARGE,
		TOTAL_SCORE_SMALL,
		TOTAL_SCORE_MEDIUM,
		TOTAL_SCORE_LARGE, 
		TOTAL_SCORE_OVERALL,
		NUM_STAT_CATAGORIES
	};

};

const std::string STAT_DESCRIPTIONS[StatEnum::NUM_STAT_CATAGORIES] = 
{
	"Number of small Games Simulated",
	"Number of medium Games Simulated",
	"Number of large Games Simulated",
	"Number of small Games Won",
	"Number of medium Games Won",
	"Number of large Games Won",
	"Number of small Games Lost",
	"Number of medium Games Lost",
	"Number of large Games Lost",
	"Total score of small games",
	"Total score of medium games",
	"Total score of large games",
	"Total score"
};



class StatTracker :	public Trackable
{
public:
	StatTracker( const std::string& filename );
	~StatTracker();
	void addToStat( StatEnum::StatCatagory catagory, UINT addition );
	void resetStat( StatEnum::StatCatagory catagory);
	void resetAllStats();
	void reportStats( std::ostream& theStream ) const ;
	void reportStats();
	void reportStatsHeaders();
	void logString(const std::string& message);

private:
	std::string mFilename;
	std::ofstream mStream;
	std::map<StatEnum::StatCatagory,UINT> mMap;
};

extern StatTracker* gpStatTracker;