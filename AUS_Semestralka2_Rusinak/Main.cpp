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

using namespace std;
using namespace structures;

int selection = 0;
GroundUnit* country = new GroundUnit(GroundUnitType::COUNTRY, L"Slovensko", NULL);
SortedSequenceTable<wstring, GroundUnit*>* nameSort;
UnsortedSequenceTable<wstring, int>* populationSort;
UnsortedSequenceTable<wstring, double>* buildedUpSort;
CriteriaGUName* nameCriteria;
CriteriaGUPopulationCount* populationCriteria;
CriteriaGUBuildedUp* buildedUpCriteria;
QuickSort<wstring, int>* sortPopulation;
QuickSort<wstring, double>* sortBuildedUp;
FilterGUBuildedUp* filterBuildedUp;
FilterGUPopulation* filterPopulation;
FilterGUName* filterName;
bool foundOp;
int selectionOp;
wstring nameOp;
int populationOp;
double buildedUpOp;

void menu()
{
	wcout << L"Vyber si operáciu: / Choose operation: " << endl;
	wcout << L"1: Vypísanie informácií o územných jednotkách / Informations about ground units" << endl;
	wcout << L"2: Zoradenie územných jednotiek spĺňajúcich daný filter / Sorting ground units by filter" << endl;
	wcout << L"3: Vypísanie informácií o územných jednotkách / Informations about ground units" << endl;
	wcout << L"4: Zoradenie územných jednotiek spĺňajúcich daný filter / Sorting ground units by filter" << endl;
	wcout << L"5: Vypísanie informácií o zoskupeniach bytových jednotiek vo zvolenej územnej jednotke / " << endl;
	wcout << L"6: Vypísanie sumárnych informácií o počtoch bytových jednotiek vo zvolených územných jednotkách / " << endl;
	wcout << L"7: Zoradenie územných jednotiek podľa sumárneho počtu zvolených bytových jednotiek / " << endl;
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
}

void operation1(FilterType type)
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
					if (filterName->meetsFilter(town->accessData(), nameOp))
					{
						wcout << town->accessData()->getName() << ", " << district->accessData()->getName() << ", " << region->accessData()->getName() << ", "  << country->getName() << endl;
						wcout << L"Počet obyvateľov: " << town->accessData()->getPopulation(PopulationType::POPULATION) << endl;
						wcout << L"Počet preproduktívnych obyvateľov: " << town->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
						wcout << L"Počet produktívnych obyvateľov: " << town->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
						wcout << L"Počet poproduktívnych obyvateľov: " << town->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
						wcout << L"Zastavanosť: " << (town->accessData()->getBuiltUpArea() / town->accessData()->getTotalArea()) * 100 << L" %" << endl;
						wcout << L"Celková výmera: " << town->accessData()->getTotalArea() << L" m2" << endl;
						wcout << L"Zastavaná plocha: " << town->accessData()->getBuiltUpArea() << L" m2" << endl << endl;
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
					if (filterPopulation->meetsFilter(town->accessData(), populationOp))
					{
						wcout << town->accessData()->getName() << ", " << district->accessData()->getName() << ", " << region->accessData()->getName() << ", " << country->getName() << endl;
						wcout << L"Počet obyvateľov: " << town->accessData()->getPopulation(PopulationType::POPULATION) << endl;
						wcout << L"Počet preproduktívnych obyvateľov: " << town->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
						wcout << L"Počet produktívnych obyvateľov: " << town->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
						wcout << L"Počet poproduktívnych obyvateľov: " << town->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
						wcout << L"Zastavanosť: " << (town->accessData()->getBuiltUpArea() / town->accessData()->getTotalArea()) * 100 << L" %" << endl;
						wcout << L"Celková výmera: " << town->accessData()->getTotalArea() << L" m2" << endl;
						wcout << L"Zastavaná plocha: " << town->accessData()->getBuiltUpArea() << L" m2" << endl << endl;
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
					if (filterBuildedUp->meetsFilter(town->accessData(), buildedUpOp))
					{
						wcout << town->accessData()->getName() << ", " << district->accessData()->getName() << ", " << region->accessData()->getName() << ", " << country->getName() << endl;
						wcout << L"Počet obyvateľov: " << town->accessData()->getPopulation(PopulationType::POPULATION) << endl;
						wcout << L"Počet preproduktívnych obyvateľov: " << town->accessData()->getPopulation(PopulationType::PREPRODUCTIVE) << endl;
						wcout << L"Počet produktívnych obyvateľov: " << town->accessData()->getPopulation(PopulationType::PRODUCTIVE) << endl;
						wcout << L"Počet poproduktívnych obyvateľov: " << town->accessData()->getPopulation(PopulationType::POSTPRODUCTIVE) << endl;
						wcout << L"Zastavanosť: " << (town->accessData()->getBuiltUpArea() / town->accessData()->getTotalArea()) * 100 << L" %" << endl;
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
}

void operation2(CriteriaType type)
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
					nameSort->insert(town->accessData()->getName(), town->accessData());
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
					populationSort->insert(town->getKey(), populationCriteria->rate(town->accessData()));
				}
			}
		}
		sortPopulation = new QuickSort<wstring, int>();
		sortPopulation->sort(*populationSort);
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
					buildedUpSort->insert(town->getKey(), buildedUpCriteria->rate(town->accessData()));
				}
			}
		}
		sortBuildedUp = new QuickSort<wstring, double>();
		sortBuildedUp->sort(*buildedUpSort);
		for (TableItem<wstring, double>* groundUnit : *buildedUpSort)
		{
			wcout << groundUnit->getKey() << ": " << groundUnit->accessData() << endl;
		}
		break;
	default:
		break;
	}
}

void operation3(FilterType type)
{
	switch (type)
	{
	case FILTERNAME:
		break;
	case FILTERPOPULATION:
		break;
	case FILTERBUILDEDUP:
		break;
	default:
		break;
	}
}

void operation4(CriteriaType type)
{
	switch (type)
	{
	case NAME:
		break;
	case POPULATIONCOUNT:
		break;
	case BUILDEDUP:
		break;
	default:
		break;
	}
}

void operationSelect()
{
	switch (selection)
	{
	case 1:
		selectionOp = 0;
		wcout << L"Vyber si filter: / Choose filter: " << endl;
		wcout << L"1: Názov / Name" << endl;
		wcout << L"2: Počet obyvateľov / Population Count" << endl;
		wcout << L"3: Zastavanosť / Builded Up" << endl << endl;

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
			wcout << L"Zadaj názov / Enter name: ";
			wcin.ignore();
			getline(wcin, nameOp);
			wcout << endl;
			operation1(FILTERNAME);
			break;
		case 2:
			populationOp = 0;
			wcout << L"Zadaj najnižší počet obyvateľov / Enter lowest population count: ";
			do
			{
				wcin >> populationOp;
			} while (populationOp <= 0);
			wcout << endl;
			operation1(FILTERPOPULATION);
			break;
		case 3:
			buildedUpOp = 0.0;
			wcout << L"Zadaj najnižšiu zastavanosť / Enter lowest builded up percentage: ";
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
		wcout << L"Vyber si kritérium: / Choose criteria: " << endl;
		wcout << L"1: Názov / Name" << endl;
		wcout << L"2: Počet obyvateľov / Population Count" << endl;
		wcout << L"3: Zastavanosť / Builded Up" << endl << endl;

		do
		{
			wcout << L"Tvoj vyber / your choice: ";
			wcin >> selectionOp;
		} while (selectionOp > 3 || selectionOp <= 0);

		wcout << endl;

		switch (selectionOp)
		{
		case 1:
			operation2(NAME);
			break;
		case 2:
			operation2(POPULATIONCOUNT);
			break;
		case 3:
			operation2(BUILDEDUP);
			break;
		default:
			break;
		}
		break;
	case 3:
		selectionOp = 0;
		wcout << L"Vyber si filter: / Choose filter: " << endl;
		wcout << L"1: Názov / Name" << endl;
		wcout << L"2: Počet obyvateľov / Population Count" << endl;
		wcout << L"3: Zastavanosť / Builded Up" << endl << endl;

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
			wcout << L"Zadaj názov / Enter name: ";
			getline(wcin, nameOp);
			wcout << endl;
			operation3(FILTERNAME);
			break;
		case 2:
			populationOp = 0;
			wcout << L"Zadaj najnižší počet obyvateľov / Enter lowest population count: ";

			do
			{
				cin >> populationOp;
			} while (populationOp <= 0);
			
			wcout << endl;
			operation3(FILTERPOPULATION);
			break;
		case 3:
			buildedUpOp = 0.0;
			wcout << L"Zadaj najnižšiu zastavanosť / Enter lowest builded up percentage: ";

			do
			{
				wcin >> buildedUpOp;
			} while (buildedUpOp <= 0);
			wcout << endl;
			operation3(FILTERBUILDEDUP);
			break;
		default:
			break;
		}
		break;
	case 4:
		selectionOp = 0;
		wcout << L"Vyber si kritérium: / Choose criteria: " << endl;
		wcout << L"1: Názov / Name" << endl;
		wcout << L"2: Počet obyvateľov / Population Count" << endl;
		wcout << L"3: Zastavanosť / Builded Up" << endl << endl;

		do
		{
			wcout << L"Tvoj vyber / your choice: ";
			wcin >> selectionOp;
		} while (selectionOp > 3 || selectionOp <= 0);

		wcout << endl;

		switch (selectionOp)
		{
		case 1:
			operation4(NAME);
			break;
		case 2:
			operation4(POPULATIONCOUNT);
			break;
		case 3:
			operation4(BUILDEDUP);
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

int main()
{
	//initHeapMonitor();
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, "slovak");
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);

    Input* input = new Input();

	input->loadGroundUnit("input_files\\2_Územné_členenie_SR.csv", country);
	input->loadPopulation("input_files\\1_Obce_SR.csv", country);

	menu();
	operationSelect();

	//delete selection;
	delete input;
	input = nullptr;
}