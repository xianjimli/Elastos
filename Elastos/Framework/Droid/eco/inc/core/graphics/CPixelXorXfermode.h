
#ifndef __CPIXELXORXFERMODE_H__
#define __CPIXELXORXFERMODE_H__

#include "_CPixelXorXfermode.h"
#include "graphics/Xfermode.h"

CarClass(CPixelXorXfermode), public Xfermode
{
public:
    CARAPI constructor(
        /* [in] */ Int32 opColor);

private:
    static SkXfermode* NativeCreate(
        /* [in] */ Int32 opColor);
};

#endif // __CPIXELXORXFERMODE_H__
