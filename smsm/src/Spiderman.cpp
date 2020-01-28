#include "Spiderman.hpp"

#include "Modules/Console.hpp"
#include "Modules/Engine.hpp"
#include "Modules/Server.hpp"

#include "SMSM.hpp"

Spiderman spiderman;

Spiderman::Spiderman()
{
}


void Spiderman::ThrowWeb()
{
    CGameTrace tr;
    CTraceFilterSimple filter;

	QAngle angle = server->angle;

    float X = DEG2RAD(angle.x);
    float Y = DEG2RAD(angle.y);

    auto cosX = std::cos(X);
    auto cosY = std::cos(Y);

    auto sinX = std::sin(X);
    auto sinY = std::sin(Y);

    Vector direction(cosY * cosX, sinY * cosX, -sinX);
    Vector pos = server->pos;
    pos.z += 64.0f;

    if (engine->ThrowRay(pos, direction, tr, filter)) {
        auto pn = tr.plane.normal;
        Vector dest = tr.endpos;
        dest.x += 32 * pn.x;
        dest.y += 32 * pn.y;
        dest.z += 64 * pn.z;

		smsm.Setpos(dest);
        //engine->AddLineOverlay(pos, tr.endpos, 255, 0, 0, false, 10);
    }
}

CON_COMMAND(test, "test")
{
    spiderman.ThrowWeb();
}
