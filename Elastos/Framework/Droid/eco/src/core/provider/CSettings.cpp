
#include "ext/frameworkdef.h"
#include "provider/CSettings.h"


ECode CSettings::GetGTalkDeviceId(
	/* [in] */ Int64 androidId,
	/* [out] */ String* deviceId)
{
    VALIDATE_NOT_NULL(deviceId);
    return Settings::GetGTalkDeviceId(androidId, deviceId);
}
