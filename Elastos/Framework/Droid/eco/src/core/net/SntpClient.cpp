
#include "net/SntpClient.h"

const CString SntpClient::TAG = "SntpClient";
const Int32 SntpClient::REFERENCE_TIME_OFFSET;
const Int32 SntpClient::ORIGINATE_TIME_OFFSET;
const Int32 SntpClient::RECEIVE_TIME_OFFSET;
const Int32 SntpClient::TRANSMIT_TIME_OFFSET;
const Int32 SntpClient::NTP_PACKET_SIZE;
const Int32 SntpClient::NTP_PORT;
const Int32 SntpClient::NTP_MODE_CLIENT;
const Int32 SntpClient::NTP_VERSION;
const Int64 SntpClient::OFFSET_1900_TO_1970;

SntpClient::SntpClient()
    : mNtpTime(0)
    , mNtpTimeReference(0)
    , mRoundTripTime(0)
{
}

Boolean SntpClient::RequestTime(
    /* [in] */ String host,
    /* [in] */ Int32 timeout)
{
    return FALSE;
}

Int64 SntpClient::GetNtpTime()
{
    return 0;
}

Int64 SntpClient::GetNtpTimeReference()
{
    return 0;
}

Int64 SntpClient::GetRoundTripTime()
{
    return 0;
}

Int64 SntpClient::Read32(
    /* [in] */ ArrayOf<Byte> buffer,
    /* [in] */ Int32 offset)
{
    return 0;
}

Int64 SntpClient::ReadTimeStamp(
    /* [in] */ ArrayOf<Byte> buffer,
    /* [in] */ Int32 offset)
{
    return 0;
}

void SntpClient::WriteTimeStamp(
    /* [in] */ ArrayOf<Byte> buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int64 time)
{
}
