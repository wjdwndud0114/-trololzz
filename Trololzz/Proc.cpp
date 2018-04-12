// Author: wjdwndud0114, Kevin Jeong
// Crappy code and crappy commenting written a long time ago :/
#include "Proc.h"

using namespace std; 

Proc::Proc(){
	//Constructor For Class, Do Not Remove!
}

int Proc::findLength(char *arr){

	//Loop Through *arr To Get Amount Of Bytes
	for (int i = 1; i < MAX_PATH; i++)
		if (arr[i] == '*')
			return i;

	cout << "\nfindLength: Failed to Find Length\n";
	return 0;
}

bool Proc::Find(int *arr, int val){

	for (int i = 0; i < 64; i++)
		if (val == arr[i] && val != 0)
			return true;

	return false;
}

void Proc::FindProc(char* pn){ //done

	//Variables
	HANDLE hP = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL); //Snapshot To View All Active Processes
	PROCESSENTRY32 pEnt;
	pEnt.dwSize = sizeof(pEnt); //Declare Structure Size And Populate It

	//Loop Through All Running Processes To Find Process
	do
		if (!strcmp(pEnt.szExeFile, pn))
		{
			//Store Process ID
			dwid = pEnt.th32ProcessID;
			CloseHandle(hP);

			//Give Our Handle All Access Rights 
			hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwid);
			return;
		}
	while (Process32Next(hP, &pEnt));

	cout << "\nPROCESS: Process Not Found\n";
	system("pause");
	exit(0);
}

DWORD Proc::Mod(LPSTR nMod){

	//Variables
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwid); //Take A Mod Snapshot Of The Process (Grab All Loaded Modules)
	MODULEENTRY32 mEntry; //Declare Mod Entry Structure
	mEntry.dwSize = sizeof(mEntry); //Declare Structure Size And Populate It With Loaded Modules

	//Scan For Mod By Name
	do
		if (!strcmp(mEntry.szModule, nMod))
		{
			CloseHandle(hModule);
			return (DWORD)mEntry.modBaseAddr;
		}
	while (Module32Next(hModule, &mEntry));

	cout << "\nMODULE: Process Platform Invalid\n";
	return 0;
}

Proc::~Proc(){

	//Clean Up! (Close Handle - Not Needed Anymore)
	CloseHandle(hProc);
}