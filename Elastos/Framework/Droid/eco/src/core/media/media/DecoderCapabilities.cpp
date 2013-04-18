
#include "media/DecoderCapabilities.h"

/**
 * Returns the list of video decoder types
 * @see android.media.DecoderCapabilities.VideoDecoder
 */
ArrayOf<DecoderCapabilities::VideoDecoder>* DecoderCapabilities::GetVideoDecoders()
{
    ArrayOf<VideoDecoder>* decoderList = ArrayOf<VideoDecoder>::Alloc(1);
    Int32 nDecoders = native_get_num_video_decoders();
    for (Int32 i = 0; i < nDecoders; ++i) {
        //decoderList->Add(VideoDecoder[native_get_video_decoder_type(i)]);
    }
    return decoderList;
}

/**
 * Returns the list of audio decoder types
 * @see android.media.DecoderCapabilities.AudioDecoder
 */
ArrayOf<DecoderCapabilities::AudioDecoder>* DecoderCapabilities::GetAudioDecoders()
{
    ArrayOf<AudioDecoder>* decoderList = ArrayOf<AudioDecoder>::Alloc(1);
    Int32 nDecoders = native_get_num_audio_decoders();
    for (Int32 i = 0; i < nDecoders; ++i) {
        //decoderList->Add(AudioDecoder[native_get_audio_decoder_type(i)]);
    }
    return decoderList;
}

DecoderCapabilities::DecoderCapabilities()
{}  // Don't call me

// Implemented by JNI
void DecoderCapabilities::native_init()
{

}

Int32 DecoderCapabilities::native_get_num_video_decoders()
{
    return 0;
}

Int32 DecoderCapabilities::native_get_video_decoder_type(
    /* [in] */ Int32 index)
{
    return 0;
}

Int32 DecoderCapabilities::native_get_num_audio_decoders()
{
    return 0;
}

Int32 DecoderCapabilities::native_get_audio_decoder_type(
    /* [in] */ Int32 index)
{
    return 0;
}

