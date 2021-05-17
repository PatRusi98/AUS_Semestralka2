#include <iostream>
#include <string>
#include "Input.h"
#include "structures/structure.h"
#include "structures/table/sequence_table.h"
#include "structures/table/sorted_sequence_table.h"
#include "GroundUnit.h"
#include <Windows.h>

using namespace std;
using namespace structures;

int selection = 0;

void menu()
{
	wcout << L"Vyber si operáciu: / Choose operation: " << endl;
	wcout << L"1: Vypísanie informácií o územných jednotkách / Informations about ground units" << endl;
	wcout << L"2: Zoradenie územných jednotiek spĺňajúcich daný filter / Sorting ground units by filter" << endl;
	wcout << L"3: Vypísanie informácií o zoskupeniach bytových jednotiek vo zvolenej územnej jednotke / " << endl;
	wcout << L"4: Vypísanie sumárnych informácií o počtoch bytových jednotiek vo zvolených územných jednotkách / " << endl;
	wcout << L"5: Zoradenie územných jednotiek podľa sumárneho počtu zvolených bytových jednotiek / " << endl;
	wcout << L"6: Vypísanie výberových kritérií pre zoskupenia bytových jednotiek vo zvolených územných jednotkách / " << endl;
	wcout << L"7: Vypísanie výberových kritérií pre územné jednotky na základe zvolených zoskupení bytových jednotiek / " << endl;
	wcout << L"0: Koniec / Exit" << endl << endl;

	do
	{
		cout << "Tvoj vyber / your choice: " << endl;
		cin >> selection;
	} while (selection > 7 || selection < 0);

	if (selection == 0)
	{
		exit(0);
	}
}

void operationSelect()
{
	switch (selection)
	{
	case 1:
		//info about ground units
		break;
	case 2:
		//sorted ground units
		break;
	case 3:
		
		break;
	case 4:

		break;
	case 5:

		break;
	case 6:

		break;
	case 7:

		break;
	case 0:

		break;
	default:
		break;
	}
}

int main()
{
	initHeapMonitor();
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, "slovak");

    Input* input = new Input();
	GroundUnit* country = new GroundUnit(GroundUnitType::COUNTRY, L"Slovensko", NULL);

	input->loadGroundUnit("input_files/2 Územnné členenie SR.csv", country);
	input->loadPopulation("input_files/1 Obce SR.csv", country);

	menu();

	//delete selection;
	delete input;
	input = nullptr;
}