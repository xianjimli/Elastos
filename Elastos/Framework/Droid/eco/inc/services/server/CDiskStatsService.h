
#ifndef __CDISKSTATSSERVICE_H__
#define __CDISKSTATSSERVICE_H__

#include "_CDiskStatsService.h"
#include <ext/frameworkext.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;
//using namespace Elastos::Core;

/**
 * This service exists only as a "dumpsys" target which reports
 * statistics about the status of the disk.
 */
CarClass(CDiskStatsService)
{
public:
    CARAPI constructor(
        /* [in] */ IContext * context);

    CARAPI GetDescription(
        /* [out] */ String * description);

protected:

    //@Override
    CARAPI_(void) Dump(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter *pw,
        /* [in] */ const ArrayOf<String> & args);

private:
    CARAPI_(void) ReportFreeSpace(
        /* [in] */ IFile* path,
        /* [in] */ const String & name,
        /* [in] */ IPrintWriter * pw);

    AutoPtr<IContext> mContext;
};

#endif //__CDISKSTATSSERVICE_H__
