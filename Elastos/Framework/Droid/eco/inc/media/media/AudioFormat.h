
#ifndef __AUDIOFORMAT_H__
#define __AUDIOFORMAT_H__

/**
 * The AudioFormat class is used to access a number of audio format and
 * channel configuration constants. They are for instance used
 * in {@link AudioTrack} and {@link AudioRecord}.
 * 
 */
class AudioFormat 
{
public:

    //---------------------------------------------------------
    // Constants
    //--------------------
    /** Invalid audio data format */
    static const Int32 ENCODING_INVALID = 0;

    /** Default audio data format */
    static const Int32 ENCODING_DEFAULT = 1;

    /** Audio data format: PCM 16 bit per sample */
    static const Int32 ENCODING_PCM_16BIT = 2; // accessed by native code

    /** Audio data format: PCM 8 bit per sample */
    static const Int32 ENCODING_PCM_8BIT = 3;  // accessed by native code

    /** Invalid audio channel configuration */
    /** @deprecated use CHANNEL_INVALID instead  */
    static const Int32 CHANNEL_CONFIGURATION_INVALID   = 0;

    /** Default audio channel configuration */
    /** @deprecated use CHANNEL_OUT_DEFAULT or CHANNEL_IN_DEFAULT instead  */
    static const Int32 CHANNEL_CONFIGURATION_DEFAULT   = 1;

    /** Mono audio configuration */
    /** @deprecated use CHANNEL_OUT_MONO or CHANNEL_IN_MONO instead  */
    static const Int32 CHANNEL_CONFIGURATION_MONO      = 2;

    /** Stereo (2 channel) audio configuration */
    /** @deprecated use CHANNEL_OUT_STEREO or CHANNEL_IN_STEREO instead  */
    static const Int32 CHANNEL_CONFIGURATION_STEREO    = 3;


    /** Invalid audio channel mask */
    static const Int32 CHANNEL_INVALID = 0;

    /** Default audio channel mask */
    static const Int32 CHANNEL_OUT_DEFAULT = 1;

    // Channel mask definitions must be kept in sync with native values in include/media/AudioSystem.h
    static const Int32 CHANNEL_OUT_FRONT_LEFT = 0x4;
    static const Int32 CHANNEL_OUT_FRONT_RIGHT = 0x8;
    static const Int32 CHANNEL_OUT_FRONT_CENTER = 0x10;
    static const Int32 CHANNEL_OUT_LOW_FREQUENCY = 0x20;
    static const Int32 CHANNEL_OUT_BACK_LEFT = 0x40;
    static const Int32 CHANNEL_OUT_BACK_RIGHT = 0x80;
    static const Int32 CHANNEL_OUT_FRONT_LEFT_OF_CENTER = 0x100;
    static const Int32 CHANNEL_OUT_FRONT_RIGHT_OF_CENTER = 0x200;
    static const Int32 CHANNEL_OUT_BACK_CENTER = 0x400;
    static const Int32 CHANNEL_OUT_MONO = CHANNEL_OUT_FRONT_LEFT;
    static const Int32 CHANNEL_OUT_STEREO = (CHANNEL_OUT_FRONT_LEFT | CHANNEL_OUT_FRONT_RIGHT);
    static const Int32 CHANNEL_OUT_QUAD = (CHANNEL_OUT_FRONT_LEFT | CHANNEL_OUT_FRONT_RIGHT |
            CHANNEL_OUT_BACK_LEFT | CHANNEL_OUT_BACK_RIGHT);
    static const Int32 CHANNEL_OUT_SURROUND = (CHANNEL_OUT_FRONT_LEFT | CHANNEL_OUT_FRONT_RIGHT |
            CHANNEL_OUT_FRONT_CENTER | CHANNEL_OUT_BACK_CENTER);
    static const Int32 CHANNEL_OUT_5POINT1 = (CHANNEL_OUT_FRONT_LEFT | CHANNEL_OUT_FRONT_RIGHT |
            CHANNEL_OUT_FRONT_CENTER | CHANNEL_OUT_LOW_FREQUENCY | CHANNEL_OUT_BACK_LEFT | CHANNEL_OUT_BACK_RIGHT);
    static const Int32 CHANNEL_OUT_7POINT1 = (CHANNEL_OUT_FRONT_LEFT | CHANNEL_OUT_FRONT_RIGHT |
            CHANNEL_OUT_FRONT_CENTER | CHANNEL_OUT_LOW_FREQUENCY | CHANNEL_OUT_BACK_LEFT | CHANNEL_OUT_BACK_RIGHT |
            CHANNEL_OUT_FRONT_LEFT_OF_CENTER | CHANNEL_OUT_FRONT_RIGHT_OF_CENTER);

    static const Int32 CHANNEL_IN_DEFAULT = 1;
    static const Int32 CHANNEL_IN_LEFT = 0x4;
    static const Int32 CHANNEL_IN_RIGHT = 0x8;
    static const Int32 CHANNEL_IN_FRONT = 0x10;
    static const Int32 CHANNEL_IN_BACK = 0x20;
    static const Int32 CHANNEL_IN_LEFT_PROCESSED = 0x40;
    static const Int32 CHANNEL_IN_RIGHT_PROCESSED = 0x80;
    static const Int32 CHANNEL_IN_FRONT_PROCESSED = 0x100;
    static const Int32 CHANNEL_IN_BACK_PROCESSED = 0x200;
    static const Int32 CHANNEL_IN_PRESSURE = 0x400;
    static const Int32 CHANNEL_IN_X_AXIS = 0x800;
    static const Int32 CHANNEL_IN_Y_AXIS = 0x1000;
    static const Int32 CHANNEL_IN_Z_AXIS = 0x2000;
    static const Int32 CHANNEL_IN_VOICE_UPLINK = 0x4000;
    static const Int32 CHANNEL_IN_VOICE_DNLINK = 0x8000;
    static const Int32 CHANNEL_IN_MONO = CHANNEL_IN_FRONT;
    static const Int32 CHANNEL_IN_STEREO = (CHANNEL_IN_LEFT | CHANNEL_IN_RIGHT);
};

#endif
