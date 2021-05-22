#include <iostream>
#include <string>
#include "Input.h"
#include "structures/structure.h"
#include "structures/table/sequence_table.h"
#include "structures/table/sorted_sequence_table.h"
#include "structures/table/unsorted_sequence_table.h"
#include "GroundUnit.h"
#include <Windows.h>
#include "CriteriaGU.h"
#include "structures/table/sort/quick_sort.h"
#include <io.h>
#include <fcntl.h>
#include "Filter.h"
#include "FilterGU.h"
#include "Enum.h"

class UserInterface
{
public:
	UserInterface(string file2, string file1);
	~UserInterface();
	void menu();
	void operation1(FilterType type);
	void operation2(CriteriaType type, bool asc);
	void operation3(FilterType type, GroundUnitType typeGU);
	void operation4(CriteriaType type, GroundUnitType typeGU, bool asc);
	void operationSelect();
	GroundUnit* getCountry();

private:
	Input* input = new Input();
	int selectionType = 0;
	int selection = 0;
	int selectionDir = 0;
	GroundUnit* country = new GroundUnit(GroundUnitType::COUNTRY, L"Slovensko", NULL);
	SortedSequenceTable<wstring, GroundUnit*>* nameSort;
	UnsortedSequenceTable<wstring, int>* populationSort;
	UnsortedSequenceTable<wstring, double>* buildedUpSort;
	CriteriaGUName* nameCriteria;
	CriteriaGUAdherence* adherenceCriteria;
	CriteriaGUPopulationCount* populationCriteria;
	CriteriaGUBuildedUp* buildedUpCriteria;
	QuickSort<wstring, int>* sortPopulation;
	QuickSort<wstring, double>* sortBuildedUp;
	FilterGUBuildedUp* filterBuildedUp = new FilterGUBuildedUp();
	FilterGUPopulation* filterPopulation = new FilterGUPopulation();
	FilterGUName* filterName = new FilterGUName();
	FilterGUType* filterType = new FilterGUType();
	FilterGUAdherence* filterAdherence = new FilterGUAdherence();
	bool foundOp;
	bool ascending;
	int selectionOp;
	wstring nameOp;
	int populationOp;
	double buildedUpOp;
};

UserInterface::UserInterface(string file2, string file1)
{
	input->loadGroundUnit(file2, country);
	input->loadPopulation(file1, country);
}

UserInterface::~UserInterface()
{
}

using namespace std;
using namespace structures;

inline void UserInterface::menu()
{
	wcout << L"Vyber si oper�ciu: / Choose operation: " << endl;
	wcout << L"1: Vyp�sanie inform�ci� o �zemn�ch jednotk�ch / Informations about ground units" << endl;
	wcout << L"2: Zoradenie �zemn�ch jednotiek sp��aj�cich dan� filter / Sorting ground units by filter" << endl;
	wcout << L"3: Vyp�sanie inform�ci� o �zemn�ch jednotk�ch / Informations about ground units" << endl;
	wcout << L"4: Zoradenie �zemn�ch jednotiek sp��aj�cich dan� filter / Sorting ground units by filter" << endl;
	wcout << L"5: Vyp�sanie inform�ci� o zoskupeniach bytov�ch jednotiek vo zvolenej �zemnej jednotke / " << endl;
	wcout << L"6: Vyp�sanie sum�rnych inform�ci� o po�toch bytov�ch jednotiek vo zvolen�ch �zemn�ch jednotk�ch / " << endl;
	wcout << L"7: Zoradenie �zemn�ch jednotiek pod�a sum�rneho po�tu zvolen�ch bytov�ch jednotiek / " << endl;
	wcout << L"8: Vyp�sanie v�berov�ch krit�ri� pre zoskupenia bytov�ch jednotiek vo zvolen�ch �zemn�ch jednotk�ch / " << endl;
	wcout << L"9: Vyp�sanie v�berov�ch krit�ri� pre �zemn� jednotky na z�klade zvolen�ch zoskupen� bytov�ch jednotiek / " << endl;
	wcout << L"0: Koniec / Exit" << endl << endl;

	do
	{
		wcout << L"Tvoj vyber / your choice: ";
		wcin >> selection;
	} while (selection > 9 || selection < 0);

	wcout << endl;

	if (selection == 0)
	{
		exit(0);
	}

	operationSelect();
}

inline void UserInterface::operation1(FilterType type)
{
	switch (type)
	{
	case FILTERNAME:
		nameCriteria = new CriteriaGUName();
		filterName = new FilterGUName();
		for (TableItem<wstring, GroundUnit*>* region : *country->getLGU())
		{
			for (TableItem<wstring, GroundUnit*>* district : *region->accessData()->getLGU())
			{
				for (TableItem<wstring, GroundUnit*>* town : *district->accessData()->getLGU())
				{
					if (filterName->meetsFilter(town->accessData(), nameOp, false))
					{
						wcout << town->accessData()->getName() << ", " << district->accessData()->getName() << ", " << region->accessData()->getName() << ", " << country->getName() << endl;
						wcout << L"Po�et obyvate�ov: " << town->accessData()->getPopulation(PopulationType::POPULATION) << endl;
						wcout << L"Po�et preprodukt�vnych obyvate�ov: " << town->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
						wcout << L"Po�et produkt�vnych obyvate�ov: " << town->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
						wcout << L"Po�et poprodukt�vnych obyvate�ov: " << town->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
						wcout << L"Zastavanos�: " << (town->accessData()->getBuiltUpArea() / town->accessData()->getTotalArea()) * 100 << L" %" << endl;
						wcout << L"Celkov� v�mera: " << town->accessData()->getTotalArea() << L" m2" << endl;
						wcout << L"Zastavan� plocha: " << town->accessData()->getBuiltUpArea() << L" m2" << endl << endl;
					}
				}
			}
		}
		break;
	case FILTERPOPULATION:
		populationCriteria = new CriteriaGUPopulationCount();
		filterPopulation = new FilterGUPopulation();
		for (TableItem<wstring, GroundUnit*>* region : *country->getLGU())
		{
			for (TableItem<wstring, GroundUnit*>* district : *region->accessData()->getLGU())
			{
				for (TableItem<wstring, GroundUnit*>* town : *district->accessData()->getLGU())
				{
					if (filterPopulation->meetsFilter(town->accessData(), populationOp, false))
					{
						wcout << town->accessData()->getName() << ", " << district->accessData()->getName() << ", " << region->accessData()->getName() << ", " << country->getName() << endl;
						wcout << L"Po�et obyvate�ov: " << town->accessData()->getPopulation(PopulationType::POPULATION) << endl;
						wcout << L"Po�et preprodukt�vnych obyvate�ov: " << town->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
						wcout << L"Po�et produkt�vnych obyvate�ov: " << town->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
						wcout << L"Po�et poprodukt�vnych obyvate�ov: " << town->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
						wcout << L"Zastavanos�: " << (town->accessData()->getBuiltUpArea() / town->accessData()->getTotalArea()) * 100 << L" %" << endl;
						wcout << L"Celkov� v�mera: " << town->accessData()->getTotalArea() << L" m2" << endl;
						wcout << L"Zastavan� plocha: " << town->accessData()->getBuiltUpArea() << L" m2" << endl << endl;
					}
				}
			}
		}
		break;
	case FILTERBUILDEDUP:
		buildedUpCriteria = new CriteriaGUBuildedUp();
		filterBuildedUp = new FilterGUBuildedUp();
		for (TableItem<wstring, GroundUnit*>* region : *country->getLGU())
		{
			for (TableItem<wstring, GroundUnit*>* district : *region->accessData()->getLGU())
			{
				for (TableItem<wstring, GroundUnit*>* town : *district->accessData()->getLGU())
				{
					if (filterBuildedUp->meetsFilter(town->accessData(), buildedUpOp, false))
					{
						wcout << town->accessData()->getName() << ", " << district->accessData()->getName() << ", " << region->accessData()->getName() << ", " << country->getName() << endl;
						wcout << L"Po�et obyvate�ov: " << town->accessData()->getPopulation(PopulationType::POPULATION) << endl;
						wcout << L"Po�et preprodukt�vnych obyvate�ov: " << town->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
						wcout << L"Po�et produkt�vnych obyvate�ov: " << town->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
						wcout << L"Po�et poprodukt�vnych obyvate�ov: " << town->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
						wcout << L"Zastavanos�: " << (town->accessData()->getBuiltUpArea() / town->accessData()->getTotalArea()) * 100 << L" %" << endl;
						wcout << L"Celkov� v�mera: " << town->accessData()->getTotalArea() << L" m2" << endl;
						wcout << L"Zastavan� plocha: " << town->accessData()->getBuiltUpArea() << L" m2" << endl << endl;
					}
				}
			}
		}
		break;
	default:
		break;
	}
}

inline void UserInterface::operation2(CriteriaType type, bool asc)
{
	switch (type)
	{
	case NAME:
		nameSort = new SortedSequenceTable<wstring, GroundUnit*>();
		for (TableItem<wstring, GroundUnit*>* region : *country->getLGU())
		{
			for (TableItem<wstring, GroundUnit*>* district : *region->accessData()->getLGU())
			{
				for (TableItem<wstring, GroundUnit*>* town : *district->accessData()->getLGU())
				{
					nameSort->insert(town->getKey(), town->accessData());
				}
			}
		}
		for (TableItem<wstring, GroundUnit*>* groundUnit : *nameSort)
		{
			wcout << groundUnit->getKey() << " : " << groundUnit->accessData()->getName() << endl;
		}
		break;
	case POPULATIONCOUNT:
		populationCriteria = new CriteriaGUPopulationCount();
		populationSort = new UnsortedSequenceTable<wstring, int>();
		for (TableItem<wstring, GroundUnit*>* region : *country->getLGU())
		{
			for (TableItem<wstring, GroundUnit*>* district : *region->accessData()->getLGU())
			{
				for (TableItem<wstring, GroundUnit*>* town : *district->accessData()->getLGU())
				{
					populationSort->insert(town->getKey(), populationCriteria->rate(town->accessData(), NULL));
				}
			}
		}
		sortPopulation = new QuickSort<wstring, int>();
		sortPopulation->sort(*populationSort, asc);
		for (TableItem<wstring, int>* groundUnit : *populationSort)
		{
			wcout << groundUnit->getKey() << ": " << groundUnit->accessData() << endl;
		}
		break;
	case BUILDEDUP:
		buildedUpCriteria = new CriteriaGUBuildedUp();
		buildedUpSort = new UnsortedSequenceTable<wstring, double>();
		for (TableItem<wstring, GroundUnit*>* region : *country->getLGU())
		{
			for (TableItem<wstring, GroundUnit*>* district : *region->accessData()->getLGU())
			{
				for (TableItem<wstring, GroundUnit*>* town : *district->accessData()->getLGU())
				{
					buildedUpSort->insert(town->getKey(), buildedUpCriteria->rate(town->accessData(), NULL));
				}
			}
		}
		sortBuildedUp = new QuickSort<wstring, double>();
		sortBuildedUp->sort(*buildedUpSort, asc);
		for (TableItem<wstring, double>* groundUnit : *buildedUpSort)
		{
			wcout << groundUnit->getKey() << ": " << groundUnit->accessData() << endl;
		}
		break;
	default:
		break;
	}
}

inline void UserInterface::operation3(FilterType type, GroundUnitType typeGU)
{
	bool undefined = false;
	if (typeGU == GroundUnitType::COUNTRY)
	{
		undefined = true;
	}

	switch (type)
	{
	case FILTERNAME:
		nameCriteria = new CriteriaGUName();
		filterName = new FilterGUName();
		for (TableItem<wstring, GroundUnit*>* region : *country->getLGU())
		{
			if (!filterType->meetsFilter(region->accessData(), typeGU, undefined))
			{
				for (TableItem<wstring, GroundUnit*>* district : *region->accessData()->getLGU())
				{
					if (!filterType->meetsFilter(district->accessData(), typeGU, undefined))
					{
						for (TableItem<wstring, GroundUnit*>* town : *district->accessData()->getLGU())
						{
							if (filterName->meetsFilter(town->accessData(), nameOp, false) && filterAdherence->meetsFilter(town->accessData(), adherenceCriteria, false) && filterType->meetsFilter(town->accessData(), typeGU, undefined))
							{
								wcout << town->accessData()->getName() << ", " << district->accessData()->getName() << ", " << region->accessData()->getName() << ", " << country->getName() << endl;
								wcout << L"Po�et obyvate�ov: " << town->accessData()->getPopulation(PopulationType::POPULATION) << endl;
								wcout << L"Po�et preprodukt�vnych obyvate�ov: " << town->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
								wcout << L"Po�et produkt�vnych obyvate�ov: " << town->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
								wcout << L"Po�et poprodukt�vnych obyvate�ov: " << town->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
								wcout << L"Zastavanos�: " << (town->accessData()->getBuiltUpArea() / town->accessData()->getTotalArea()) * 100 << L" %" << endl;
								wcout << L"Celkov� v�mera: " << town->accessData()->getTotalArea() << L" m2" << endl;
								wcout << L"Zastavan� plocha: " << town->accessData()->getBuiltUpArea() << L" m2" << endl << endl;
							}
						}
					}
					if (filterAdherence->meetsFilter(district->accessData(), adherenceCriteria, false) && filterType->meetsFilter(district->accessData(), typeGU, undefined))
					{
						wcout << district->accessData()->getName() << ", " << region->accessData()->getName() << ", " << country->getName() << endl;
						wcout << L"Po�et obyvate�ov: " << district->accessData()->getPopulation(PopulationType::POPULATION) << endl;
						wcout << L"Po�et preprodukt�vnych obyvate�ov: " << district->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
						wcout << L"Po�et produkt�vnych obyvate�ov: " << district->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
						wcout << L"Po�et poprodukt�vnych obyvate�ov: " << district->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
						wcout << L"Zastavanos�: " << (district->accessData()->getBuiltUpArea() / district->accessData()->getTotalArea()) * 100 << L" %" << endl;
						wcout << L"Celkov� v�mera: " << district->accessData()->getTotalArea() << L" m2" << endl;
						wcout << L"Zastavan� plocha: " << district->accessData()->getBuiltUpArea() << L" m2" << endl << endl;
					}
				}
			}
			if (filterAdherence->meetsFilter(region->accessData(), adherenceCriteria, false) && filterType->meetsFilter(region->accessData(), typeGU, undefined))
			{
				wcout << region->accessData()->getName() << ", " << country->getName() << endl;
				wcout << L"Po�et obyvate�ov: " << region->accessData()->getPopulation(PopulationType::POPULATION) << endl;
				wcout << L"Po�et preprodukt�vnych obyvate�ov: " << region->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
				wcout << L"Po�et produkt�vnych obyvate�ov: " << region->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
				wcout << L"Po�et poprodukt�vnych obyvate�ov: " << region->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
				wcout << L"Zastavanos�: " << (region->accessData()->getBuiltUpArea() / region->accessData()->getTotalArea()) * 100 << L" %" << endl;
				wcout << L"Celkov� v�mera: " << region->accessData()->getTotalArea() << L" m2" << endl;
				wcout << L"Zastavan� plocha: " << region->accessData()->getBuiltUpArea() << L" m2" << endl << endl;
			}
		}
		break;
	case FILTERPOPULATION:
		nameCriteria = new CriteriaGUName();
		filterName = new FilterGUName();
		for (TableItem<wstring, GroundUnit*>* region : *country->getLGU())
		{
			if (!filterType->meetsFilter(region->accessData(), typeGU, undefined))
			{
				for (TableItem<wstring, GroundUnit*>* district : *region->accessData()->getLGU())
				{
					if (!filterType->meetsFilter(district->accessData(), typeGU, undefined))
					{
						for (TableItem<wstring, GroundUnit*>* town : *district->accessData()->getLGU())
						{
							if (filterPopulation->meetsFilter(town->accessData(), populationOp, false) && filterAdherence->meetsFilter(town->accessData(), adherenceCriteria, false) && filterType->meetsFilter(town->accessData(), typeGU, undefined))
							{
								wcout << town->accessData()->getName() << ", " << district->accessData()->getName() << ", " << region->accessData()->getName() << ", " << country->getName() << endl;
								wcout << L"Po�et obyvate�ov: " << town->accessData()->getPopulation(PopulationType::POPULATION) << endl;
								wcout << L"Po�et preprodukt�vnych obyvate�ov: " << town->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
								wcout << L"Po�et produkt�vnych obyvate�ov: " << town->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
								wcout << L"Po�et poprodukt�vnych obyvate�ov: " << town->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
								wcout << L"Zastavanos�: " << (town->accessData()->getBuiltUpArea() / town->accessData()->getTotalArea()) * 100 << L" %" << endl;
								wcout << L"Celkov� v�mera: " << town->accessData()->getTotalArea() << L" m2" << endl;
								wcout << L"Zastavan� plocha: " << town->accessData()->getBuiltUpArea() << L" m2" << endl << endl;
							}
						}
					}
					if (filterPopulation->meetsFilter(district->accessData(), populationOp, false) && filterAdherence->meetsFilter(district->accessData(), adherenceCriteria, false) && filterType->meetsFilter(district->accessData(), typeGU, undefined))
					{
						wcout << district->accessData()->getName() << ", " << region->accessData()->getName() << ", " << country->getName() << endl;
						wcout << L"Po�et obyvate�ov: " << district->accessData()->getPopulation(PopulationType::POPULATION) << endl;
						wcout << L"Po�et preprodukt�vnych obyvate�ov: " << district->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
						wcout << L"Po�et produkt�vnych obyvate�ov: " << district->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
						wcout << L"Po�et poprodukt�vnych obyvate�ov: " << district->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
						wcout << L"Zastavanos�: " << (district->accessData()->getBuiltUpArea() / district->accessData()->getTotalArea()) * 100 << L" %" << endl;
						wcout << L"Celkov� v�mera: " << district->accessData()->getTotalArea() << L" m2" << endl;
						wcout << L"Zastavan� plocha: " << district->accessData()->getBuiltUpArea() << L" m2" << endl << endl;
					}
				}
			}
			if (filterPopulation->meetsFilter(region->accessData(), populationOp, false) && filterAdherence->meetsFilter(region->accessData(), adherenceCriteria, false) && filterType->meetsFilter(region->accessData(), typeGU, undefined))
			{
				wcout << region->accessData()->getName() << ", " << country->getName() << endl;
				wcout << L"Po�et obyvate�ov: " << region->accessData()->getPopulation(PopulationType::POPULATION) << endl;
				wcout << L"Po�et preprodukt�vnych obyvate�ov: " << region->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
				wcout << L"Po�et produkt�vnych obyvate�ov: " << region->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
				wcout << L"Po�et poprodukt�vnych obyvate�ov: " << region->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
				wcout << L"Zastavanos�: " << (region->accessData()->getBuiltUpArea() / region->accessData()->getTotalArea()) * 100 << L" %" << endl;
				wcout << L"Celkov� v�mera: " << region->accessData()->getTotalArea() << L" m2" << endl;
				wcout << L"Zastavan� plocha: " << region->accessData()->getBuiltUpArea() << L" m2" << endl << endl;
			}
		}
		break;
	case FILTERBUILDEDUP:
		nameCriteria = new CriteriaGUName();
		filterName = new FilterGUName();
		for (TableItem<wstring, GroundUnit*>* region : *country->getLGU())
		{
			if (!filterType->meetsFilter(region->accessData(), typeGU, undefined))
			{
				for (TableItem<wstring, GroundUnit*>* district : *region->accessData()->getLGU())
				{
					if (!filterType->meetsFilter(district->accessData(), typeGU, undefined))
					{
						for (TableItem<wstring, GroundUnit*>* town : *district->accessData()->getLGU())
						{
							if (filterBuildedUp->meetsFilter(town->accessData(), buildedUpOp, false) && filterAdherence->meetsFilter(town->accessData(), adherenceCriteria, false) && filterType->meetsFilter(town->accessData(), typeGU, undefined))
							{
								wcout << town->accessData()->getName() << ", " << district->accessData()->getName() << ", " << region->accessData()->getName() << ", " << country->getName() << endl;
								wcout << L"Po�et obyvate�ov: " << town->accessData()->getPopulation(PopulationType::POPULATION) << endl;
								wcout << L"Po�et preprodukt�vnych obyvate�ov: " << town->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
								wcout << L"Po�et produkt�vnych obyvate�ov: " << town->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
								wcout << L"Po�et poprodukt�vnych obyvate�ov: " << town->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
								wcout << L"Zastavanos�: " << (town->accessData()->getBuiltUpArea() / town->accessData()->getTotalArea()) * 100 << L" %" << endl;
								wcout << L"Celkov� v�mera: " << town->accessData()->getTotalArea() << L" m2" << endl;
								wcout << L"Zastavan� plocha: " << town->accessData()->getBuiltUpArea() << L" m2" << endl << endl;
							}
						}
					}
					if (filterBuildedUp->meetsFilter(district->accessData(), buildedUpOp, false) && filterAdherence->meetsFilter(district->accessData(), adherenceCriteria, false) && filterType->meetsFilter(district->accessData(), typeGU, undefined))
					{
						wcout << district->accessData()->getName() << ", " << region->accessData()->getName() << ", " << country->getName() << endl;
						wcout << L"Po�et obyvate�ov: " << district->accessData()->getPopulation(PopulationType::POPULATION) << endl;
						wcout << L"Po�et preprodukt�vnych obyvate�ov: " << district->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
						wcout << L"Po�et produkt�vnych obyvate�ov: " << district->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
						wcout << L"Po�et poprodukt�vnych obyvate�ov: " << district->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
						wcout << L"Zastavanos�: " << (district->accessData()->getBuiltUpArea() / district->accessData()->getTotalArea()) * 100 << L" %" << endl;
						wcout << L"Celkov� v�mera: " << district->accessData()->getTotalArea() << L" m2" << endl;
						wcout << L"Zastavan� plocha: " << district->accessData()->getBuiltUpArea() << L" m2" << endl << endl;
					}
				}
			}
			if (filterBuildedUp->meetsFilter(region->accessData(), buildedUpOp, false) && filterAdherence->meetsFilter(region->accessData(), adherenceCriteria, false) && filterType->meetsFilter(region->accessData(), typeGU, undefined))
			{
				wcout << region->accessData()->getName() << ", " << country->getName() << endl;
				wcout << L"Po�et obyvate�ov: " << region->accessData()->getPopulation(PopulationType::POPULATION) << endl;
				wcout << L"Po�et preprodukt�vnych obyvate�ov: " << region->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
				wcout << L"Po�et produkt�vnych obyvate�ov: " << region->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
				wcout << L"Po�et poprodukt�vnych obyvate�ov: " << region->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
				wcout << L"Zastavanos�: " << (region->accessData()->getBuiltUpArea() / region->accessData()->getTotalArea()) * 100 << L" %" << endl;
				wcout << L"Celkov� v�mera: " << region->accessData()->getTotalArea() << L" m2" << endl;
				wcout << L"Zastavan� plocha: " << region->accessData()->getBuiltUpArea() << L" m2" << endl << endl;
			}
		}
		break;
	default:
		break;
	}
}

inline void UserInterface::operation4(CriteriaType type, GroundUnitType typeGU, bool asc)
{
	bool undefined = false;
	if (typeGU == GroundUnitType::COUNTRY)
	{
		undefined = true;
	}

	adherenceCriteria = new CriteriaGUAdherence();
	switch (type)
	{
	case NAME:
		nameSort = new SortedSequenceTable<wstring, GroundUnit*>();
		for (TableItem<wstring, GroundUnit*>* region : *country->getLGU())
		{
			if (!filterType->meetsFilter(region->accessData(), typeGU, undefined))
			{
				for (TableItem<wstring, GroundUnit*>* district : *region->accessData()->getLGU())
				{
					if (!filterType->meetsFilter(district->accessData(), typeGU, undefined))
					{
						for (TableItem<wstring, GroundUnit*>* town : *district->accessData()->getLGU())
						{
							if (filterType->meetsFilter(town->accessData(), typeGU, undefined) && filterAdherence->meetsFilter(town->accessData(), adherenceCriteria, false))
							{
								nameSort->insert(town->getKey(), town->accessData());
							}
						}
					}
					if (filterAdherence->meetsFilter(district->accessData(), adherenceCriteria, false) && !undefined)
					{
						nameSort->insert(district->getKey(), district->accessData());
					}
				}
			}
			if (filterAdherence->meetsFilter(region->accessData(), adherenceCriteria, false) && !undefined)
			{
				nameSort->insert(region->getKey(), region->accessData());
			} 
		}
		for (TableItem<wstring, GroundUnit*>* groundUnit : *nameSort)
		{
			wcout << groundUnit->getKey() << " : " << groundUnit->accessData()->getName() << endl;
		}
		break;
	case POPULATIONCOUNT:
		populationCriteria = new CriteriaGUPopulationCount();
		populationSort = new UnsortedSequenceTable<wstring, int>();
		for (TableItem<wstring, GroundUnit*>* region : *country->getLGU())
		{
			if (!filterType->meetsFilter(region->accessData(), typeGU, undefined))
			{
				for (TableItem<wstring, GroundUnit*>* district : *region->accessData()->getLGU())
				{
					if (!filterType->meetsFilter(district->accessData(), typeGU, undefined))
					{
						for (TableItem<wstring, GroundUnit*>* town : *district->accessData()->getLGU())
						{
							if (filterType->meetsFilter(town->accessData(), typeGU, undefined) && filterAdherence->meetsFilter(town->accessData(), adherenceCriteria, false))
							{
								populationSort->insert(town->getKey(), populationCriteria->rate(town->accessData(), L""));
							}
						}
					}
					if (filterAdherence->meetsFilter(district->accessData(), adherenceCriteria, false) && !undefined)
					{
						populationSort->insert(district->getKey(), populationCriteria->rate(district->accessData(), L""));
					}
				}
			}
			if (filterAdherence->meetsFilter(region->accessData(), adherenceCriteria, false) && !undefined)
			{
				populationSort->insert(region->getKey(), populationCriteria->rate(region->accessData(), L""));
			}
		}
		sortPopulation = new QuickSort<wstring, int>();
		sortPopulation->sort(*populationSort, asc);
		for (TableItem<wstring, int>* groundUnit : *populationSort)
		{
			wcout << groundUnit->getKey() << ": " << groundUnit->accessData() << endl;
		}
		break;
	case BUILDEDUP:
		buildedUpCriteria = new CriteriaGUBuildedUp();
		buildedUpSort = new UnsortedSequenceTable<wstring, double>();
		for (TableItem<wstring, GroundUnit*>* region : *country->getLGU())
		{
			if (!filterType->meetsFilter(region->accessData(), typeGU, undefined))
			{
				for (TableItem<wstring, GroundUnit*>* district : *region->accessData()->getLGU())
				{
					if (!filterType->meetsFilter(district->accessData(), typeGU, undefined))
					{
						for (TableItem<wstring, GroundUnit*>* town : *district->accessData()->getLGU())
						{
							if (filterType->meetsFilter(town->accessData(), typeGU, undefined) && filterAdherence->meetsFilter(town->accessData(), adherenceCriteria, false))
							{
								buildedUpSort->insert(town->getKey(), buildedUpCriteria->rate(town->accessData(), L""));
							}
						}
					}
					if (filterAdherence->meetsFilter(district->accessData(), adherenceCriteria, false) && !undefined)
					{
						buildedUpSort->insert(district->getKey(), buildedUpCriteria->rate(district->accessData(), L""));
					}
				}
			}
			if (filterAdherence->meetsFilter(region->accessData(), adherenceCriteria, false) && !undefined)
			{
				buildedUpSort->insert(region->getKey(), buildedUpCriteria->rate(region->accessData(), L""));
			}
		}
		sortBuildedUp = new QuickSort<wstring, double>();
		sortBuildedUp->sort(*buildedUpSort, asc);
		for (TableItem<wstring, double>* groundUnit : *buildedUpSort)
		{
			wcout << groundUnit->getKey() << ": " << groundUnit->accessData() << endl;
		}
		break;
	default:
		break;
	}
}

inline void UserInterface::operationSelect()
{
	switch (selection)
	{
	case 1:
		selectionOp = 0;
		wcout << L"Vyber si filter: / Choose filter: " << endl;
		wcout << L"1: N�zov / Name" << endl;
		wcout << L"2: Po�et obyvate�ov / Population Count" << endl;
		wcout << L"3: Zastavanos� / Builded Up" << endl << endl;

		do
		{
			wcout << L"Tvoj vyber / your choice: ";
			wcin >> selectionOp;
		} while (selectionOp > 3 || selectionOp <= 0);

		wcout << endl;

		switch (selectionOp)
		{
		case 1:
			nameOp = L" ";
			wcout << L"Zadaj n�zov / Enter name: ";
			wcin.ignore();
			getline(wcin, nameOp);
			wcout << endl;
			operation1(FILTERNAME);
			break;
		case 2:
			populationOp = 0;
			wcout << L"Zadaj najni��� po�et obyvate�ov / Enter lowest population count: ";
			do
			{
				wcin >> populationOp;
			} while (populationOp <= 0);
			wcout << endl;
			operation1(FILTERPOPULATION);
			break;
		case 3:
			buildedUpOp = 0.0;
			wcout << L"Zadaj najni��iu zastavanos� / Enter lowest builded up percentage: ";
			do
			{
				wcin >> buildedUpOp;
			} while (buildedUpOp <= 0.0);
			wcout << endl;
			operation1(FILTERBUILDEDUP);
			break;
		default:
			break;
		}
		break;
	case 2:
		selectionOp = 0;
		selectionDir = 0;

		wcout << L"Vyber si krit�rium: / Choose criteria: " << endl;
		wcout << L"1: N�zov / Name" << endl;
		wcout << L"2: Po�et obyvate�ov / Population Count" << endl;
		wcout << L"3: Zastavanos� / Builded Up" << endl << endl;

		do
		{
			wcout << L"Tvoj vyber / your choice: ";
			wcin >> selectionOp;
		} while (selectionOp > 3 || selectionOp <= 0);

		wcout << L"1: Vzostupne / Ascending" << endl;
		wcout << L"2: Zostupne / Descending" << endl;

		do
		{
			wcout << L"Tvoj vyber / your choice: ";
			wcin >> selectionDir;
		} while (selectionDir > 2 || selectionDir <= 0);

		ascending = false;
		if (selectionDir <= 1)
		{
			ascending = true;
		}
		else
		{
			ascending = false;
		}

		wcout << endl;

		switch (selectionOp)
		{
		case 1:
			operation2(CriteriaType::NAME, ascending);
			break;
		case 2:
			operation2(CriteriaType::POPULATIONCOUNT, ascending);
			break;
		case 3:
			operation2(CriteriaType::BUILDEDUP, ascending);
			break;
		default:
			break;
		}
		break;
	case 3:
		selectionOp = 0;
		selectionType = 0;

		wcout << L"Vyber si typ �zemnej jednotky: / Choose ground unit type: " << endl;
		wcout << L"1: Obec / Town" << endl;
		wcout << L"2: Okres / District" << endl;
		wcout << L"3: Kraj / Region" << endl;
		wcout << L"4: Nedefinovan� / Undefined " << endl << endl;

		do
		{
			wcout << L"Tvoj vyber / your choice: ";
			wcin >> selectionType;
		} while (selectionType > 4 || selectionType <= 0);

		wcout << L"Vyber si filter: / Choose filter: " << endl;
		wcout << L"1: N�zov / Name" << endl;
		wcout << L"2: Po�et obyvate�ov / Population Count" << endl;
		wcout << L"3: Zastavanos� / Builded Up" << endl << endl;

		do
		{
			wcout << L"Tvoj vyber / your choice: ";
			wcin >> selectionOp;
		} while (selectionOp > 3 || selectionOp <= 0);

		wcout << endl;

		switch (selectionOp)
		{
		case 1:
			nameOp = L" ";
			wcout << L"Zadaj n�zov / Enter name: ";
			wcin.ignore();
			getline(wcin, nameOp);
			wcout << endl;
			operation3(FILTERNAME, static_cast<GroundUnitType>(selectionType - 1));
			break;
		case 2:
			populationOp = 0;
			wcout << L"Zadaj najni��� po�et obyvate�ov / Enter lowest population count: ";
			do
			{
				wcin >> populationOp;
			} while (populationOp <= 0);
			wcout << endl;
			operation3(FILTERPOPULATION, static_cast<GroundUnitType>(selectionType - 1));
			break;
		case 3:
			buildedUpOp = 0.0;
			wcout << L"Zadaj najni��iu zastavanos� / Enter lowest builded up percentage: ";
			do
			{
				wcin >> buildedUpOp;
			} while (buildedUpOp <= 0.0);
			wcout << endl;
			operation3(FILTERBUILDEDUP, static_cast<GroundUnitType>(selectionType - 1));
			break;
		default:
			break;
		}
	case 4:
		selectionOp = 0;
		selectionType = 0;
		selectionDir = 0;

		wcout << L"Vyber si typ �zemnej jednotky: / Choose ground unit type: " << endl;
		wcout << L"1: Obec / Town" << endl;
		wcout << L"2: Okres / District" << endl;
		wcout << L"3: Kraj / Region" << endl; 
		wcout << L"4: Nedefinovan� / Undefined "<< endl << endl;

		do
		{
			wcout << L"Tvoj vyber / your choice: ";
			wcin >> selectionType;
		} while (selectionType > 4 || selectionType <= 0);

		wcout << L"Vyber si krit�rium: / Choose criteria: " << endl;
		wcout << L"1: N�zov / Name" << endl;
		wcout << L"2: Po�et obyvate�ov / Population Count" << endl;
		wcout << L"3: Zastavanos� / Builded Up" << endl << endl;

		do
		{
			wcout << L"Tvoj vyber / your choice: ";
			wcin >> selectionOp;
		} while (selectionOp > 3 || selectionOp <= 0);

		wcout << L"1: Vzostupne / Ascending" << endl;
		wcout << L"2: Zostupne / Descending" << endl;

		do
		{
			wcout << L"Tvoj vyber / your choice: ";
			wcin >> selectionDir;
		} while (selectionDir > 2 || selectionDir <= 0);

		ascending = false;
		if (selectionDir <= 1)
		{
			ascending = true;
		}
		else
		{
			ascending = false;
		}

		wcout << endl;

		switch (selectionOp)
		{
		case 1:
			operation4(CriteriaType::NAME, static_cast<GroundUnitType>(selectionType - 1), ascending);
			break;
		case 2:
			operation4(CriteriaType::POPULATIONCOUNT, static_cast<GroundUnitType>(selectionType - 1), ascending);
			break;
		case 3:
			operation4(CriteriaType::BUILDEDUP, static_cast<GroundUnitType>(selectionType - 1), ascending);
			break;
		default:
			break;
		}
		break;
	case 5:

		break;
	case 6:

		break;
	case 7:

		break;
	case 8:
		break;
	case 9:
		break;
	case 0:
		exit(0);
		break;
	default:
		break;
	}
}

inline GroundUnit* UserInterface::getCountry()
{
	return country;
}