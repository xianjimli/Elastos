
#ifndef __HH_STRINGBLOCK_H
#define __HH_STRINGBLOCK_H

#include "utils/ResourceTypes.h"
#include "ext/frameworkdef.h"
#include <elastos.h>
#include <elastos/AutoPtr.h>
#include <elastos/Vector.h>
#include <elastos/Mutex.h>

using namespace Elastos;
using namespace Elastos::System::Threading;

class StringBlock
{
public:
    /**
     * Create from an existing string block native object.  This is
     * -extremely- dangerous -- only use it if you absolutely know what you
     *  are doing!  The given native object must exist for the entire lifetime
     *  of this newly creating StringBlock.
     */
    /*package*/ StringBlock(
        /* [in] */ const android::ResStringPool* pool,
        /* [in] */ Boolean useSparse);

    virtual ~StringBlock();

    CARAPI Get(
        /* [in] */ Int32 idx,
        /* [out] */ ICharSequence** text);

private:
    static Int32 NativeCreate(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 size);

    static Int32 NativeGetSize(
        /* [in] */ const android::ResStringPool* pool);

    static CARAPI NativeGetString(
        /* [in] */ const android::ResStringPool* pool,
        /* [in] */ Int32 idx,
        /* [out] */ String* str);

    static const ArrayOf<Int32>* NativeGetStyle(
        /* [in] */ const android::ResStringPool* pool,
        /* [in] */ Int32 idx);

    static Int32 NativeIndexOfString(
        /* [in] */ const android::ResStringPool* pool,
        /* [in] */ String str);

    static void NativeDestroy(
        /* [in] */ const android::ResStringPool* pool);

private:
    static const String TAG;
    static const Boolean sLocalLOGV = FALSE; // Config.LOGV || false;

    Mutex mSyncLock;
    const android::ResStringPool* mNative;
    Boolean mUseSparse;
    Boolean mOwnsNative;
    Vector<AutoPtr<ICharSequence> >* mStrings;
//	    private SparseArray<CharSequence> mSparseStrings;
//	    /*package*/ StyleIDs mStyleIDs = null;
};

#endif // __HH_STRINGBLOCK_H
