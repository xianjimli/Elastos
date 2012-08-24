
#include "server/ConnectionRecord.h"

ConnectionRecord::ConnectionRecord(
    /* [in] */ AppBindRecord* binding,
    /* [in] */ CActivityRecord* activity,
    /* [in] */ IServiceConnectionInner* conn,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 clientLabel,
    /* [in] */ CPendingIntent* clientIntent) :
    mBinding(binding),
    mActivity(activity),
    mConn(conn),
    mFlags(flags),
    mClientLabel(clientLabel),
    mClientIntent(clientIntent)
{}

ECode ConnectionRecord::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}
