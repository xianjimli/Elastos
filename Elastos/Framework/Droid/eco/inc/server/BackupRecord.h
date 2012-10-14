
#ifndef __BACKUPRECORD_H__
#define __BACKUPRECORD_H__

#include <elastos/AutoPtr.h>
//#include "content/CApplicationInfo.h"
#include "server/ProcessRecord.h"

class BackupRecord
{

public:
    String mStringName;                     // cached toString() output
    AutoPtr<IApplicationInfo> mAppInfo;         // information about BackupAgent's app
    Int32 mBackupMode;                  // full backup / incremental / restore
    ProcessRecord* mApp;                     // where this agent is running or null
};

#endif //__BACKUPRECORD_H__
