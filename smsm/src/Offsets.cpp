#include "Offsets.hpp"

namespace Offsets {

// CCvar
int RegisterConCommand;
int UnregisterConCommand;
int FindCommandBase;
int m_pConCommandList;

// CEngineClient
int ClientCmd;
int GetActiveSplitScreenPlayerSlot;

// ConVar
int InternalSetValue;
int InternalSetFloatValue;
int InternalSetIntValue;

// CClientState
int SetSignonState;

// CVEngineServer
int ClientCommand;

// CBaseHudChat
int ChatPrintf;

// CIVDebugOverlay
int AddLineOverlay;

// CCommandBuffer
int m_bWaitEnabled;

// IEngineTrace
int TraceRay;

// CBaseEntity
int m_fFlags;
int m_nTickBase;
int S_m_vecAbsOrigin;
int S_m_angAbsRotation;

// CServerGameDLL
int Think;

// Others
int GetClientStateFunction;
int cl;
int AutoCompletionFunc;
int HostState_OnClientConnected;
int hoststate;
int Cbuf_AddText;
int s_CommandBuffer;
int CCommandBufferSize;
int GetHud;
int FindElement;
int ProcessMovement;
int UTIL_PlayerByIndex;
int gpGlobals;
}
