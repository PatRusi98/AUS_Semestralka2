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
#include <cstdlib>

using namespace std;
using namespace structures;

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
	void rerun(bool wcinIgnore);
	void operationSelect();

	GroundUnit* getCountry();

private:
	Input* input;
	int selectionType;
	int selection;
	int selectionDir;
	wstring selectionAdherence;
	GroundUnit* country;
	bool foundOp;
	bool ascending;
	int selectionOp;
	wstring nameOp;
	int populationOp;
	double buildedUpOp;
	bool wcinIgnore;
};

UserInterface::UserInterface(string file2, string file1)
{
	input = new Input();
	country = new GroundUnit(GroundUnitType::COUNTRY, L"Slovensko", NULL);
	input->loadGroundUnit(file2, country);
	input->loadPopulation(file1, country);
	selectionType = 0;
	selection = 0;
	selectionDir = 0;
	wcinIgnore = false;
	foundOp = false;
	ascending = false;
	selectionOp = 0;
	nameOp = L"";
	populationOp = 0;
	buildedUpOp = 0.0;
	wcinIgnore = false;
}

UserInterface::~UserInterface()
{
	delete input;
	input = nullptr;
	delete country;
	country = nullptr;
}

inline void UserInterface::menu()
{
	wcout << L"Vyber si operáciu: / Choose operation: " << endl;
	wcout << L"1: Vypísanie informácií o územných jednotkách / Informations about ground units" << endl;
	wcout << L"2: Zoradenie územných jednotiek spåòajúcich daný filter / Sorting ground units by filter" << endl;
	wcout << L"3: Vypísanie informácií o územných jednotkách / Informations about ground units" << endl;
	wcout << L"4: Zoradenie územných jednotiek spåòajúcich daný filter / Sorting ground units by filter" << endl;
	wcout << L"5: Vypísanie informácií o zoskupeniach bytových jednotiek vo zvolenej územnej jednotke / " << endl;
	wcout << L"6: Vypísanie sumárnych informácií o poètoch bytových jednotiek vo zvolených územných jednotkách / " << endl;
	wcout << L"7: Zoradenie územných jednotiek pod¾a sumárneho poètu zvolených bytových jednotiek / " << endl;
	wcout << L"8: Vypísanie výberových kritérií pre zoskupenia bytových jednotiek vo zvolených územných jednotkách / " << endl;
	wcout << L"9: Vypísanie výberových kritérií pre územné jednotky na základe zvolených zoskupení bytových jednotiek / " << endl;
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
	CriteriaGUName* nameCriteria = new CriteriaGUName();
	FilterGUName* filterName = new FilterGUName();
	CriteriaGUPopulationCount* populationCriteria = new CriteriaGUPopulationCount();
	FilterGUPopulation* filterPopulation = new FilterGUPopulation();
	CriteriaGUBuildedUp* buildedUpCriteria = new CriteriaGUBuildedUp();
	FilterGUBuildedUp* filterBuildedUp = new FilterGUBuildedUp();

	switch (type)
	{
	case FILTERNAME:

		for (TableItem<wstring, GroundUnit*>* region : *country->getLGU())
		{
			for (TableItem<wstring, GroundUnit*>* district : *region->accessData()->getLGU())
			{
				for (TableItem<wstring, GroundUnit*>* town : *district->accessData()->getLGU())
				{
					if (filterName->meetsFilter(town->accessData(), nameOp, false))
					{
						wcout << town->accessData()->getName() << ", " << district->accessData()->getName() << ", " << region->accessData()->getName() << ", " << country->getName() << endl;
						wcout << L"Poèet obyvate¾ov: " << town->accessData()->getPopulation(PopulationType::POPULATION) << endl;
						wcout << L"Poèet preproduktívnych obyvate¾ov: " << town->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
						wcout << L"Poèet produktívnych obyvate¾ov: " << town->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
						wcout << L"Poèet poproduktívnych obyvate¾ov: " << town->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
						wcout << L"Zastavanos: " << (town->accessData()->getBuiltUpArea() / town->accessData()->getTotalArea()) * 100 << L" %" << endl;
						wcout << L"Celková výmera: " << town->accessData()->getTotalArea() << L" m2" << endl;
						wcout << L"Zastavaná plocha: " << town->accessData()->getBuiltUpArea() << L" m2" << endl << endl;
					}
				}
			}
		}

		break;

	case FILTERPOPULATION:

		for (TableItem<wstring, GroundUnit*>* region : *country->getLGU())
		{
			for (TableItem<wstring, GroundUnit*>* district : *region->accessData()->getLGU())
			{
				for (TableItem<wstring, GroundUnit*>* town : *district->accessData()->getLGU())
				{
					if (filterPopulation->meetsFilter(town->accessData(), populationOp, false))
					{
						wcout << town->accessData()->getName() << ", " << district->accessData()->getName() << ", " << region->accessData()->getName() << ", " << country->getName() << endl;
						wcout << L"Poèet obyvate¾ov: " << town->accessData()->getPopulation(PopulationType::POPULATION) << endl;
						wcout << L"Poèet preproduktívnych obyvate¾ov: " << town->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
						wcout << L"Poèet produktívnych obyvate¾ov: " << town->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
						wcout << L"Poèet poproduktívnych obyvate¾ov: " << town->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
						wcout << L"Zastavanos: " << (town->accessData()->getBuiltUpArea() / town->accessData()->getTotalArea()) * 100 << L" %" << endl;
						wcout << L"Celková výmera: " << town->accessData()->getTotalArea() << L" m2" << endl;
						wcout << L"Zastavaná plocha: " << town->accessData()->getBuiltUpArea() << L" m2" << endl << endl;
					}
				}
			}
		}

		break;

	case FILTERBUILDEDUP:

		for (TableItem<wstring, GroundUnit*>* region : *country->getLGU())
		{
			for (TableItem<wstring, GroundUnit*>* district : *region->accessData()->getLGU())
			{
				for (TableItem<wstring, GroundUnit*>* town : *district->accessData()->getLGU())
				{
					if (filterBuildedUp->meetsFilter(town->accessData(), buildedUpOp, false))
					{
						wcout << town->accessData()->getName() << ", " << district->accessData()->getName() << ", " << region->accessData()->getName() << ", " << country->getName() << endl;
						wcout << L"Poèet obyvate¾ov: " << town->accessData()->getPopulation(PopulationType::POPULATION) << endl;
						wcout << L"Poèet preproduktívnych obyvate¾ov: " << town->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
						wcout << L"Poèet produktívnych obyvate¾ov: " << town->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
						wcout << L"Poèet poproduktívnych obyvate¾ov: " << town->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
						wcout << L"Zastavanos: " << (town->accessData()->getBuiltUpArea() / town->accessData()->getTotalArea()) * 100 << L" %" << endl;
						wcout << L"Celková výmera: " << town->accessData()->getTotalArea() << L" m2" << endl;
						wcout << L"Zastavaná plocha: " << town->accessData()->getBuiltUpArea() << L" m2" << endl << endl;
					}
				}
			}
		}

		break;

	default:
		break;
	}

	delete nameCriteria;
	nameCriteria = nullptr;
	delete filterName;
	filterName = nullptr;
	delete populationCriteria;
	populationCriteria = nullptr;
	delete filterPopulation;
	filterPopulation = nullptr;
	delete buildedUpCriteria;
	buildedUpCriteria = nullptr;
	delete filterBuildedUp;
	filterBuildedUp = nullptr;
}

inline void UserInterface::operation2(CriteriaType type, bool asc)
{
	CriteriaGUName* nameCriteria = new CriteriaGUName();
	UnsortedSequenceTable<wstring, wstring>* nameSort = new UnsortedSequenceTable<wstring, wstring>();
	CriteriaGUPopulationCount* populationCriteria = new CriteriaGUPopulationCount();
	UnsortedSequenceTable<wstring, int>* populationSort = new UnsortedSequenceTable<wstring, int>();
	CriteriaGUBuildedUp* buildedUpCriteria = new CriteriaGUBuildedUp();
	UnsortedSequenceTable<wstring, double>* buildedUpSort = new UnsortedSequenceTable<wstring, double>();
	QuickSort<wstring, wstring>* sortName = new QuickSort<wstring, wstring>();
	QuickSort<wstring, int>* sortPopulation = new QuickSort<wstring, int>();
	QuickSort<wstring, double>* sortBuildedUp = new QuickSort<wstring, double>();


	switch (type)
	{
	case NAME:

		for (TableItem<wstring, GroundUnit*>* region : *country->getLGU())
		{
			for (TableItem<wstring, GroundUnit*>* district : *region->accessData()->getLGU())
			{
				for (TableItem<wstring, GroundUnit*>* town : *district->accessData()->getLGU())
				{
					nameSort->insert(town->getKey(), nameCriteria->rate(town->accessData(), L""));
				}
			}
		}

		sortName->sort(*nameSort, asc);

		for (TableItem<wstring, wstring>* groundUnit : *nameSort)
		{
			wcout << groundUnit->getKey() << " : " << groundUnit->accessData() << endl;
		}

		break;

	case POPULATIONCOUNT:

		for (TableItem<wstring, GroundUnit*>* region : *country->getLGU())
		{
			for (TableItem<wstring, GroundUnit*>* district : *region->accessData()->getLGU())
			{
				for (TableItem<wstring, GroundUnit*>* town : *district->accessData()->getLGU())
				{
					populationSort->insert(town->getKey(), populationCriteria->rate(town->accessData(), L""));
				}
			}
		}

		sortPopulation->sort(*populationSort, asc);

		for (TableItem<wstring, int>* groundUnit : *populationSort)
		{
			wcout << groundUnit->getKey() << ": " << groundUnit->accessData() << endl;
		}

		break;

	case BUILDEDUP:

		for (TableItem<wstring, GroundUnit*>* region : *country->getLGU())
		{
			for (TableItem<wstring, GroundUnit*>* district : *region->accessData()->getLGU())
			{
				for (TableItem<wstring, GroundUnit*>* town : *district->accessData()->getLGU())
				{
					buildedUpSort->insert(town->getKey(), buildedUpCriteria->rate(town->accessData(), L""));
				}
			}
		}

		sortBuildedUp->sort(*buildedUpSort, asc);

		for (TableItem<wstring, double>* groundUnit : *buildedUpSort)
		{
			wcout << groundUnit->getKey() << ": " << groundUnit->accessData() << endl;
		}

		break;

	default:
		break;
	}

	/*for (TableItem<wstring, GroundUnit*>* groundUnit : *nameSort)
	{
		if (groundUnit->accessData() != nullptr)
		{
			delete groundUnit->accessData();
			groundUnit = nullptr;
		}
	}

	for (TableItem<wstring, int>* groundUnit : *populationSort)
	{
		if (groundUnit != nullptr)
		{
			delete groundUnit;
			groundUnit = nullptr;
		}
	}

	for (TableItem<wstring, double>* groundUnit : *buildedUpSort)
	{
		if (groundUnit != nullptr)
		{
			delete groundUnit;
			groundUnit = nullptr;
		}
	}*/

	delete nameSort;
	nameSort = nullptr;
	delete populationCriteria;
	populationCriteria = nullptr;
	delete populationSort;
	populationSort = nullptr;
	delete sortPopulation;
	sortPopulation = nullptr;
	delete buildedUpCriteria;
	buildedUpCriteria = nullptr;
	delete buildedUpSort;
	buildedUpSort = nullptr;
	delete sortBuildedUp;
	sortBuildedUp = nullptr;
}

inline void UserInterface::operation3(FilterType type, GroundUnitType typeGU)
{
	FilterGUName* filterName = new FilterGUName();
	FilterGUType* filterType = new FilterGUType();
	FilterGUAdherence* filterAdherence = new FilterGUAdherence();
	CriteriaGUAdherence* adherenceCriteria = new CriteriaGUAdherence();
	FilterGUPopulation* filterPopulation = new FilterGUPopulation();
	FilterGUBuildedUp* filterBuildedUp = new FilterGUBuildedUp();

	bool undefined = false;
	if (typeGU == GroundUnitType::UNDEFINED)
	{
		undefined = true;
	}

	switch (type)
	{
	case FILTERNAME:

		for (TableItem<wstring, GroundUnit*>* region : *country->getLGU())
		{
			if (!filterType->meetsFilter(region->accessData(), typeGU, undefined) || undefined)
			{
				for (TableItem<wstring, GroundUnit*>* district : *region->accessData()->getLGU())
				{
					if (!filterType->meetsFilter(district->accessData(), typeGU, undefined) || undefined)
					{
						for (TableItem<wstring, GroundUnit*>* town : *district->accessData()->getLGU())
						{
							if (filterName->meetsFilter(town->accessData(), nameOp, false) && filterAdherence->meetsFilter(town->accessData(), adherenceCriteria, false) && (filterType->meetsFilter(town->accessData(), typeGU, undefined) || undefined) && (town->accessData()->getHGU()->getName() == selectionAdherence || selectionAdherence == L""))
							{
								wcout << town->accessData()->getName() << ", " << district->accessData()->getName() << ", " << region->accessData()->getName() << ", " << country->getName() << endl;
								wcout << L"Poèet obyvate¾ov: " << town->accessData()->getPopulation(PopulationType::POPULATION) << endl;
								wcout << L"Poèet preproduktívnych obyvate¾ov: " << town->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
								wcout << L"Poèet produktívnych obyvate¾ov: " << town->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
								wcout << L"Poèet poproduktívnych obyvate¾ov: " << town->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
								wcout << L"Zastavanos: " << (town->accessData()->getBuiltUpArea() / town->accessData()->getTotalArea()) * 100 << L" %" << endl;
								wcout << L"Celková výmera: " << town->accessData()->getTotalArea() << L" m2" << endl;
								wcout << L"Zastavaná plocha: " << town->accessData()->getBuiltUpArea() << L" m2" << endl << endl;
							}
						}
					}
					if (filterName->meetsFilter(district->accessData(), nameOp, false) && filterAdherence->meetsFilter(district->accessData(), adherenceCriteria, false) && (filterType->meetsFilter(district->accessData(), typeGU, undefined) || undefined) && (district->accessData()->getHGU()->getName() == selectionAdherence || selectionAdherence == L""))
					{
						wcout << district->accessData()->getName() << ", " << region->accessData()->getName() << ", " << country->getName() << endl;
						wcout << L"Poèet obyvate¾ov: " << district->accessData()->getPopulation(PopulationType::POPULATION) << endl;
						wcout << L"Poèet preproduktívnych obyvate¾ov: " << district->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
						wcout << L"Poèet produktívnych obyvate¾ov: " << district->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
						wcout << L"Poèet poproduktívnych obyvate¾ov: " << district->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
						wcout << L"Zastavanos: " << (district->accessData()->getBuiltUpArea() / district->accessData()->getTotalArea()) * 100 << L" %" << endl;
						wcout << L"Celková výmera: " << district->accessData()->getTotalArea() << L" m2" << endl;
						wcout << L"Zastavaná plocha: " << district->accessData()->getBuiltUpArea() << L" m2" << endl << endl;
					}
				}
			}
			if (filterName->meetsFilter(region->accessData(), nameOp, false) && filterAdherence->meetsFilter(region->accessData(), adherenceCriteria, false) && (filterType->meetsFilter(region->accessData(), typeGU, undefined) || undefined) && (region->accessData()->getHGU()->getName() == selectionAdherence || selectionAdherence == L""))
			{
				wcout << region->accessData()->getName() << ", " << country->getName() << endl;
				wcout << L"Poèet obyvate¾ov: " << region->accessData()->getPopulation(PopulationType::POPULATION) << endl;
				wcout << L"Poèet preproduktívnych obyvate¾ov: " << region->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
				wcout << L"Poèet produktívnych obyvate¾ov: " << region->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
				wcout << L"Poèet poproduktívnych obyvate¾ov: " << region->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
				wcout << L"Zastavanos: " << (region->accessData()->getBuiltUpArea() / region->accessData()->getTotalArea()) * 100 << L" %" << endl;
				wcout << L"Celková výmera: " << region->accessData()->getTotalArea() << L" m2" << endl;
				wcout << L"Zastavaná plocha: " << region->accessData()->getBuiltUpArea() << L" m2" << endl << endl;
			}
		}

		break;

	case FILTERPOPULATION:

		for (TableItem<wstring, GroundUnit*>* region : *country->getLGU())
		{
			if (!filterType->meetsFilter(region->accessData(), typeGU, undefined) || undefined)
			{
				for (TableItem<wstring, GroundUnit*>* district : *region->accessData()->getLGU())
				{
					if (!filterType->meetsFilter(district->accessData(), typeGU, undefined) || undefined)
					{
						for (TableItem<wstring, GroundUnit*>* town : *district->accessData()->getLGU())
						{
							if (filterPopulation->meetsFilter(town->accessData(), populationOp, false) && filterAdherence->meetsFilter(town->accessData(), adherenceCriteria, false) && (filterType->meetsFilter(town->accessData(), typeGU, undefined) || undefined) && (town->accessData()->getHGU()->getName() == selectionAdherence || selectionAdherence == L""))
							{
								wcout << town->accessData()->getName() << ", " << district->accessData()->getName() << ", " << region->accessData()->getName() << ", " << country->getName() << endl;
								wcout << L"Poèet obyvate¾ov: " << town->accessData()->getPopulation(PopulationType::POPULATION) << endl;
								wcout << L"Poèet preproduktívnych obyvate¾ov: " << town->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
								wcout << L"Poèet produktívnych obyvate¾ov: " << town->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
								wcout << L"Poèet poproduktívnych obyvate¾ov: " << town->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
								wcout << L"Zastavanos: " << (town->accessData()->getBuiltUpArea() / town->accessData()->getTotalArea()) * 100 << L" %" << endl;
								wcout << L"Celková výmera: " << town->accessData()->getTotalArea() << L" m2" << endl;
								wcout << L"Zastavaná plocha: " << town->accessData()->getBuiltUpArea() << L" m2" << endl << endl;
							}
						}
					}
					if (filterPopulation->meetsFilter(district->accessData(), populationOp, false) && filterAdherence->meetsFilter(district->accessData(), adherenceCriteria, false) && (filterType->meetsFilter(district->accessData(), typeGU, undefined) || undefined) && (district->accessData()->getHGU()->getName() == selectionAdherence || selectionAdherence == L""))
					{
						wcout << district->accessData()->getName() << ", " << region->accessData()->getName() << ", " << country->getName() << endl;
						wcout << L"Poèet obyvate¾ov: " << district->accessData()->getPopulation(PopulationType::POPULATION) << endl;
						wcout << L"Poèet preproduktívnych obyvate¾ov: " << district->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
						wcout << L"Poèet produktívnych obyvate¾ov: " << district->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
						wcout << L"Poèet poproduktívnych obyvate¾ov: " << district->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
						wcout << L"Zastavanos: " << (district->accessData()->getBuiltUpArea() / district->accessData()->getTotalArea()) * 100 << L" %" << endl;
						wcout << L"Celková výmera: " << district->accessData()->getTotalArea() << L" m2" << endl;
						wcout << L"Zastavaná plocha: " << district->accessData()->getBuiltUpArea() << L" m2" << endl << endl;
					}
				}
			}
			if (filterPopulation->meetsFilter(region->accessData(), populationOp, false) && filterAdherence->meetsFilter(region->accessData(), adherenceCriteria, false) && (filterType->meetsFilter(region->accessData(), typeGU, undefined) || undefined) && (region->accessData()->getHGU()->getName() == selectionAdherence || selectionAdherence == L""))
			{
				wcout << region->accessData()->getName() << ", " << country->getName() << endl;
				wcout << L"Poèet obyvate¾ov: " << region->accessData()->getPopulation(PopulationType::POPULATION) << endl;
				wcout << L"Poèet preproduktívnych obyvate¾ov: " << region->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
				wcout << L"Poèet produktívnych obyvate¾ov: " << region->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
				wcout << L"Poèet poproduktívnych obyvate¾ov: " << region->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
				wcout << L"Zastavanos: " << (region->accessData()->getBuiltUpArea() / region->accessData()->getTotalArea()) * 100 << L" %" << endl;
				wcout << L"Celková výmera: " << region->accessData()->getTotalArea() << L" m2" << endl;
				wcout << L"Zastavaná plocha: " << region->accessData()->getBuiltUpArea() << L" m2" << endl << endl;
			}
		}

		break;

	case FILTERBUILDEDUP:

		for (TableItem<wstring, GroundUnit*>* region : *country->getLGU())
		{
			if (!filterType->meetsFilter(region->accessData(), typeGU, undefined) || undefined)
			{
				for (TableItem<wstring, GroundUnit*>* district : *region->accessData()->getLGU())
				{
					if (!filterType->meetsFilter(district->accessData(), typeGU, undefined) || undefined)
					{
						for (TableItem<wstring, GroundUnit*>* town : *district->accessData()->getLGU())
						{
							if (filterBuildedUp->meetsFilter(town->accessData(), buildedUpOp, false) && filterAdherence->meetsFilter(town->accessData(), adherenceCriteria, false) && (filterType->meetsFilter(town->accessData(), typeGU, undefined) || undefined) && (town->accessData()->getHGU()->getName() == selectionAdherence || selectionAdherence == L""))
							{
								wcout << town->accessData()->getName() << ", " << district->accessData()->getName() << ", " << region->accessData()->getName() << ", " << country->getName() << endl;
								wcout << L"Poèet obyvate¾ov: " << town->accessData()->getPopulation(PopulationType::POPULATION) << endl;
								wcout << L"Poèet preproduktívnych obyvate¾ov: " << town->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
								wcout << L"Poèet produktívnych obyvate¾ov: " << town->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
								wcout << L"Poèet poproduktívnych obyvate¾ov: " << town->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
								wcout << L"Zastavanos: " << (town->accessData()->getBuiltUpArea() / town->accessData()->getTotalArea()) * 100 << L" %" << endl;
								wcout << L"Celková výmera: " << town->accessData()->getTotalArea() << L" m2" << endl;
								wcout << L"Zastavaná plocha: " << town->accessData()->getBuiltUpArea() << L" m2" << endl << endl;
							}
						}
					}
					if (filterBuildedUp->meetsFilter(district->accessData(), buildedUpOp, false) && filterAdherence->meetsFilter(district->accessData(), adherenceCriteria, false) && (filterType->meetsFilter(district->accessData(), typeGU, undefined) || undefined) && (district->accessData()->getHGU()->getName() == selectionAdherence || selectionAdherence == L""))
					{
						wcout << district->accessData()->getName() << ", " << region->accessData()->getName() << ", " << country->getName() << endl;
						wcout << L"Poèet obyvate¾ov: " << district->accessData()->getPopulation(PopulationType::POPULATION) << endl;
						wcout << L"Poèet preproduktívnych obyvate¾ov: " << district->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
						wcout << L"Poèet produktívnych obyvate¾ov: " << district->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
						wcout << L"Poèet poproduktívnych obyvate¾ov: " << district->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
						wcout << L"Zastavanos: " << (district->accessData()->getBuiltUpArea() / district->accessData()->getTotalArea()) * 100 << L" %" << endl;
						wcout << L"Celková výmera: " << district->accessData()->getTotalArea() << L" m2" << endl;
						wcout << L"Zastavaná plocha: " << district->accessData()->getBuiltUpArea() << L" m2" << endl << endl;
					}
				}
			}
			if (filterBuildedUp->meetsFilter(region->accessData(), buildedUpOp, false) && filterAdherence->meetsFilter(region->accessData(), adherenceCriteria, false) && (filterType->meetsFilter(region->accessData(), typeGU, undefined) || undefined) && (region->accessData()->getHGU()->getName() == selectionAdherence || selectionAdherence == L""))
			{
				wcout << region->accessData()->getName() << ", " << country->getName() << endl;
				wcout << L"Poèet obyvate¾ov: " << region->accessData()->getPopulation(PopulationType::POPULATION) << endl;
				wcout << L"Poèet preproduktívnych obyvate¾ov: " << region->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
				wcout << L"Poèet produktívnych obyvate¾ov: " << region->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
				wcout << L"Poèet poproduktívnych obyvate¾ov: " << region->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
				wcout << L"Zastavanos: " << (region->accessData()->getBuiltUpArea() / region->accessData()->getTotalArea()) * 100 << L" %" << endl;
				wcout << L"Celková výmera: " << region->accessData()->getTotalArea() << L" m2" << endl;
				wcout << L"Zastavaná plocha: " << region->accessData()->getBuiltUpArea() << L" m2" << endl << endl;
			}
		}

		break;

	default:
		break;
	}

	delete filterName;
	filterName = nullptr;
	delete filterType;
	filterType = nullptr;
	delete filterAdherence;
	filterAdherence = nullptr;
	delete adherenceCriteria;
	adherenceCriteria = nullptr;
	delete filterPopulation;
	filterPopulation = nullptr;
	delete filterBuildedUp;
	filterBuildedUp = nullptr;
}

inline void UserInterface::operation4(CriteriaType type, GroundUnitType typeGU, bool asc)
{
	CriteriaGUName* nameCriteria = new CriteriaGUName();
	UnsortedSequenceTable<wstring, wstring>* nameSort = new UnsortedSequenceTable<wstring, wstring>();
	CriteriaGUPopulationCount* populationCriteria = new CriteriaGUPopulationCount();
	UnsortedSequenceTable<wstring, int>* populationSort = new UnsortedSequenceTable<wstring, int>();
	CriteriaGUBuildedUp* buildedUpCriteria = new CriteriaGUBuildedUp();
	UnsortedSequenceTable<wstring, double>* buildedUpSort = new UnsortedSequenceTable<wstring, double>();
	FilterGUType* filterType = new FilterGUType();
	FilterGUAdherence* filterAdherence = new FilterGUAdherence();
	CriteriaGUAdherence* adherenceCriteria = new CriteriaGUAdherence();
	QuickSort<wstring, wstring>* sortName = new QuickSort<wstring, wstring>();
	QuickSort<wstring, int>* sortPopulation = new QuickSort<wstring, int>();
	QuickSort<wstring, double>* sortBuildedUp = new QuickSort<wstring, double>();

	bool undefined = false;
	if (typeGU == GroundUnitType::UNDEFINED)
	{
		undefined = true;
	}

	switch (type)
	{
	case NAME:

		for (TableItem<wstring, GroundUnit*>* region : *country->getLGU())
		{
			if (!filterType->meetsFilter(region->accessData(), typeGU, undefined) || undefined)
			{
				for (TableItem<wstring, GroundUnit*>* district : *region->accessData()->getLGU())
				{
					if (!filterType->meetsFilter(district->accessData(), typeGU, undefined) || undefined)
					{
						for (TableItem<wstring, GroundUnit*>* town : *district->accessData()->getLGU())
						{
							if ((filterType->meetsFilter(town->accessData(), typeGU, undefined) || undefined) && filterAdherence->meetsFilter(town->accessData(), adherenceCriteria, false) && (town->accessData()->getHGU()->getName() == selectionAdherence || selectionAdherence == L""))
							{
								nameSort->insert(town->getKey(), nameCriteria->rate(town->accessData(), L""));
							}
						}
					}
					if ((filterType->meetsFilter(district->accessData(), typeGU, undefined) || undefined) && filterAdherence->meetsFilter(district->accessData(), adherenceCriteria, false) && !undefined && (district->accessData()->getHGU()->getName() == selectionAdherence || selectionAdherence == L""))
					{
						nameSort->insert(district->getKey(), nameCriteria->rate(district->accessData(), L""));
					}
				}
			}
			if ((filterType->meetsFilter(region->accessData(), typeGU, undefined) || undefined) && filterAdherence->meetsFilter(region->accessData(), adherenceCriteria, false) && !undefined && (region->accessData()->getHGU()->getName() == selectionAdherence || selectionAdherence == L""))
			{
				nameSort->insert(region->getKey(), nameCriteria->rate(region->accessData(), L""));
			} 
		}

		sortName->sort(*nameSort, asc);

		for (TableItem<wstring, wstring>* groundUnit : *nameSort)
		{
			wcout << groundUnit->getKey() << " : " << groundUnit->accessData() << endl;
		}

		break;

	case POPULATIONCOUNT:

		for (TableItem<wstring, GroundUnit*>* region : *country->getLGU())
		{
			if (!filterType->meetsFilter(region->accessData(), typeGU, undefined) || undefined)
			{
				for (TableItem<wstring, GroundUnit*>* district : *region->accessData()->getLGU())
				{
					if (!filterType->meetsFilter(district->accessData(), typeGU, undefined) || undefined)
					{
						for (TableItem<wstring, GroundUnit*>* town : *district->accessData()->getLGU())
						{
							if ((filterType->meetsFilter(town->accessData(), typeGU, undefined) || undefined) && filterAdherence->meetsFilter(town->accessData(), adherenceCriteria, false) && (town->accessData()->getHGU()->getName() == selectionAdherence || selectionAdherence == L""))
							{
								populationSort->insert(town->getKey(), populationCriteria->rate(town->accessData(), L""));
							}
						}
					}
					if ((filterType->meetsFilter(district->accessData(), typeGU, undefined) || undefined) && filterAdherence->meetsFilter(district->accessData(), adherenceCriteria, false) && !undefined && (district->accessData()->getHGU()->getName() == selectionAdherence || selectionAdherence == L""))
					{
						populationSort->insert(district->getKey(), populationCriteria->rate(district->accessData(), L""));
					}
				}
			}
			if ((filterType->meetsFilter(region->accessData(), typeGU, undefined) || undefined) && filterAdherence->meetsFilter(region->accessData(), adherenceCriteria, false) && !undefined && (region->accessData()->getHGU()->getName() == selectionAdherence || selectionAdherence == L""))
			{
				populationSort->insert(region->getKey(), populationCriteria->rate(region->accessData(), L""));
			}
		}
		
		sortPopulation->sort(*populationSort, asc);
		for (TableItem<wstring, int>* groundUnit : *populationSort)
		{
			wcout << groundUnit->getKey() << ": " << groundUnit->accessData() << endl;
		}

		break;

	case BUILDEDUP:

		for (TableItem<wstring, GroundUnit*>* region : *country->getLGU())
		{
			if (!filterType->meetsFilter(region->accessData(), typeGU, undefined) || undefined)
			{
				for (TableItem<wstring, GroundUnit*>* district : *region->accessData()->getLGU())
				{
					if (!filterType->meetsFilter(district->accessData(), typeGU, undefined) || undefined)
					{
						for (TableItem<wstring, GroundUnit*>* town : *district->accessData()->getLGU())
						{
							if ((filterType->meetsFilter(town->accessData(), typeGU, undefined) || undefined) && filterAdherence->meetsFilter(town->accessData(), adherenceCriteria, false) && (town->accessData()->getHGU()->getName() == selectionAdherence || selectionAdherence == L""))
							{
								buildedUpSort->insert(town->getKey(), buildedUpCriteria->rate(town->accessData(), L""));
							}
						}
					}
					if ((filterType->meetsFilter(district->accessData(), typeGU, undefined) || undefined) && filterAdherence->meetsFilter(district->accessData(), adherenceCriteria, false) && !undefined && (district->accessData()->getHGU()->getName() == selectionAdherence || selectionAdherence == L""))
					{
						buildedUpSort->insert(district->getKey(), buildedUpCriteria->rate(district->accessData(), L""));
					}
				}
			}
			if ((filterType->meetsFilter(region->accessData(), typeGU, undefined) || undefined) && filterAdherence->meetsFilter(region->accessData(), adherenceCriteria, false) && !undefined && (region->accessData()->getHGU()->getName() == selectionAdherence || selectionAdherence == L""))
			{
				buildedUpSort->insert(region->getKey(), buildedUpCriteria->rate(region->accessData(), L""));
			}
		}
		
		sortBuildedUp->sort(*buildedUpSort, asc);
		for (TableItem<wstring, double>* groundUnit : *buildedUpSort)
		{
			wcout << groundUnit->getKey() << ": " << groundUnit->accessData() << endl;
		}

		break;
	}

	/*for (TableItem<wstring, GroundUnit*>* groundUnit : *nameSort)
	{
		if (groundUnit->accessData() != nullptr)
		{
			delete groundUnit->accessData();
			groundUnit = nullptr;
		}
	}

	for (TableItem<wstring, int>* groundUnit : *populationSort)
	{
		if (groundUnit != nullptr)
		{
			delete groundUnit;
			groundUnit = nullptr;
		}
	}

	for (TableItem<wstring, double>* groundUnit : *buildedUpSort)
	{
		if (groundUnit != nullptr)
		{
			delete groundUnit;
			groundUnit = nullptr;
		}
	}*/

	delete populationCriteria;
	populationCriteria = nullptr;
	delete populationSort;
	populationSort = nullptr;
	delete buildedUpCriteria;
	buildedUpCriteria = nullptr;
	delete buildedUpSort;
	buildedUpSort = nullptr;
	delete nameSort;
	nameSort = nullptr;
	delete filterType;
	filterType = nullptr;
	delete filterAdherence;
	filterAdherence = nullptr;
	delete adherenceCriteria;
	adherenceCriteria = nullptr;
	delete sortPopulation;
	sortPopulation = nullptr;
	delete sortBuildedUp;
	sortBuildedUp = nullptr;
}

inline void UserInterface::operationSelect()
{
	switch (selection)
	{
	case 1:
		selectionOp = 0;
		wcout << L"Vyber si filter: / Choose filter: " << endl;
		wcout << L"1: Názov / Name" << endl;
		wcout << L"2: Poèet obyvate¾ov / Population Count" << endl;
		wcout << L"3: Zastavanos / Builded Up" << endl << endl;

		do
		{
			wcout << L"Tvoj vyber / your choice: ";
			wcin >> selectionOp;
		} while (selectionOp > 3 || selectionOp <= 0);

		wcout << endl;

		switch (selectionOp)
		{
		case 1:
			nameOp = L"";
			wcout << L"Zadaj názov / Enter name: ";
			wcin.ignore();
			getline(wcin, nameOp);
			wcout << endl;
			operation1(FILTERNAME);
			rerun(true);
			break;

		case 2:
			populationOp = 0;
			wcout << L"Zadaj najnižší poèet obyvate¾ov / Enter lowest population count: ";
			do
			{
				wcin >> populationOp;
			} while (populationOp <= 0);
			wcout << endl;
			operation1(FILTERPOPULATION);
			rerun(false);
			break;

		case 3:
			buildedUpOp = 0.0;
			wcout << L"Zadaj najnižšiu zastavanos / Enter lowest builded up percentage: ";
			do
			{
				wcin >> buildedUpOp;
			} while (buildedUpOp <= 0.0);
			wcout << endl;
			operation1(FILTERBUILDEDUP);
			rerun(false);
			break;
		default:
			break;
		}
		break;

	case 2:
		selectionOp = 0;
		selectionDir = 0;

		wcout << L"Vyber si kritérium: / Choose criteria: " << endl;
		wcout << L"1: Názov / Name" << endl;
		wcout << L"2: Poèet obyvate¾ov / Population Count" << endl;
		wcout << L"3: Zastavanos / Builded Up" << endl << endl;

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
			rerun(false);

			break;

		case 2:
			operation2(CriteriaType::POPULATIONCOUNT, ascending);
			rerun(false);

			break;

		case 3:
			operation2(CriteriaType::BUILDEDUP, ascending);
			rerun(false);

			break;

		default:
			break;
		}
		break;

	case 3:
		selectionOp = 0;
		selectionType = 0;
		selectionAdherence = L"";
		nameOp = L"";

		wcout << L"Vyber si typ územnej jednotky: / Choose ground unit type: " << endl;
		wcout << L"1: Obec / Town" << endl;
		wcout << L"2: Okres / District" << endl;
		wcout << L"3: Kraj / Region" << endl;
		wcout << L"4: Štát / Country" << endl;
		wcout << L"5: Nedefinovaný / Undefined " << endl << endl;

		do
		{
			wcout << L"Tvoj vyber / your choice: ";
			wcin >> selectionType;
		} while (selectionType > 5 || selectionType <= 0);

		wcout << L"Zadaj príslušnos (vyššiu územnú jednotku): / Enter adherence (higher ground unit)" << endl;
		wcout << L"(pre nedefinovanú stlaè enter / for undefined press enter)" << endl;
		wcout << L"Tvoj vyber / your choice: ";

		wcin.ignore();
		getline(wcin, selectionAdherence);
		wcout << endl << endl;

		wcout << L"Vyber si filter: / Choose filter: " << endl;
		wcout << L"1: Názov / Name" << endl;
		wcout << L"2: Poèet obyvate¾ov / Population Count" << endl;
		wcout << L"3: Zastavanos / Builded Up" << endl << endl;

		do
		{
			wcout << L"Tvoj vyber / your choice: ";
			wcin >> selectionOp;
		} while (selectionOp > 3 || selectionOp <= 0);

		wcout << endl << endl;

		switch (selectionOp)
		{
		case 1:
			wcout << L"Zadaj názov / Enter name: ";
			do
			{
				getline(wcin, nameOp);
			} while (nameOp == L"");
			wcout << endl;
			operation3(FILTERNAME, static_cast<GroundUnitType>(selectionType - 1));
			rerun(true);

			break;

		case 2:
			populationOp = 0;
			wcout << L"Zadaj najnižší poèet obyvate¾ov / Enter lowest population count: ";
			do
			{
				wcin >> populationOp;
			} while (populationOp <= 0);
			wcout << endl;
			operation3(FILTERPOPULATION, static_cast<GroundUnitType>(selectionType - 1));
			rerun(false);

			break;

		case 3:
			buildedUpOp = 0.0;
			wcout << L"Zadaj najnižšiu zastavanos / Enter lowest builded up percentage: ";
			do
			{
				wcin >> buildedUpOp;
			} while (buildedUpOp <= 0.0);
			wcout << endl;
			operation3(FILTERBUILDEDUP, static_cast<GroundUnitType>(selectionType - 1));
			rerun(false);

			break;

		default:
			break;
		}

	case 4:
		selectionOp = 0;
		selectionType = 0;
		selectionDir = 0;

		wcout << L"Vyber si typ územnej jednotky: / Choose ground unit type: " << endl;
		wcout << L"1: Obec / Town" << endl;
		wcout << L"2: Okres / District" << endl;
		wcout << L"3: Kraj / Region" << endl; 
		wcout << L"4: Štát / Country" << endl;
		wcout << L"5: Nedefinovaný / Undefined "<< endl << endl;

		do
		{
			wcout << L"Tvoj vyber / your choice: ";
			wcin >> selectionType;
		} while (selectionType > 5 || selectionType <= 0);

		wcout << L"Zadaj príslušnos (vyššiu územnú jednotku): / Enter adherence (higher ground unit)" << endl;
		wcout << L"(pre nedefinovanú stlaè enter / for undefined press enter)" << endl;
		wcout << L"Tvoj vyber / your choice: ";

		wcin.ignore();
		getline(wcin, selectionAdherence);
		wcout << endl << endl;

		wcout << L"Vyber si kritérium: / Choose criteria: " << endl;
		wcout << L"1: Názov / Name" << endl;
		wcout << L"2: Poèet obyvate¾ov / Population Count" << endl;
		wcout << L"3: Zastavanos / Builded Up" << endl << endl;

		do
		{
			wcout << L"Tvoj vyber / your choice: ";
			wcin >> selectionOp;
		} while (selectionOp > 3 || selectionOp <= 0);

		wcout << endl;

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
			rerun(true);

			break;

		case 2:
			operation4(CriteriaType::POPULATIONCOUNT, static_cast<GroundUnitType>(selectionType - 1), ascending);
			rerun(true);

			break;

		case 3:
			operation4(CriteriaType::BUILDEDUP, static_cast<GroundUnitType>(selectionType - 1), ascending);
			rerun(true);

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

inline void UserInterface::rerun(bool wcinIgnore)
{
	wcout << L"\nChceš pokraèova? / Do you want to continue? (y/n): ";
	wstring again;
	do
	{
		if (!wcinIgnore)
		{
			wcin.ignore();
		}
		getline(wcin, again);
		if (again == L"y" || again == L"n")
		{
			break;
		}
	} while (true);

	if (again == L"y")
	{
		menu();
	}
	else
	{
		exit(0);
	}
}

inline GroundUnit* UserInterface::getCountry()
{
	return country;
}