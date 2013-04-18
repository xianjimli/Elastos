
#include "media/EncoderCapabilities.h"

/**
 * The VideoEncoderCap class represents a video encoder's
 * supported parameter range in:
 *
 * <ul>
 * <li>Resolution: the frame size (width/height) in pixels;
 * <li>Bit rate: the compressed output bit rate in bits per second;
 * <li>Frame rate: the output number of frames per second.
 * </ul>
 *
 */
EncoderCapabilities::VideoEncoderCap::VideoEncoderCap(
    /* [in] */ Int32 codec,
    /* [in] */ Int32 minBitRate, 
    /* [in] */ Int32 maxBitRate,
    /* [in] */ Int32 minFrameRate, 
    /* [in] */ Int32 maxFrameRate,
    /* [in] */ Int32 minFrameWidth, 
    /* [in] */ Int32 maxFrameWidth,
    /* [in] */ Int32 minFrameHeight, 
    /* [in] */ Int32 maxFrameHeight)
{
        mCodec = codec;
        mMinBitRate = minBitRate;
        mMaxBitRate = maxBitRate;
        mMinFrameRate = minFrameRate;
        mMaxFrameRate = maxFrameRate;
        mMinFrameWidth = minFrameWidth;
        mMaxFrameWidth = maxFrameWidth;
        mMinFrameHeight = minFrameHeight;
        mMaxFrameHeight = maxFrameHeight;
}



// Private constructor called by JNI
EncoderCapabilities::AudioEncoderCap::AudioEncoderCap(
    /* [in] */ Int32 codec,
    /* [in] */ Int32 minBitRate, 
    /* [in] */ Int32 maxBitRate,
    /* [in] */ Int32 minSampleRate, 
    /* [in] */ Int32 maxSampleRate,
    /* [in] */ Int32 minChannels, 
    /* [in] */ Int32 maxChannels)
{
       mCodec = codec;
       mMinBitRate = minBitRate;
       mMaxBitRate = maxBitRate;
       mMinSampleRate = minSampleRate;
       mMaxSampleRate = maxSampleRate;
       mMinChannels = minChannels;
       mMaxChannels = maxChannels;
};


/**
 * Returns the array of supported output file formats.
 * @see android.media.MediaRecorder.OutputFormat
 */
ArrayOf<Int32>* EncoderCapabilities::GetOutputFileFormats() 
{
    Int32 nFormats = native_get_num_file_formats();
    if (nFormats == 0) return NULL;

    ArrayOf<Int32>* formats = ArrayOf<Int32>::Alloc(nFormats);
    for (Int32 i = 0; i < nFormats; ++i) {
        (*formats)[i] = native_get_file_format(i);
    }
    return formats;
}

/**
 * Returns the capabilities of the supported video encoders.
 * @see android.media.EncoderCapabilities.VideoEncoderCap
 */
ArrayOf<EncoderCapabilities::VideoEncoderCap*>* EncoderCapabilities::GetVideoEncoders()
{
    Int32 nEncoders = native_get_num_video_encoders();
    if (nEncoders == 0) return NULL;

    ArrayOf<VideoEncoderCap*>* encoderList = ArrayOf<VideoEncoderCap*>::Alloc(1);
    for (Int32 i = 0; i < nEncoders; ++i) {
        //encoderList->Add(native_get_video_encoder_cap(i));
    }
    return encoderList;
}

/**
 * Returns the capabilities of the supported audio encoders.
 * @see android.media.EncoderCapabilities.AudioEncoderCap
 */
ArrayOf<EncoderCapabilities::AudioEncoderCap*>* EncoderCapabilities::GetAudioEncoders()
{
    Int32 nEncoders = native_get_num_audio_encoders();
    if (nEncoders == 0) return NULL;

    ArrayOf<AudioEncoderCap*>* encoderList = ArrayOf<AudioEncoderCap*>::Alloc(1);
    for (Int32 i = 0; i < nEncoders; ++i) {
        //encoderList->Add(native_get_audio_encoder_cap(i));
    }
    return encoderList;
}


EncoderCapabilities::EncoderCapabilities()
{}  // Don't call me

// Implemented by JNI
void EncoderCapabilities::native_init()
{

}

Int32 EncoderCapabilities::native_get_num_file_formats()
{
    return 0;
}

Int32 EncoderCapabilities::native_get_file_format(
    /* [in] */ Int32 index)
{
    return 0;
}

Int32 EncoderCapabilities::native_get_num_video_encoders()
{
    return 0;
}

EncoderCapabilities::VideoEncoderCap* EncoderCapabilities::native_get_video_encoder_cap(
    /* [in] */ Int32 index)
{
    return NULL;
}

Int32 EncoderCapabilities::native_get_num_audio_encoders()
{
    return 0;
}

EncoderCapabilities::AudioEncoderCap* EncoderCapabilities::native_get_audio_encoder_cap(
    /* [in] */ Int32 index)
{
    return NULL;
}

