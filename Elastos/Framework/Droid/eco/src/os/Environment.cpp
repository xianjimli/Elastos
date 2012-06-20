
#include "os/Environment.h"

ECode Environment::GetRootDirectory(
    /* [out] */ IFile** file)
{
    assert(file);

    // TODO:
    return CFile::New("/system", file);
}

ECode Environment::GetDataDirectory(
    /* [out] */ IFile** file)
{
    assert(file);

    // TODO:
    return CFile::New("/data", file);
}
