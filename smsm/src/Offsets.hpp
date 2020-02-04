#pragma once

namespace Offsets {

// CCvar
extern int RegisterConCommand;
extern int UnregisterConCommand;
extern int FindCommandBase;
extern int m_pConCommandList;

// CEngineClient
extern int ClientCmd;
extern int GetActiveSplitScreenPlayerSlot;
extern int RenderView;
extern int HudUpdate;

// ConVar
extern int InternalSetValue;
extern int InternalSetFloatValue;
extern int InternalSetIntValue;

// CClientState
extern int SetSignonState;
extern int Disconnect;

// CVEngineServer
extern int ClientCommand;

// CBaseHudChat
extern int ChatPrintf;

// CCommandBuffer
extern int m_bWaitEnabled;

// CIVDebugOverlay
extern int AddLineOverlay;

// IEngineTrace
extern int TraceRay;

// CClientTools
extern int NextParticleSystem;

// CBaseEntity
extern int m_fFlags;
extern int m_nTickBase;
extern int S_m_vecAbsOrigin;
extern int S_m_angAbsRotation;

// CServerGameDLL
extern int Think;
extern int m_bDucking;

// CScriptManager
extern int CreateVM;

// Others
extern int GetClientStateFunction;
extern int cl;
extern int AutoCompletionFunc;
extern int HostState_OnClientConnected;
extern int hoststate;
extern int Cbuf_AddText;
extern int s_CommandBuffer;
extern int CCommandBufferSize;
extern int GetHud;
extern int FindElement;
extern int ProcessMovement;
extern int UTIL_PlayerByIndex;
extern int gpGlobals;
}
