
#include "widget/CReflectionAction.h"

const Int32 CReflectionAction::T_BOOLEAN = 1;
const Int32 CReflectionAction::T_BYTE = 2;
const Int32 CReflectionAction::T_SHORT = 3;
const Int32 CReflectionAction::T_INT = 4;
const Int32 CReflectionAction::T_LONG = 5;
const Int32 CReflectionAction::T_FLOAT = 6;
const Int32 CReflectionAction::T_DOUBLE = 7;
const Int32 CReflectionAction::T_CHAR = 8;
const Int32 CReflectionAction::T_STRING = 9;
const Int32 CReflectionAction::T_CHAR_SEQUENCE = 10;
const Int32 CReflectionAction::T_URI = 11;
const Int32 CReflectionAction::T_BITMAP = 12;
const Int32 CReflectionAction::T_BUNDLE = 13;

CReflectionAction::CReflectionAction()
    : mViewId(0)
    , mType(0)
{
}

CReflectionAction::~CReflectionAction()
{
    if (mType == CReflectionAction::T_STRING) {
        if (mValue.mString != NULL) mValue.mString->Release();
    }
    else if (mType == CReflectionAction::T_CHAR_SEQUENCE) {
        if (mValue.mCharSequence != NULL) mValue.mCharSequence->Release();
    }
    else if (mType == CReflectionAction::T_URI) {
        if (mValue.mUri != NULL) mValue.mUri->Release();
    }
    else if (mType == CReflectionAction::T_BITMAP) {
        if (mValue.mBitmap != NULL) mValue.mBitmap->Release();
    }
    else if (mType == CReflectionAction::T_BUNDLE) {
        if (mValue.mBundle != NULL) mValue.mBundle->Release();
    }
}

ECode CReflectionAction::Apply(
    /* [in] */ IView* root)
{
    AutoPtr<IView> view;
    FAIL_RETURN(root->FindViewById(mViewId, (IView**)&view));

    PInterface intf = view->Probe(EIID_IInterface);
    AutoPtr<IInterfaceInfo> interfaceInfo;
    FAIL_RETURN(CObject::ReflectInterfaceInfo(
        intf, (IInterfaceInfo**)&interfaceInfo));

    AutoPtr<IMethodInfo> methodInfo;
    FAIL_RETURN(interfaceInfo->GetMethodInfo(
        mMethodName, (IMethodInfo**)&methodInfo));

    AutoPtr<IArgumentList> argumentList;
    FAIL_RETURN(methodInfo->CreateArgumentList((IArgumentList**)&argumentList));

    switch (mType) {
        case CReflectionAction::T_BOOLEAN:
            FAIL_RETURN(argumentList->SetInputArgumentOfBoolean(0, mValue.mBoolean));
            break;
        case CReflectionAction::T_BYTE:
            FAIL_RETURN(argumentList->SetInputArgumentOfByte(0, mValue.mByte));
            break;
        case CReflectionAction::T_SHORT:
            FAIL_RETURN(argumentList->SetInputArgumentOfInt16(0, mValue.mInt16));
            break;
        case CReflectionAction::T_INT:
            FAIL_RETURN(argumentList->SetInputArgumentOfInt32(0, mValue.mInt32));
            break;
        case CReflectionAction::T_LONG:
            FAIL_RETURN(argumentList->SetInputArgumentOfInt64(0, mValue.mInt64));
            break;
        case CReflectionAction::T_FLOAT:
            FAIL_RETURN(argumentList->SetInputArgumentOfFloat(0, mValue.mFloat));
            break;
        case CReflectionAction::T_DOUBLE:
            FAIL_RETURN(argumentList->SetInputArgumentOfDouble(0, mValue.mDouble));
            break;
        case CReflectionAction::T_CHAR:
            FAIL_RETURN(argumentList->SetInputArgumentOfChar16(0, mValue.mChar16));
            break;
        case CReflectionAction::T_STRING: {
            String temp;
            FAIL_RETURN(mValue.mString->ToString(&temp));
            FAIL_RETURN(argumentList->SetInputArgumentOfString(0, temp));
            break;
        }
        case CReflectionAction::T_CHAR_SEQUENCE: {
            PInterface intf1 = mValue.mCharSequence->Probe(EIID_IInterface);
            FAIL_RETURN(argumentList->SetInputArgumentOfObjectPtr(0, intf1));
            break;
        }
        case CReflectionAction::T_URI: {
            PInterface intf1 = mValue.mUri->Probe(EIID_IInterface);
            FAIL_RETURN(argumentList->SetInputArgumentOfObjectPtr(0, intf1));
            break;
        }
        case CReflectionAction::T_BITMAP: {
            PInterface intf1 = mValue.mBitmap->Probe(EIID_IInterface);
            FAIL_RETURN(argumentList->SetInputArgumentOfObjectPtr(0, intf1));
            break;
        }
        case CReflectionAction::T_BUNDLE: {
            PInterface intf1 = mValue.mBundle->Probe(EIID_IInterface);
            FAIL_RETURN(argumentList->SetInputArgumentOfObjectPtr(0, intf1));
            break;
        }
        default:
            break;
    }

    if (FAILED(methodInfo->Invoke(intf, argumentList))) {
        return E_ACTION_EXCEPTION;
    }

    return NOERROR;
}

ECode CReflectionAction::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CReflectionAction::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    FAIL_RETURN(dest->WriteInt32(ReflectionAction_TAG));
    FAIL_RETURN(dest->WriteInt32(mViewId));
    FAIL_RETURN(dest->WriteString(mMethodName));
    FAIL_RETURN(dest->WriteInt32(mType));
    //noinspection ConstantIfStatement
    // if (false) {
    //     Log.d("RemoteViews", "write viewId=0x" + Integer.toHexString(this.viewId)
    //             + " methodName=" + this.methodName + " type=" + this.type);
    // }
    switch (mType) {
        case CReflectionAction::T_BOOLEAN:
            FAIL_RETURN(dest->WriteBoolean(mValue.mBoolean));
            break;
        case CReflectionAction::T_BYTE:
            FAIL_RETURN(dest->WriteByte(mValue.mByte));
            break;
        case CReflectionAction::T_SHORT:
            FAIL_RETURN(dest->WriteInt16(mValue.mInt16));
            break;
        case CReflectionAction::T_INT:
            FAIL_RETURN(dest->WriteInt32(mValue.mInt32));
            break;
        case CReflectionAction::T_LONG:
            FAIL_RETURN(dest->WriteInt64(mValue.mInt64));
            break;
        case CReflectionAction::T_FLOAT:
            FAIL_RETURN(dest->WriteFloat(mValue.mFloat));
            break;
        case CReflectionAction::T_DOUBLE:
            FAIL_RETURN(dest->WriteDouble(mValue.mDouble));
            break;
        case CReflectionAction::T_CHAR:
            FAIL_RETURN(dest->WriteChar16(mValue.mChar16));
            break;
        case CReflectionAction::T_STRING: {
            String temp;
            FAIL_RETURN(mValue.mString->ToString(&temp));
            FAIL_RETURN(dest->WriteString(temp));
            break;
        }
        case CReflectionAction::T_CHAR_SEQUENCE:
            // TODO: ALEX need TextUtils.writeToParcel
            return E_NOT_IMPLEMENTED;
            // TextUtils.writeToParcel((CharSequence)this.value, out, flags);
            break;
        case CReflectionAction::T_URI: {
            AutoPtr<IParcelable> parcelable =
                (IParcelable*)(mValue.mUri->Probe(EIID_IParcelable));
            FAIL_RETURN(parcelable->WriteToParcel(dest));
            break;
        }
        case CReflectionAction::T_BITMAP: {
            AutoPtr<IParcelable> parcelable =
                (IParcelable*)(mValue.mBitmap->Probe(EIID_IParcelable));
            FAIL_RETURN(parcelable->WriteToParcel(dest));
            break;
        }
        case CReflectionAction::T_BUNDLE:
            // TODO: ALEX need IParcel::WriteBundle
            return E_NOT_IMPLEMENTED;
            // FAIL_RETURN(dest->WriteBundle(mValue.mBundle));
            break;
        default:
            break;
    }

    return NOERROR;
}

ECode CReflectionAction::constructor(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ Int32 type,
    /* [in] */ IInterface* value)
{
    mViewId = viewId;
    mMethodName = methodName;
    mType = type;

    switch (mType) {
        case CReflectionAction::T_BOOLEAN:
            mValue.mBoolean = *(Boolean*)value;
            break;
        case CReflectionAction::T_BYTE:
            mValue.mByte = *(Byte*)value;
            break;
        case CReflectionAction::T_SHORT:
            mValue.mInt16 = *(Int16*)value;
            break;
        case CReflectionAction::T_INT:
            mValue.mInt32 = *(Int32*)value;
            break;
        case CReflectionAction::T_LONG:
            mValue.mInt64 = *(Int64*)value;
            break;
        case CReflectionAction::T_FLOAT:
            mValue.mFloat = *(Float*)value;
            break;
        case CReflectionAction::T_DOUBLE:
            mValue.mDouble = *(Double*)value;
            break;
        case CReflectionAction::T_CHAR:
            mValue.mChar16 = *(Char16*)value;
            break;
        case CReflectionAction::T_STRING:
            FAIL_RETURN(CStringWrapper::New(*(String*)value, &mValue.mString));
            break;
        case CReflectionAction::T_CHAR_SEQUENCE:
            mValue.mCharSequence = (ICharSequence*)value;
            if (mValue.mCharSequence) mValue.mCharSequence->AddRef();
            break;
        case CReflectionAction::T_URI:
            mValue.mUri = (IUri*)value;
            if (mValue.mUri) mValue.mUri->AddRef();
            break;
        case CReflectionAction::T_BITMAP:
            mValue.mBitmap = (IBitmap*)value;
            if (mValue.mBitmap) mValue.mBitmap->AddRef();
            break;
        case CReflectionAction::T_BUNDLE:
            mValue.mBundle = (IBundle*)value;
            if (mValue.mBundle) mValue.mBundle->AddRef();
            break;
        default:
            break;
    }

    return NOERROR;
}

ECode CReflectionAction::constructor(
    /* [in] */ IParcel* parcel)
{
    FAIL_RETURN(parcel->ReadInt32(&mViewId));
    FAIL_RETURN(parcel->ReadString(&mMethodName));
    FAIL_RETURN(parcel->ReadInt32(&mType));
    //noinspection ConstantIfStatement
    // if (false) {
    //     Log.d("RemoteViews", "read viewId=0x" + Integer.toHexString(this.viewId)
    //             + " methodName=" + this.methodName + " type=" + this.type);
    // }
    switch (mType) {
        case CReflectionAction::T_BOOLEAN:
            FAIL_RETURN(parcel->ReadBoolean(&mValue.mBoolean));
            break;
        case CReflectionAction::T_BYTE:
            FAIL_RETURN(parcel->ReadByte(&mValue.mByte));
            break;
        case CReflectionAction::T_SHORT:
            FAIL_RETURN(parcel->ReadInt16(&mValue.mInt16));
            break;
        case CReflectionAction::T_INT:
            FAIL_RETURN(parcel->ReadInt32(&mValue.mInt32));
            break;
        case CReflectionAction::T_LONG:
            FAIL_RETURN(parcel->ReadInt64(&mValue.mInt64));
            break;
        case CReflectionAction::T_FLOAT:
            FAIL_RETURN(parcel->ReadFloat(&mValue.mFloat));
            break;
        case CReflectionAction::T_DOUBLE:
            FAIL_RETURN(parcel->ReadDouble(&mValue.mDouble));
            break;
        case CReflectionAction::T_CHAR:
            FAIL_RETURN(parcel->ReadChar16(&mValue.mChar16));
            break;
        case CReflectionAction::T_STRING: {
            String temp;
            FAIL_RETURN(parcel->ReadString(&temp));
            FAIL_RETURN(CStringWrapper::New(temp, &mValue.mString));
            break;
        }
        case CReflectionAction::T_CHAR_SEQUENCE:
            // TODO: ALEX need TextUtils::CHAR_SEQUENCE_CREATOR::createFromParcel
            return E_NOT_IMPLEMENTED;
            // this.value = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(in);
            break;
        case CReflectionAction::T_URI:
            // TODO: ALEX need Uri::CREATOR::createFromParcel
            return E_NOT_IMPLEMENTED;
            // this.value = Uri.CREATOR.createFromParcel(in);
            break;
        case CReflectionAction::T_BITMAP:
            // TODO: ALEX need Bitmap::CREATOR::createFromParcel
            return E_NOT_IMPLEMENTED;
            // this.value = Bitmap.CREATOR.createFromParcel(in);
            break;
        case CReflectionAction::T_BUNDLE:
            // TODO: ALEX need IParcel::ReadBundle
            return E_NOT_IMPLEMENTED;
            // this.value = in.readBundle();
            break;
        default:
            break;
    }

    return NOERROR;
}

