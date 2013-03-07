
#include "media/ToneGenerator.h"


ToneGenerator::ToneGenerator()
{

}

/**
 * ToneGenerator class contructor specifying output stream type and volume.
 *
 * @param streamType The streame type used for tone playback (e.g. STREAM_MUSIC).
 * @param volume     The volume of the tone, given in percentage of maximum volume (from 0-100).
 *
 */
ToneGenerator::ToneGenerator(
    /* [in] */ Int32 streamType, 
    /* [in] */ Int32 volume) 
{
    native_setup(streamType, volume);
}

/**
 * This method starts the playback of a tone of the specified type.
 * only one tone can play at a time: if a tone is playing while this method is called,
 * this tone is stopped and replaced by the one requested.
 * @param toneType   The type of tone generated chosen from the following list:
 * <ul>
 * <li>{@link #TONE_DTMF_0}
 * <li>{@link #TONE_DTMF_1}
 * <li>{@link #TONE_DTMF_2}
 * <li>{@link #TONE_DTMF_3}
 * <li>{@link #TONE_DTMF_4}
 * <li>{@link #TONE_DTMF_5}
 * <li>{@link #TONE_DTMF_6}
 * <li>{@link #TONE_DTMF_7}
 * <li>{@link #TONE_DTMF_8}
 * <li>{@link #TONE_DTMF_9}
 * <li>{@link #TONE_DTMF_A}
 * <li>{@link #TONE_DTMF_B}
 * <li>{@link #TONE_DTMF_C}
 * <li>{@link #TONE_DTMF_D}
 * <li>{@link #TONE_SUP_DIAL}
 * <li>{@link #TONE_SUP_BUSY}
 * <li>{@link #TONE_SUP_CONGESTION}
 * <li>{@link #TONE_SUP_RADIO_ACK}
 * <li>{@link #TONE_SUP_RADIO_NOTAVAIL}
 * <li>{@link #TONE_SUP_ERROR}
 * <li>{@link #TONE_SUP_CALL_WAITING}
 * <li>{@link #TONE_SUP_RINGTONE}
 * <li>{@link #TONE_PROP_BEEP}
 * <li>{@link #TONE_PROP_ACK}
 * <li>{@link #TONE_PROP_NACK}
 * <li>{@link #TONE_PROP_PROMPT}
 * <li>{@link #TONE_PROP_BEEP2}
 * <li>{@link #TONE_SUP_INTERCEPT}
 * <li>{@link #TONE_SUP_INTERCEPT_ABBREV}
 * <li>{@link #TONE_SUP_CONGESTION_ABBREV}
 * <li>{@link #TONE_SUP_CONFIRM}
 * <li>{@link #TONE_SUP_PIP}
 * <li>{@link #TONE_CDMA_DIAL_TONE_LITE}
 * <li>{@link #TONE_CDMA_NETWORK_USA_RINGBACK}
 * <li>{@link #TONE_CDMA_INTERCEPT}
 * <li>{@link #TONE_CDMA_ABBR_INTERCEPT}
 * <li>{@link #TONE_CDMA_REORDER}
 * <li>{@link #TONE_CDMA_ABBR_REORDER}
 * <li>{@link #TONE_CDMA_NETWORK_BUSY}
 * <li>{@link #TONE_CDMA_CONFIRM}
 * <li>{@link #TONE_CDMA_ANSWER}
 * <li>{@link #TONE_CDMA_NETWORK_CALLWAITING}
 * <li>{@link #TONE_CDMA_PIP}
 * <li>{@link #TONE_CDMA_CALL_SIGNAL_ISDN_NORMAL}
 * <li>{@link #TONE_CDMA_CALL_SIGNAL_ISDN_INTERGROUP}
 * <li>{@link #TONE_CDMA_CALL_SIGNAL_ISDN_SP_PRI}
 * <li>{@link #TONE_CDMA_CALL_SIGNAL_ISDN_PAT3}
 * <li>{@link #TONE_CDMA_CALL_SIGNAL_ISDN_PING_RING}
 * <li>{@link #TONE_CDMA_CALL_SIGNAL_ISDN_PAT5}
 * <li>{@link #TONE_CDMA_CALL_SIGNAL_ISDN_PAT6}
 * <li>{@link #TONE_CDMA_CALL_SIGNAL_ISDN_PAT7}
 * <li>{@link #TONE_CDMA_HIGH_L}
 * <li>{@link #TONE_CDMA_MED_L}
 * <li>{@link #TONE_CDMA_LOW_L}
 * <li>{@link #TONE_CDMA_HIGH_SS}
 * <li>{@link #TONE_CDMA_MED_SS}
 * <li>{@link #TONE_CDMA_LOW_SS}
 * <li>{@link #TONE_CDMA_HIGH_SSL}
 * <li>{@link #TONE_CDMA_MED_SSL}
 * <li>{@link #TONE_CDMA_LOW_SSL}
 * <li>{@link #TONE_CDMA_HIGH_SS_2}
 * <li>{@link #TONE_CDMA_MED_SS_2}
 * <li>{@link #TONE_CDMA_LOW_SS_2}
 * <li>{@link #TONE_CDMA_HIGH_SLS}
 * <li>{@link #TONE_CDMA_MED_SLS}
 * <li>{@link #TONE_CDMA_LOW_SLS}
 * <li>{@link #TONE_CDMA_HIGH_S_X4}
 * <li>{@link #TONE_CDMA_MED_S_X4}
 * <li>{@link #TONE_CDMA_LOW_S_X4}
 * <li>{@link #TONE_CDMA_HIGH_PBX_L}
 * <li>{@link #TONE_CDMA_MED_PBX_L}
 * <li>{@link #TONE_CDMA_LOW_PBX_L}
 * <li>{@link #TONE_CDMA_HIGH_PBX_SS}
 * <li>{@link #TONE_CDMA_MED_PBX_SS}
 * <li>{@link #TONE_CDMA_LOW_PBX_SS}
 * <li>{@link #TONE_CDMA_HIGH_PBX_SSL}
 * <li>{@link #TONE_CDMA_MED_PBX_SSL}
 * <li>{@link #TONE_CDMA_LOW_PBX_SSL}
 * <li>{@link #TONE_CDMA_HIGH_PBX_SLS}
 * <li>{@link #TONE_CDMA_MED_PBX_SLS}
 * <li>{@link #TONE_CDMA_LOW_PBX_SLS}
 * <li>{@link #TONE_CDMA_HIGH_PBX_S_X4}
 * <li>{@link #TONE_CDMA_MED_PBX_S_X4}
 * <li>{@link #TONE_CDMA_LOW_PBX_S_X4}
 * <li>{@link #TONE_CDMA_ALERT_NETWORK_LITE}
 * <li>{@link #TONE_CDMA_ALERT_AUTOREDIAL_LITE}
 * <li>{@link #TONE_CDMA_ONE_MIN_BEEP}
 * <li>{@link #TONE_CDMA_KEYPAD_VOLUME_KEY_LITE}
 * <li>{@link #TONE_CDMA_PRESSHOLDKEY_LITE}
 * <li>{@link #TONE_CDMA_ALERT_INCALL_LITE}
 * <li>{@link #TONE_CDMA_EMERGENCY_RINGBACK}
 * <li>{@link #TONE_CDMA_ALERT_CALL_GUARD}
 * <li>{@link #TONE_CDMA_SOFT_ERROR_LITE}
 * <li>{@link #TONE_CDMA_CALLDROP_LITE}
 * <li>{@link #TONE_CDMA_NETWORK_BUSY_ONE_SHOT}
 * <li>{@link #TONE_CDMA_ABBR_ALERT}
 * <li>{@link #TONE_CDMA_SIGNAL_OFF}
 * </ul>
 * @see #ToneGenerator(Int32, Int32)
*/
Boolean ToneGenerator::StartTone(
    /* [in] */ Int32 toneType) 
{
    return StartTone(toneType, -1);
}

/**
 * This method starts the playback of a tone of the specified type for the specified duration.
 * @param toneType   The type of tone generated @see {@link #startTone(Int32)}.
 * @param durationMs  The tone duration in milliseconds. If the tone is limited in time by definition,
 * the actual duration will be the minimum of durationMs and the defined tone duration. Setting durationMs to -1,
 * is equivalent to calling {@link #startTone(Int32)}.
*/
Boolean ToneGenerator::StartTone(
    /* [in] */ Int32 toneType, 
    /* [in] */ Int32 durationMs)
{
    return FALSE;
}

/**
 * This method stops the tone currently playing playback.
 * @see #ToneGenerator(Int32, Int32)
 */
void ToneGenerator::StopTone()
{

}

/**
 * Releases resources associated with this ToneGenerator object. It is good
 * practice to call this method when you're done using the ToneGenerator.
 */
void ToneGenerator::Release()
{

}

void ToneGenerator::native_setup(
    /* [in] */ Int32 streamType, 
    /* [in] */ Int32 volume)
{

}

void ToneGenerator::native_finalize()
{

}

void ToneGenerator::Finalize() 
{ 
    native_finalize(); 
}


