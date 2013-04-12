
#include "ext/frameworkext.h"

#include "media/CResampleInputStream.h"

const Int32 CResampleInputStream::nFir21;// = sizeof(fir21) / sizeof(fir21[0]);;

const Int32 CResampleInputStream::BUF_SIZE;


CString CResampleInputStream::TAG = "ResampleInputStream";

const Int32 CResampleInputStream::mFirLength;

/**
 * Create a new ResampleInputStream, which converts the sample rate
 * @param inputStream InputStream containing 16 bit PCM.
 * @param rateIn the input sample rate.
 * @param rateOut the output sample rate.
 * This only handles rateIn == rateOut / 2 for the moment.
 */
CResampleInputStream::CResampleInputStream() : mBuf(NULL)
{

}

CResampleInputStream::~CResampleInputStream()
{
    if (mInputStream != NULL) {
        Close();
        //throw new IllegalStateException("someone forgot to close ResampleInputStream");
    }

    ArrayOf<Byte>::Free(mBuf);
}

ECode CResampleInputStream::Available(
    /* [out] */ Int32 * number)
{
    VALIDATE_NOT_NULL(number);

    *number = 0;
    return NOERROR;
}

ECode CResampleInputStream::Close()
{
    //try {
        if (mInputStream != NULL) mInputStream->Close();
    //} finally {
        mInputStream = NULL;
    //}

        return NOERROR;
}

ECode CResampleInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return NOERROR;
}

ECode CResampleInputStream::IsMarkSupported(
    /* [out] */ Boolean * isSupported)
{
    VALIDATE_NOT_NULL(isSupported);

    *isSupported = FALSE;
    return NOERROR;
}

ECode CResampleInputStream::Read(
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value);
    Int32 rtn;
    FAIL_RETURN(ReadBufferEx( 0, 1, &mOneByte,&rtn));
    *value = rtn == 1 ? (0xff & mOneByte[0]) : -1;
    return NOERROR;
}

ECode CResampleInputStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte> * buffer,
    /* [out] */ Int32 * number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return ReadBufferEx(0, buffer->GetLength(), buffer, number);
}

ECode CResampleInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte> * buffer,
    /* [out] */ Int32 * number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    if (mInputStream == NULL) {
        *number = 0;// throw new IllegalStateException("not open");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    // ensure that mBuf is big enough to cover requested 'length'
    Int32 nIn = ((length / 2) * mRateIn / mRateOut + mFirLength) * 2;
    if (mBuf == NULL) {
        mBuf = ArrayOf<Byte>::Alloc(nIn);
    } else if (nIn > mBuf->GetLength()) {
        ArrayOf<Byte>* bf = ArrayOf<Byte>::Alloc(nIn);
        memcpy(bf, mBuf, mBufCount);
        ArrayOf<Byte>::Free(mBuf);
        mBuf = bf;
    }

    // read until we have enough data for at least one output sample
    while (TRUE) {
        Int32 len = ((mBufCount / 2 - mFirLength) * mRateOut / mRateIn) * 2;
        if (len > 0) {
            length = len < length ? len : (length / 2) * 2;
            break;
        }
        // TODO: should mBuf.length below be nIn instead?
        Int32 n;
        mInputStream->ReadBufferEx(mBufCount, mBuf->GetLength() - mBufCount, mBuf, &n);
        if (n == -1) {
            *number = -1;
            return NOERROR;
        }

        mBufCount += n;
    }

    // resample input data
    Fir21(mBuf, 0, buffer, offset, length / 2);

    // move any unused bytes to front of mBuf
    Int32 nFwd = length * mRateIn / mRateOut;
    mBufCount -= nFwd;
    if (mBufCount > 0) memcpy(mBuf, mBuf + nFwd, mBufCount);

    *number = length;
    return NOERROR;
}

ECode CResampleInputStream::Reset()
{
    return E_IO_EXCEPTION;
}

ECode CResampleInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64 * pNumber)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CResampleInputStream::constructor(
    /* [in] */ IInputStream * inputStream,
    /* [in] */ Int32 rateIn,
    /* [in] */ Int32 rateOut)
{
    // only support 2:1 at the moment
    if (rateIn != 2 * rateOut) {
        return E_INVALID_ARGUMENT;//throw new IllegalArgumentException("only support 2:1 at the moment");
    }
    rateIn = 2;
    rateOut = 1;

    mInputStream = inputStream;
    mRateIn = rateIn;
    mRateOut = rateOut;

    return NOERROR;
}


void CResampleInputStream::Fir21(
   /* [in] */ ArrayOf<Byte>* inArray, 
   /* [in] */ Int32 inOffset,
   /* [in] */ ArrayOf<Byte>* outArray, 
   /* [in] */ Int32 outOffset, 
   /* [in] */ Int32 npoints)
{
    // safety first!
    if (nFir21 + npoints * 2 > BUF_SIZE) {
        /*throwException(env, "java/lang/IllegalArgumentException",
                "FIR+data too long %d", nFir21 + jNpoints);*/
        return;
    }
    
    // get input data
    Int16 in[BUF_SIZE];
    memcpy(in, inArray->GetPayload() + inOffset, (npoints * 2 + nFir21 - 1) * 2);
    
    // compute filter
    Int16 out[BUF_SIZE];
    for (Int32 i = 0; i < npoints; i++) {
        Int64 sum = 0;
        const Int16* firp = &fir21[0];
        const Int16* inp = &in[i * 2];
        for (Int32 n = nFir21; --n >= 0; ) {
            sum += ((Int64)*firp++) * ((Int64)*inp++);
        }
        out[i] = (Int16)(sum >> 16);
    }

    // save new values
    memcpy(outArray->GetPayload() + outOffset, out, npoints * 2);
}