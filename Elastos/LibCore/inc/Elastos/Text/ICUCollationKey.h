#ifndef __ICUCOLLATIONKEY_H__
#define __ICUCOLLATIONKEY_H__

#include "Elastos.Text_server.h"
#include "CollationKey.h"
#include <elastos/ElRefBase.h>

//extern "C" const InterfaceID EIID_ICUCollationKey;

class ICUCollationKey
    : public ElRefBase
    , public ICollationKey
    , public CollationKey
{
public:
    ICUCollationKey(
        /* [in] */ const String& source,
        /* [in] */ const ArrayOf<Byte>& bytes);

    ~ICUCollationKey();

    CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface* object,
        /* [out] */ InterfaceID* IID);

    CARAPI CompareTo(
        /* [in] */ ICollationKey* value,
        /* [out] */ Int32 * result);

    CARAPI GetSourceString(
        /* [out] */ String* sourceString);

    CARAPI GetByteArray(
        /* [out, callee] */ ArrayOf<Byte>** array);
private:
    /**
     * The key.
     */
    ArrayOf<Byte>* mBytes;
};
#endif //__ICUCOLLATIONKEY_H__