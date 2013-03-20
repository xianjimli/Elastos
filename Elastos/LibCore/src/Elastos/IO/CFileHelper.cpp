
#include "CFileHelper.h"
#include "File.h"

ECode CFileHelper::CreateTempFile(
    /* [in] */ const String& prefix,
    /* [in] */ const String& suffix,
    /* [out] */ IFile** file)
{
    return File::CreateTempFile(prefix, suffix, file);
}

ECode CFileHelper::CreateTempFileEx(
    /* [in] */ const String& prefix,
    /* [in] */ const String& suffix,
    /* [in] */ IFile* directory,
    /* [out] */ IFile** file)
{
	return File::CreateTempFileEx(prefix, suffix, directory, file);
}