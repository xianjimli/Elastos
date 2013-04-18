
#ifndef __CRESAMPLEINPUTSTREAM_H__
#define __CRESAMPLEINPUTSTREAM_H__

#include "_CResampleInputStream.h"
#include <elastos/AutoPtr.h>

#define FIR_COEF(coef) (Int16)(0x10000 * coef)
static const Int16 fir21[] = {
    FIR_COEF(-0.006965742326),
    FIR_COEF(-0.008428945737),
    FIR_COEF( 0.004241280174),
    FIR_COEF( 0.022141096893),
    FIR_COEF( 0.018765669437),
    FIR_COEF(-0.009871891152),
    FIR_COEF(-0.024842433247),
    FIR_COEF( 0.006121772058),
    FIR_COEF( 0.045890841611),
    FIR_COEF( 0.021573503509),
    FIR_COEF(-0.059681984668),
    FIR_COEF(-0.076036275138),
    FIR_COEF( 0.072405390275),
    FIR_COEF( 0.308255674582),
    FIR_COEF( 0.424321210495),
    FIR_COEF( 0.308255674582),
    FIR_COEF( 0.072405390275),
    FIR_COEF(-0.076036275138),
    FIR_COEF(-0.059681984668),
    FIR_COEF( 0.021573503509),
    FIR_COEF( 0.045890841611),
    FIR_COEF( 0.006121772058),
    FIR_COEF(-0.024842433247),
    FIR_COEF(-0.009871891152),
    FIR_COEF( 0.018765669437),
    FIR_COEF( 0.022141096893),
    FIR_COEF( 0.004241280174),
    FIR_COEF(-0.008428945737),
    FIR_COEF(-0.006965742326)
};

CarClass(CResampleInputStream)
{
public:

    /**
     * Create a new ResampleInputStream, which converts the sample rate
     * @param inputStream InputStream containing 16 bit PCM.
     * @param rateIn the input sample rate.
     * @param rateOut the output sample rate.
     * This only handles rateIn == rateOut / 2 for the moment.
     */
    CResampleInputStream();

    ~CResampleInputStream();

    CARAPI Available(
        /* [out] */ Int32 * pNumber);

    CARAPI Close();

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean * supported);

    CARAPI Read(
        /* [out] */ Int32 * value);

    CARAPI ReadBuffer(
        /* [out] */ ArrayOf<Byte> * buffer,
        /* [out] */ Int32 * number);

    CARAPI ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ ArrayOf<Byte> * buffer,
        /* [out] */ Int32 * number);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64 * pNumber);

    CARAPI constructor(
        /* [in] */ IInputStream * inputStream,
        /* [in] */ Int32 rateIn,
        /* [in] */ Int32 rateOut);

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

    static CString TAG;// = "ResampleInputStream";

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
    ArrayOf_<Byte, 1> mOneByte;

    static const Int32 nFir21 = sizeof(fir21) / sizeof(fir21[0]);

    static const Int32 BUF_SIZE = 2048;
};

#endif // __CRESAMPLEINPUTSTREAM_H__
