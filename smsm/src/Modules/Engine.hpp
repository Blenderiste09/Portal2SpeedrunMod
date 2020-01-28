#pragma once
#include "Module.hpp"
#include "Utils.hpp"
#include "Variable.hpp"

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

    _GetActiveSplitScreenPlayerSlot GetActiveSplitScreenPlayerSlot = nullptr;
    _Cbuf_AddText Cbuf_AddText = nullptr;
    _ClientCommand ClientCommand = nullptr;
    _AddLineOverlay AddLineOverlay = nullptr;

    void* s_CommandBuffer = nullptr;

    Engine();
    bool Init() override;
    void Shutdown() override;
    const char* Name() override { return MODULE("engine"); }

    // IEngineTrace::TraceRay
    DECL_DETOUR(TraceRay, const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, CGameTrace* pTrace);

    template <typename T>
    bool ThrowRay(Vector &origin, Vector &direction, CGameTrace& tr, T& traceFilter, float maxTraceLength = MAX_TRACE_LENGTH)
    {
        Ray_t ray;
        ray.m_IsRay = true;
        ray.m_IsSwept = true;
        ray.m_Delta = VectorAligned(direction.x * maxTraceLength, direction.y * maxTraceLength, direction.z * maxTraceLength);
        ray.m_Start = VectorAligned(origin.x, origin.y, origin.z);
        ray.m_StartOffset = VectorAligned();
        ray.m_Extents = VectorAligned();

        engine->TraceRay(engine->engineTrace->ThisPtr(), ray, MASK_PLAYERSOLID, &traceFilter, &tr);

        /*
	   That m_Start.x condition is literally pointless, but if it's gone then
	   it would seem like TraceRay sets it to 0. It makes no fucking sense
	   and I gave up on trying to find out why it's happening, so I'm just leaving it here.
	   Fuck.
	   */
        if (ray.m_Start.x == origin.x && tr.fraction < 1) {
            return true;
        } else {
            return false;
        }
    }
};

extern Engine* engine;

extern Variable sv_cheats;
