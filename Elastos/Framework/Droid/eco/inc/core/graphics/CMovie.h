
#ifndef __CMOVIE_H__
#define __CMOVIE_H__

#include "_CMovie.h"



CarClass(CMovie)
{
public:
    CARAPI Width(
        /* [out] */ Int32 * pWidth);

    CARAPI Height(
        /* [out] */ Int32 * pHeight);

    CARAPI IsOpaque(
        /* [out] */ Boolean * pIsOpaque);

    CARAPI Duration(
        /* [out] */ Int32 * pHeight);

    CARAPI SetTime(
        /* [in] */ Int32 relativeMilliseconds,
        /* [out] */ Boolean * pIsSet);

    CARAPI Draw(
        /* [in] */ ICanvas * pCanvas,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPaint * pPaint);

    CARAPI DrawEx(
        /* [in] */ ICanvas * pCanvas,
        /* [in] */ Float x,
        /* [in] */ Float y);

    CARAPI constructor(
        /* [in] */ Int32 nativeMovie);

private:
    Int32 mNativeMovie;
};

#endif // __CMOVIE_H__
