
#ifndef __ACTIVITYRESULT_H__
#define __ACTIVITYRESULT_H__

#include "ext/frameworkext.h"
#include "server/CActivityRecord.h"

class CActivityRecord;

class ActivityResult
{
public:
    ActivityResult(
        /* [in] */ CActivityRecord* from,
        /* [in] */ const String& resultWho,
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* data);

public:
    AutoPtr<IResultInfo> mResultInfo;
    AutoPtr<CActivityRecord> mFrom;
};

#endif //__ACTIVITYRESULT_H__
