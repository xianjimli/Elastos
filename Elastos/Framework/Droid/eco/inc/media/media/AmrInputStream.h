
#ifndef __AMRINPUTSTREAM_H__
#define __AMRINPUTSTREAM_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>

/**
 * AmrInputStream
 * @hide
 */
class AmrInputStream
{    
    /*static {
        System.loadLibrary("media_jni");
    }*/
    
public:
    
    AmrInputStream();

    /**
     * Create a new AmrInputStream, which converts 16 bit PCM to AMR
     * @param inputStream InputStream containing 16 bit PCM.
     */
    AmrInputStream(
        /* [in] */ IInputStream* inputStream);

    virtual CARAPI_(Int32) Read();
    
    virtual CARAPI_(Int32) Read(
        /* [in] */ ArrayOf<Byte>* b);

    virtual CARAPI_(Int32) Read(
        /* [in] */ ArrayOf<Byte>* b, 
        /* [in] */ Int32 offset, 
        /* [in] */ Int32 length);

    virtual CARAPI Close();

protected:
    virtual CARAPI Finalize();
    
private:
    //
    // AudioRecord JNI interface
    //
    static CARAPI_(Int32) GsmAmrEncoderNew();

    static CARAPI_(void) GsmAmrEncoderInitialize(
        /* [in] */ Int32 gae);
    
    static CARAPI_(Int32) GsmAmrEncoderEncode(
        /* [in] */ Int32 gae,
        /* [in] */ ArrayOf<Byte>* pcm, 
        /* [in] */ Int32 pcmOffset, 
        /* [in] */ ArrayOf<Byte>* amr, 
        /* [in] */ Int32 amrOffset);
    
    static CARAPI_(void) GsmAmrEncoderCleanup(
        /* [in] */ Int32 gae);
    
    static CARAPI_(void) GsmAmrEncoderDelete(
        /* [in] */ Int32 gae);

    static String TAG;// = "AmrInputStream";

    // frame is 20 msec at 8.000 khz
    static const Int32 SAMPLES_PER_FRAME = 8000 * 20 / 1000;

    // pcm input stream
    AutoPtr<IInputStream> mInputStream;

    // native handle
    Int32 mGae;

    // result amr stream
    ArrayOf<Byte>* mBuf;// = new Byte[SAMPLES_PER_FRAME * 2];

    Int32 mBufIn;// = 0;

    Int32 mBufOut;// = 0;

    // helper for bytewise read()
    ArrayOf<Byte>* mOneByte;// = new Byte[1];

};

#endif
