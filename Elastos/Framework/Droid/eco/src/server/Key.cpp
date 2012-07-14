
#include "server/CActivityManagerService.h"
#include "server/Key.h"
#include <elastos/etl_hash_fun.h>

const Int32 Key::ODD_PRIME_NUMBER;

Key::Key(
    /* [in] */ Int32 t,
    /* [in] */ const String& p,
    /* [in] */ CActivityRecord* a,
    /* [in] */ const String& w,
    /* [in] */ Int32 r,
    /* [in] */ CIntent* i,
    /* [in] */ const String& it,
    /* [in] */ Int32 f) :
    mType(t),
    mActivity(a),
    mRequestCode(r),
    mRequestIntent(i),
    mFlags(f)
{
    mCapsuleName = p;
    mWho = w;
    mRequestResolvedType = it;

    Int32 hash = 23;
    hash = (ODD_PRIME_NUMBER * hash) + f;
    hash = (ODD_PRIME_NUMBER * hash) + r;
    if (!w.IsNull()) {
        hash = (ODD_PRIME_NUMBER * hash) + (Int32)HashString(w);
    }
    if (a != NULL) {
        hash = (ODD_PRIME_NUMBER * hash) + a->GetHashCode();
    }
    if (i != NULL) {
        Int32 tmp;
        i->GetFilterHashCode(&tmp);
        hash = (ODD_PRIME_NUMBER * hash) + tmp;
    }
    if (!it.IsNull()) {
        hash = (ODD_PRIME_NUMBER * hash) + (Int32)HashString(w);
    }
    hash = (ODD_PRIME_NUMBER * hash) + (Int32)HashString(p);
    hash = (ODD_PRIME_NUMBER * hash) + t;
    mHashCode = hash;
    //Slog.i(ActivityManagerService.TAG, this + " hashCode=0x"
    //        + Integer.toHexString(hashCode));
}

Boolean Key::Equals(
    /* [in] */ const Key* otherObj) const
{
    if (otherObj == NULL) {
        return FALSE;
    }
//    try {
//        Key other = (Key)otherObj;
//        if (type != other.type) {
//            return false;
//        }
//        if (!packageName.equals(other.packageName)) {
//            return false;
//        }
//        if (activity != other.activity) {
//            return false;
//        }
//        if (who != other.who) {
//            if (who != null) {
//                if (!who.equals(other.who)) {
//                    return false;
//                }
//            } else if (other.who != null) {
//                return false;
//            }
//        }
//        if (requestCode != other.requestCode) {
//            return false;
//        }
//        if (requestIntent != other.requestIntent) {
//            if (requestIntent != null) {
//                if (!requestIntent.filterEquals(other.requestIntent)) {
//                    return false;
//                }
//            } else if (other.requestIntent != null) {
//                return false;
//            }
//        }
//        if (requestResolvedType != other.requestResolvedType) {
//            if (requestResolvedType != null) {
//                if (!requestResolvedType.equals(other.requestResolvedType)) {
//                    return false;
//                }
//            } else if (other.requestResolvedType != null) {
//                return false;
//            }
//        }
//        if (flags != other.flags) {
//            return false;
//        }
//        return true;
//    } catch (ClassCastException e) {
//    }
    return FALSE;
}

Int32 Key::GetHashCode()
{
    return mHashCode;
}
