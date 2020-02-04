#pragma once
#include "Module.hpp"
#include "Utils.hpp"
#include "Variable.hpp"
#include "Command.hpp"

class Engine : public Module {
public:
    CHostState* hoststate = nullptr;
    Interface* engineTrace = nullptr;
    Interface* debugoverlay = nullptr;

    using _Cbuf_AddText = void(__cdecl*)(int slot, const char* pText, int nTickDelay);
#ifdef _WIN32
    using _GetActiveSplitScreenPlayerSlot = int (*)();
#else
    using _GetActiveSplitScreenPlayerSlot = int (*)(void* thisptr);
#endif
    using _ClientCommand = int(*)(void* thisptr, void* pEdict, const char* szFmt, ...);
    using _AddLineOverlay = int(__stdcall*)(const Vector& origin, const Vector& dest, int r, int g, int b, bool noDepthText, float duration);
    using _TraceRay = void(__func*)(void* thisptr, const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, CGameTrace* pTrace);

    _GetActiveSplitScreenPlayerSlot GetActiveSplitScreenPlayerSlot = nullptr;
    _Cbuf_AddText Cbuf_AddText = nullptr;
    _ClientCommand ClientCommand = nullptr;
    _AddLineOverlay AddLineOverlay = nullptr;
    _TraceRay TraceRay = nullptr;

    void* s_CommandBuffer = nullptr;

    Engine();

    bool Init() override;
    void Shutdown() override;
    const char* Name() override { return MODULE("engine"); }


	// CClientState::Disconnect
    DECL_DETOUR(Disconnect, bool bShowMainMenu);

	// CClientState::SetSignonState
    DECL_DETOUR(SetSignonState, int state, int count, void* unk);

    template <typename T>
    bool ThrowRay(Vector& origin, Vector& direction, CGameTrace& tr, T& traceFilter, float maxTraceLength = MAX_TRACE_LENGTH)
    {
        Ray_t ray;
        ray.m_IsRay = true;
        ray.m_IsSwept = true;
        ray.m_Delta = VectorAligned(direction.x * maxTraceLength, direction.y * maxTraceLength, direction.z * maxTraceLength);
        ray.m_Start = VectorAligned(origin.x, origin.y, origin.z);
        ray.m_StartOffset = VectorAligned();
        ray.m_Extents = VectorAligned();

        engine->TraceRay(engine->engineTrace->ThisPtr(), ray, MASK_PLAYERSOLID, &traceFilter, &tr);

        if (tr.fraction < 1) {
            return true;
        } else {
            return false;
        }
    }
};

extern Engine* engine;

extern Variable sv_cheats;
