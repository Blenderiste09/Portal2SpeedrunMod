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
int RenderView;
int HudUpdate;

// ConVar
int InternalSetValue;
int InternalSetFloatValue;
int InternalSetIntValue;

// CClientState
int SetSignonState;
int Disconnect;

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

// CClientTools
int NextParticleSystem;

// CBaseEntity
int m_fFlags;
int m_nTickBase;
int S_m_vecAbsOrigin;
int S_m_angAbsRotation;

// CServerGameDLL
int Think;
int m_bDucking;

// CScriptManager
int CreateVM;

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
