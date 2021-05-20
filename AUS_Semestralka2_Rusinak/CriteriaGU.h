#pragma once
#include "Criteria.h"
#include "GroundUnit.h"
#include "Enum.h"

using namespace std;

template <typename T>
class CriteriaGU : public Criteria<GroundUnit*, T>
{
public:
	virtual T rate(GroundUnit* groundUnit) const = 0;
};

class CriteriaGUName : public CriteriaGU<wstring>
{
public:
	inline wstring rate(GroundUnit* groundUnit) const override
	{
		return groundUnit->getName();
	}
};

class CriteriaGUType : public CriteriaGU<GroundUnitType>
{
public:
	inline GroundUnitType rate(GroundUnit* groundUnit) const override
	{
		return groundUnit->getType();
	}
};

class CriteriaGUAdherence : public CriteriaGU<bool>
{
public:
	inline bool rate(GroundUnit* groundUnit) const override
	{
		return groundUnit->hasParent(groundUnit->getHGU());
	}
};

class CriteriaGUPreProductiveCount : public CriteriaGU<int>
{
public:
	inline int rate(GroundUnit* groundUnit) const override
	{
		return groundUnit->getPopulation(PopulationType::PREPRODUCTIVE);
	}
};

class CriteriaGUProductiveCount : public CriteriaGU<int>
{
public:
	inline int rate(GroundUnit* groundUnit) const override
	{
		return groundUnit->getPopulation(PopulationType::PRODUCTIVE);
	}
};

class CriteriaGUPostProductiveCount : public CriteriaGU<int>
{
public:
	inline int rate(GroundUnit* groundUnit) const override
	{
		return groundUnit->getPopulation(PopulationType::POSTPRODUCTIVE);
	}
};

class CriteriaGUPopulationCount : public CriteriaGU<int>
{
public:
	inline int rate(GroundUnit * groundUnit) const override
	{
		return	groundUnit->getPopulation(PopulationType::POPULATION);
	}
};

class CriteriaGUTotalArea : public CriteriaGU<double>
{
public:
	inline double rate(GroundUnit* groundUnit) const override
	{
		return groundUnit->getTotalArea();
	}
};

class CriteriaGUBuiltUpArea : public CriteriaGU<double>
{
public:
	inline double rate(GroundUnit* groundUnit) const override
	{
		return groundUnit->getBuiltUpArea();
	}
};

class CriteriaGUBuildedUp : public CriteriaGU<double>
{
public:
	inline double rate(GroundUnit* groundUnit) const override
	{
		return 100 * (groundUnit->getBuiltUpArea() / groundUnit->getTotalArea());
	}
};