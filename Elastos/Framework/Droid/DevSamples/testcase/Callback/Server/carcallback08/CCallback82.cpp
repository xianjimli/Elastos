#include "CCallback82.h"
#include "stdio.h"

ECode CCallback82::Hello82()
{
    Callback::OnTrigger81();
    return NOERROR;
}

ECode CCallback82::OnTrigger81()
{
    m_nCount++;
    return NOERROR;
}

ECode CCallback82::GetCount(Int32* pnCount)
{
    if (!pnCount) {
        printf("ERROR! E_INVALID_POINTER!\n");
        return E_INVALID_POINTER;
    }
    *pnCount = m_nCount;
    return NOERROR;
}
