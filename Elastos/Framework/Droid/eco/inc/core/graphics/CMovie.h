
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

    CARAPI decodeFile(
        /* [in] */ const String& pathName,
        /* [out] */ IMovie ** ppMovie);

    CARAPI decodeTempStream(
        /* [in] */ IInputStream * pIs,
        /* [out] */ IMovie ** ppMovie);

    CARAPI constructor(
        /* [in] */ Int32 nativeMovie);

    static ECode DecodeStream(IInputStream * pIs,
                                IMovie ** ppMovie);

    static ECode decodeByteArray(ArrayOf<Byte> data,
									 Int32 offset,
    								 Int32 length,
									 IMovie ** ppMovie);

private:
    Int32 mNativeMovie;

    Int32 width();
    Int32 height();
    Boolean isOpaque();
    Int32 duration();

    Boolean setTime(Int32 relativeMilliseconds);

    void draw(ICanvas* canvas, Float x, Float y, IPaint* paint);

};

#endif // __CMOVIE_H__
