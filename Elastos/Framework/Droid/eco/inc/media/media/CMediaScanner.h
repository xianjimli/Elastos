
#ifndef __CMEDIASCANNER_H__
#define __CMEDIASCANNER_H__

#include "_CMediaScanner.h"

#include "media/MediaScanner.h"

CarClass(CMediaScanner), public MediaScanner
{
public:
    CARAPI ScanDirectories(
        /* [in] */ const ArrayOf<String> & directories,
        /* [in] */ const String& volumeName);

    CARAPI ScanSingleFile(
        /* [in] */ const String& path,
        /* [in] */ const String& volumeName,
        /* [in] */ const String& mimeType,
        /* [out] */ IUri ** ppUri);

    CARAPI ExtractAlbumArt(
        /* [in] */ IFileDescriptor* fd,
        /* [out, callee] */ ArrayOf<Byte>** albumArt);

    CARAPI constructor(
        /* [in] */ IContext * pC);

private:
    // TODO: Add your private member variables here.
};

#endif // __CMEDIASCANNER_H__
