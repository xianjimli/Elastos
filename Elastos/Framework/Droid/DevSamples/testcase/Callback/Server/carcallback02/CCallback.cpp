#include "CCallback.h"
#include <stdio.h>

ECode CCallback::Hello()
{
    m_nCount += 0x1;
    return NOERROR;
}

ECode CCallback::Bar()
{
    Callback::OnTrigger();
    Callback::OnTriggerLocal();
    return NOERROR;
}

ECode CCallback::Foo()
{
    m_nCount += 0x10000;
    Callback::OnTriggerLocal();
    return NOERROR;
}

ECode CCallback::GetCount(Int32* pnCount)
{
    if (!pnCount) {
        printf("ERROR! E_INVALID_POINTER!\n");
        return E_INVALID_POINTER;
    }
    *pnCount = m_nCount;
    return NOERROR;
}
