#pragma once
#include "Interface.hpp"
#include "Module.hpp"
#include "Utils.hpp"

class Server : public Module {
public:
    Interface* GameMovement = nullptr;
    Interface* g_ServerGameDLL = nullptr;

	using _UTIL_PlayerByIndex = void*(__cdecl*)(int index);

	_UTIL_PlayerByIndex UTIL_PlayerByIndex = nullptr;
    
	CGlobalVars* gpGlobals = nullptr;

	Vector pos;
    QAngle angle;

public:
    Server();
    bool Init() override;
    void Shutdown() override;
    const char* Name() override { return MODULE("server"); }

	DECL_M(GetAbsOrigin, Vector);
    DECL_M(GetAbsAngles, QAngle);

	void* Server::GetPlayer(int index);

    //CGameMovement::ProcessMovement
    DECL_DETOUR(ProcessMovement, void* pPlayer, CMoveData* pMove);
};

extern Server* server;
