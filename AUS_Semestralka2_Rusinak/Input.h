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

using namespace std;
using namespace structures;

class Input
{
public:
	Input();
	~Input();
	void loadGroundUnit(const string& fileName, GroundUnit* country);
	void loadPopulation(const string& fileName, GroundUnit* country);
};

Input::Input()
{ 

}

inline Input::~Input()
{

}

inline void Input::loadGroundUnit(const string& fileName, GroundUnit* country)
{
	wifstream loader(fileName);
	loader.imbue(locale("zh_CN.UTF-8"));

	if (!loader.is_open())
	{
		throw logic_error("Can't open " + fileName);
	}

	wstring trash;
	wstring town;
	wstring distinct;
	wstring region;

	getline(loader, trash, L'\n');

	while (!loader.eof())
	{
		getline(loader, town, L';');
		getline(loader, distinct, L';');
		getline(loader, region, L';');



		if (!country->getLGU()->containsKey(region))
		{
			
		}

		//wcout << town << L" " << distinct << L" " << region << endl;
	}
}

inline void Input::loadPopulation(const string& fileName, GroundUnit* country)
{
	wifstream loader(fileName);
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
	long totalArea = 0;
	long builtUpArea = 0;

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
		getline(loader, trash, L';');
		builtUpArea = stoi(trash);

		//Iterator<TableItem<wstring, GroundUnit*>*>* iterator;
		
		for (TableItem<wstring, GroundUnit*>* regions : *country->getLGU())  //jebat smerniky
		{
			for (TableItem<wstring, GroundUnit*>* distincts : *regions->accessData()->getLGU())
			{
				for (TableItem<wstring, GroundUnit*>* towns : *distincts->accessData()->getLGU())
				{
					
					if (towns->accessData()->getName() == town)
					{
						towns->accessData()->setData(preProductive, productive, postProductive, totalArea, builtUpArea);
						distincts->accessData()->setData(preProductive, productive, postProductive, totalArea, builtUpArea);
						regions->accessData()->setData(preProductive, productive, postProductive, totalArea, builtUpArea);
					}
				}
			}
		}
	}
}