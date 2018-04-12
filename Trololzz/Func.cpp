// Author: wjdwndud0114, Kevin Jeong
// Crappy code and crappy commenting written a long time ago :/
#include "Func.h"
#include "Proc.h"
#include "Offsets.h"

Proc mem;
Store EList[32];

IDirect3D9Ex* p_Object = 0;
IDirect3DDevice9Ex* p_Device = 0;
D3DPRESENT_PARAMETERS p_Params;

ID3DXLine* p_Line;
ID3DXFont* pFontSmall = 0;

Store::Store(){};

int Main::DirectXInit(HWND hWnd)
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		exit(1);

	ZeroMemory(&p_Params, sizeof(p_Params));
	p_Params.Windowed = TRUE;
	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_Params.hDeviceWindow = hWnd;
	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_Params.BackBufferWidth = Width;
	p_Params.BackBufferHeight = Height;
	p_Params.EnableAutoDepthStencil = TRUE;
	p_Params.AutoDepthStencilFormat = D3DFMT_D16;
	p_Params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device)))
		exit(1);

	if (!p_Line)
		D3DXCreateLine(p_Device, &p_Line);

	D3DXCreateFont(p_Device, 18, 0, 0, 0, false, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Calibri", &pFontSmall);

	return 0;
}

Main::Main()
{
	tE = false;
	eE = false;
	cE = false;
	bE = false;

	mem.FindProc("csgo.exe");
	
	update();

	dwEng = mem.Mod("engine.dll");
	dwAng = mem.Read<DWORD>(dwEng + dwClientState);
	dwRS = mem.Read<DWORD>(dwC + dwRadarBase);
	dwRP = mem.Read<DWORD>(dwRS + RadarPointer);
	MaxAimbot = 50;
}

void Main::update(){
	
	dwPB = mem.Read<DWORD>(dwC + dwLocalPlayer );
	dwC = mem.Mod("client.dll");
}

void Main::Reading(bool on)
{
	if (on){
		for (int i = 1; i < 64; i++)
		{

			dwE = mem.Read<DWORD>(dwC + dwEntityList + (i * 0x10)); //each iteration. 0x10

			if (!dwE){
				continue;
			}

			i_team = mem.Read<int>(dwPB + TeamNum);
			e_team = mem.Read<int>(dwE + TeamNum);

			if (e_team != i_team)
			{
				if (tE){
					i_E[i_C] = mem.Read<int>(dwE + 0x64);//Ent ID ever 0x64
				}
				if (eE || bE)
				{
					iPos = mem.Read<Vec3>(dwPB + VecOrigin);//dwLocalPlayer  coords
					W2S_M = mem.Read<WorldToScreenMatrix_t>(dwC + dwViewMatrix);//view matrix	
					EList[i_C].ePos = mem.Read<Vec3>(dwE + VecOrigin);//coords					
					EList[i_C].iHealth = mem.Read<int>(dwE + health);//HP	
					EList[i_C].fFlags = mem.Read<int>(dwE + Flag);//flag
					EList[i_C].lifeState = mem.Read<int>(dwE + LifeState);//lifestate check
					EList[i_C].dormant = mem.Read<int>(dwE + Dormant);//dormant check
					EList[i_C].BoneBase = mem.Read<int>(dwE + m_dwBoneMatrix);//bone matrix
					EList[i_C].info = mem.Read<player_info_t>(dwRP + ((i+1) * RadarSize) + RadarName); //player name
				}
				i_C++;
			}
		}
	}
}

void Main::Trigger()
{
	if (tE && mem.Find(i_E, mem.Read<int>(dwPB + m_iCrosshairId)))
	{
		Sleep(20);
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		Sleep(1);
		while(GetAsyncKeyState(VK_LBUTTON) & 1){
			Sleep(10);
		}
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); 
	}
	return;
}

bool Main::WTS(Vec3 src, float * dst)
{
	dst[0] = W2S_M.flMatrix[0][0] * src.x + W2S_M.flMatrix[0][1] * src.y + W2S_M.flMatrix[0][2] * src.z + W2S_M.flMatrix[0][3];
	dst[1] = W2S_M.flMatrix[1][0] * src.x + W2S_M.flMatrix[1][1] * src.y + W2S_M.flMatrix[1][2] * src.z + W2S_M.flMatrix[1][3];

	float View = W2S_M.flMatrix[2][0] * src.x + W2S_M.flMatrix[2][1] * src.y + W2S_M.flMatrix[2][2] * src.z + W2S_M.flMatrix[2][3];
	
	if (View < 0.01f)
		return false;

	float invw = 1.0f / View;
	dst[0] *= invw;
	dst[1] *= invw;

	float x = Width / 2;
	float y = Height / 2;

	x += 0.5f * dst[0] * Width + 0.5f;
	y -= 0.5f * dst[1] * Height + 0.5f;
	
	dst[0] = x;
	dst[1] = y;

	return true;
}

void Main::DrawESP(int x, int y, int distance, int Health, int flags, char pname[32]){
	int width, height;
	
	if (distance == 0) return;

	if (flags == 775)
	{
		width = 24950 / distance;
		height = 53000 / distance;
	} else {
		width = 19100 / distance;
		height = 63000 / distance;
	}

	FillRGB(x - (width / 2), y - (height), width, 1,255,20,20,255);
	FillRGB(x - (width / 2), y - (height), 1, height, 255, 20, 20, 255);
	FillRGB((x - (width / 2) + width), y - (height), 1, height, 255, 20, 20, 255);
	FillRGB(x - (width / 2), y, width + 1, 1, 255, 20, 20, 255);
	
	std::stringstream hp;

	hp << Health;

	char * HealthInfo = new char[hp.str().size() + 1];
	strcpy(HealthInfo, hp.str().c_str());

	DrawBox(x - (width/2), y+5, width, 9, 1, 0, 0, 0, 255);
	FillRGB(x - (width/2)+1, y+6, (width*Health/100)-1, 8, 0, 255, 0, 255);

	DrawShadowString(HealthInfo, x - 5, y + 16, 255, 200, 20, pFontSmall);

	DrawShadowString(pname, x - 15, y - height - 16, 255, 100, 20, pFontSmall);

	delete[] HealthInfo;
}

Vec3 Main::GetBonePos(DWORD boneBase, int boneID)
{
	Vec3 bonePos;
	bonePos.x = mem.Read<float>(boneBase + 0x30 * boneID + 0x0C);//x
	bonePos.y = mem.Read<float>(boneBase + 0x30 * boneID + 0x1C);//y
	bonePos.z = mem.Read<float>(boneBase + 0x30 * boneID + 0x2C);//z
	return bonePos;
}

void Main::DrawBone(int bone1, int bone2, DWORD boneBase)
{
	Vec3 Bone1, Bone2;
	float WTSBone2[3], WTSBone1[3];
	Bone1 = GetBonePos(boneBase, bone1);
	Bone2 = GetBonePos(boneBase, bone2);

	if (WTS(Bone2, WTSBone2) && WTS(Bone1, WTSBone1) && iPos.G3D(Bone1) < 1600 && iPos.G3D(Bone2) < 1600){
		DrawLine(WTSBone1[0], WTSBone1[1], WTSBone2[0], WTSBone2[1], 20, 255, 255, 255);
	}
}

void Main::CalcAng(Vec3 src, Vec3 dst, float *angles)
{
	float delta[3] = { (src.x - dst.x), (src.y - dst.y), (src.z - dst.z) };
	float hyp = sqrt(delta[0] * delta[0] + delta[1] * delta[1]);
	angles[0] = atanf(delta[2] / hyp) * 180.0f / 3.14159265f;
	angles[1] = atanf(delta[1] / delta[0]) * 57.295779513082f;
	angles[2] = 0.0f;

	if (delta[0] >= 0.0f){
		angles[1] += 180.0f;
	}
}

int Main::FindNear(){
	int index = 0; 
	int tempDist = 10000000;

	for (int i = 1; i != i_C; i++){
		if (EList[i].lifeState == 0 && !EList[i].dormant){
			Vec3 head = GetBonePos(EList[i].BoneBase, 6);
			float WTSHead[3];
			WTS(head, WTSHead);
			float d = iPos.G3D(EList[i].ePos);
			float dist = findDist(Width / 2, Height / 2, WTSHead[0], WTSHead[1]);
			if (dist < MaxAimbot && dist < tempDist){
				index = i;
				tempDist = dist;
			}
		}
	}
	return index;
}

float Main::findDist(float x1, float y1, float x2, float y2){
	return sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1)));
}

int Main::Render()
{
	p_Device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
	p_Device->BeginScene();

	i_C = 1;
	Reading(eE || tE || bE);
	update();
	nearest = FindNear();

	if (tWnd == GetForegroundWindow())
	{
		Vec3 punchAngs = mem.Read<Vec3>(dwPB + m_aimPunchAngle);

		DrawShadowString("Kevin's External Hack: TROLOLZZ v1.4", 5, 0, 255, 100, 100, pFontSmall);
		
		//crosshair
		if (cE){
			DrawShadowString("CrossHair {4}", 5, 60, 240, 240, 250, pFontSmall);
			//FillRGB(Width / 2 - 11, Height / 2, 22, 1, 240, 240, 250, 255);
			//FillRGB(Width / 2, Height / 2 - 11, 1, 22, 240, 240, 250, 255);
			Circle(Width / 2, Height / 2, MaxAimbot, 30, 0, 204, 0, 255);
			
			/* DRAW RECOIL CROSSHAIR*/
			int crX = Width / 2, crY = Height / 2;
			int dy = Height / 90;
			int dx = Width / 90;

			int drX = crX - (int)(dx * (punchAngs.y));
			int drY = crY + (int)(dy * (punchAngs.x));

			DrawLine(drX - 10, drY, drX + 10, drY, 200, 20, 20, 255);
			DrawLine(drX, drY - 10, drX, drY + 10, 200, 20, 20, 255);
		}

		//trigger
		if (tE){
			DrawShadowString("Trigger {3}", 5, 45, 240, 240, 250, pFontSmall);
			Trigger();
		}

		if (eE || bE){

			if (eE){
				DrawShadowString("ESP {1}", 5, 15, 240, 240, 250, pFontSmall);
			}
			if (bE){
				DrawShadowString("Bone ESP {2}", 5, 30, 240, 240, 250, pFontSmall);
			}

			for (int i = 1; i != i_C; i++){
				if (EList[i].lifeState == 0 && !EList[i].dormant){
					if (eE){

						float EnemyXY[3];

						if (WTS(EList[i].ePos, EnemyXY)){
							char pname[32];
							sprintf_s(pname, "%ls", EList[i].info.name);
							DrawESP(EnemyXY[0], EnemyXY[1], iPos.G3D(EList[i].ePos), EList[i].iHealth, EList[i].fFlags, pname);
						}
					}
					if (bE){
						//chest
						DrawBone(41, 11, EList[i].BoneBase);
						DrawBone(11, 0, EList[i].BoneBase);
						DrawBone(0, 41, EList[i].BoneBase);

						//Crotch to neck
						DrawBone(0, 7, EList[i].BoneBase);
						//neck to head
						DrawBone(7, 8, EList[i].BoneBase);

						//Left Arm
						DrawBone(11, 12, EList[i].BoneBase);
						DrawBone(12, 13, EList[i].BoneBase);
						//DrawBone(9, 32, EList[i].BoneBase);

						//Right Arm
						DrawBone(41, 42, EList[i].BoneBase);
						DrawBone(42, 43, EList[i].BoneBase);
						//DrawBone(37, 60, EList[i].BoneBase);

						//Left Leg
						DrawBone(0, 70, EList[i].BoneBase);
						DrawBone(70, 71, EList[i].BoneBase);
						DrawBone(71, 72, EList[i].BoneBase);
						
						//Right Leg
						DrawBone(0, 77, EList[i].BoneBase);
						DrawBone(77, 78, EList[i].BoneBase);
						DrawBone(78, 79, EList[i].BoneBase);

						//drawhead
						Vec3 head = GetBonePos(EList[i].BoneBase, 8);
						float WTSHead[3];
						float width = 6366 / iPos.G3D(EList[i].ePos);
						if (WTS(head, WTSHead)){
							DrawBox(WTSHead[0] - (width / 2), WTSHead[1] - (width * 13 / 16), width, width, 1, 20, 255, 255, 255);
						}
					}
				}
			}
		}
		
		if (GetAsyncKeyState(VK_XBUTTON2) && nearest > 0){// aimbot
			Vec3 head = GetBonePos(EList[nearest].BoneBase, 8); //8 head //7 head //6 upper chest //5 middle chest
			vecView = mem.Read<Vec3>(dwPB + VecView);
			eyePos.x = vecView.x + iPos.x;
			eyePos.y = vecView.y + iPos.y;
			eyePos.z = vecView.z + iPos.z;
			CalcAng(eyePos, head, EList[nearest].Ang);
			mem.Write(dwAng + dwClientState_ViewAngles, EList[nearest].Ang[0]);
			mem.Write(dwAng + dwClientState_ViewAngles2, EList[nearest].Ang[1]);
		}
		if (rE){
			DrawShadowString("Recoil Reducer {5}", 5, 75, 240, 240, 250, pFontSmall);
			if (GetAsyncKeyState(VK_LBUTTON)){// no recoil
				Vec3 ang;
				ang.x = mem.Read<float>(dwAng + dwClientState_ViewAngles) - (punchAngs.x * 2.0f) + (lastPunch.x * 2.0f);
				ang.y = mem.Read<float>(dwAng + dwClientState_ViewAngles2) - (punchAngs.y * 2.0f) + (lastPunch.y * 2.0f);
				mem.Write(dwAng + dwClientState_ViewAngles, ang.x);
				mem.Write(dwAng + dwClientState_ViewAngles2, ang.y);
				lastPunch = punchAngs;
			}
			else{
				lastPunch.x = lastPunch.y = 0;
			}
		}
	}
	Sleep(1);
	p_Device->EndScene();
	p_Device->Present(NULL, NULL, NULL, NULL);
	return 0;
}

//De-Constructors
Store::~Store(){
	//De-Constructor
};
Main::~Main()
{}
