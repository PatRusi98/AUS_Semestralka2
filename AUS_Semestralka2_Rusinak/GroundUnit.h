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
	int getPopulation(PopulationType type);
	double getTotalArea();
	double getBuiltUpArea();
	void setLGU(GroundUnit* lgu);
	void setData(int preProductive, int productive, int postProductive, double totalArea, double builtUpArea);
	bool hasParent(GroundUnit* parent);

private:
	SortedSequenceTable<wstring, GroundUnit*>* LGUnits;
	GroundUnit* higherGroundUnit;
	wstring name;
	GroundUnitType type;
	int preProductive = 0;
	int productive = 0;
	int postProductive = 0;
	double totalArea = 0.0;
	double builtUpArea = 0.0;
};

GroundUnit::GroundUnit(GroundUnitType type, wstring name, GroundUnit* higherGU)
{
	this->type = type;
	this->name = name;
	this->higherGroundUnit = higherGU;
	this->LGUnits = new SortedSequenceTable<wstring, GroundUnit*>();
}

GroundUnit::~GroundUnit()
{
	/*delete higherGroundUnit;
	higherGroundUnit = nullptr;*/
	//delete LGUnits;
	//LGUnits = nullptr;
	for(TableItem<wstring, GroundUnit*>* item : *LGUnits)
	{
		if (item->accessData() != nullptr)
		{
			delete item->accessData();
		}
	}

	delete LGUnits;
	LGUnits = nullptr;

}

inline wstring GroundUnit::getName()
{
	return this->name;
}

inline GroundUnitType GroundUnit::getType()
{
	return this->type;
}

inline GroundUnit* GroundUnit::getHGU()
{
	return this->higherGroundUnit;
}

inline SortedSequenceTable<wstring, GroundUnit*>* GroundUnit::getLGU()
{
	return this->LGUnits;
}

inline int GroundUnit::getPopulation(PopulationType type)
{
	switch (type)
	{
	case PREPRODUCTIVE:
		return this->preProductive;
		break;
	case PRODUCTIVE:
		return this->productive;
		break;
	case POSTPRODUCTIVE:
		return this->postProductive;
		break;
	case POPULATION:
		return this->preProductive + this->productive + this->postProductive;
	default:
		return 0;
		break;
	}
}

inline double GroundUnit::getTotalArea()
{
	return this->totalArea;
}

inline double GroundUnit::getBuiltUpArea()
{
	return this->builtUpArea;
}

inline void GroundUnit::setLGU(GroundUnit* lgu)
{
	this->LGUnits->insert(lgu->getName(), lgu);
}

inline void GroundUnit::setData(int preProductive, int productive, int postProductive, double totalArea, double builtUpArea)
{
	this->preProductive += preProductive;
	this->productive += productive;
	this->postProductive += postProductive;
	this->totalArea += totalArea;
	this->builtUpArea += builtUpArea;
}

inline bool GroundUnit::hasParent(GroundUnit* parent) 
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

	delete myParent;
	myParent = nullptr;

	return false;
}