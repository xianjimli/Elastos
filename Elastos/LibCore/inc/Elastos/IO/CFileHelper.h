
#ifndef __CFILEHELPER_H__
#define __CFILEHELPER_H__

#include "_CFileHelper.h"

CarClass(CFileHelper)
{
public:
    CARAPI CreateTempFile(
        /* [in] */ const String& prefix,
        /* [in] */ const String& suffix,
        /* [out] */ IFile** file);

    CARAPI CreateTempFileEx(
        /* [in] */ const String& prefix,
        /* [in] */ const String& suffix,
        /* [in] */ IFile* directory,
        /* [out] */ IFile** file);
};

#endif //__CFILEHELPER_H__