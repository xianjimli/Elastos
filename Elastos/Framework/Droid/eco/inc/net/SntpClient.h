
#ifndef __SNTPCLIENT_H__
#define __SNTPCLIENT_H__

#include "ext/frameworkext.h"

using namespace Elastos;

/**
 * {@hide}
 *
 * Simple SNTP client class for retrieving network time.
 *
 * Sample usage:
 * <pre>SntpClient client = new SntpClient();
 * if (client.requestTime("time.foo.com")) {
 *     long now = client.getNtpTime() + SystemClock.elapsedRealtime() - client.getNtpTimeReference();
 * }
 * </pre>
 */
class SntpClient
{
private:
    static const CString TAG;

    static const Int32 REFERENCE_TIME_OFFSET = 16;
    static const Int32 ORIGINATE_TIME_OFFSET = 24;
    static const Int32 RECEIVE_TIME_OFFSET = 32;
    static const Int32 TRANSMIT_TIME_OFFSET = 40;
    static const Int32 NTP_PACKET_SIZE = 48;

    static const Int32 NTP_PORT = 123;
    static const Int32 NTP_MODE_CLIENT = 3;
    static const Int32 NTP_VERSION = 3;

    // Number of seconds between Jan 1, 1900 and Jan 1, 1970
    // 70 years plus 17 leap days
    static const Int64 OFFSET_1900_TO_1970 = ((365L * 70L) + 17L) * 24L * 60L * 60L;

public:
    SntpClient();

    /**
     * Sends an SNTP request to the given host and processes the response.
     *
     * @param host host name of the server.
     * @param timeout network timeout in milliseconds.
     * @return true if the transaction was successful.
     */
    CARAPI_(Boolean) RequestTime(
        /* [in] */ String host,
        /* [in] */ Int32 timeout);

    /**
     * Returns the time computed from the NTP transaction.
     *
     * @return time value computed from NTP server response.
     */
    CARAPI_(Int64) GetNtpTime();

    /**
     * Returns the reference clock value (value of SystemClock.elapsedRealtime())
     * corresponding to the NTP time.
     *
     * @return reference clock corresponding to the NTP time.
     */
    CARAPI_(Int64) GetNtpTimeReference();

    /**
     * Returns the round trip time of the NTP transaction
     *
     * @return round trip time in milliseconds.
     */
    CARAPI_(Int64) GetRoundTripTime();

    /**
     * Reads an unsigned 32 bit big endian number from the given offset in the buffer.
     */
    CARAPI_(Int64) Read32(
        /* [in] */ ArrayOf<Byte> buffer,
        /* [in] */ Int32 offset);

    /**
     * Reads the NTP time stamp at the given offset in the buffer and returns
     * it as a system time (milliseconds since January 1, 1970).
     */
    CARAPI_(Int64) ReadTimeStamp(
        /* [in] */ ArrayOf<Byte> buffer,
        /* [in] */ Int32 offset);

    /**
     * Writes system time (milliseconds since January 1, 1970) as an NTP time stamp
     * at the given offset in the buffer.
     */
    CARAPI_(void) WriteTimeStamp(
        /* [in] */ ArrayOf<Byte> buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int64 time);

private:
    // system time computed from NTP server response
    Int64 mNtpTime;

    // value of SystemClock.elapsedRealtime() corresponding to mNtpTime
    Int64 mNtpTimeReference;

    // round trip time in milliseconds
    Int64 mRoundTripTime;
};

#endif //__SntpClient_H__
