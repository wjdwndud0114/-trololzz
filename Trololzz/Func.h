#ifndef FUNC_H
#define FUNC_H

#include "Have.h"
#include "Start.h"
#include "Drawings.h"
#include <Windows.h>
#include <iostream>
#include "Vec3.h"

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

extern IDirect3D9Ex* p_Object;
extern IDirect3DDevice9Ex* p_Device;
extern D3DPRESENT_PARAMETERS p_Params;
extern ID3DXLine* p_Line;
extern ID3DXFont* pFontSmall;

struct Store{
	Store();
	~Store();
public:
	bool tE, eE, cE, bE, rE;
	int i_team, e_team, iHealth, i_C, fFlags, BoneBase, nearest, lifeState, dormant;
	float Ang[3];
	Vec3 iPos, ePos, viewAng, vecView, eyePos, lastPunch;

	typedef struct player_info_t
	{
	public:
		wchar_t name[32];
	};

	player_info_t info;
};

class Main : public Store{
public:

#pragma region FUNCTION PROTOTYPES

	//Misc & Reading Functions
	Main();
	void Reading(bool on);
	~Main();

	//Triggerbot
	void Trigger();

	int DirectXInit(HWND hWnd);
	int Render();
	bool WTS(Vec3 src, float * dst);
	void ESP();
	void DrawESP(int x, int y, int distance, int Health, int flags, char pname[32]);
	Vec3 GetBonePos(DWORD boneBase, int boneID);
	void DrawBone(int bone1, int bone2, DWORD BoneBase);
	void CalcAng(Vec3 src, Vec3 dst, float * angles);
	int FindNear();
	float findDist(float x1, float y1, float x2, float y2);

	void update();
#pragma endregion

#pragma region Variables

	//stuff
	int MaxAimbot;
	DWORD dwEng, dwC, dwPB, dwE, dwRS, dwRP, dwAng;
	int i_E[32];

	//World To Screen Info
	typedef struct{
		float flMatrix[4][4];
	}WorldToScreenMatrix_t;

	WorldToScreenMatrix_t W2S_M;

#pragma endregion
};

#endif