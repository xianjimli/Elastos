
#ifndef __DECODERCAPABILITIES_H__
#define __DECODERCAPABILITIES_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

/**
 * {@hide}
 *
 * The DecoderCapabilities class is used to retrieve the types of the
 * video and audio decoder(s) supported on a specific Android platform.
 */
class DecoderCapabilities
{
public:
    /**
     * The VideoDecoder class represents the type of a video decoder
     *
     */
    enum VideoDecoder {
        VIDEO_DECODER_WMV,
    };

    /**
     * The AudioDecoder class represents the type of an audio decoder
     */
    enum AudioDecoder {
        AUDIO_DECODER_WMA,
    };

    /*static {
        System.loadLibrary("media_jni");
        native_init();
    }*/

    /**
     * Returns the list of video decoder types
     * @see android.media.DecoderCapabilities.VideoDecoder
     */
    static CARAPI_(ArrayOf<VideoDecoder>*) GetVideoDecoders();

    /**
     * Returns the list of audio decoder types
     * @see android.media.DecoderCapabilities.AudioDecoder
     */
    static CARAPI_(ArrayOf<AudioDecoder>*) GetAudioDecoders();

private:
    DecoderCapabilities();

    // Implemented by JNI
    static CARAPI_(void) native_init();

    static CARAPI_(Int32) native_get_num_video_decoders();

    static CARAPI_(Int32) native_get_video_decoder_type(
        /* [in] */ Int32 index);

    static CARAPI_(Int32) native_get_num_audio_decoders();

    static CARAPI_(Int32) native_get_audio_decoder_type(
        /* [in] */ Int32 index);
};

#endif
