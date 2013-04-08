
#ifndef __CAMRINPUTSTREAM_H__
#define __CAMRINPUTSTREAM_H__

#include "_CAmrInputStream.h"
#include <elastos/AutoPtr.h>

CarClass(CAmrInputStream)
{
public:
    CAmrInputStream();

    ~CAmrInputStream();

    /**
     * Create a new AmrInputStream, which converts 16 bit PCM to AMR
     * @param inputStream InputStream containing 16 bit PCM.
     */
    CARAPI constructor(
        /* [in] */ IInputStream* inputStream);

    CARAPI Available(
        /* [out] */ Int32* number);

    CARAPI Close();

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean* isSupported);

    CARAPI Read(
        /* [out] */ Int32* value);

    CARAPI ReadBuffer(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    CARAPI ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64 * pNumber);

private:
    //
    // AudioRecord JNI interface
    //
    static CARAPI GsmAmrEncoderNew(
        /* [out] */ Int32* gae);

    static CARAPI GsmAmrEncoderInitialize(
        /* [in] */ Int32 gae);

    static CARAPI GsmAmrEncoderEncode(
        /* [in] */ Int32 gae,
        /* [in] */ ArrayOf<Byte>* pcm,
        /* [in] */ Int32 pcmOffset,
        /* [in] */ ArrayOf<Byte>* amr,
        /* [in] */ Int32 amrOffset,
        /* [out] */ Int32* number);

    static CARAPI_(void) GsmAmrEncoderCleanup(
        /* [in] */ Int32 gae);

    static CARAPI_(void) GsmAmrEncoderDelete(
        /* [in] */ Int32 gae);

private:
    static CString TAG;

    // frame is 20 msec at 8.000 khz
    static const Int32 SAMPLES_PER_FRAME = 8000 * 20 / 1000;

    // pcm input stream
    AutoPtr<IInputStream> mInputStream;

    // native handle
    Int32 mGae;

    // result amr stream
    ArrayOf<Byte>* mBuf;

    Int32 mBufIn;

    Int32 mBufOut;

    // helper for bytewise read()
    ArrayOf_<Byte, 1> mOneByte;
};

#endif // __CAMRINPUTSTREAM_H__
