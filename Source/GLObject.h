#pragma once

#include "GLCommon.h"

#ifdef VECTORX_NS
    #define vx VECTORX_NS
#else
    #define vx
#endif

struct GLObject
{
    vx::Vector<3> Position;
    vx::Vector<4> Rotation;
    vx::Vector<3> Scaling;
};