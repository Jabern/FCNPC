/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Address.cpp
	- Author(s): OrMisicL

  =========================================*/


#include "Main.h"

// Functions
DWORD CAddress::FUNC_CreateNPC_RPC = NULL;
DWORD CAddress::FUNC_CPlayerPool__DeletePlayer = NULL;

DWORD CAddress::FUNC_CPlayer__SpawnForWorld = NULL;
DWORD CAddress::FUNC_CPlayer__Kill = NULL;
DWORD CAddress::FUNC_CPlayer__EnterVehicle = NULL;
DWORD CAddress::FUNC_CPlayer__ExitVehicle = NULL;

DWORD CAddress::FUNC_CConfig__GetValueAsInteger = NULL;

DWORD CAddress::FUNC_GetVehicleModelInfo = NULL;

DWORD CAddress::FUNC_RakServer__Send = NULL;

// Variables
DWORD CAddress::VAR_ServerPtr = NULL;
DWORD CAddress::VAR_ConfigPtr = NULL;
DWORD CAddress::VAR_RakPeerPtr = NULL;
DWORD CAddress::VAR_ServerAuthentication = NULL;

// Offsets
DWORD CAddress::OFFSET_NetVersion = NULL;
DWORD CAddress::OFFSET_RemoteSystemManager = NULL;
DWORD CAddress::OFFSET_RemoteSystemSize = NULL;
DWORD CAddress::OFFSET_RemoteSystem__ConnectMode = NULL;
DWORD CAddress::OFFSET_RemoteSystem__Unknown = NULL;
BYTE  CAddress::OFFSET_SendBullet_RPC = NULL;

void CAddress::Initialize()
{
#ifdef _WIN32
	// Functions
	FUNC_CPlayerPool__DeletePlayer = CUtils::FindPattern("\x8B\x44\x24\x10\x64\x89\x25\x00\x00\x00\x00\x81\xEC\x20\x01\x00\x00\x66\x3D\xE8\x03\x53", "xxxxxxxxxxxxxxxxxxxxxx") - 0xE;
	FUNC_CPlayer__Kill = CUtils::FindPattern("\xB6\x9C\x24\x38\x01\x00\x00\x66\x8B\xAC\x24\x3C", "xxxxxxxxxxxx") - 0x3F;
	FUNC_CPlayer__EnterVehicle = CUtils::FindPattern("\x6A\xFF\x68\x5B\x96\x4A\x00\x64\xA1\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x81\xEC\x1C\x01\x00\x00\x53\x55\x56\x8B\xF1\x57\x8D\x4C\x24\x18", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	FUNC_CPlayer__ExitVehicle = CUtils::FindPattern("\x6A\xFF\x68\x5B\x96\x4A\x00\x64\xA1\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x81\xEC\x18\x01\x00\x00\x53\x56\x8B\xF1\x57\x8D\x4C\x24", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	FUNC_CPlayer__SpawnForWorld = CUtils::FindPattern("\x83\x3B\x00\x74\x08\x57\x8B\xCE", "xxxxxxxx") - 0x33;
	FUNC_GetVehicleModelInfo = CUtils::FindPattern("\x8B\x44\x24\x04\x3D\x90\x01\x00\x00\x0F\x8C\x83\x00\x00\x00\x3D\x63\x02\x00\x00\x7F\x7C", "xxxxxxxxxxxxxxxxxxxxxx");
	FUNC_CConfig__GetValueAsInteger = CUtils::FindPattern("\x85\xC0\x74\x0D\x83\x38\x01\x75\x08\x8B\x48\x08\x8B\x01", "xxxxxxxxxxxxxx") - 0x0A;
	FUNC_CreateNPC_RPC = CUtils::FindPattern("\x8B\x39\x85\xFF\x0F\x84\x4E\x04\x00\x00\x85\xC0\x0F\x84\x46\x04\x00\x00\x99\x83\xE2\x07\x03\xC2\x83\xC1\x08", "xxxxxxxxxxxxxxxxxxxxxxxxxxx") - 0x2A;
	FUNC_RakServer__Send = CUtils::FindPattern("\x8B\x44\x24\x1C\x50\x8B\x44\x24\x18\x83\xEC\x08\x8B\xD4\x89\x02\x66\x8B\x44\x24\x24\x66\x89\x42\x04", "xxxxxxxxxxxxxxxxxxxxxxxxx");

	// Variables
	VAR_ServerAuthentication = *(DWORD *)(CUtils::FindPattern("\x8B\x4C\x24\x28\xA1\xF0\x6C\x4F\x00\x81\xF1\xD9\x0F\x00\x00\x3B\xC1", "xxxxx????xx????xx") + 5);
	VAR_ServerPtr = CSAMPFunctions::GetNetGame();
	VAR_ConfigPtr = CSAMPFunctions::GetConsole();
	VAR_RakPeerPtr = CSAMPFunctions::GetRakServer();

	// Offsets
	OFFSET_NetVersion = *(DWORD *)(CUtils::FindPattern("\x8B\x4C\x24\x28\xA1\xF0\x6C\x4F\x00\x81\xF1\xD9\x0F\x00\x00\x3B\xC1", "xxxxx????xx????xx") + 11);
	OFFSET_RemoteSystemManager = *(DWORD *)(CUtils::FindPattern("\x8B\x96\x3C\x03\x00\x00\x8A\x0C\x3A\x84\xC9\x8D\x04\x3A", "xx????xxxxxxxx") + 2); 
	OFFSET_RemoteSystemSize = *(DWORD *)(CUtils::FindPattern("\x0F\xB7\x56\x08\x43\x81\xC7\xB8\x0C\x00\x00\x3B\xDA", "xxxxxxx????xx") + 7);
	OFFSET_RemoteSystem__ConnectMode = *(DWORD *)(CUtils::FindPattern("\x83\xB8\xB0\x0C\x00\x00\x08\x75\x1A\x40\x8B\xD0\x8B\x44\x24\x04", "xx????xxxxxxxxxx") + 2);
	OFFSET_RemoteSystem__Unknown = *(DWORD *)(CUtils::FindPattern("\x80\xB8\xB5\x0C\x00\x00\x02\x0F\x85\xEE\x01\x00\x00\x8B\x7C\x24\x2C\x8B\xCF", "xx????xx??????xxxxx") + 2);
	OFFSET_SendBullet_RPC = *(BYTE *)(CUtils::FindPattern("\x6A\x01\x6A\x08\x8D\x4C\x24\x10\x51\x8D\x4C\x24\x1C\xC7\x84\x24\x38\x01\x00\x00\x00\x00\x00\x00\xC6\x44\x24\x14", "xxxxxxxxxxxxxxxxxxxxxxxxxxxx") + 28);
#else

	// Functions
	FUNC_CPlayerPool__DeletePlayer                  = CUtils::FindPattern("\x55\x89\xE5\x81\xEC\x68\x01\x00\x00\x89\x5D\xF4\x0F\xB7\x5D\x0C\x0F\xB6\x45\x10", "xxxxxxxxxxxxxxxxxxx"); 
	FUNC_CreateNPC_RPC                              = CUtils::FindPattern("\x55\x89\xE5\x81\xEC\xC8\x03\x00\x00\x89\x5D\xF4\x8B\x4D\x08\x89\x75\xF8\x89\x7D\xFC", "xxxxxxxxxxxxxxxxxxxx");
	FUNC_CPlayer__SpawnForWorld                     = CUtils::FindPattern("\x55\x89\xE5\x56\x53\x83\xEC\x10\x8B\x75\x08\xA1\x14\x07\x1A\x08", "xxxxxxxxxxxx????"); 
	FUNC_CPlayer__Kill                              = CUtils::FindPattern("\x55\x89\xE5\x81\xEC\x68\x01\x00\x00\x0F\xB6\x45\x0C\x89\x7D\xFC\x8B\x7D\x08\x8D\x95\xC8\xFE\xFF\xFF", "xxxxxxxxxxxxxxxxxxxxxxxxx"); 
	FUNC_CPlayer__EnterVehicle                      = CUtils::FindPattern("\x55\x89\xE5\x81\xEC\x68\x01\x00\x00\x89\x5D\xF4\x8B\x45\x08\x0F\xB6\x5D\x10\x89\x75\xF8\x89\x7D\xFC", "xxxxxxxxxxxxxxxxxxxxxxxxx");
	FUNC_CPlayer__ExitVehicle                       = CUtils::FindPattern("\x55\x89\xE5\x81\xEC\x68\x01\x00\x00\x89\x5D\xF4\x89\x75\xF8\x8D\xB5\xC8\xFE\xFF\xFF\x0F\xB7\x5D\x0C\x89\x7D\xFC\x8B\x7D\x08\x89\x34\x24", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	FUNC_CConfig__GetValueAsInteger                 = CUtils::FindPattern("\x55\x89\xE5\x83\xEC\x08\x8B\x45\x0C\x89\x44\x24\x04\x8B\x45\x08\x89\x04\x24\xE8\x68\xFA\xFF\xFF\x85\xC0\x74\x05\x83\x38\x01", "xxxxxxxxxxxxxxxxxxxx????xxx?xxx") + 0x30; 
	FUNC_GetVehicleModelInfo                        = CUtils::FindPattern("\x55\x89\xE5\x57\x8B\x45\x08\x8B\x4D\x0C\x2D\x90\x01\x00\x00\x3D\xD3\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxx"); 
	FUNC_RakServer__Send							= CUtils::FindPattern("\x55\x89\xE5\x83\xEC\x18\x8B\x55\x08\x8D\x45\xFC\x8B\x4A\x04\x89\x44\x24\x08\x83\xC2\x04", "xxxxxxxxxxxxxxxxxxxxxx");

	// Variables
	VAR_ServerAuthentication                        = *(DWORD *)(CUtils::FindPattern("\x8B\x85\x7C\xFC\xFF\xFF\x35", "xxxxxxx") + 13); 
	VAR_ServerPtr									= CSAMPFunctions::GetNetGame();
	VAR_ConfigPtr									= CSAMPFunctions::GetConsole();
	VAR_RakPeerPtr									= CSAMPFunctions::GetRakServer();

	// Offsets
	OFFSET_NetVersion = *(DWORD *)(CUtils::FindPattern("\x8B\x85\x7C\xFC\xFF\xFF\x35", "xxxxxxx") + 7);
	OFFSET_RemoteSystemManager = *(DWORD *)(CUtils::FindPattern("\x8B\x87\x34\x03\x00\x00\x80\x3C\x03\x00\x74\xE6\x8D\x55\x0C\x8D\x44\x18\x01\x89\x54\x24\x04", "xx????xxxxx?xxxxxxxxxxx") + 2);	
	OFFSET_RemoteSystemSize = *(DWORD *)(CUtils::FindPattern("\x46\x0F\xB7\xC2\x81\xC3\x69\x0C\x00\x00\x39\xF0", "xxxxx????xxx") + 6);	
	OFFSET_RemoteSystem__ConnectMode = *(DWORD *)(CUtils::FindPattern("\x83\xBC\x18\x62\x0C\x00\x00\x08\x75\xD1\x01\xD8\x8B\x50\x01\x89\x11", "xxx????xx?xxxxxxx") + 3);
	OFFSET_RemoteSystem__Unknown = *(DWORD *)(CUtils::FindPattern("\x80\xB8\x67\x0C\x00\x00\x02\x74\x1D\x8B\x45\xD8\x89\x04\x24", "xx????xx?xxxxxx") + 2);
	OFFSET_SendBullet_RPC = *(BYTE *)(CUtils::FindPattern("\xC6\x85\xC5\xFE\xFF\xFF\xCE\xB8\x01\x00\x00\x00\x89\x44\x24\x0C\xB8\x08\x00\x00\x00\x89\x44\x24\x08\x8D\x85\xC5\xFE\xFF\xFF", "xxxxxx?xxxxxxxxxxxxxxxxxxxxxxxx") + 6);

#endif
}