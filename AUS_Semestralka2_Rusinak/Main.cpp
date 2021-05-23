#include <iostream>
#include <string>
#include "Input.h"
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include "UserInterface.h"

using namespace std;
using namespace structures;



int main()
{
	initHeapMonitor();
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, "slovak");
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);

	wcout << L"Loading..." << endl;

	string file1 = "input_files\\1_Obce_SR.csv";
	string file2 = "input_files\\2_Územné_členenie_SR.csv";

	UserInterface* ui = new UserInterface(file2, file1);

	ui->menu();

	delete ui;
	ui = nullptr;
}