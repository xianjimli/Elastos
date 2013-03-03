
#include "ext/frameworkdef.h"
#include "graphics/CMovieHelper.h"


ECode CMovieHelper::DecodeFile(
    /* [in] */ const String& pathName,
    /* [out] */ IMovie** movie)
{
    VALIDATE_NOT_NULL(movie);

    return CMovie::DecodeFile(pathName, movie);
}

ECode CMovieHelper::DecodeStream(
    /* [in] */ IInputStream* is,
    /* [out] */ IMovie** movie)
{
    VALIDATE_NOT_NULL(movie);

    return CMovie::DecodeStream(is, movie);
}

ECode CMovieHelper::DecodeByteArray(
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ IMovie** movie)
{
    VALIDATE_NOT_NULL(movie);

    return CMovie::DecodeByteArray(data, offset, length, movie);
}
