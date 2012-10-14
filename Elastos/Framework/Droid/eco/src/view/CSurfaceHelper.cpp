
#include "view/CSurfaceHelper.h"
#include <surfaceflinger/SurfaceComposerClient.h>

ECode CSurfaceHelper::OpenTransaction()
{
    android::SurfaceComposerClient::openGlobalTransaction();
    return NOERROR;
}

ECode CSurfaceHelper::CloseTransaction()
{
    android::SurfaceComposerClient::closeGlobalTransaction();
    return NOERROR;
}