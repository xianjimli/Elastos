
#ifndef __CMOVIEHELPER_H__
#define __CMOVIEHELPER_H__

#include "CMovie.h"
#include "_CMovieHelper.h"

CarClass(CMovieHelper)
{
public:
    CARAPI DecodeFile(
        /* [in] */ const String& pathName,
        /* [out] */ IMovie** movie);

    CARAPI DecodeStream(
        /* [in] */ IInputStream* is,
        /* [out] */ IMovie** movie);

    CARAPI DecodeByteArray(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ IMovie** movie);
};

#endif // __CMOVIEHELPER_H__
