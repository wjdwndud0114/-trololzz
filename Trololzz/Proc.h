#ifndef PROCMEM_H //If Not Defined
#define PROCMEM_H //Define Now

#include "Have.h"

class Proc{

public:

	//STORAGE
	HANDLE hProc;
	DWORD dwid;

	//FUNCTION PROTOTYPES
	Proc();
	~Proc();
	__int32 findLength(char *chArray);
	bool Find(int *arr, int val);
	void FindProc(char* pName);
	DWORD Mod(LPSTR mName);

#pragma region TEMPLATE MEMORY FUNCTIONS

	//READ MEMORY 
	template <class cData>
	cData Read(DWORD dwAddress)
	{
		cData cRead; //Generic Variable To Store Data
		ReadProcessMemory(hProc, (LPVOID)dwAddress, &cRead, sizeof(cData), NULL); //Win API - Reads Data At Specified Location 
		return cRead; //Returns Value At Specified dwAddress
	}
	
	//READ MEMORY - Pointer
	template <class cData>
	cData Read(DWORD dwAddress, char *Offset, BOOL Type)
	{
		//Variables
		__int32 iSize = findLength(Offset) - 1; //Size Of *Array Of Offsets 
		dwAddress = Read<DWORD>(dwAddress); //HEX VAL

		//Loop Through Each Offset & Store Hex Value (Address) In dwTMP
		for (__int32 i = 0; i < iSize; i++)
			dwAddress = Read<DWORD>(dwAddress + Offset[i]);

		if (!Type)
			return dwAddress + Offset[iSize]; //FALSE - Return Address
		else
			return Read<cData>(dwAddress + Offset[iSize]); //TRUE - Return Value
	}

	//write mem
	template <class cData>
	void Write(DWORD dwAdd, cData val){
		WriteProcessMemory(hProc, (LPVOID)dwAdd, &val, sizeof(cData), NULL);
	}

#pragma endregion	
};
#endif


