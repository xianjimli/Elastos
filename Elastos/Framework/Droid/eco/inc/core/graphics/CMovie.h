
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

    CARAPI constructor(
        /* [in] */ Handle32 nativeMovie);

    static CARAPI DecodeFile(
        /* [in] */ const String& pathName,
        /* [out] */ IMovie** movie);

    static CARAPI DecodeStream(
        /* [in] */ IInputStream* is,
        /* [out] */ IMovie** movie);

    static CARAPI DecodeByteArray(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ IMovie** movie);

private:
    static CARAPI DecodeTempStream(
        /* [in] */ IInputStream* is,
        /* [out] */IMovie** movie);

private:
    Handle32 mNativeMovie;
};

#endif // __CMOVIE_H__
