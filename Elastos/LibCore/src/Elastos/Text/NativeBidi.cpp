#include "NativeBidi.h"
#include "CBidiRun.h"
#include "cmdef.h"
#include <unicode/ubidi.h>

struct BiDiData {
    BiDiData(UBiDi* biDi) : mBiDi(biDi), mEmbeddingLevels(NULL) {
    }

    ~BiDiData() {
        ubidi_close(mBiDi);
        ArrayOf<Byte>::Free(mEmbeddingLevels);
    }

    UBiDiLevel* embeddingLevels() {
        return reinterpret_cast<UBiDiLevel*>(&mEmbeddingLevels[0]);
    }

    void setEmbeddingLevels(ArrayOf<Byte>* newEmbeddingLevels) {
        if (newEmbeddingLevels != NULL) {
            mEmbeddingLevels = newEmbeddingLevels->Clone();
        }
        else mEmbeddingLevels = NULL;
    }

    UBiDi* uBiDi() {
        return mBiDi;
    }

private:
    UBiDi* mBiDi;
    ArrayOf<Byte>* mEmbeddingLevels;

    // Disallow copy and assignment.
    BiDiData(const BiDiData&);
    void operator=(const BiDiData&);
};

static BiDiData* biDiData(Int64 ptr)
{
    return reinterpret_cast<BiDiData*>(static_cast<uintptr_t>(ptr));
}

static UBiDi* uBiDi(Int64 ptr)
{
    return reinterpret_cast<BiDiData*>(static_cast<uintptr_t>(ptr))->uBiDi();
}

static ECode icu4_error(UErrorCode errorCode)
{
    if (errorCode <= U_ZERO_ERROR || errorCode >= U_ERROR_LIMIT) {
        return NOERROR;
    }

    switch (errorCode) {
    case U_ILLEGAL_ARGUMENT_ERROR:
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        return jniThrowException(env, "java/lang/IllegalArgumentException", message);
    case U_INDEX_OUTOFBOUNDS_ERROR:
    case U_BUFFER_OVERFLOW_ERROR:
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        return jniThrowException(env, "java/lang/ArrayIndexOutOfBoundsException", message);
    case U_UNSUPPORTED_ERROR:
        return E_UNSUPPORTED_OPERATION_EXCEPTION;
//        return jniThrowException(env, "java/lang/UnsupportedOperationException", message);
    default:
        return E_RUNTIME_EXCEPTION;
//        return jniThrowRuntimeException(env, message);
    }
}

Int64 NativeBidi::Ubidi_open()
{
    return reinterpret_cast<uintptr_t>(new BiDiData(ubidi_open()));
}

void NativeBidi::Ubidi_close(
    /* [in] */ Int64 pBiDi)
{
    delete biDiData(pBiDi);
}

ECode NativeBidi::Ubidi_setPara(
    /* [in] */ Int64 pBiDi,
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 length,
    /* [in] */ Int32 paraLevel,
    /* [in] */ ArrayOf<Byte>* embeddingLevels)
{
    BiDiData* data = biDiData(pBiDi);
    // Copy the new embedding levels from the Java heap to the native heap.
    if (embeddingLevels != NULL) {
        ArrayOf<Byte>* dst = embeddingLevels->Clone();
        data->setEmbeddingLevels(dst);
        ArrayOf<Byte>::Free(dst);
    }
    else {
        data->setEmbeddingLevels(NULL);
    }

    if (text == NULL) {
        return NOERROR;
    }
    UErrorCode err = U_ZERO_ERROR;
    Int32 len = text->GetLength();
    UChar chars[len];
    for (Int32 i = 0; i < len; ++i) {
        chars[i] = (*text)[i];
    }
    ubidi_setPara(data->uBiDi(), chars, length, paraLevel,
            data->embeddingLevels(), &err);
    return icu4_error(err);
}

ECode NativeBidi::Ubidi_setLine(
    /* [in] */ Int64 pParaBiDi,
    /* [in] */ Int32 start,
    /* [in] */ Int32 limit,
    /* [out] */ Int64* line)
{
    VALIDATE_NOT_NULL(line);

    UErrorCode err = U_ZERO_ERROR;
    UBiDi* sized = ubidi_openSized(limit - start, 0, &err);
    ECode ec = icu4_error(err);
    if (FAILED(ec)) {
        *line = 0;
        return ec;
    }

    BiDiData* lineData(new BiDiData(sized));
    ubidi_setLine(uBiDi(pParaBiDi), start, limit, lineData->uBiDi(), &err);
    FAIL_RETURN(icu4_error(err));
    *line = reinterpret_cast<uintptr_t>(lineData);
    return NOERROR;
}

Int32 NativeBidi::Ubidi_getDirection(
    /* [in] */ Int64 pBiDi)
{
    return ubidi_getDirection(uBiDi(pBiDi));
}

Int32 NativeBidi::Ubidi_getLength(
    /* [in] */ Int64 pBiDi)
{
    return ubidi_getLength(uBiDi(pBiDi));
}

Byte NativeBidi::Ubidi_getParaLevel(
    /* [in] */ Int64 pBiDi)
{
    return ubidi_getParaLevel(uBiDi(pBiDi));
}

ECode NativeBidi::Ubidi_getLevels(
    /* [in] */ Int64 pBiDi,
    /* [out] */ ArrayOf<Byte>** levelsByte)
{
    VALIDATE_NOT_NULL(levelsByte);

    UErrorCode err = U_ZERO_ERROR;
    const UBiDiLevel* levels = ubidi_getLevels(uBiDi(pBiDi), &err);
    ECode ec = icu4_error(err);
    if (FAILED(ec)) {
        *levelsByte = NULL;
        return ec;
    }
    Int32 len = ubidi_getLength(uBiDi(pBiDi));
    ArrayOf<Byte>* result = ArrayOf<Byte>::Alloc(len);
    memcpy(result->GetPayload(), reinterpret_cast<const Byte*>(levels), len);
    *levelsByte = result;

    return NOERROR;
}

ECode NativeBidi::Ubidi_countRuns(
    /* [in] */ Int64 pBiDi,
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    UErrorCode err = U_ZERO_ERROR;
    Int32 c = ubidi_countRuns(uBiDi(pBiDi), &err);
    FAIL_RETURN(icu4_error(err));
    *count = c;
    return NOERROR;
}

ECode NativeBidi::Ubidi_getRuns(
    /* [in] */ Int64 pBidi,
    /* [out] */ ArrayOf<AutoPtr<IBidiRun> >** runs)
{
    VALIDATE_NOT_NULL(*runs);

    UBiDi* ubidi = uBiDi(pBidi);
    UErrorCode err = U_ZERO_ERROR;
    Int32 runCount = ubidi_countRuns(ubidi, &err);
    ECode ec = icu4_error(err);
    if (FAILED(ec)) {
        *runs = NULL;
        return ec;
    }

    // jmethodID bidiRunConstructor = env->GetMethodID(JniConstants::bidiRunClass, "<init>", "(III)V");
    // jobjectArray runs = env->NewObjectArray(runCount, JniConstants::bidiRunClass, NULL);
    ArrayOf<AutoPtr<IBidiRun> >* temp = ArrayOf<AutoPtr<IBidiRun> >::Alloc(runCount);
    UBiDiLevel level = 0;
    Int32 start = 0;
    Int32 limit = 0;
    for (Int32 i = 0; i < runCount; ++i) {
        ubidi_getLogicalRun(ubidi, start, &limit, &level);
        AutoPtr<IBidiRun> run;
        CBidiRun::New(start, limit, level, (IBidiRun**)&run);
        (*temp)[i] = run;
        start = limit;
    }

    *runs = temp;
    return NOERROR;
}

ArrayOf<Int32>* NativeBidi::Ubidi_reorderVisual(
    /* [in] */ ArrayOf<Byte>* levelBytes,
    /* [in] */ Int32 length)
{
    if (levelBytes == NULL) {
        return NULL;
    }

    const UBiDiLevel* levels = reinterpret_cast<const UBiDiLevel*>(levelBytes->GetPayload());

    Int32 indexMap[length];
    ubidi_reorderVisual(levels, length, &indexMap[0]);

    ArrayOf<Int32>* result = ArrayOf<Int32>::Alloc(length);
    memcpy(result->GetPayload(), &indexMap[0], length);
    return result;
}