#include "Portal2.hpp"

#include "Game.hpp"
#include "Offsets.hpp"

Portal2::Portal2()
{
    this->version = SourceGame_Portal2;
}
void Portal2::LoadOffsets()
{
    using namespace Offsets;

    // engine.dll

    InternalSetValue = 12; // ConVar
    InternalSetFloatValue = 13; // ConVar
    InternalSetIntValue = 14; // ConVar
    ClientCmd = 7; // CEngineClient
    Disconnect = 16; //  CClientState
    GetClientStateFunction = 4; // CEngineClient::ClientCmd
    Cbuf_AddText = 46; // CEngineClient::ClientCmd
    s_CommandBuffer = 82; // Cbuf_AddText
    CCommandBufferSize = 9556; // Cbuf_AddText
    m_bWaitEnabled = 8265; // CCommandBuffer::AddText
    GetActiveSplitScreenPlayerSlot = 127; // CEngineClient
    SetSignonState = 15; // CClientState
    HostState_OnClientConnected = 684; // CClientState::SetSignonState
    hoststate = 1; // HostState_OnClientConnected
    AutoCompletionFunc = 66; // listdemo_CompletionFunc
    ClientCommand = 39; // CVEngineServer
    TraceRay = 5; // IEngineTrace
    ProcessMovement = 1; // CGameMovement
    m_fFlags = 204; // CBaseEntity
    m_nTickBase = 3792; // CBaseEntity, m_nTickBase -> 3792
    AddLineOverlay = 4; // CIVDebugOverlay
    m_bDucking = 2272; // CBaseEntity

    // client.dll

    GetHud = 125; // cc_leaderboard_enable
    FindElement = 135; // cc_leaderboard_enable
    ChatPrintf = 22; // CBaseHudChat

	// server.dll
    S_m_vecAbsOrigin = 460; // CBaseEntity
    S_m_angAbsRotation = 472; // CBaseEntity
    Think = 31; // CServerGameDLL
    UTIL_PlayerByIndex = 39; // CServerGameDLL::Think
    gpGlobals = 14; // UTIL_PlayerByIndex

    // vstdlib.dll

    RegisterConCommand = 9; // CCVar
    UnregisterConCommand = 10; // CCvar
    FindCommandBase = 13; // CCVar
    m_pConCommandList = 48; // CCvar
}
const char* Portal2::Version()
{
    return "Portal 2 (7054)";
}
const char* Portal2::Process()
{
    return "portal2.exe";
}
