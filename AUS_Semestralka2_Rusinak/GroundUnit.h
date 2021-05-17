#pragma once
#include <string>
#include "structures/structure.h"
#include "structures/table/sequence_table.h"
#include "structures/table/sorted_sequence_table.h"
#include "Enum.h"

using namespace std;
using namespace structures;

class GroundUnit
{
public:
	GroundUnit(GroundUnitType type, wstring name, GroundUnit* higherGU);
	~GroundUnit();
	wstring getName();
	GroundUnitType getType();
	GroundUnit* getHGU();
	SortedSequenceTable<wstring, GroundUnit*>* getLGU();
	void setLGU(GroundUnit* lgu);
	void setData(int preProductive, int productive, int postProductive, int totalArea, int builtUpArea);
	boolean hasParent(GroundUnit* parent);

private:
	SortedSequenceTable<wstring, GroundUnit*>* LGUnits;
	GroundUnit* higherGroundUnit;
	GroundUnit* lowerGroundUnit;
	wstring name;
	GroundUnitType type;
};

GroundUnit::GroundUnit(GroundUnitType type, wstring name, GroundUnit* higherGU)
{
	this->type = type;
	this->name = name;
	this->higherGroundUnit = higherGU;
	this->LGUnits = new SortedSequenceTable<wstring, GroundUnit*>();
	//this->lowerGroundUnit = new SortedSequenceTable<wstring, GroundUnit*>();
}

GroundUnit::~GroundUnit()
{
}

inline wstring GroundUnit::getName()
{
	return name;
}

inline GroundUnitType GroundUnit::getType()
{
	return type;
}

inline GroundUnit* GroundUnit::getHGU()
{
	return higherGroundUnit;
}

inline SortedSequenceTable<wstring, GroundUnit*>* GroundUnit::getLGU()
{
	return LGUnits;
}

inline void GroundUnit::setLGU(GroundUnit* lgu)
{
	LGUnits->insert(lgu->getName(), lgu);
}

inline void GroundUnit::setData(int preProductive, int productive, int postProductive, int totalArea, int builtUpArea)
{

}

inline boolean GroundUnit::hasParent(GroundUnit* parent) 
{
	GroundUnit* myParent = higherGroundUnit;
	while (myParent != nullptr) 
	{
		if (myParent == parent)
		{
			return true;
		}
		myParent = myParent->getHGU();
	}
	return false;
}