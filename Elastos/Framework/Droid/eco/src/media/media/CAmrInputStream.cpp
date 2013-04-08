
#include "ext/frameworkext.h"
#include "media/CAmrInputStream.h"
#include <media/gsmamr_enc.h>


// Corresponds to max bit rate of 12.2 kbps.
static const int MAX_OUTPUT_BUFFER_SIZE = 32;
static const int FRAME_DURATION_MS = 20;
static const int SAMPLING_RATE_HZ = 8000;
static const int SAMPLES_PER_FRAME = ((SAMPLING_RATE_HZ * FRAME_DURATION_MS) / 1000);
static const int BYTES_PER_SAMPLE = 2;  // Assume 16-bit PCM samples
static const int BYTES_PER_FRAME = (SAMPLES_PER_FRAME * BYTES_PER_SAMPLE);

struct GsmAmrEncoderState
{
    GsmAmrEncoderState()
        : mEncState(NULL)
        , mSidState(NULL)
        , mLastModeUsed(0)
    {}

    ~GsmAmrEncoderState() {}

    void*   mEncState;
    void*   mSidState;
    int32_t mLastModeUsed;
};

CString CAmrInputStream::TAG = "AmrInputStream";

// frame is 20 msec at 8.000 khz
const Int32 CAmrInputStream::SAMPLES_PER_FRAME;


CAmrInputStream::CAmrInputStream()
    : mBuf(ArrayOf<Byte>::Alloc(SAMPLES_PER_FRAME * 2))
    , mBufIn(0)
    , mBufOut(0)
{
    assert(mBuf != NULL);
}

CAmrInputStream::~CAmrInputStream()
{
    if (mGae != 0) {
        Close();
        //throw new IllegalStateException("someone forgot to close AmrInputStream");
    }
    ArrayOf<Byte>::Free(mBuf);
}

ECode CAmrInputStream::constructor(
    /* [in] */ IInputStream* inputStream)
{
    mInputStream = inputStream;
    FAIL_RETURN(GsmAmrEncoderNew(&mGae));
    return GsmAmrEncoderInitialize(mGae);
}

ECode CAmrInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    FAIL_RETURN(ReadBufferEx(0, 1, &mOneByte, value));
    *value = *value == 1 ? (0xff & mOneByte[0]) : -1;
    return NOERROR;
}

ECode CAmrInputStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return ReadBufferEx(0, buffer->GetLength(), buffer, number);
}

ECode CAmrInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    if (mGae == 0) {
        // throw new IllegalStateException("not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    // local buffer of amr encoded audio empty
    if (mBufOut >= mBufIn) {
        // reset the buffer
        mBufOut = 0;
        mBufIn = 0;

        // fetch a 20 msec frame of pcm
        for (Int32 i = 0; i < SAMPLES_PER_FRAME * 2; ) {
            Int32 n;
            mInputStream->ReadBufferEx(i, SAMPLES_PER_FRAME * 2 - i, mBuf, &n);
            if (n == -1) {
                *number = -1;
                return NOERROR;
            }
            i += n;
        }

        // encode it
        FAIL_RETURN(GsmAmrEncoderEncode(mGae, mBuf, 0, mBuf, 0, &mBufIn));
    }

    // return encoded audio to user
    if (length > mBufIn - mBufOut) length = mBufIn - mBufOut;
    memcpy(buffer + offset, mBuf + mBufOut, length);
    mBufOut += length;

    *number = length;
    return NOERROR;
}

ECode CAmrInputStream::Close()
{
    //try {
    if (mInputStream != NULL) mInputStream->Close();
    //} finally {
    mInputStream = NULL;
        //try {
    if (mGae != 0) GsmAmrEncoderCleanup(mGae);
        //} finally {
            //try {
    if (mGae != 0) GsmAmrEncoderDelete(mGae);
            //} finally {
    mGae = 0;
            //}
        //}
    //}

    return NOERROR;
}

ECode CAmrInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    *number = 0;
    return NOERROR;
}

ECode CAmrInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return NOERROR;
}

ECode CAmrInputStream::IsMarkSupported(
    /* [out] */ Boolean* isSupported)
{
    VALIDATE_NOT_NULL(isSupported);

    *isSupported = FALSE;
    return NOERROR;
}

ECode CAmrInputStream::Reset()
{
    return E_IO_EXCEPTION;
}

ECode CAmrInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64 * pNumber)
{
//  return Streams.skipByReading(this, byteCount);
    return E_NOT_IMPLEMENTED;
}

ECode CAmrInputStream::GsmAmrEncoderNew(
    /* [out] */ Int32* gae)
{
    VALIDATE_NOT_NULL(gae);

    GsmAmrEncoderState* gaeObj = new GsmAmrEncoderState();
    if (gaeObj == NULL) {
        // throwException(env, "java/lang/RuntimeException",
        //         "Out of memory", 0);
        *gae = 0;
        return E_RUNTIME_EXCEPTION;
    }
    *gae = (Int32)gaeObj;
    return NOERROR;
}

ECode CAmrInputStream::GsmAmrEncoderInitialize(
    /* [in] */ Int32 gae)
{
    GsmAmrEncoderState *state = (GsmAmrEncoderState *)gae;
    int32_t nResult = AMREncodeInit(&state->mEncState, &state->mSidState, false);
    if (nResult != 0) {
        // throwException(env, "java/lang/IllegalArgumentException",
        //         "GsmAmrEncoder initialization failed %d", nResult);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode CAmrInputStream::GsmAmrEncoderEncode(
    /* [in] */ Int32 gae,
    /* [in] */ ArrayOf<Byte>* pcm,
    /* [in] */ Int32 pcmOffset,
    /* [in] */ ArrayOf<Byte>* amr,
    /* [in] */ Int32 amrOffset,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    Byte inBuf[BYTES_PER_FRAME];
    Byte outBuf[MAX_OUTPUT_BUFFER_SIZE];

    memcpy(inBuf, pcm->GetPayload() + pcmOffset, sizeof(inBuf));
    GsmAmrEncoderState *state = (GsmAmrEncoderState *)gae;
    int32_t length = AMREncode(state->mEncState, state->mSidState,
                                (Mode)MR122,
                                (int16_t *)inBuf,
                                (unsigned char *)outBuf,
                                (Frame_Type_3GPP*)&state->mLastModeUsed,
                                AMR_TX_WMF);
    if (length < 0) {
        // throwException(env, "java/io/IOException",
        //         "Failed to encode a frame with error code: %d", length);
        *number = -1;
        return E_IO_EXCEPTION;
    }

    // The 1st byte of PV AMR frames are WMF (Wireless Multimedia Forum)
    // bitpacked, i.e.;
    //    [P(4) + FT(4)]. Q=1 for good frame, P=padding bit, 0
    // Here we are converting the header to be as specified in Section 5.3 of
    // RFC 3267 (AMR storage format) i.e.
    //    [P(1) + FT(4) + Q(1) + P(2)].
    if (length > 0) {
        outBuf[0] = (outBuf[0] << 3) | 0x4;
    }

    memcpy(amr + amrOffset, outBuf, length);

    *number = length;
    return NOERROR;
}

void CAmrInputStream::GsmAmrEncoderCleanup(
    /* [in] */ Int32 gae)
{
    GsmAmrEncoderState *state = (GsmAmrEncoderState *)gae;
    AMREncodeExit(&state->mEncState, &state->mSidState);
    state->mEncState = NULL;
    state->mSidState = NULL;
}

void CAmrInputStream::GsmAmrEncoderDelete(
    /* [in] */ Int32 gae)
{
    delete (GsmAmrEncoderState*)gae;
}
