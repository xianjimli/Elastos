
#ifndef __CMOVIEHELPER_H__
#define __CMOVIEHELPER_H__

#include "CMovie.h"
#include "_CMovieHelper.h"

CarClass(CMovieHelper)
{
public:
    CARAPI DecodeFile(
        /* [in] */ const String& pathName,
        /* [out] */ IMovie ** ppMovie);

    CARAPI DecodeStream(
        /* [in] */ IInputStream * pPIs,
        /* [out] */ IMovie ** ppMovie);

    CARAPI DecodeByteArray(
        /* [in] */ const ArrayOf<Byte> & data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ IMovie ** ppMovie);
private:
    CARAPI DecodeTempStream(
        /* [in] */ IInputStream * pIs,
        /* [out] */ IMovie ** ppMovie);
};

#endif // __CMOVIEHELPER_H__
