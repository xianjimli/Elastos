
#ifndef __CMOVIE_H__
#define __CMOVIE_H__

#include "_CMovie.h"

CarClass(CMovie)
{
public:
    CARAPI Width(
        /* [out] */ Int32* width);

    CARAPI Height(
        /* [out] */ Int32* height);

    CARAPI IsOpaque(
        /* [out] */ Boolean* isOpaque);

    CARAPI Duration(
        /* [out] */ Int32* height);

    CARAPI SetTime(
        /* [in] */ Int32 relativeMilliseconds,
        /* [out] */ Boolean* isSet);

    CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPaint* paint);

    CARAPI DrawEx(
        /* [in] */ ICanvas* canvas,
        /* [in] */ Float x,
        /* [in] */ Float y);

    static CARAPI DecodeStream(
        /* [in] */ IInputStream* is,
        /* [out] */ IMovie** movie);

    CARAPI constructor(
        /* [in] */ Int32 nativeMovie);

private:
    // TODO: Add your private member variables here.
};

#endif // __CMOVIE_H__
