
#ifndef __PATHEFFECT_H__
#define __PATHEFFECT_H__

#include <elastos.h>
#include <SkPathEffect.h>

using namespace Elastos;

extern "C" const InterfaceID EIID_PathEffect;

/**
 * PathEffect is the base class for objects in the Paint that affect
 * the geometry of a drawing primitive before it is transformed by the
 * canvas' matrix and drawn.
 */
class PathEffect
{
    friend class Paint;

protected:
    ~PathEffect();

private:
    static CARAPI_(void) NativeDestructor(
        /* [in] */ SkPathEffect* nObj);

protected:
    SkPathEffect* mNativeInstance;
};

#endif //__PATHEFFECT_H__
