
#ifndef __ACTIVITYRESULT_H__
#define __ACTIVITYRESULT_H__

#include "app/CResultInfo.h"
#include "server/CActivityRecord.h"

class CActivityRecord;

class ActivityResult
{
public:
    ActivityResult(
        /* [in] */ CActivityRecord* from,
        /* [in] */ String resultWho,
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* data);

public:
    AutoPtr<CResultInfo> mResultInfo;
    AutoPtr<CActivityRecord> mFrom;
};

#endif //__ACTIVITYRESULT_H__
