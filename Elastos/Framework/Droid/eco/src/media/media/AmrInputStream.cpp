
#include "media/AmrInputStream.h"

AmrInputStream::AmrInputStream()
{

}

/**
 * Create a new AmrInputStream, which converts 16 bit PCM to AMR
 * @param inputStream InputStream containing 16 bit PCM.
 */
AmrInputStream::AmrInputStream(
    /* [in] */ IInputStream* inputStream)
{
    mInputStream = inputStream;
    mGae = GsmAmrEncoderNew();
    GsmAmrEncoderInitialize(mGae);
}

Int32 AmrInputStream::Read()
{
    Int32 rtn = Read(mOneByte, 0, 1);
    return rtn == 1 ? (0xff & (*mOneByte)[0]) : -1;
}

Int32 AmrInputStream::Read(
    /* [in] */ ArrayOf<Byte>* b)
{
    return Read(b, 0, b->GetLength());
}

Int32 AmrInputStream::Read(
    /* [in] */ ArrayOf<Byte>* b, 
    /* [in] */ Int32 offset, 
    /* [in] */ Int32 length)
{
    //if (mGae == 0) throw new IllegalStateException("not open");
    
    // local buffer of amr encoded audio empty
    if (mBufOut >= mBufIn) {
        // reset the buffer
        mBufOut = 0;
        mBufIn = 0;
        
        // fetch a 20 msec frame of pcm
        for (Int32 i = 0; i < SAMPLES_PER_FRAME * 2; ) {
            Int32 n;
            mInputStream->ReadBufferEx(i, SAMPLES_PER_FRAME * 2 - i, mBuf, &n);
            if (n == -1) return -1;
            i += n;
        }
        
        // encode it
        mBufIn = GsmAmrEncoderEncode(mGae, mBuf, 0, mBuf, 0);
    }
    
    // return encoded audio to user
    if (length > mBufIn - mBufOut) length = mBufIn - mBufOut;
    //System.arraycopy(mBuf, mBufOut, b, offset, length);
    mBufOut += length;
    
    return length;
}

ECode AmrInputStream::Close()
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

ECode AmrInputStream::Finalize()
{
    if (mGae != 0) {
        Close();
        //throw new IllegalStateException("someone forgot to close AmrInputStream");
    }

    return NOERROR;
}

//
// AudioRecord JNI interface
//
Int32 AmrInputStream::GsmAmrEncoderNew()
{
    return 0;
}

void AmrInputStream::GsmAmrEncoderInitialize(
    /* [in] */ Int32 gae)
{

}

Int32 AmrInputStream::GsmAmrEncoderEncode(
    /* [in] */ Int32 gae,
    /* [in] */ ArrayOf<Byte>* pcm, 
    /* [in] */ Int32 pcmOffset, 
    /* [in] */ ArrayOf<Byte>* amr, 
    /* [in] */ Int32 amrOffset)
{
    return 0;
}

void AmrInputStream::GsmAmrEncoderCleanup(
    /* [in] */ Int32 gae)
{

}

void AmrInputStream::GsmAmrEncoderDelete(
    /* [in] */ Int32 gae)
{

}

