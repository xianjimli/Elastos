
#ifndef __RESAMPLEINPUTSTREAM_H__
#define __RESAMPLEINPUTSTREAM_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>

/**
 * ResampleInputStream
 * @hide
 */
class ResampleInputStream
{    
    /*static {
        System.loadLibrary("media_jni");
    }*/
    
public:
    ResampleInputStream();

    /**
     * Create a new ResampleInputStream, which converts the sample rate
     * @param inputStream InputStream containing 16 bit PCM.
     * @param rateIn the input sample rate.
     * @param rateOut the output sample rate.
     * This only handles rateIn == rateOut / 2 for the moment.
     */
    ResampleInputStream(
        /* [in] */ IInputStream* inputStream, 
        /* [in] */ Int32 rateIn, 
        /* [in] */ Int32 rateOut);

    virtual CARAPI_(Int32) Read();
    
    virtual CARAPI_(Int32) Read(
        /* [in] */ ArrayOf<Byte>* b);

    virtual CARAPI_(Int32) Read(
        /* [in] */ ArrayOf<Byte>* b, 
        /* [in] */ Int32 offset, 
        /* [in] */ Int32 length);

/*
    @Override
    public Int32 available() throws IOException {
        Int32 nsamples = (mIn - mOut + mInputStream.available()) / 2;
        return ((nsamples - mFirLength) * mRateOut / mRateIn) * 2;
    }
*/

    virtual CARAPI Close();

protected:
    virtual CARAPI_(void) Finalize();

private:
    //
    // fir filter code JNI interface
    //
    static CARAPI_(void) Fir21(
        /* [in] */ ArrayOf<Byte>* in, 
        /* [in] */ Int32 inOffset,
        /* [in] */ ArrayOf<Byte>* out, 
        /* [in] */ Int32 outOffset, 
        /* [in] */ Int32 npoints);

    static String TAG;// = "ResampleInputStream";

    // pcm input stream
    AutoPtr<IInputStream> mInputStream;

    // sample rates, assumed to be normalized
    Int32 mRateIn;
    Int32 mRateOut;

    // input pcm data
    ArrayOf<Byte>* mBuf;
    Int32 mBufCount;

    // length of 2:1 fir
    static const Int32 mFirLength = 29;

    // helper for bytewise read()
    ArrayOf<Byte>* mOneByte;// = new Byte[1];

};

#endif
