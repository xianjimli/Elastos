
#include "cmdef.h"
#include "ContentHandler.h"
#include <Com.Kortide.Platform.h>
#include <elastos/AutoPtr.h>

ECode ContentHandler::GetContentEx(
    /* [in] */ IURLConnection* uConn,
    /* [in] */ const ArrayOf<IInterface*>& types,
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj);

    AutoPtr<IInterface> content;
    GetContent(uConn, (IInterface**)&content);
    for (Int32 i = 0; i < types.GetLength(); i++) {
        if (types[i] == content/*types[i].isInstance(content)*/) {
            *obj = content;
            return NOERROR;
        }
    }

    *obj = NULL;
    return NOERROR;
}
