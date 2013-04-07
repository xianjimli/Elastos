
#ifndef __CREFLECTIONACTION_H__
#define __CREFLECTIONACTION_H__

#include "_CReflectionAction.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

#define ReflectionAction_TAG (2)

CarClass(CReflectionAction)
{
public:
    static const Int32 T_BOOLEAN;
    static const Int32 T_BYTE;
    static const Int32 T_SHORT;
    static const Int32 T_INT;
    static const Int32 T_LONG;
    static const Int32 T_FLOAT;
    static const Int32 T_DOUBLE;
    static const Int32 T_CHAR;
    static const Int32 T_STRING;
    static const Int32 T_CHAR_SEQUENCE;
    static const Int32 T_URI;
    static const Int32 T_BITMAP;
    static const Int32 T_BUNDLE;

public:
    CReflectionAction();

    virtual ~CReflectionAction();

    CARAPI Apply(
        /* [in] */ IView* root);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ Int32 type,
        /* [in] */ IInterface* value);

    CARAPI constructor(
        /* [in] */ IParcel* parcel);

private:
    Int32 mViewId;
    String mMethodName;
    Int32 mType;

    union {
        Boolean mBoolean;
        Byte mByte;
        Int16 mInt16;
        Int32 mInt32;
        Int64 mInt64;
        Float mFloat;
        Double mDouble;
        Char16 mChar16;
        ICharSequence* mString;
        ICharSequence* mCharSequence;
        IUri* mUri;
        IBitmap* mBitmap;
        IBundle* mBundle;
    } mValue;
};

#endif // __CREFLECTIONACTION_H__
