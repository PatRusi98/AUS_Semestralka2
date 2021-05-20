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
class FilterGU : public Filter<GroundUnit*, T>
{
public:
	virtual bool meetsFilter(GroundUnit* groundUnit, T criteria) = 0;
};

class FilterGUName : public FilterGU<wstring>
{
public:
	inline bool meetsFilter(GroundUnit* groundUnit, wstring criteria) override
	{
		CriteriaGUName* name = new CriteriaGUName();
		if (name->rate(groundUnit) == criteria)
		{
			return true;
		}
		return false;
	};
};

class FilterGUType : public FilterGU<GroundUnitType>
{
public:
	inline bool meetsFilter(GroundUnit* groundUnit, GroundUnitType criteria) override
	{
		CriteriaGUType* type = new CriteriaGUType();
		if (type->rate(groundUnit) == criteria)
		{
			return true;
		}
		return false;
	}
};

class FilterGUAdherence : public FilterGU<bool>
{
public:
	inline bool meetsFilter(GroundUnit* groundUnit, bool criteria) override
	{
		CriteriaGUAdherence* adherence = new CriteriaGUAdherence();
		if (adherence->rate(groundUnit))
		{
			return true;
		}
		return false;
	}
};

class FilterGUPopulation : public FilterGU<int>
{
public:
	inline bool meetsFilter(GroundUnit* groundUnit, int criteria) override
	{
		CriteriaGUPopulationCount* population = new CriteriaGUPopulationCount();
		if (population->rate(groundUnit) >= criteria)
		{
			return true;
		}
		return false;
	}
};

class FilterGUBuildedUp : public FilterGU<double>
{
public:
	inline bool meetsFilter(GroundUnit* groundUnit, double criteria) override
	{
		CriteriaGUBuildedUp* buildedUp = new CriteriaGUBuildedUp();
		if (buildedUp->rate(groundUnit) >= criteria)
		{
			return true;
		}
		return false;
	}
};