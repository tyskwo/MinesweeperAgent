#include "StatTracker.h"
#include <fstream>
#include <cassert>

using namespace std;

StatTracker* gpStatTracker = NULL;

StatTracker::StatTracker( const std::string& filename )
	:mFilename( filename )
{
	mStream.open(filename);
	resetAllStats();
}

StatTracker::~StatTracker()
{
	//reportStats();
	mStream.close();
}

void StatTracker::addToStat(StatEnum::StatCatagory catagory, UINT addition)
{
	UINT val = mMap[ catagory];
	val += addition;
	mMap[ catagory ] = val;
}

void StatTracker::resetStat(StatEnum::StatCatagory catagory)
{
	mMap[ catagory ] = 0;
}

void StatTracker::reportStats(std::ostream& theStream) const
{
	/*
	for (UINT i = 0; i<StatEnum::NUM_STAT_CATAGORIES; i++)
	{
		StatEnum::StatCatagory catagory = (StatEnum::StatCatagory)i;
		theStream << STAT_DESCRIPTIONS[i] << " = " << mMap.find(catagory)->second << endl;
	}*/
	for (UINT i = 0; i<StatEnum::NUM_STAT_CATAGORIES; i++)
	{
		StatEnum::StatCatagory catagory = (StatEnum::StatCatagory)i;
		theStream << mMap.find(catagory)->second << ",";
	}
	theStream << endl;
}

void StatTracker::reportStats()
{
	if (mStream.good())
	{
		reportStats(mStream);
	}
}

void StatTracker::reportStatsHeaders()
{
	if (mStream.good())
	{
		mStream << "Name,";

		for (UINT i = 0; i<StatEnum::NUM_STAT_CATAGORIES; i++)
		{
			StatEnum::StatCatagory catagory = (StatEnum::StatCatagory)i;
			mStream << STAT_DESCRIPTIONS[i] << ",";
		}
		mStream << endl;
	}
}

void StatTracker::resetAllStats()
{
	mMap.clear();
	for (UINT i = 0; i < StatEnum::NUM_STAT_CATAGORIES; i++)
	{
		mMap[(StatEnum::StatCatagory)i] = 0;
	}
}

void StatTracker::logString(const std::string& message)
{
	assert(mStream.good());
	mStream << message << ",";
}

