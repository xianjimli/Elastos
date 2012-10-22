//
// Copyright (c) elastos.org
//

#include <elasysapi.h>
#include <stdlib.h>

_ELASTOS_NAMESPACE_USING
extern ECode ElastosMain(const BufferOf<String>& args);

int main(int argc, char *argv[])
{
	ECode ec;

    BufferOf<String> box((String *)argv, argc+1, argc);

    IInterface *pOrgCallbackContext;
    IInterface *m_pCallbackContext;

    _Impl_CallbackSink_InitCallbackContext(&m_pCallbackContext);
    pOrgCallbackContext = (PInterface)pthread_getspecific(TL_CALLBACK_SLOT);
    if (NULL != pOrgCallbackContext) {
        pOrgCallbackContext->Release();
    }
    pthread_setspecific(TL_CALLBACK_SLOT, m_pCallbackContext);

    ec = ElastosMain(box);

    ec = _Impl_CallbackSink_TryToHandleEvents(m_pCallbackContext);

    exit(ec);
}

