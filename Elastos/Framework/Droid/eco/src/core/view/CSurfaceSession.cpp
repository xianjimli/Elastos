
#include "view/CSurfaceSession.h"

CSurfaceSession::CSurfaceSession()
{
    Init();
}

CSurfaceSession::~CSurfaceSession()
{
    Destroy();
}

ECode CSurfaceSession::Kill()
{
    if (mClient != NULL) {
        mClient->dispose();
        mClient = NULL;
    }

    return NOERROR;
}

ECode CSurfaceSession::Init()
{
    mClient = new android::SurfaceComposerClient();
    assert(mClient != NULL);
    return NOERROR;
}

ECode CSurfaceSession::Destroy()
{
    mClient =NULL;

    return NOERROR;
}
