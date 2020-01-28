#include "Server.hpp"

#include "Command.hpp"
#include "Interface.hpp"
#include "Offsets.hpp"
#include "Utils.hpp"

#include "Console.hpp"
#include "SMSM.hpp"
#include "CelesteMoveset.hpp"


MDECL(Server::GetAbsOrigin, Vector, S_m_vecAbsOrigin);
MDECL(Server::GetAbsAngles, QAngle, S_m_angAbsRotation);

void* Server::GetPlayer(int index)
{
    return this->UTIL_PlayerByIndex(index);
}

// CGameMovement::ProcessMovement
REDECL(Server::ProcessMovement);
DETOUR(Server::ProcessMovement, void* pPlayer, CMoveData* pMove) {
    auto result = Server::ProcessMovement(thisptr, pPlayer, pMove);

    celesteMoveset.ProcessMovement(pPlayer, pMove);

	server->pos = pMove->m_vecAbsOrigin;
    server->angle = pMove->m_vecAbsViewAngles;

    return result;
}


Server::Server()
    : Module() {
}
bool Server::Init() {
    this->GameMovement = Interface::Create(this->Name(), "GameMovement0");
    if (this->GameMovement) {
        this->GameMovement->Hook(Server::ProcessMovement_Hook, Server::ProcessMovement, Offsets::ProcessMovement);
    }

	this->g_ServerGameDLL = Interface::Create(this->Name(), "ServerGameDLL0");
    if (this->g_ServerGameDLL) {
        auto Think = this->g_ServerGameDLL->Original(Offsets::Think);
        Memory::Read<_UTIL_PlayerByIndex>(Think + Offsets::UTIL_PlayerByIndex, &this->UTIL_PlayerByIndex);
        Memory::DerefDeref<CGlobalVars*>((uintptr_t)this->UTIL_PlayerByIndex + Offsets::gpGlobals, &this->gpGlobals);
    }

    return this->hasLoaded = this->GameMovement;
}
void Server::Shutdown() {
    if (this->GameMovement) {
        Interface::Delete(this->GameMovement);
        Interface::Delete(this->g_ServerGameDLL);
    }
}

Server* server;
