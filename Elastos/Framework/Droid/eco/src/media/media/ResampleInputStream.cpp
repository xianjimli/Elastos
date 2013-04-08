
#include "media/ResampleInputStream.h"
    
ResampleInputStream::ResampleInputStream()
{
}

/**
 * Create a new ResampleInputStream, which converts the sample rate
 * @param inputStream InputStream containing 16 bit PCM.
 * @param rateIn the input sample rate.
 * @param rateOut the output sample rate.
 * This only handles rateIn == rateOut / 2 for the moment.
 */
ResampleInputStream::ResampleInputStream(
    /* [in] */ IInputStream* inputStream, 
    /* [in] */ Int32 rateIn, 
    /* [in] */ Int32 rateOut) 
{
    // only support 2:1 at the moment
    if (rateIn != 2 * rateOut) return;//throw new IllegalArgumentException("only support 2:1 at the moment");
    rateIn = 2;
    rateOut = 1;

    mInputStream = inputStream;
    mRateIn = rateIn;
    mRateOut = rateOut;
}

Int32 ResampleInputStream::Read() 
{
    Int32 rtn = Read(mOneByte, 0, 1);
    return rtn == 1 ? (0xff & (*mOneByte)[0]) : -1;
}

Int32 ResampleInputStream::Read(
    /* [in] */ ArrayOf<Byte>* b)
{
    return Read(b, 0, b->GetLength());
}

Int32 ResampleInputStream::Read(
    /* [in] */ ArrayOf<Byte>* b, 
    /* [in] */ Int32 offset, 
    /* [in] */ Int32 length)
{
    if (mInputStream == NULL) return 0;// throw new IllegalStateException("not open");

    // ensure that mBuf is big enough to cover requested 'length'
    Int32 nIn = ((length / 2) * mRateIn / mRateOut + mFirLength) * 2;
    if (mBuf == NULL) {
        mBuf = ArrayOf<Byte>::Alloc(nIn);
    } else if (nIn > mBuf->GetLength()) {
        //Byte[] bf = new Byte[nIn];
        ArrayOf<Byte>* bf = ArrayOf<Byte>::Alloc(nIn);
        //System.arraycopy(mBuf, 0, bf, 0, mBufCount);
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
        //mInputStream->Read(mBuf, mBufCount, mBuf->GetLength() - mBufCount, &n);
        if (n == -1) return -1;
        mBufCount += n;
    }

    // resample input data
    Fir21(mBuf, 0, b, offset, length / 2);
    
    // move any unused bytes to front of mBuf
    Int32 nFwd = length * mRateIn / mRateOut;
    mBufCount -= nFwd;
    //if (mBufCount > 0) System.arraycopy(mBuf, nFwd, mBuf, 0, mBufCount);
    
    return length;
}

/*
@Override
public Int32 available() throws IOException {
    Int32 nsamples = (mIn - mOut + mInputStream.available()) / 2;
    return ((nsamples - mFirLength) * mRateOut / mRateIn) * 2;
}
*/

ECode ResampleInputStream::Close() 
{
    //try {
        if (mInputStream != NULL) mInputStream->Close();
    //} finally {
        mInputStream = NULL;
    //}
}

void ResampleInputStream::Finalize() 
{
    if (mInputStream != NULL) {
        Close();
        //throw new IllegalStateException("someone forgot to close ResampleInputStream");
    }
}

//
// fir filter code JNI interface
//
void ResampleInputStream::Fir21(
    /* [in] */ ArrayOf<Byte>* in, 
    /* [in] */ Int32 inOffset,
    /* [in] */ ArrayOf<Byte>* out, 
    /* [in] */ Int32 outOffset, 
    /* [in] */ Int32 npoints)
{

}

