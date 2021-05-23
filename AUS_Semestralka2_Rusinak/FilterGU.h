#pragma once
#include "Filter.h"
#include <string>
#include <iostream>
#include <fstream>
#include "GroundUnit.h"
#include "structures/table/unsorted_sequence_table.h"
#include "structures/table/sequence_table.h"
#include "Criteria.h"
#include "CriteriaGU.h"

using namespace std;
using namespace structures;

template <typename T>
class FilterGU : public Filter<GroundUnit*, T, bool>
{
public:
	virtual bool meetsFilter(GroundUnit* groundUnit, T criteria, bool undefined) = 0;
};

class FilterGUName : public FilterGU<wstring>
{
public:
	inline bool meetsFilter(GroundUnit* groundUnit, wstring criteria, bool undefined) override
	{
		CriteriaGUName* name = new CriteriaGUName();

		if (name->rate(groundUnit, L"") == criteria)
		{
			return true;
		}
		return false;

		delete name;
		name = nullptr;
	}
};

class FilterGUType : public FilterGU<GroundUnitType>
{
public:
	inline bool meetsFilter(GroundUnit* groundUnit, GroundUnitType criteria, bool undefined) override
	{
		CriteriaGUType* type = new CriteriaGUType();
		if (undefined)
		{
			return true;
		}
		else if (type->rate(groundUnit, L"") == criteria)
		{
			return true;
		} 
		return false;

		delete type;
		type = nullptr;
	}
};

class FilterGUAdherence : public FilterGU<bool>
{
public:
	inline bool meetsFilter(GroundUnit* groundUnit, bool criteria, bool undefined) override
	{
		CriteriaGUAdherence* adherence = new CriteriaGUAdherence();
		if (adherence->rate(groundUnit, groundUnit->getHGU()->getName()))
		{
			return true;
		}
		return false;

		delete adherence;
		adherence = nullptr;
	}
};

class FilterGUPopulation : public FilterGU<int>
{
public:
	inline bool meetsFilter(GroundUnit* groundUnit, int criteria, bool undefined) override
	{
		CriteriaGUPopulationCount* population = new CriteriaGUPopulationCount();
		if (population->rate(groundUnit, L"") >= criteria)
		{
			return true;
		}
		return false;

		delete population;
		population = nullptr;
	}
};

class FilterGUBuildedUp : public FilterGU<double>
{
public:
	inline bool meetsFilter(GroundUnit* groundUnit, double criteria, bool undefined) override
	{
		CriteriaGUBuildedUp* buildedUp = new CriteriaGUBuildedUp();
		if (buildedUp->rate(groundUnit, L"") >= criteria)
		{
			return true;
		}
		return false;

		delete buildedUp;
		buildedUp = nullptr;
	}
};