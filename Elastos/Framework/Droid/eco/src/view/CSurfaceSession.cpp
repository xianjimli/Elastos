
#include "view/CSurfaceSession.h"

CSurfaceSession::CSurfaceSession()
{
    Init();
}

ECode CSurfaceSession::Kill()
{
    return E_NOT_IMPLEMENTED;
}

ECode CSurfaceSession::Finalize()
{
    return Destroy();
}

ECode CSurfaceSession::Init()
{
    mClient = new android::SurfaceComposerClient();
    assert(mClient != NULL);
    return NOERROR;
}

ECode CSurfaceSession::Destroy()
{
    return E_NOT_IMPLEMENTED;
}
