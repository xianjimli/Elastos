
#include "CTestUnit.h"

CTestUnit::CTestUnit()
{
    IInterface* pOrgCallbackContext;

    _Impl_CallbackSink_InitCallbackContext(&m_pCallbackContext);
    pOrgCallbackContext = (PInterface)pthread_getspecific(TL_CALLBACK_SLOT);
    if (NULL != pOrgCallbackContext) {
        pOrgCallbackContext->Release();
    }
    pthread_setspecific(TL_CALLBACK_SLOT, m_pCallbackContext);
}

CTestUnit::~CTestUnit()
{
    
}

ECode CTestUnit::Start(BufferOf<String> &args)
{
    ECode ec = NOERROR;
    
    ec = Run(args);
    
    if (SUCCEEDED(ec)) {
        ec = EnterEventLoop();
    }
    
    return ec;
}

ECode CTestUnit::EnterEventLoop()
{
    return _Impl_CallbackSink_TryToHandleEvents(m_pCallbackContext);
}

ECode CTestUnit::Finish(Int32 flag)
{
    return _Impl_CallbackSink_RequestToFinish(m_pCallbackContext, flag);   
}
