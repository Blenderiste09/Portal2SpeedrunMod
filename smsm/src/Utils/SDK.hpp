#pragma once
#include <cmath>
#include <float.h>

#ifdef _WIN32
#define __funcc __thiscall
#else
#define __funcc __attribute__((__cdecl__))
#endif

struct Vector {
    float x, y, z;
    Vector() : x(0), y(0), z(0) {};
    Vector(float x, float y, float z) { this->x = x; this->y = y;  this->z = z; };
    inline float Length()
    {
        return std::sqrt(x * x + y * y + z * z);
    }
    inline float Length2D()
    {
        return std::sqrt(x * x + y * y);
    }
	inline Vector Normalize()
	{
         Vector res;
         float radius = sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
         float iradius = 1.f / (radius + FLT_EPSILON);

         res.x = x * iradius;
         res.y = y * iradius;
         res.z = z * iradius;
         return res;
	}
    inline Vector operator*(float fl)
    {
        Vector res;
        res.x = x * fl;
        res.y = y * fl;
        res.z = z * fl;
        return res;
    }
    inline Vector operator+(Vector vec)
    {
        Vector res;
        res.x = x + vec.x;
        res.y = y + vec.y;
        res.z = z + vec.z;
        return res;
    }
    inline Vector operator-(Vector vec)
    {
        Vector res;
        res.x = x - vec.x;
        res.y = y - vec.y;
        res.z = z - vec.z;
        return res;
    }
    inline Vector operator/(float f)
    {
        Vector res;
        res.x = x / f;
        res.y = y / f;
        res.z = z / f;
        return res;
    }
    inline float& operator[](int i)
    {
        return ((float*)this)[i];
    }
    inline float operator[](int i) const
    {
        return ((float*)this)[i];
    }
};

struct QAngle {
    float x, y, z;
};

struct Color {
    Color()
    {
        *((int*)this) = 255;
    }
    Color(int _r, int _g, int _b)
    {
        SetColor(_r, _g, _b, 255);
    }
    Color(int _r, int _g, int _b, int _a)
    {
        SetColor(_r, _g, _b, _a);
    }
    void SetColor(int _r, int _g, int _b, int _a = 255)
    {
        _color[0] = (unsigned char)_r;
        _color[1] = (unsigned char)_g;
        _color[2] = (unsigned char)_b;
        _color[3] = (unsigned char)_a;
    }
    inline int r() const { return _color[0]; }
    inline int g() const { return _color[1]; }
    inline int b() const { return _color[2]; }
    inline int a() const { return _color[3]; }
    unsigned char _color[4];
};

enum TextColor {
    COLOR_NORMAL = 1, // 255, 178.5, 0.0, 255
    COLOR_USEOLDCOLORS = 2, // 255, 178.5, 0.0, 255
    COLOR_PLAYERNAME = 3, // 204, 204, 204, 255
    COLOR_LOCATION = 4, // 153, 255, 153, 255
    COLOR_ACHIEVEMENT = 5, // 64, 255, 64, 255
    COLOR_MAX
};

#define FCVAR_DEVELOPMENTONLY (1 << 1)
#define FCVAR_HIDDEN (1 << 4)
#define FCVAR_NOTIFY (1 << 8)
#define FCVAR_NEVER_AS_STRING (1 << 12)
#define FCVAR_CHEAT (1 << 14)

#define COMMAND_COMPLETION_MAXITEMS 64
#define COMMAND_COMPLETION_ITEM_LENGTH 64

struct CCommand;
class ConCommandBase;

using _CommandCallback = void (*)(const CCommand& args);
using _CommandCompletionCallback = int (*)(const char* partial, char commands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH]);
using _InternalSetValue = void(__funcc*)(void* thisptr, const char* value);
using _InternalSetFloatValue = void(__funcc*)(void* thisptr, float value);
using _InternalSetIntValue = void(__funcc*)(void* thisptr, int value);
using _RegisterConCommand = void(__funcc*)(void* thisptr, ConCommandBase* pCommandBase);
using _UnregisterConCommand = void(__funcc*)(void* thisptr, ConCommandBase* pCommandBase);
using _FindCommandBase = void*(__funcc*)(void* thisptr, const char* name);
using _AutoCompletionFunc = int(__funcc*)(void* thisptr, char const* partial, char commands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH]);

class ConCommandBase {
public:
    void* ConCommandBase_VTable; // 0
    ConCommandBase* m_pNext; // 4
    bool m_bRegistered; // 8
    const char* m_pszName; // 12
    const char* m_pszHelpString; // 16
    int m_nFlags; // 20

public:
    ConCommandBase()
        : ConCommandBase_VTable(nullptr)
        , m_pNext(nullptr)
        , m_bRegistered(false)
        , m_pszName(nullptr)
        , m_pszHelpString(nullptr)
        , m_nFlags(0)
    {
    }
};

struct CCommand {
    enum {
        COMMAND_MAX_ARGC = 64,
        COMMAND_MAX_LENGTH = 512
    };
    int m_nArgc;
    int m_nArgv0Size;
    char m_pArgSBuffer[COMMAND_MAX_LENGTH];
    char m_pArgvBuffer[COMMAND_MAX_LENGTH];
    const char* m_ppArgv[COMMAND_MAX_ARGC];

    int ArgC() const
    {
        return this->m_nArgc;
    }
    const char* Arg(int nIndex) const
    {
        return this->m_ppArgv[nIndex];
    }
    const char* operator[](int nIndex) const
    {
        return Arg(nIndex);
    }
};

class ConCommand : public ConCommandBase {
public:
    union {
        void* m_fnCommandCallbackV1;
        _CommandCallback m_fnCommandCallback;
        void* m_pCommandCallback;
    };

    union {
        _CommandCompletionCallback m_fnCompletionCallback;
        void* m_pCommandCompletionCallback;
    };

    bool m_bHasCompletionCallback : 1;
    bool m_bUsingNewCommandCallback : 1;
    bool m_bUsingCommandCallbackInterface : 1;

public:
    ConCommand()
        : ConCommandBase()
        , m_fnCommandCallbackV1(nullptr)
        , m_fnCompletionCallback(nullptr)
    {
    }
};

class ConVar : public ConCommandBase {
public:
    void* ConVar_VTable; // 24
    ConVar* m_pParent; // 28
    const char* m_pszDefaultValue; // 32
    char* m_pszString; // 36
    int m_StringLength; // 40
    float m_fValue; // 44
    int m_nValue; // 48
    bool m_bHasMin; // 52
    float m_fMinVal; // 56
    bool m_bHasMax; // 60
    float m_fMaxVal; // 64
    void* m_fnChangeCallback; // 68

public:
    ConVar()
        : ConCommandBase()
        , ConVar_VTable(nullptr)
        , m_pParent(nullptr)
        , m_pszDefaultValue(nullptr)
        , m_pszString(nullptr)
        , m_StringLength(0)
        , m_fValue(0)
        , m_nValue(0)
        , m_bHasMin(0)
        , m_fMinVal(0)
        , m_bHasMax(0)
        , m_fMaxVal(0)
        , m_fnChangeCallback(nullptr)
    {
    }
    ~ConVar()
    {
        if (this->m_pszString) {
            delete[] this->m_pszString;
            this->m_pszString = nullptr;
        }
    }
};

class ConVar2 : public ConVar {
public:
    // CUtlVector<FnChangeCallback_t> m_fnChangeCallback
    // CUtlMemory<FnChangeCallback_t> m_Memory
    int m_nAllocationCount; // 72
    int m_nGrowSize; // 76
    int m_Size; // 80
    void* m_pElements; // 84

public:
    ConVar2()
        : ConVar()
        , m_nAllocationCount(0)
        , m_nGrowSize(0)
        , m_Size(0)
        , m_pElements(nullptr)
    {
    }
};

enum SignonState {
    None = 0,
    Challenge = 1,
    Connected = 2,
    New = 3,
    Prespawn = 4,
    Spawn = 5,
    Full = 6,
    Changelevel = 7
};

struct CUserCmd {
    void* VMT; // 0
    int command_number; // 4
    int tick_count; // 8
    QAngle viewangles; // 12, 16, 20
    float forwardmove; // 24
    float sidemove; // 28
    float upmove; // 32
    int buttons; // 36
    unsigned char impulse; // 40
    int weaponselect; // 44
    int weaponsubtype; // 48
    int random_seed; // 52
    short mousedx; // 56
    short mousedy; // 58
    bool hasbeenpredicted; // 60
};

class CMoveData {
public:
    bool m_bFirstRunOfFunctions : 1; // 0
    bool m_bGameCodeMovedPlayer : 1; // 2
    void* m_nPlayerHandle; // 4
    int m_nImpulseCommand; // 8
    QAngle m_vecViewAngles; // 12, 16, 20
    QAngle m_vecAbsViewAngles; // 24, 28, 32
    int m_nButtons; // 36
    int m_nOldButtons; // 40
    float m_flForwardMove; // 44
    float m_flSideMove; // 48
    float m_flUpMove; // 52
    float m_flMaxSpeed; // 56
    float m_flClientMaxSpeed; // 60
    Vector m_vecVelocity; // 64, 68, 72
    QAngle m_vecAngles; // 76, 80, 84
    QAngle m_vecOldAngles; // 88, 92, 96
    float m_outStepHeight; // 100
    Vector m_outWishVel; // 104, 108, 112
    Vector m_outJumpVel; // 116, 120, 124
    Vector m_vecConstraintCenter; // 128, 132, 136
    float m_flConstraintRadius; // 140
    float m_flConstraintWidth; // 144
    float m_flConstraintSpeedFactor; // 148
    float m_unknown;
    Vector m_vecAbsOrigin; // 156
};

class CHLMoveData : public CMoveData {
public:
    bool m_bIsSprinting;
};

#define IN_ATTACK (1 << 0)
#define IN_JUMP (1 << 1)
#define IN_DUCK (1 << 2)
#define IN_FORWARD (1 << 3)
#define IN_BACK (1 << 4)
#define IN_USE (1 << 5)
#define IN_MOVELEFT (1 << 9)
#define IN_MOVERIGHT (1 << 10)
#define IN_ATTACK2 (1 << 11)
#define IN_RELOAD (1 << 13)
#define IN_SPEED (1 << 17)

#define FL_ONGROUND (1 << 0)
#define FL_DUCKING (1 << 1)
#define FL_FROZEN (1 << 5)
#define FL_ATCONTROLS (1 << 6)

#define WL_Feet 1
#define WL_Waist 2

#define MOVETYPE_LADDER 9
#define MOVETYPE_NOCLIP 8

typedef enum {
    HS_NEW_GAME = 0,
    HS_LOAD_GAME = 1,
    HS_CHANGE_LEVEL_SP = 2,
    HS_CHANGE_LEVEL_MP = 3,
    HS_RUN = 4,
    HS_GAME_SHUTDOWN = 5,
    HS_SHUTDOWN = 6,
    HS_RESTART = 7
} HOSTSTATES;

struct CHostState {
    HOSTSTATES m_currentState; // 0
    HOSTSTATES m_nextState; // 4
    Vector m_vecLocation; // 8, 12, 16
    QAngle m_angLocation; // 20, 24, 28
    char m_levelName[256]; // 32
    char m_landmarkName[256]; // 288
    char m_saveName[256]; // 544
    float m_flShortFrameTime; // 800
    bool m_activeGame; // 804
    bool m_bRememberLocation; // 805
    bool m_bBackgroundLevel; // 806
    bool m_bWaitingForConnection; // 807
};

#define INTERFACEVERSION_ISERVERPLUGINCALLBACKS "ISERVERPLUGINCALLBACKS002"

typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);
typedef void* (*InstantiateInterfaceFn)();

struct InterfaceReg {
    InstantiateInterfaceFn m_CreateFn;
    const char* m_pName;
    InterfaceReg* m_pNext;
    static InterfaceReg* s_pInterfaceRegs;

    InterfaceReg(InstantiateInterfaceFn fn, const char* pName)
        : m_pName(pName)
    {
        m_CreateFn = fn;
        m_pNext = s_pInterfaceRegs;
        s_pInterfaceRegs = this;
    }
};

class IServerPluginCallbacks {
public:
    virtual bool Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) = 0;
    virtual void Unload() = 0;
    virtual void Pause() = 0;
    virtual void UnPause() = 0;
    virtual const char* GetPluginDescription() = 0;
    virtual void LevelInit(char const* pMapName) = 0;
    virtual void ServerActivate(void* pEdictList, int edictCount, int clientMax) = 0;
    virtual void GameFrame(bool simulating) = 0;
    virtual void LevelShutdown() = 0;
    virtual void ClientFullyConnect(void* pEdict) = 0;
    virtual void ClientActive(void* pEntity) = 0;
    virtual void ClientDisconnect(void* pEntity) = 0;
    virtual void ClientPutInServer(void* pEntity, char const* playername) = 0;
    virtual void SetCommandClient(int index) = 0;
    virtual void ClientSettingsChanged(void* pEdict) = 0;
    virtual int ClientConnect(bool* bAllowConnect, void* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen) = 0;
    virtual int ClientCommand(void* pEntity, const void*& args) = 0;
    virtual int NetworkIDValidated(const char* pszUserName, const char* pszNetworkID) = 0;
    virtual void OnQueryCvarValueFinished(int iCookie, void* pPlayerEntity, int eStatus, const char* pCvarName, const char* pCvarValue) = 0;
    virtual void OnEdictAllocated(void* edict) = 0;
    virtual void OnEdictFreed(const void* edict) = 0;
};

struct CPlugin {
    char m_szName[128]; //0
    bool m_bDisable; // 128
    IServerPluginCallbacks* m_pPlugin; // 132
    int m_iPluginInterfaceVersion; // 136
    void* m_pPluginModule; // 140
};

#define EXPOSE_INTERFACE_FN(functionName, interfaceName, versionName) \
    static InterfaceReg __g_Create##interfaceName##_reg(functionName, versionName);

#define EXPOSE_INTERFACE(className, interfaceName, versionName)                                           \
    static void* __Create##className##_interface() { return static_cast<interfaceName*>(new className); } \
    static InterfaceReg __g_Create##className##_reg(__Create##className##_interface, versionName);

#define EXPOSE_SINGLE_INTERFACE_GLOBALVAR(className, interfaceName, versionName, globalVarName)                           \
    static void* __Create##className##interfaceName##_interface() { return static_cast<interfaceName*>(&globalVarName); } \
    static InterfaceReg __g_Create##className##interfaceName##_reg(__Create##className##interfaceName##_interface, versionName);

#define EXPOSE_SINGLE_INTERFACE(className, interfaceName, versionName) \
    static className __g_##className##_singleton;                      \
    EXPOSE_SINGLE_INTERFACE_GLOBALVAR(className, interfaceName, versionName, __g_##className##_singleton)

struct CEventAction {
    const char* m_iTarget; // 0
    const char* m_iTargetInput; // 4
    const char* m_iParameter; // 8
    float m_flDelay; // 12
    int m_nTimesToFire; // 16
    int m_iIDStamp; //20
    CEventAction* m_pNext; // 24
};

struct EventQueuePrioritizedEvent_t {
    float m_flFireTime; // 0
    char* m_iTarget; // 4
    char* m_iTargetInput; // 8
    int m_pActivator; // 12
    int m_pCaller; // 16
    int m_iOutputID; // 20
    int m_pEntTarget; // 24
    char m_VariantValue[20]; // 28
    EventQueuePrioritizedEvent_t* m_pNext; // 48
    EventQueuePrioritizedEvent_t* m_pPrev; // 52
};

struct CEventQueue {
    EventQueuePrioritizedEvent_t m_Events; // 0
    int m_iListCount; // 56
};

struct CGlobalVarsBase {
    float realtime; // 0
    int framecount; // 4
    float absoluteframetime; // 8
    float curtime; // 12
    float frametime; // 16
    int maxClients; // 20
    int tickcount; // 24
    float interval_per_tick; // 28
    float interpolation_amount; // 32
    int simTicksThisFrame; // 36
    int network_protocol; // 40
    void* pSaveData; // 44
    bool m_bClient; // 48
    int nTimestampNetworkingBase; // 52
    int nTimestampRandomizeWindow; // 56
};

struct CEntInfo {
    void* m_pEntity; // 0
    int m_SerialNumber; // 4
    CEntInfo* m_pPrev; // 8
    CEntInfo* m_pNext; // 12
};

struct CEntInfo2 : CEntInfo {
    void* unk1; // 16
    void* unk2; // 20
};

typedef enum {
    DPT_Int = 0,
    DPT_Float,
    DPT_Vector,
    DPT_VectorXY,
    DPT_String,
    DPT_Array,
    DPT_DataTable,
    DPT_Int64,
    DPT_NUMSendPropTypes
} SendPropType;

struct SendProp;
struct RecvProp;
struct SendTable;

typedef void (*RecvVarProxyFn)(const void* pData, void* pStruct, void* pOut);
typedef void (*ArrayLengthRecvProxyFn)(void* pStruct, int objectID, int currentArrayLength);
typedef void (*DataTableRecvVarProxyFn)(const RecvProp* pProp, void** pOut, void* pData, int objectID);
typedef void (*SendVarProxyFn)(const SendProp* pProp, const void* pStructBase, const void* pData, void* pOut, int iElement, int objectID);
typedef int (*ArrayLengthSendProxyFn)(const void* pStruct, int objectID);
typedef void* (*SendTableProxyFn)(const SendProp* pProp, const void* pStructBase, const void* pData, void* pRecipients, int objectID);

struct RecvTable {
    RecvProp* m_pProps;
    int m_nProps;
    void* m_pDecoder;
    char* m_pNetTableName;
    bool m_bInitialized;
    bool m_bInMainList;
};

struct RecvProp {
    char* m_pVarName;
    SendPropType m_RecvType;
    int m_Flags;
    int m_StringBufferSize;
    bool m_bInsideArray;
    const void* m_pExtraData;
    RecvProp* m_pArrayProp;
    ArrayLengthRecvProxyFn m_ArrayLengthProxy;
    RecvVarProxyFn m_ProxyFn;
    DataTableRecvVarProxyFn m_DataTableProxyFn;
    RecvTable* m_pDataTable;
    int m_Offset;
    int m_ElementStride;
    int m_nElements;
    const char* m_pParentArrayPropName;
};

struct SendProp {
    void* VMT; // 0
    RecvProp* m_pMatchingRecvProp; // 4
    SendPropType m_Type; // 8
    int m_nBits; // 12
    float m_fLowValue; // 16
    float m_fHighValue; // 20
    SendProp* m_pArrayProp; // 24
    ArrayLengthSendProxyFn m_ArrayLengthProxy; // 28
    int m_nElements; // 32
    int m_ElementStride; //36
    char* m_pExcludeDTName; // 40
    char* m_pParentArrayPropName; // 44
    char* m_pVarName; // 48
    float m_fHighLowMul; // 52
    int m_Flags; // 56
    SendVarProxyFn m_ProxyFn; // 60
    SendTableProxyFn m_DataTableProxyFn; // 64
    SendTable* m_pDataTable; // 68
    int m_Offset; // 72
    const void* m_pExtraData; // 76
};

struct SendProp2 {
    void* VMT; // 0
    RecvProp* m_pMatchingRecvProp; // 4
    SendPropType m_Type; // 8
    int m_nBits; // 12
    float m_fLowValue; // 16
    float m_fHighValue; // 20
    SendProp2* m_pArrayProp; // 24
    ArrayLengthSendProxyFn m_ArrayLengthProxy; // 28
    int m_nElements; // 32
    int m_ElementStride; // 36
    char* m_pExcludeDTName; // 40
    char* m_pParentArrayPropName; // 44
    char* m_pVarName; // 48
    float m_fHighLowMul; // 52
    char m_priority; // 56
    int m_Flags; // 60
    SendVarProxyFn m_ProxyFn; // 64
    SendTableProxyFn m_DataTableProxyFn; // 68
    SendTable* m_pDataTable; // 72
    int m_Offset; // 76
    const void* m_pExtraData; // 80
};

struct SendTable {
    SendProp* m_pProps;
    int m_nProps;
    char* m_pNetTableName;
    void* m_pPrecalc;
    bool m_bInitialized : 1;
    bool m_bHasBeenWritten : 1;
    bool m_bHasPropsEncodedAgainstCurrentTickCount : 1;
};

typedef void* (*CreateClientClassFn)(int entnum, int serialNum);
typedef void* (*CreateEventFn)();

struct ClientClass {
    CreateClientClassFn m_pCreateFn;
    CreateEventFn m_pCreateEventFn;
    char* m_pNetworkName;
    RecvTable* m_pRecvTable;
    ClientClass* m_pNext;
    int m_ClassID;
};

struct ServerClass {
    char* m_pNetworkName;
    SendTable* m_pTable;
    ServerClass* m_pNext;
    int m_ClassID;
    int m_InstanceBaselineIndex;
};

enum MapLoadType_t {
    MapLoad_NewGame = 0,
    MapLoad_LoadGame = 1,
    MapLoad_Transition = 2,
    MapLoad_Background = 3
};

struct CGlobalVars : CGlobalVarsBase {
    char* mapname; // 60
    int mapversion; // 64
    char* startspot; // 68
    MapLoadType_t eLoadType; // 72
    bool bMapLoadFailed; // 76
    bool deathmatch; // 80
    bool coop; // 84
    bool teamplay; // 88
    int maxEntities; // 92
};

class IGameEvent {
public:
    virtual ~IGameEvent() = default;
    virtual const char* GetName() const = 0;
    virtual bool IsReliable() const = 0;
    virtual bool IsLocal() const = 0;
    virtual bool IsEmpty(const char* key = 0) = 0;
    virtual bool GetBool(const char* key = 0, bool default_value = false) = 0;
    virtual int GetInt(const char* key = 0, int default_value = 0) = 0;
    virtual float GetFloat(const char* key = 0, float default_value = 0.0f) = 0;
    virtual const char* GetString(const char* key = 0, const char* default_value = "") = 0;
    virtual void SetBool(const char* key, bool value) = 0;
    virtual void SetInt(const char* key, int value) = 0;
    virtual void SetFloat(const char* key, float value) = 0;
    virtual void SetString(const char* key, const char* value) = 0;
};

class IGameEventListener2 {
public:
    virtual ~IGameEventListener2() = default;
    virtual void FireGameEvent(IGameEvent* event) = 0;
    virtual int GetEventDebugID() = 0;
};

static const char* EVENTS[] = {
    "player_spawn_blue",
    "player_spawn_orange"
};

struct csurface_t {
    const char* name;
    short surfaceProps;
    unsigned short flags;
};

struct cplane_t {
    Vector normal;
    float dist;
    unsigned char type;
    unsigned char signbits;
    unsigned char pad[2];
};

struct CBaseTrace {
    Vector startpos;
    Vector endpos;
    cplane_t plane;
    float fraction;
    int contents;
    unsigned short dispFlags;
    bool allsolid;
    bool startsolid;
};

struct CGameTrace : public CBaseTrace {
    float fractionleftsolid;
    csurface_t surface;
    int hitgroup;
    short physicsbone;
    unsigned short worldSurfaceIndex;
    int hitbox;
};

struct __declspec(align(16)) VectorAligned : public Vector {
    VectorAligned() : Vector(), w(0) {};
    VectorAligned(float x, float y, float z) : Vector(x, y, z) , w(0) {}
    float w;
};

struct matrix3x4_t {
    float m_flMatVal[3][4];
};

#define MAX_TRACE_LENGTH 1.732050807569 * 2 * 16384

struct Ray_t {
    VectorAligned m_Start; // starting point, centered within the extents
    VectorAligned m_Delta; // direction + length of the ray
    VectorAligned m_StartOffset; // Add this to m_Start to get the actual ray start
    VectorAligned m_Extents; // Describes an axis aligned box extruded along a ray
    const matrix3x4_t* m_pWorldAxisTransform = nullptr;
    bool m_IsRay; // are there extents zero
    bool m_IsSwept; // is delta != 0
};

enum TraceType_t {
    TRACE_EVERYTHING = 0,
    TRACE_WORLD_ONLY,
    TRACE_ENTITIES_ONLY,
    TRACE_EVERYTHING_FILTER_PROPS,
};

class ITraceFilter
{
public:
    virtual bool ShouldHitEntity(void * pEntity, int contentsMask) = 0;
    virtual TraceType_t	GetTraceType() const = 0;
};

class CTraceFilter : public ITraceFilter {
public:
    virtual TraceType_t	GetTraceType() const {
        return TRACE_EVERYTHING;
    }
};

class CTraceFilterEntitiesOnly : public ITraceFilter {
public:
    virtual TraceType_t	GetTraceType() const {
        return TRACE_ENTITIES_ONLY;
    }
};

class CTraceFilterWorldOnly : public ITraceFilter {
public:
    bool ShouldHitEntity(void* pServerEntity, int contentsMask) {
        return false;
    }
    virtual TraceType_t	GetTraceType() const {
        return TRACE_WORLD_ONLY;
    }
};

class CTraceFilterWorldAndPropsOnly : public ITraceFilter {
public:
    bool ShouldHitEntity(void* pServerEntity, int contentsMask) {
        return false;
    }
    virtual TraceType_t	GetTraceType() const {
        return TRACE_EVERYTHING;
    }
};

class CTraceFilterHitAll : public CTraceFilter {
public:
    virtual bool ShouldHitEntity(void* pServerEntity, int contentsMask) {
        return true;
    }
};

class CTraceFilterSimple : public CTraceFilter {
public:
    virtual void SetPassEntity(const void* pPassEntity) { m_pPassEnt = pPassEntity; }
    virtual void SetCollisionGroup(int iCollisionGroup) { m_collisionGroup = iCollisionGroup; }

    const void* GetPassEntity(void) { return m_pPassEnt; }

    virtual bool ShouldHitEntity(void* pServerEntity, int contentsMask) {
        return false;
        //return pServerEntity != m_pPassEnt;
    }

private:
    const void* m_pPassEnt;
    int m_collisionGroup;
};


#define	CONTENTS_EMPTY			0		// No contents

#define	CONTENTS_SOLID			0x1		// an eye is never valid in a solid
#define	CONTENTS_WINDOW			0x2		// translucent, but not watery (glass)
#define	CONTENTS_AUX			0x4
#define	CONTENTS_GRATE			0x8		// alpha-tested "grate" textures.  Bullets/sight pass through, but solids don't
#define	CONTENTS_SLIME			0x10
#define	CONTENTS_WATER			0x20
#define	CONTENTS_BLOCKLOS		0x40	// block AI line of sight
#define CONTENTS_OPAQUE			0x80	// things that cannot be seen through (may be non-solid though)
#define	LAST_VISIBLE_CONTENTS	0x80

#define ALL_VISIBLE_CONTENTS (LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))

#define CONTENTS_TESTFOGVOLUME	0x100
#define CONTENTS_UNUSED			0x200	

// unused 
// NOTE: If it's visible, grab from the top + update LAST_VISIBLE_CONTENTS
// if not visible, then grab from the bottom.
#define CONTENTS_UNUSED6		0x400

#define CONTENTS_TEAM1			0x800	// per team contents used to differentiate collisions 
#define CONTENTS_TEAM2			0x1000	// between players and objects on different teams

// ignore CONTENTS_OPAQUE on surfaces that have SURF_NODRAW
#define CONTENTS_IGNORE_NODRAW_OPAQUE	0x2000

// hits entities which are MOVETYPE_PUSH (doors, plats, etc.)
#define CONTENTS_MOVEABLE		0x4000

// remaining contents are non-visible, and don't eat brushes
#define	CONTENTS_AREAPORTAL		0x8000

#define	CONTENTS_PLAYERCLIP		0x10000
#define	CONTENTS_MONSTERCLIP	0x20000

// currents can be added to any other contents, and may be mixed
#define	CONTENTS_CURRENT_0		0x40000
#define	CONTENTS_CURRENT_90		0x80000
#define	CONTENTS_CURRENT_180	0x100000
#define	CONTENTS_CURRENT_270	0x200000
#define	CONTENTS_CURRENT_UP		0x400000
#define	CONTENTS_CURRENT_DOWN	0x800000

#define	CONTENTS_ORIGIN			0x1000000	// removed before bsping an entity

#define	CONTENTS_MONSTER		0x2000000	// should never be on a brush, only in game
#define	CONTENTS_DEBRIS			0x4000000
#define	CONTENTS_DETAIL			0x8000000	// brushes to be added after vis leafs
#define	CONTENTS_TRANSLUCENT	0x10000000	// auto set if any surface has trans
#define	CONTENTS_LADDER			0x20000000
#define CONTENTS_HITBOX			0x40000000	// use accurate hitboxes on trace


// NOTE: These are stored in a short in the engine now.  Don't use more than 16 bits
#define	SURF_LIGHT		0x0001		// value will hold the light strength
#define	SURF_SKY2D		0x0002		// don't draw, indicates we should skylight + draw 2d sky but not draw the 3D skybox
#define	SURF_SKY		0x0004		// don't draw, but add to skybox
#define	SURF_WARP		0x0008		// turbulent water warp
#define	SURF_TRANS		0x0010
#define SURF_NOPORTAL	0x0020	// the surface can not have a portal placed on it
#define	SURF_TRIGGER	0x0040	// FIXME: This is an xbox hack to work around elimination of trigger surfaces, which breaks occluders
#define	SURF_NODRAW		0x0080	// don't bother referencing the texture

#define	SURF_HINT		0x0100	// make a primary bsp splitter

#define	SURF_SKIP		0x0200	// completely ignore, allowing non-closed brushes
#define SURF_NOLIGHT	0x0400	// Don't calculate light
#define SURF_BUMPLIGHT	0x0800	// calculate three lightmaps for the surface for bumpmapping
#define SURF_NOSHADOWS	0x1000	// Don't receive shadows
#define SURF_NODECALS	0x2000	// Don't receive decals
#define SURF_NOCHOP		0x4000	// Don't subdivide patches on this surface 
#define SURF_HITBOX		0x8000	// surface is part of a hitbox



// -----------------------------------------------------
// spatial content masks - used for spatial queries (traceline,etc.)
// -----------------------------------------------------
#define	MASK_ALL					(0xFFFFFFFF)
// everything that is normally solid
#define	MASK_SOLID					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// everything that blocks player movement
#define	MASK_PLAYERSOLID			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// blocks npc movement
#define	MASK_NPCSOLID				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// water physics in these contents
#define	MASK_WATER					(CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME)
// everything that blocks lighting
#define	MASK_OPAQUE					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE)
// everything that blocks lighting, but with monsters added.
#define MASK_OPAQUE_AND_NPCS		(MASK_OPAQUE|CONTENTS_MONSTER)
// everything that blocks line of sight for AI
#define MASK_BLOCKLOS				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_BLOCKLOS)
// everything that blocks line of sight for AI plus NPCs
#define MASK_BLOCKLOS_AND_NPCS		(MASK_BLOCKLOS|CONTENTS_MONSTER)
// everything that blocks line of sight for players
#define	MASK_VISIBLE					(MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE)
// everything that blocks line of sight for players, but with monsters added.
#define MASK_VISIBLE_AND_NPCS		(MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE)
// bullets see these as solid
#define	MASK_SHOT					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)
// non-raycasted weapons see this as solid (includes grates)
#define MASK_SHOT_HULL				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE)
// hits solids (not grates) and passes through everything else
#define MASK_SHOT_PORTAL			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER)
// everything normally solid, except monsters (world+brush only)
#define MASK_SOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE)
// everything normally solid for player movement, except monsters (world+brush only)
#define MASK_PLAYERSOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE)
// everything normally solid for npc movement, except monsters (world+brush only)
#define MASK_NPCSOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
// just the world, used for route rebuilding
#define MASK_NPCWORLDSTATIC			(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
// These are things that can split areaportals
#define MASK_SPLITAREAPORTAL		(CONTENTS_WATER|CONTENTS_SLIME)
// UNDONE: This is untested, any moving water
#define MASK_CURRENT				(CONTENTS_CURRENT_0|CONTENTS_CURRENT_90|CONTENTS_CURRENT_180|CONTENTS_CURRENT_270|CONTENTS_CURRENT_UP|CONTENTS_CURRENT_DOWN)