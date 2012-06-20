
#ifndef __CPORTERDUFFXFERMODE_H__
#define __CPORTERDUFFXFERMODE_H__

#include "_CPorterDuffXfermode.h"
#include "graphics/Xfermode.h"

CarClass(CPorterDuffXfermode), public Xfermode
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ PorterDuffMode mode);

private:
    static CARAPI_(SkXfermode*) NativeCreateXfermode(
        /* [in] */ PorterDuffMode mode);
};

#endif //__CPORTERDUFFXFERMODE_H__
