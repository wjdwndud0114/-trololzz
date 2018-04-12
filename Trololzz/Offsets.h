#include <Windows.h>

# define dwEntityList 0x4A8387C
# define dwLocalPlayer  0xAA6614
# define dwRadarBase 0x4EB8564
# define dwViewMatrix 0x4A75294
# define dwClientState 0x57D894 
# define m_iCrosshairId 0xB2A4
# define m_dwBoneMatrix 0x2698
# define m_aimPunchAngle 0x301C //aimpunchangle
# define m_fAccuracyPenalty 0x32B0

# define dwClientState_ViewAngles 0x4D10 //clientstate view angles
# define dwClientState_ViewAngles2 0x4D14 //add 4

# define RadarPointer 0x54
# define RadarName 0x24
# define RadarSize 0x1E0
# define VecOrigin 0x134
# define TeamNum 0xF0
# define LifeState 0x25B
# define Dormant 0xE9
# define VecView 0x104
# define EyeView 0xAA08 //angEyeAngles
# define health 0xFC
# define Flag 0x100

# define Normalize(angle) if(angle > 180.0f) { angle -= 360.0f; } else if( angle <- 180.0f ) { angle += 360.0f; } 