#pragma once

#include <string>
#include "MineBrain.h"

class Event;
class TransactionHandler;

extern "C"
{
	__declspec(dllexport) void makeDecision(const Event& theEvent, TransactionHandler* pHandler);
}

void init(TransactionHandler* pHandler);
std::string getMessageFromStringTable();
std::string getMessageFromFileResource();

MineBrain* brain;
