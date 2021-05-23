#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <io.h>
#include <fcntl.h>
#include <Windows.h>
#include "structures/structure.h"
#include "structures/table/sequence_table.h"
#include "structures/table/sorted_sequence_table.h"
#include "GroundUnit.h"
#include "Enum.h"

using namespace std;
using namespace structures;

class Input
{
public:
	Input();
	~Input();
	void loadGroundUnit(const string& fileName, GroundUnit* country);
	void loadPopulation(const string& fileName, GroundUnit* country);
private:
	wifstream loader;
};

Input::Input()
{ 
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	setlocale(LC_ALL, "slovak");
	loader.imbue(locale("zh_CN.UTF-8"));
}

inline Input::~Input()
{
}

inline void Input::loadGroundUnit(const string& fileName, GroundUnit* country)
{
	loader.open(fileName);
	loader.imbue(locale("zh_CN.UTF-8"));

	if (!loader.is_open())
	{
		throw logic_error("Can't open " + fileName);
	}

	wstring trash;
	wstring town;
	wstring district;
	wstring region;
	GroundUnit* tempRegion;
	GroundUnit* tempDistrict;
	GroundUnit* tempTown;

	getline(loader, trash, L'\n');

	while (!loader.eof())
	{
		getline(loader, town, L';');
		getline(loader, district, L';');
		getline(loader, region, L'\n');

		if (!country->getLGU()->containsKey(region))
		{
			tempRegion = new GroundUnit(GroundUnitType::REGION, region, country);
			country->setLGU(tempRegion);
		}
		else
		{
			tempRegion = (*country->getLGU())[region];
		}

		if (!tempRegion->getLGU()->containsKey(district))
		{
			tempDistrict = new GroundUnit(GroundUnitType::DISTRICT, district, tempRegion);
			tempRegion->setLGU(tempDistrict);
		}
		else
		{
			tempDistrict = (*tempRegion->getLGU())[district];
		}

		tempTown = new GroundUnit(GroundUnitType::TOWN, town, tempDistrict);
		tempDistrict->setLGU(tempTown);

		/*delete tempRegion;
		tempRegion = nullptr;
		delete tempDistrict;
		tempDistrict = nullptr;
		delete tempTown;
		tempTown = nullptr;*/
	}

	loader.close();
}

inline void Input::loadPopulation(const string& fileName, GroundUnit* country)
{
	loader.open(fileName);
	loader.imbue(locale("zh_CN.UTF-8"));

	if (!loader.is_open())
	{
		throw logic_error("Can't open " + fileName);
	}

	wstring trash;
	wstring town;
	int preProductive = 0;
	int productive = 0;
	int postProductive = 0;
	double totalArea = 0;
	double builtUpArea = 0;

	getline(loader, trash, L'\n');

	while (!loader.eof())
	{
		getline(loader, town, L';');
		getline(loader, trash, L';');
		preProductive = stoi(trash);
		getline(loader, trash, L';');
		productive = stoi(trash);
		getline(loader, trash, L';');
		postProductive = stoi(trash);
		getline(loader, trash, L';');
		totalArea = stoi(trash);
		getline(loader, trash, L'\n');
		builtUpArea = stoi(trash);
		bool found = false;

		
		for (TableItem<wstring, GroundUnit*>* aregion : *country->getLGU())
		{
			for (TableItem<wstring, GroundUnit*>* adistinct : *aregion->accessData()->getLGU())
			{
				for (TableItem<wstring, GroundUnit*>* atown : *adistinct->accessData()->getLGU())
				{
					if (atown->accessData()->getName() == town)
					{
						atown->accessData()->setData(preProductive, productive, postProductive, totalArea, builtUpArea);
						adistinct->accessData()->setData(preProductive, productive, postProductive, totalArea, builtUpArea);
						aregion->accessData()->setData(preProductive, productive, postProductive, totalArea, builtUpArea);
						country->setData(preProductive, productive, postProductive, totalArea, builtUpArea);
						found = true;
					}
					if (found)
					{
						break;
					}
				}
				if (found)
				{
					break;
				}
			}
			if (found)
			{
				break;
			}
		}
	}

	loader.close();
}