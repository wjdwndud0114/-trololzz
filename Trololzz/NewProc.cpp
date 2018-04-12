#include "Proc.h"

using namespace std; 

Proc::Proc(){

}

void Proc::GetProcess(char* pName){
	PROCESSENTRY32 ent;
	ent.dwSize = sizeof(ent);
	HANDLE id = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	do
		if (strcmp(ent.szExeFile, pName) == false){
			dwid = ent.th32ProcessID;
			closeHandle(id);
			proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwid);
			return;
		}
	while (Process32Next(id));
	cout << "\nERROR: UNABLE TO FIND PROCESS\n";
	system("pasue");
	exit(0);
}

Proc::~Proc(){
	CloseHandle(hProcess);
}