#include "NativeBidi.h"
//#include "unicode/ubidi.h"


Int64 NativeBidi::Ubidi_open()
{
    //return reinterpret_cast<uintptr_t>(new BiDiData(ubidi_open()));
    return 0L;
}

//static BiDiData* biDiData(jlong ptr) {
    //return reinterpret_cast<BiDiData*>(static_cast<uintptr_t>(ptr));
//}

void NativeBidi::Ubidi_close(
    /* [in] */ Int64 pBiDi)
{
    //delete biDiData(ptr);
}

void NativeBidi::Ubidi_setPara(
    /* [in] */ Int64 pBiDi,
    /* [in] */ ArrayOf<Char32> * text,
    /* [in] */ Int32 length,
    /* [in] */ Int32 paraLevel,
    /* [in] */ ArrayOf<Byte> * embeddingLevels)
{/*
    BiDiData* data = biDiData(pBiDi);
    // Copy the new embedding levels from the Java heap to the native heap.
    if (newEmbeddingLevels != NULL) {
        jbyte* dst;
        data->setEmbeddingLevels(dst = new jbyte[length]);
        env->GetByteArrayRegion(newEmbeddingLevels, 0, length, dst);
    } else {
        data->setEmbeddingLevels(NULL);
    }
    ScopedCharArrayRO chars(env, text);
    if (chars.get() == NULL) {
        return;
    }
    UErrorCode err = U_ZERO_ERROR;
    ubidi_setPara(data->uBiDi(), chars.get(), length, paraLevel, data->embeddingLevels(), &err);
    icu4_error(err);*/
}

Int64 NativeBidi::Ubidi_setLine(
    /* [in] */ const Int64& pParaBiDi,
    /* [in] */ Int32 start,
    /* [in] */ Int32 limit)
{
    return 0;
}

Int32 NativeBidi::Ubidi_getDirection(
    /* [in] */ const Int64& pBiDi)
{
    return 0;
}

Int32 NativeBidi::Ubidi_getLength(
    /* [in] */ const Int64& pBiDi)
{
    return 0;
}

Byte NativeBidi::Ubidi_getParaLevel(
    /* [in] */ const Int64& pBiDi)
{
    return 0;
}

ArrayOf<Byte> * NativeBidi::Ubidi_getLevels(
    /* [in] */ Int64 pBiDi)
{
    return NULL;
}

Int64 NativeBidi::Ubidi_countRuns(
    /* [in] */ Int64 pBiDi)
{
    return 0L;
}

ArrayOf<IBidiRun * > * NativeBidi::Ubidi_getRuns(
    /* [in] */ Int64 pBidi)
{
    return NULL;
}

ArrayOf<Int32> * NativeBidi::Ubidi_reorderVisual(
    /* [in] */ ArrayOf<Byte> * levels,
    /* [in] */ Int32 length)
{
    return NULL;
}