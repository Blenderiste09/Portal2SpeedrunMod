#include "Spiderman.hpp"

#include "Modules/Console.hpp"
#include "Modules/Engine.hpp"
#include "Modules/Server.hpp"

#include "Offsets.hpp"

#include "SMSM.hpp"

Spiderman spiderman;

Spiderman::Spiderman() : wantsToSwing(false)
{
}

void Spiderman::ProcessMovement(void* pPlayer, CMoveData* pMove)
{
	if (smsm.GetMode() != SpidermanMode) return;

    int tickBase = *reinterpret_cast<int*>((uintptr_t)pPlayer + Offsets::m_nTickBase);
    if (tickBase > this->lastTickBase) {
        float ft = (tickBase - lastTickBase) / 60.0f;
        
		this->isDucking = *reinterpret_cast<bool*>((uintptr_t)pPlayer + Offsets::m_bDucking);

        if (this->wantsToSwing) {
            this->Swing(pPlayer, pMove, ft);
        }
    }

    lastTickBase = tickBase;
}

void Spiderman::ThrowWeb()
{
    QAngle angle = server->angle;

    float X = DEG2RAD(angle.x);
    float Y = DEG2RAD(angle.y);

    auto cosX = std::cos(X);
    auto cosY = std::cos(Y);

    auto sinX = std::sin(X);
    auto sinY = std::sin(Y);

    Vector direction(cosY * cosX, sinY * cosX, -sinX);
    Vector pos = server->pos;
    if (!this->isDucking) {
        pos.z += 64.0f;
    }

    CGameTrace tr;
    CTraceFilterSimple traceFilter;

    if (!engine->ThrowRay(pos, direction, tr, traceFilter)) {
        this->wantsToSwing = false;
        return;
    }

    Vector length;
    Math::VectorSubstract(tr.endpos, pos, length);
    this->tetherLength = length.Length();
    this->tetherPoint = tr.endpos;
    this->wantsToSwing = true;

    engine->AddLineOverlay(pos, this->tetherPoint, 255, 255, 255, false, 0.25);

    return;
}

void Spiderman::Swing(void* pPlayer, CMoveData* pMove, float ft)
{
    Vector pos = pMove->m_vecAbsOrigin;
    if (!this->isDucking) {
        pos.z += 64.0f;
    }

    bool holdingSpace = (pMove->m_nButtons & 0x2);
    if (holdingSpace && this->tetherLength > 64) {
        this->tetherLength -= 3;
    }

    Vector testPosition = pos + pMove->m_vecVelocity * ft; //Position after the frame
    if ((testPosition - tetherPoint).Length() > tetherLength) { //If next distance is > rope length
        pMove->m_vecVelocity = (tetherPoint + ((testPosition - tetherPoint).Normalize() * tetherLength) - pos) / ft;
    }
}

void IN_WebUp(const CCommand &args) {
    if (smsm.GetMode() != SpidermanMode) return;

    if (spiderman.wantsToSwing) {
        spiderman.wantsToSwing = false;
    } else {
        spiderman.ThrowWeb();
    }
}

void IN_WebDown(const CCommand& args)
{
}

Command in_webup("+web", IN_WebUp, "Throw a web (spiderman-mode only).");
Command in_webdown("-web", IN_WebDown, "Throw a web (spiderman-mode only).");
