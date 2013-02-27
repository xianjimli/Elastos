
#include "graphics/CMovieHelper.h"
#include <skia/images/SkMovie.h>


ECode CMovieHelper::DecodeTempStream(
    /* [in] */ IInputStream * pIs,
    /* [out] */ IMovie ** ppMovie)
{
    DecodeStream(pIs, ppMovie);
    pIs->Close();

    return NOERROR;
}

ECode CMovieHelper::DecodeFile(
    /* [in] */ const String& pathName,
    /* [out] */ IMovie ** ppMovie)
{
    IInputStream* is;
    CFileInputStream::New(pathName, (IFileInputStream**)&is);
    if (is == NULL) {
        return E_RUNTIME_EXCEPTION;
    } else {
        DecodeTempStream(is, ppMovie);
    }

    return NOERROR;
}


ECode CMovieHelper::DecodeStream(
    /* [in] */ IInputStream * pIs,
    /* [out] */ IMovie ** ppMovie)
{

    // what is the lifetime of the array? Can the skstream hold onto it?
    //Byte byteArray[16 * 1024];
    // SkStream* strm = CreateJavaInputStreamAdaptor(env, istream, byteArray);
    // if (NULL == strm) {
    //     return 0;
    // }

    // SkMovie* moov = SkMovie::DecodeStream(strm);
    // strm->unref();
    // return create_jmovie(env, moov);

    return NOERROR;
}

static IMovie* Create_jmovie(
    /* [in] */ SkMovie* moov) {
    if (NULL == moov) {
        return NULL;
    }

    // jobject obj = env->AllocObject(gMovie_class);
    // if (obj) {
    //     env->CallVoidMethod(obj, gMovie_constructorMethodID, (jint)moov);
    // }

    return NULL;
}

ECode CMovieHelper::DecodeByteArray(
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ IMovie ** ppMovie)
{
    if (data.GetPayload() == NULL) {
        return NULL;
    }

    Int32 totalLength = data.GetLength();
    if ((offset | length) < 0 || offset + length > totalLength) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    SkMovie* moov = SkMovie::DecodeMemory(data.GetPayload() + offset, length);
    *ppMovie = Create_jmovie(moov);

    return NOERROR;
}
