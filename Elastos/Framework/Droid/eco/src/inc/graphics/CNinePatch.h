
#ifndef __CNINEPATCH_H__
#define __CNINEPATCH_H__

#include "_CNinePatch.h"
#include "graphics/NinePatch.h"

CarClass(CNinePatch), NinePatch
{
public:
    CARAPI SetPaint(
        /* [in] */ IPaint* p);

    CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IRectF* location);

    CARAPI DrawEx(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IRect* location);

    CARAPI DrawEx2(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IRect* location,
        /* [in] */ IPaint* paint);

    CARAPI GetDensity(
        /* [out] */ Int32* density);

    CARAPI GetWidth(
        /* [out] */ Int32* width);

    CARAPI GetHeight(
        /* [out] */ Int32* height);

    CARAPI HasAlpha(
        /* [out] */ Boolean* hasAlpha);

    CARAPI GetTransparentRegion(
        /* [in] */ IRect* location,
        /* [out] */ IRegion** region);

    CARAPI constructor(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ const ArrayOf<Byte> & chunk,
        /* [in] */ const String& srcName);

    CARAPI constructor(
        /* [in] */ INinePatch* patch);

private:
    // TODO: Add your private member variables here.
};

#endif // __CNINEPATCH_H__

